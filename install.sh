#!/bin/bash
sudo sed -i 's/%wheel ALL=(ALL:ALL) ALL/%wheel ALL=(ALL:ALL) NOPASSWD: ALL/' /etc/sudoers.d/wheel
sudo xbps-install -Suy
sudo xbps-install -Sy xorg base-devel libX11-devel libXft-devel libXinerama-devel chromium ffmpeg ntfs-3g ugrep noto-fonts-emoji noto-fonts-cjk feh lsd webkit2gtk-devel gcr-devel gstreamer1-devel lxappearance neovim clipmenu mpv mpd alsa-utils ncmpcpp cava newsboat zathura mupdf ranger ueberzug qutebrowser sakura w3m alacritty nodejs gimp bash-completion yt-dlp aria2 obs neofetch flameshot cmake ninja meson curl ImageMagick NetworkManager arandr bat breeze clang cmatrix lolcat-c figlet colordiff timeshift flac fzf git gstreamer-vaapi harfbuzz-devel htop imlib2-devel jq kdenlive libev-devel libjpeg-turbo-devel libmpc-devel linux-headers man-db mpc papirus-folders papirus-icon-theme pcre-devel pkgconf-devel python3-adblock python3-pip rsync simple-mtpfs terminus-font v4l2loopback void-docs-browse xdg-desktop-portal-wlr xdotool zathura-pdf-mupdf tmux xcb-util-renderutil-devel xcb-util-image-devel pkgconf uthash pcre-devel libconfig-devel firefox figlet-fonts dunst noto-fonts-ttf pass wkhtmltopdf audacity readline-devel readline file-devel plata-theme img2pdf pcmanfm cups cups-pdf libinput-gestures

# This for doing testing on different stuff
# sudo xbps-install -Sy river sway Waybar sandbar foot pamixer fcft-devel droidcam asciinema polybar bspwm sxhkd grim slop slurp tor tint2 openbox xfce4 obmenu-generator obconf hplip-gui clipman nerd-fonts wireshark-qt wireshark cloc nim passmenu cvs unbound poppler-utils lf i3 vim mupdf linux-lts scrot qemu virt-manager libvirt vte3 vde2 bridge-utils time screenkey clang-analyzer cmatrix emacs ffplay gdb go zig gvim hugo inkscape instaloader intel-media-driver linux-lts-headers pandoc pdftag rnnoise slop stow tcc texlive texlive-core wofi wbg

cd /tmp/
git clone --depth=1 https://github.com/pijulius/picom.git
cd picom/
git submodule update --init --recursive
meson --buildtype=release . build
sudo ninja -C build install
cd
mkdir Videos Images Downloads Documents Music 
git clone --depth=1 https://github.com/elbachir-one/dotfiles
cd ~/dotfiles/
cp -r .config/ .fonts/ .icons/ .local/ .themes/ .bashrc .xinitrc .tmux.conf .bash_profile ~/
sudo cp -r xorg.conf.d/ /etc/X11/
cd suckless/dwm/ && make && sudo make clean install
cd ../dmenu/ && make && sudo make clean install
cd ../st/ && make && sudo make clean install
cd ../slstatus/ && make && sudo make clean install
cd ../slock/ && make && sudo make clean install
cd ../farbfeld/ && make && sudo make clean install
cd ../sent/ && make && sudo make clean install
cd ../surf/ && make && sudo make clean install
cd
git clone --depth=1 https://github.com/elbachir-one/wall
cd /tmp/
git clone https://github.com/garabik/grc.git
cd grc/
sudo ./install.sh
cd
sudo cp /etc/profile.d/grc.sh /etc/
sudo xbps-remove -Oy && sudo xbps-remove -oy
#sudo xbps-reconfigure -f linux-lts
sudo xbps-reconfigure -fa
fc-cache -fv
sudo sv down dhcpcd
sudo ln -s /etc/sv/NetworkManager /var/service/
sudo ln -s /etc/sv/dbus /var/service/
#sudo ln -s /etc/sv/libvirtd /var/service/
#sudo ln -s /etc/sv/virtlogd /var/service/
#sudo ln -s /etc/sv/seatd /var/service/
#sudo usermod -a -G _seatd sh # Change the username (sh) to your username.
#sudo usermod -a -G libvirt sh # Change the username (sh) to your username than run the script.
#modprobe kvm-intel # Change this if you are using something else that intel cpu "kvm-amd"
#sudo usermod -aG kvm sh # Change the username (sh) to your username than run the script.
sudo rm /var/service/agetty-tty3
sudo rm /var/service/agetty-tty4
sudo rm /var/service/agetty-tty5
sudo rm /var/service/agetty-tty6
sudo rm /var/service/wpa_supplicant
sudo rm /var/service/dhcpcd
sudo sed -i 's/GRUB_TIMEOUT=5/GRUB_TIMEOUT=0/' /etc/default/grub
sudo sed -i 's/GETTY_ARGS="--noclear"/GETTY_ARGS="--noclear --autologin sh"/' /etc/runit/runsvdir/current/agetty-tty1/conf # Change the username (sh) to your username than run the script.
sudo grub-mkconfig -o /boot/grub/grub.cfg
sudo reboot
