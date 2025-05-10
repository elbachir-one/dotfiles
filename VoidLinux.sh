#!/bin/bash
set -e  # Exit on any error
set -o pipefail

USERNAME=$(logname)

# Allow wheel group passwordless sudo
sudo sed -i 's/^%wheel ALL=(ALL:ALL) ALL/%wheel ALL=(ALL:ALL) NOPASSWD: ALL/' /etc/sudoers.d/wheel

# System update
sudo xbps-install -Suy

# Install packages
sudo xbps-install -Sy \
	xorg base-devel libX11-devel libXft-devel libXinerama-devel chromium ffmpeg ntfs-3g \
	ugrep noto-fonts-emoji noto-fonts-cjk feh lsd webkit2gtk-devel gcr-devel gstreamer1-devel \
	lxappearance neovim clipmenu mpv mpd alsa-utils ncmpcpp cava newsboat zathura mupdf \
	ranger ueberzug sakura nodejs bash-completion yt-dlp aria2 wget neofetch stow \
	flameshot cmake ninja meson curl ImageMagick NetworkManager bat breeze clang \
	cmatrix lolcat-c figlet colordiff timeshift flac fzf git gstreamer-vaapi harfbuzz-devel \
	htop imlib2-devel jq libev-devel libjpeg-turbo-devel libmpc-devel linux-headers \
	man-db mpc papirus-folders papirus-icon-theme pcre-devel pkgconf-devel python3-adblock \
	python3-pip rsync simple-mtpfs terminus-font v4l2loopback \
	xdg-desktop-portal-wlr xdotool zathura-pdf-mupdf tmux xcb-util-renderutil-devel \
	xcb-util-image-devel pkgconf uthash pcre-devel libconfig-devel figlet-fonts dunst \
	pass wkhtmltopdf audacity readline-devel readline file-devel plata-theme \
	img2pdf cups cups-pdf libinput-gestures

# Compile and install picom
cd /tmp/
git clone --depth=1 https://github.com/pijulius/picom.git
cd picom
git submodule update --init --recursive
meson --buildtype=release . build
sudo ninja -C build install

# Create user directories
cd ~
mkdir -p Videos Images Downloads Documents Music

# Clone and copy dotfiles
git clone --depth=1 https://github.com/elbachir-one/dotfiles
cd dotfiles
cp -r .config .fonts .icons .local .themes .bashrc .xinitrc .tmux.conf .bash_profile ~/
sudo cp -r xorg.conf.d/ /etc/X11/

# Build suckless programs
SUCKLESS_DIR="$HOME/dotfiles/suckless"

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
cd ~
git clone --depth=1 https://github.com/elbachir-one/wall

# Install grc
cd /tmp/
git clone https://github.com/garabik/grc.git
cd grc
sudo ./install.sh
sudo cp /etc/profile.d/grc.sh /etc/

# Clean up orphan packages
sudo xbps-remove -Oy
sudo xbps-remove -oy

# Reconfigure all packages
sudo xbps-reconfigure -fa

# Update font cache
fc-cache -fv

# Switch to NetworkManager
sudo sv down dhcpcd
sudo ln -sf /etc/sv/NetworkManager /var/service/
sudo ln -sf /etc/sv/dbus /var/service/

# Optional service links (uncomment if needed)
# sudo ln -s /etc/sv/libvirtd /var/service/
# sudo ln -s /etc/sv/virtlogd /var/service/
# sudo ln -s /etc/sv/seatd /var/service/
# sudo usermod -aG _seatd "$USERNAME"
# sudo usermod -aG libvirt "$USERNAME"
# sudo modprobe kvm-intel  # Use 'kvm-amd' for AMD CPUs
# sudo usermod -aG kvm "$USERNAME"

# Remove unwanted ttys and services
for tty in tty3 tty4 tty5 tty6; do
	sudo rm -f /var/service/agetty-$tty
done

sudo rm -f /var/service/wpa_supplicant
sudo rm -f /var/service/dhcpcd

# GRUB and auto-login config
sudo sed -i 's/^GRUB_TIMEOUT=.*/GRUB_TIMEOUT=0/' /etc/default/grub
sudo sed -i "s/GETTY_ARGS=\"--noclear\"/GETTY_ARGS=\"--noclear --autologin $USERNAME\"/" \
	/etc/runit/runsvdir/current/agetty-tty1/conf

# Update GRUB config
sudo grub-mkconfig -o /boot/grub/grub.cfg

# Reboot system
sudo reboot
