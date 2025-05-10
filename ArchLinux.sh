#!/bin/bash
set -e  # Exit on any error
set -o pipefail

# Ensure script is not run as root
if [[ $EUID -eq 0 ]]; then
	echo "Please run this script as a regular user, not root."
	exit 1
fi

USERNAME=$(logname)

# Allow wheel group passwordless sudo
echo "Configuring sudoers for wheel group..."
sudo sed -i 's/^%wheel ALL=(ALL:ALL) ALL/%wheel ALL=(ALL:ALL) NOPASSWD: ALL/' /etc/sudoers

# System update
echo "Updating system..."
sudo pacman -Syu --noconfirm

# Configure DNS (fix redirection issue)
echo "Setting DNS servers..."
echo "nameserver 1.1.1.1" | sudo tee -a /etc/resolv.conf
echo "nameserver 8.8.8.8" | sudo tee -a /etc/resolv.conf

# Install required base packages for yay
echo "Installing base-devel, go, and git..."
sudo pacman -S --noconfirm go git base-devel

# Setup yay
echo "Cloning and building yay..."
cd /tmp/
git clone --depth=1 https://aur.archlinux.org/yay
cd yay
makepkg -si --noconfirm
cd ~

# Install packages
echo "Installing software packages..."
yay -Sy --noconfirm \
	xorg chromium vim ffmpeg ntfs-3g ugrep noto-fonts-emoji noto-fonts-cjk \
	feh lsd webkit2gtk gcr gstreamer lxappearance clipmenu mpv mpd alsa-utils \
	ncmpcpp cava newsboat zathura ranger ueberzug sakura nodejs npm bash-completion \
	yt-dlp stow flameshot cmake ninja meson curl imagemagick bat breeze cmatrix \
	lolcat figlet colordiff timeshift flac fzf gstreamer-vaapi harfbuzz \
	htop imlib2 jq libev libjpeg-turbo libmpc linux-headers man-db mpc papirus-folders \
	papirus-icon-theme pcre pkgconf python-pip rsync simple-mtpfs terminus-font \
	v4l2loopback-dkms v4l2loopback-utils xdg-desktop-portal-wlr xdotool zathura-pdf-mupdf \
	tmux xcb-util-renderutil xcb-util-image uthash libconfig dunst pass \
	readline file seatd img2pdf cups cups-pdf libinput-gestures

# Compile and install picom
echo "Cloning and building picom..."
cd /tmp/
git clone --depth=1 https://github.com/pijulius/picom.git
cd picom
git submodule update --init --recursive
meson --buildtype=release . build
sudo ninja -C build install

# Create user directories
echo "Creating user directories..."
cd ~
mkdir -p Videos Images Downloads Documents Music

# Clone and copy dotfiles (non-overwriting)
echo "Cloning and applying dotfiles..."
git clone --depth=1 https://github.com/elbachir-one/dotfiles
cd dotfiles
cp -rn .config .fonts .icons .local .themes .bashrc .xinitrc .tmux.conf .bash_profile ~/
sudo cp -r xorg.conf.d/ /etc/X11/

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
git clone --depth=1 https://github.com/elbachir-one/wall

# Install grc
echo "Installing grc..."
cd /tmp/
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

# Optional service link
echo "Adding $USERNAME to seat group..."
sudo usermod -aG seat "$USERNAME"

# Final reboot
echo "Setup complete. Rebooting..."
sudo reboot
