#!/usr/bin/env bash

set -e
set -o pipefail

# Allow wheel group passwordless sudo
echo "%wheel ALL=(ALL:ALL) NOPASSWD: ALL" | sudo tee /etc/sudoers.d/wheel
sudo chmod 440 /etc/sudoers.d/wheel

# System update/installing some packages
sudo xbps-install -Suy

sudo xbps-install -Sy xorg base-devel libX11-devel libXft-devel libXinerama-devel \
	chromium ffmpeg ntfs-3g ugrep noto-fonts-emoji noto-fonts-cjk feh lsd \
	libwebkit2gtk41-devel gcr-devel gstreamer1-devel lxappearance neovim clipmenu \
	mpv mpd alsa-utils ncmpcpp cava newsboat zathura mupdf libinput-gestures xwallpaper \
	ranger ueberzug sakura nodejs bash-completion yt-dlp aria2 wget ufetch stow \
	flameshot cmake ninja meson curl ImageMagick NetworkManager bat breeze clang \
	cmatrix lolcat-c figlet colordiff timeshift flac fzf git gstreamer-vaapi harfbuzz-devel \
	htop imlib2-devel jq libev-devel libjpeg-turbo-devel libmpc-devel linux-headers \
	man-db mpc papirus-folders papirus-icon-theme pcre-devel pkgconf-devel python3-adblock \
	python3-pip rsync simple-mtpfs terminus-font v4l2loopback xdg-desktop-portal-wlr \
	xdotool zathura-pdf-mupdf tmux xcb-util-renderutil-devel xcb-util-image-devel \
	pkgconf uthash pcre-devel libconfig-devel figlet-fonts dunst pass wkhtmltopdf \
	audacity readline-devel readline file-devel plata-theme img2pdf cups cups-pdf

# Compile/Install Picom
cd /tmp/
[ -d picom ] && rm -rf picom
git clone --depth=1 https://github.com/pijulius/picom.git
cd picom
git submodule update --init --recursive
meson setup --buildtype=release . build
sudo ninja -C build install

# Clone/Copy dotfiles
cd ~
[ -d dotfiles ] && rm -rf dotfiles
git clone --depth=1 https://github.com/elbachir-one/dotfiles
cd dotfiles/
cp -r .config .fonts .icons .local .themes .bashrc .xinitrc .tmux.conf .bash_profile ~/

# Build suckless programs
SUCKLESS_DIR="$HOME/dotfiles/suckless"

for prog in dwm dmenu st slstatus slock farbfeld sent; do
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
[ -d wall ] && rm -rf wall
git clone --depth=1 https://github.com/elbachir-one/wall

# Installing grc
cd /tmp/
[ -d grc ] && rm -rf grc
git clone https://github.com/garabik/grc.git
cd grc/
sudo ./install.sh
sudo cp /etc/profile.d/grc.sh /etc/

# Clean up orphan packages
sudo xbps-remove -Oy
sudo xbps-remove -oy

# Reconfigure all packages
sudo xbps-reconfigure -fa

# Reboot system
sudo reboot
