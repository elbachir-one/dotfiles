#!/bin/bash
sudo sed -i 's/%wheel ALL=(ALL:ALL) ALL/%wheel ALL=(ALL:ALL) NOPASSWD: ALL/' /etc/sudoers.d/wheel
sudo xbps-install -Suy
sudo xbps-install -Sy xorg base-devel libX11-devel libXft-devel libXinerama-devel chromium ffmpeg ntfs-3g ugrep nerd-fonts noto-fonts-emoji noto-fonts-cjk feh lsd webkit2gtk-devel gcr-devel gstreamer1-devel lxappearance vim neovim clipmenu mpv mpd alsa-utils ncmpcpp cava newsboat zathura mupdf ranger ueberzug qutebrowser sakura w3m alacritty nodejs gimp bash-completion linux-lts yt-dlp aria2 obs neofetch scrot flameshot qemu virt-manager libvirt vte3 vde2 bridge-utils cmake ninja meson curl time screenkey ImageMagick NetworkManager arandr bat breeze clang-analyzer clang cmatrix lolcat-c figlet colordiff timeshift emacs ffplay flac fzf gdb git go zig gstreamer-vaapi gvim harfbuzz-devel htop hugo imlib2-devel inkscape instaloader jq intel-media-driver kdenlive libev-devel libjpeg-turbo-devel libmpc-devel linux-lts-headers man-db mpc pandoc papirus-folders papirus-icon-theme pcre-devel pdftag pkgconf-devel python3-adblock python3-pip rnnoise rsync simple-mtpfs slop stow tcc terminus-font texlive texlive-core tty-clock v4l2loopback void-docs-browse wireshark-qt wireshark wlroots-devel xdg-desktop-portal-wlr xdotool zathura-pdf-mupdf wbg tmux xcb-util-renderutil-devel xcb-util-image-devel pkgconf uthash pcre-devel libconfig-devel lf firefox
cd /tmp/
git clone https://github.com/pijulius/picom.git
cd picom/
git submodule update --init --recursive
meson --buildtype=release . build
sudo ninja -C build install
cd
git clone https://github.com/elbachir-one/dotfiles
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
git clone https://github.com/elbachir-one/wall
cd /tmp/
git clone https://github.com/garabik/grc.git
cd grc/
sudo ./install.sh
cd
sudo cp /etc/profile.d/grc.sh /etc/
sudo xbps-remove -Oy && sudo xbps-remove -oy
sudo xbps-reconfigure -f linux-lts
sudo xbps-reconfigure -fa
fc-cache -fv
sudo sv down dhcpcd
sudo ln -s /etc/sv/NetworkManager /var/service/
sudo ln -s /etc/sv/dbus /var/service/
sudo ln -s /etc/sv/libvirtd /var/service/
sudo ln -s /etc/sv/virtlogd /var/service/
sudo usermod -a -G libvirt sh
modprobe kvm-intel
sudo usermod -aG kvm sh # sh is the my username
sudo rm /var/service/agetty-tty3
sudo rm /var/service/agetty-tty4
sudo rm /var/service/agetty-tty5
sudo rm /var/service/agetty-tty6
sudo rm /var/service/wpa_supplicant
sudo rm /var/service/dhcpcd
sudo sed -i 's/GRUB_TIMEOUT=5/GRUB_TIMEOUT=0/' /etc/default/grub
sudo sed -i 's/GETTY_ARGS="--noclear"/GETTY_ARGS="--noclear --autologin sh"/' /etc/runit/runsvdir/current/agetty-tty1/conf
sudo grub-mkconfig -o /boot/grub/grub.cfg
sudo reboot
