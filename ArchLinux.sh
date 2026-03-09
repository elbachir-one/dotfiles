#!/usr/bin/env bash

set -e
set -o pipefail

# Ensure script is not run as root
if [[ $EUID -eq 0 ]]; then
	echo "Please run this script as a regular user, not root."
	exit 1
fi

# Allow wheel group passwordless sudo
echo "Configuring sudoers for wheel group..."
sudo sed -i 's/^%wheel ALL=(ALL:ALL) ALL/%wheel ALL=(ALL:ALL) NOPASSWD: ALL/' /etc/sudoers

# System update
echo "Updating system..."
sudo pacman -Syu --noconfirm

# Configure DNS
echo "Setting DNS servers..."
echo "nameserver 1.1.1.1" | sudo tee -a /etc/resolv.conf
echo "nameserver 8.8.8.8" | sudo tee -a /etc/resolv.conf

# Install required base packages for yay
echo "Installing base-devel, go, and git..."
sudo pacman -Sy --noconfirm go git base-devel

# Install packages
echo "Installing software packages..."
sudo pacman -Sy --noconfirm \
	xorg xorg-xinit chromium vim ffmpeg ntfs-3g ugrep noto-fonts-{emoji,cjk} \
	feh lsd webkit2gtk gcr gstreamer lxappearance clipmenu mpv mpd alsa-utils \
	ncmpcpp cava newsboat zathura ranger ueberzug sakura nodejs npm bash-completion \
	yt-dlp stow flameshot cmake ninja meson curl imagemagick bat breeze cmatrix \
	lolcat figlet colordiff timeshift flac fzf harfbuzz fastfetch xwallpaper \
	htop imlib2 jq libev libjpeg-turbo libmpc linux-headers man-db mpc cups \
	papirus-icon-theme pcre pkgconf python-pip rsync terminus-font cups-pdf \
	v4l2loopback-dkms v4l2loopback-utils xdg-desktop-portal-wlr xdotool zathura-pdf-mupdf \
	tmux xcb-util-renderutil xcb-util-image uthash libconfig dunst pass readline \
	file seatd img2pdf

echo "Installing AUR packages..."
if ! command -v yay &>/dev/null; then
	echo "yay not found, installing..."
	git clone https://aur.archlinux.org/yay.git /tmp/yay
	cd /tmp/yay
	makepkg -si --noconfirm
fi

yay -S --noconfirm \
	simple-mtpfs libinput-gestures papirus-folders

# Compile and install picom
echo "Cloning and building picom..."
cd /tmp/
[ -d picom ] && rm -rf picom
git clone --depth=1 https://github.com/pijulius/picom.git
cd picom
git submodule update --init --recursive
meson --buildtype=release . build
sudo ninja -C build install

# Clone dotfiles
cd ~
[ -d dotfiles ] && rm -rf dotfiles
git clone --depth=1 https://github.com/elbachir-one/dotfiles
cd dotfiles
cp -rn .config .fonts .icons .local .themes .bashrc .xinitrc .tmux.conf .bash_profile ~/

# Build suckless programs
SUCKLESS_DIR="$HOME/dotfiles/suckless"
echo "Building suckless programs..."
for prog in dwm dmenu st slstatus slock farbfeld sent surf; do
	PROG_PATH="$SUCKLESS_DIR/$prog"
	if [ -d "$PROG_PATH" ]; then
		echo "Building $prog..."
		cd "$PROG_PATH"
		make
		sudo make clean install
	else
		echo "Warning: $PROG_PATH does not exist, skipping."
	fi
done

# Clone wallpapers
echo "Cloning wallpapers..."
cd ~
[ -d wall ] && rm -rf wall
git clone --depth=1 https://github.com/elbachir-one/wall

# Install grc
echo "Installing grc..."
cd /tmp/
[ -d grc ] && rm -rf grc
git clone https://github.com/garabik/grc.git
cd grc
sudo ./install.sh
sudo cp /etc/profile.d/grc.sh /etc/

# Clean up orphan packages
echo "Cleaning up AUR cache..."
yay -Sc --noconfirm

# Update font cache
echo "Updating font cache..."
fc-cache -fv

# Rebuild the initramfs
echo "Re-building the Initramfs"
sudo mkinitcpio -P -v

# Final reboot
echo "Setup complete. Rebooting..."
sleep 2s
sudo reboot
