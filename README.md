![Void_DWM](assets/Void_DWM.jpg)
![Void_DWM](assets/Void_DWM2.jpg)
![Void_DWM](assets/Void_DWM.gif)
# My dotfiles

## This is my personal configuration, feel free to change it as you see fit your needs.

## Installation

If you want to install my stuff, there are some dependences needed for everting to work.

## Void Linux

### Option 1

Using the install script:

```bash
wget https://raw.githubusercontent.com/elbachir-one/dotfiles/main/install.sh

chmod +x install.sh

./install.sh
```

### Option 2

```bash
sudo xbps-install -Sy nerd-fonts-ttf feh xorg libXft-devel libX11-devel \
libXinerama-devel lsd mpv mpd ncmpcpp zathura neovim vim gvim newsboat picom \
alacritty lxappearance alsa-utils htop tmux ranger qutebrowser chromium ffmpeg \
rsync ueberzug mediainfo base-devel clang yt-dlp clipmenu wget curl tree nodejs \
jq fzf terminus-font scrot ninja networkmanager linux-lts harfbuzz-devel git \
cmake bash-completion noto-fonts-emoji
```

## FreeBSD

```bash
doas pkg install nerd-fonts feh xorg mpv neovim vim git picom tmux chromium \
yt-dlp terminus-font bash noto-fonts-emoji
```
### Note 1
The main thing about my dotfiles is the use of Suckless utils.
Keybinding used in my build of dwm

| MODKEY -> ALT |
| Open ST terminal -> MODKEY + Shift + Return |
| Launch Dmenu -> MODKEY + p |
| Tiling to Floating -> MODKEY + Space |
| Restart or Quit DWM -> MODKEY + q |
| Close any Window -> MODKEY + Shift + c |
| Scratchpad -> MODKEY + s |

### Note

The src directory contains some wayland stuff, like dwl or dmenu for wayland.
If you are looking to use my build of DWL,
you first need to see what version of Wlroots is available for your system.
This build is specific to Wlroots. 17

Dwl has some patches added to it:

- always center
- attachbottom
- autostart
- ipc
- pertag
- swallow
- vanity gaps

```bash
cd dwl/
make
sudo make clean install
```

As for Dmenu-wayland

```bash
cd dmenu-wayland/
mkdir build
meson build
ninja -C build
sudo ninja -C build install
```

dwl-bar does have some patches added to it like ipc and hide vacant tags.

```bash
cd dwl-bar/
make
sudo make clean install
```
