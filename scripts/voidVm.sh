#!/usr/bin/env bash

# --- Void Linux Auto Installer For Qemu Made by ALPHA ---

# ========================
# part1: Base installation
# ========================
printf '\033c'
echo "Welcome to Void Mega"

device="/dev/vda"
echo "Selected device: $device"

xbps-install -S
xbps-install -uy xbps
echo "Installing Parted"
xbps-install -Sy parted

# Partition the disk
parted --script "$device" -- mklabel gpt \
	mkpart ESP fat32 1MiB 512MiB \
	set 1 esp on \
	mkpart primary 512MiB 100%

# Format boot
mkfs.vfat -F32 "${device}1"

# Format root
mkfs.ext4 "${device}2"

# Mount partitions
mount "${device}2" /mnt
mount --mkdir "${device}1" /mnt/boot

# Keys
mkdir -p /mnt/var/db/xbps/keys
cp /var/db/xbps/keys/* /mnt/var/db/xbps/keys/

# Install base system
xbps-install -Sy -R https://repo-default.voidlinux.org/current -r /mnt \
	base-system grub-x86_64-efi vim efibootmgr bash-completion curl chrony

# Generate fstab
root_uuid=$(blkid -s UUID -o value "${device}2")
boot_uuid=$(blkid -s UUID -o value "${device}1")

tee > /mnt/etc/fstab <<EOF
UUID=$root_uuid / ext4 defaults 0 1
UUID=$boot_uuid /boot vfat defaults 0 2
tmpfs /tmp tmpfs defaults,nosuid,nodev 0 0
EOF

# Copy second stage of script into new system
sed '1,/^#part2$/d' "$0" > /mnt/voidMega.sh
chmod +x /mnt/voidMega.sh

# Chroot into system
xchroot /mnt ./voidMega.sh

exit

#part2
# ========================
# part2: System configure
# ========================
printf '\033c'

# Chroot
chown root:root /
chmod 755 /

# Hostname
echo VOID > /etc/hostname

# Create user
useradd -mG wheel sh
echo "%wheel ALL=(ALL:ALL) NOPASSWD: ALL" > /etc/sudoers.d/wheel
chmod 440 /etc/sudoers.d/wheel

# Config
cat > /etc/rc.conf <<EOF
TIMEZONE="Africa/Casablanca"
HARDWARECLOCK="UTC"
KEYMAP=fr
EOF

# Locale
echo "LANG=en_US.UTF-8" > /etc/locale.conf
echo "en_US.UTF-8 UTF-8" >> /etc/default/libc-locales
xbps-reconfigure -f glibc-locales

# Hosts
tee > /etc/hosts <<EOF
127.0.0.1       localhost
::1             localhost
127.0.1.1       VOID.localdomain    VOID
EOF

# Grub
sed -i 's/^GRUB_TIMEOUT=.*/GRUB_TIMEOUT=0/' /etc/default/grub

sed -i 's|^GRUB_CMDLINE_LINUX_DEFAULT=.*|GRUB_CMDLINE_LINUX_DEFAULT="loglevel=4 console=ttyS0,115200n8"|' /etc/default/grub

grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=GRUB --recheck

grub-mkconfig -o /boot/grub/grub.cfg

# Re-configure all
echo "Reconfigure all"
xbps-reconfigure -fa

# Services
ln -s /etc/sv/agetty-ttyS0 /etc/runit/runsvdir/default/
ln -s /etc/sv/agetty-tty1 /etc/runit/runsvdir/default/
ln -s /etc/sv/dhcpcd /etc/runit/runsvdir/default/
ln -s /etc/sv/chronyd /etc/runit/runsvdir/default/
ln -s /etc/sv/udevd /etc/runit/runsvdir/default/

ls -alh /etc/runit/runsvdir/default/

# Root and User passwd
passwd root
passwd sh

echo "Installation complete! Type reboot."
exit
