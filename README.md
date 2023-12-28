![Void_DWM](assets/Void_DWM.jpg)
![Void_DWM](assets/Void_DWM2.jpg)

# My dotfiles

## This is my personal configuration, feel free to change it as you see fit your needs.

## Installation

If you want to install my stuff, there are some dependences needed for everting to work.

## Void Linux

### Option 1: Using the install script

```bash
wget https://raw.githubusercontent.com/elbachir-one/dotfiles/main/install.sh

chmod +x install.sh

./install.sh
```

Note: The install script only work on Void Linux and it's a personal script.

### Option 2: Manual installation

```bash
sudo xbps-install -Sy nerd-fonts-ttf feh xorg libXft-devel libX11-devel \
libXinerama-devel lsd mpv mpd ncmpcpp zathura neovim vim gvim newsboat picom \
alacritty lxappearance alsa-utils htop tmux ranger qutebrowser chromium ffmpeg \
rsync ueberzug mediainfo base-devel clang yt-dlp clipmenu wget curl tree nodejs \
jq fzf terminus-font scrot ninja networkmanager linux-lts harfbuzz-devel git \
cmake bash-completion noto-fonts-emoji
```

Note: Install dependences and copy what you need from the dotfiles.

## FreeBSD

```bash
doas pkg install nerd-fonts feh xorg mpv neovim vim git picom tmux chromium \
yt-dlp terminus-font bash noto-fonts-emoji
```

### DWM

The main focus of my dotfiles is the use of Suckless utilities.

![DWM](assets/dwm.jpg)
![DWM](assets/dwm.webm)

**Patches included:**

- [alpha](https://dwm.suckless.org/patches/alpha/)
- [always center](https://dwm.suckless.org/patches/alwayscenter/)
- [attach bottom](https://dwm.suckless.org/patches/attachbottom/)
- [azerty](https://dwm.suckless.org/patches/azerty/) (Specific to my keyboard layout)
- [bar height](https://dwm.suckless.org/patches/bar_height/)
- [bar padding](https://dwm.suckless.org/patches/barpadding/)
- [bidi](https://dwm.suckless.org/patches/bidi/)
- [cool auto start](https://dwm.suckless.org/patches/cool_autostart/)
- [cursor wrap](https://dwm.suckless.org/patches/cursorwarp/)
- [floating border colors](https://dwm.suckless.org/patches/float_border_color/)
- [full screen](https://dwm.suckless.org/patches/fullscreen/)
- [functional gaps](https://dwm.suckless.org/patches/functionalgaps/)
- [hide vacant tags](https://dwm.suckless.org/patches/hide_vacant_tags/)
- [preserv on restart](https://dwm.suckless.org/patches/preserveonrestart/)
- [Scratch pads](https://dwm.suckless.org/patches/scratchpads/)
- [swallow](https://dwm.suckless.org/patches/swallow/)
- [true centered title](https://dwm.suckless.org/patches/truecenteredtitle/)
- [Window map](https://dwm.suckless.org/patches/windowmap/)

Keybindings used in my build of DWM:

MODKEY=ALT

- Open ST terminal: MODKEY + Shift + Return
- Launch Dmenu: MODKEY + p
- Tiling to Floating: MODKEY + Space
- Restart or Quit DWM: MODKEY + q
- Close any Window: MODKEY + Shift + c
- Scratchpad: MODKEY + s

```bash
cd ~/dotfiles/suckless/dwm/
make
sudo make clean install
```

### ST

![ST](assets/st.jpg)

```bash
cd ~/dotfiles/suckless/st/
make
sudo make clean install
```

### Dmenu

![Dmenu](assets/Dmenu.jpg)

```bash
cd ~/dotfiles/suckless/dmenu/
make
sudo make clean install
```

### DWL

The src directory contains some Wayland stuff, like dwl or dmenu for Wayland.
If you want to use my build of DWL,
first check the version of Wlroots available for your system.
This build is specific to Wlroots 17.

- always center
- attachbottom
- autostart
- ipc
- pertag
- swallow
- vanity gaps

![Void DWL](assets/dwl.jpg)

```bash
cd ~/dotfiles/src/dwl/
make
sudo make clean install
```

Note: Keybindings are the same as my DWM.

As for Dmenu-wayland

```bash
cd ~/dotfiles/src/dmenu-wayland/
mkdir build
meson build
ninja -C build
sudo ninja -C build install
```

dwl-bar does have some patches added to it like.

- ipc
- hide vacant tags

```bash
cd ~/dotfiles/src/dwl-bar/
make
sudo make clean install
```
