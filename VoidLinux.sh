#!/usr/bin/env bash

set -e
set -o pipefail

# Allow wheel group passwordless sudo
echo "%wheel ALL=(ALL:ALL) NOPASSWD: ALL" | sudo tee /etc/sudoers.d/wheel
sudo chmod 440 /etc/sudoers.d/wheel

# System update/installing some packages
sudo xbps-install -Suy

sudo xbps-install -Sy \
	xorg libX{11,Xft,Xinerama}-devel chromium ffmpeg ntfs-3g ugrep plata-theme \
	noto-fonts-{emoji,cjk} feh lsd clipmenu cups{,-pdf} mupdf cava \
	lxappearance neovim mp{v,d} alsa-utils ncmpcpp newsboat zathura \
	libinput-gestures xwallpaper ranger ueberzug sakura nodejs bash-completion \
	yt-dlp aria2 wget ufetch stow flameshot cmake ninja meson curl ImageMagick \
	NetworkManager bat breeze clang cmatrix lolcat-c figlet colordiff timeshift \
	flac fzf git gstreamer-vaapi htop jq man-db mpc rsync simple-mtpfs uthash \
	{libwebkit2gtk41,gcr,gstreamer1,base,harfbuzz,pcre,libconfig,file,readline,}-devel \
	imlib2-devel jq lib{ev,jpeg-turbo,mpc}-devel linux-headers tmux pass dunst \
	papirus-{folders,icon-theme} python3-{adblock,pip} terminus-font img2pdf \
	v4l2loopback xdg-desktop-portal-wlr xdotool zathura-pdf-mupdf audacity \
	xcb-util-{renderutil,image}-devel figlet-fonts wkhtmltopdf \

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

# Enable TRIM
sudo mkdir /etc/cron.weekly
sudo tee /etc/cron.weekly/fstrim > /dev/null <<EOF
#!/bin/sh

fstrim /
EOF

sudo chmod u+x /etc/cron.weekly/fstrim
# Reboot system
sudo reboot
