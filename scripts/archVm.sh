#!/usr/bin/env bash

# --- Arch Linux Auto Installer For Qemu Made by ALPHA ---

# ========================
# part1: Base installation
# ========================
setfont -d
printf '\033c'
echo "Welcome to Arch Mega"

device="/dev/vda"
echo "Selected device: $device"

# Update keyring
sed -i 's/^#Color/Color/' /etc/pacman.conf
sed -i 's/^#\?ParallelDownloads.*/ParallelDownloads = 1/' /etc/pacman.conf
pacman -Sy --noconfirm
pacman --noconfirm -S archlinux-keyring

# Partition the disk
parted --script "$device" -- mklabel gpt \
	mkpart ESP fat32 1MiB 1024MiB \
	set 1 esp on \
	mkpart primary 1024MiB 100%

# Format boot
mkfs.vfat -F32 "${device}1"

# Format root
mkfs.ext4 "${device}2"

# Mount partitions
mount "${device}2" /mnt
mount --mkdir "${device}1" /mnt/boot

# Install base system
pacstrap -K /mnt linux-lts linux-lts-headers base base-devel vim \
	terminus-font efibootmgr git go openssh mtools ntfs-3g dosfstools \
	reflector alsa-utils bash-completion freetype2 libisoburn fuse3 curl wget

# Generate fstab
genfstab -U /mnt > /mnt/etc/fstab

# UUID copy
getUUID=$(blkid -s UUID -o value "${device}2")
echo "$getUUID" > /mnt/getuuid

# Copy second stage of script into new system
sed '1,/^#part2$/d' "$0" > /mnt/archMega.sh
chmod +x /mnt/archMega.sh

# Chroot into system
arch-chroot /mnt ./archMega.sh

exit

#part2
# ========================
# part2: System configure
# ========================
printf '\033c'

# Prompt for bootloader
echo "Which bootloader do you want to use?"
echo "1) systemd-boot (default)"
echo "2) GRUB"
read -rp "Enter choice [1-2]: " boot_choice
boot_choice=${boot_choice:-1}  # default to systemd-boot
echo

# Variables
rootpassword="arch"
username="sh"
userpassword="arch"
hostname="ARCH"
keymap="fr"
font="ter-d20b"
zone="Africa/Casablanca"

# Hostname
echo "$hostname" > /etc/hostname

# Root password
echo "root:$rootpassword" | chpasswd

# Create user
useradd -mG wheel "$username"
echo "$username:$userpassword" | chpasswd
echo "%wheel ALL=(ALL:ALL) NOPASSWD: ALL" > /etc/sudoers

# Timezone & clock
ln -sf "/usr/share/zoneinfo/$zone" /etc/localtime

hwclock --systohc

# Locale
echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen
locale-gen
echo "LANG=en_US.UTF-8" > /etc/locale.conf

# Console keymap & font
cat > /etc/vconsole.conf <<EOF
KEYMAP=$keymap
FONT=$font
EOF

# Pacman config
sed -i 's/^#Color/Color/' /etc/pacman.conf
sed -i 's/^#\?ParallelDownloads.*/ParallelDownloads = 1/' /etc/pacman.conf

# Hosts
cat > /etc/hosts <<EOF
127.0.0.1       localhost
::1             localhost
127.0.1.1       $hostname.localdomain $hostname
EOF

# Systemd networkd
tee > /etc/systemd/network/en.network <<EOF
[Match]
Name=en*
[Network]
DHCP=ipv4
EOF

# Enabling services
systemctl enable systemd-networkd
systemctl enable systemd-resolved
systemctl enable systemd-timesyncd
sudo systemctl enable reflector
sudo systemctl enable reflector.timer
systemctl enable sshd

# Bootloader installation
if [ "$boot_choice" -eq 1 ]; then
	bootctl install

	getUUID=$(cat getuuid)

	tee > /boot/loader/loader.conf <<EOF
default arch
timeout 0
console-mode max
EOF

tee > /boot/loader/entries/arch.conf <<EOF
title Arch Linux
linux /vmlinuz-linux-lts
initrd /initramfs-linux-lts.img
options root=UUID=$getUUID rw console=ttyS0,115200n8
EOF

else
	pacman -S --noconfirm grub

	sed -i 's/^GRUB_TIMEOUT=.*/GRUB_TIMEOUT=0/' /etc/default/grub

	sed -i 's|^GRUB_CMDLINE_LINUX_DEFAULT=.*|GRUB_CMDLINE_LINUX_DEFAULT="console=ttyS0,115200n8"|' /etc/default/grub

	grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=GRUB --recheck

	grub-mkconfig -o /boot/grub/grub.cfg
fi

# DNS
tee > /etc/resolv.conf <<EOF
nameserver 1.1.1.1
nameserver 8.8.8.8
EOF

# Re-build the initramfs
echo "Rebuilding the Initramfs"
mkinitcpio -P

echo "Installation complete! Type reboot."
echo "The User name is ($username) and the password is ($userpassword) also the root password is ($rootpassword)"
exit
