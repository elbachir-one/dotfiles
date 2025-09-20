#!/usr/bin/env bash

# This is stage 2 after installing the system, where all the necessary setup
# tasks are handled automatically instead of manually.

set -euo pipefail

# Get the Hostname
if command -v hostnamectl >/dev/null 2>&1; then
	HOSTNAME=$(hostnamectl --static)
else
	HOSTNAME=$(hostname)
fi

if [[ "$HOSTNAME" == *"ARCH"* ]]; then
	echo "Detected Arch Linux system ($HOSTNAME)"

	sudo reflector --latest 10 --protocol https --sort rate --save /etc/pacman.d/mirrorlist

	echo
	sudo tee /etc/resolv.conf > /dev/null <<EOF
	nameserver 1.1.1.1
	nameserver 8.8.8.8
EOF

	echo
	sudo pacman -Syu --noconfirm
	sudo pacman -S --noconfirm git go base-devel

	printf "[Service]\nExecStart=\nExecStart=-/sbin/agetty --autologin %s --keep-baud 115200,38400,9600 %%I \$TERM\n" "$(whoami)" | sudo systemctl edit serial-getty@ttyS0.service --stdin
	echo
	cat /etc/systemd/system/serial-getty@ttyS0.service.d/override.conf
	echo

	cd /tmp/
	git clone https://aur.archlinux.org/yay.git
	cd yay && makepkg -si --noconfirm
	cd ~

	echo
	yay -Syu --noconfirm

	echo
	yay -S --noconfirm bat lsd yt-dlp fzf tmux fontconfig htop fastfetch xclip \
		xdotool awesome-terminal-fonts libxkbcommon-x11 python-zstandard time \
		python-h2 ffmpeg aria2 rtmpdump libpulse imagemagick chafa dconf tree \
		ddcutil rrdtool noto-fonts-{cjk,emoji,ttf} xterm alsa-utils python3 \
		harfbuzz dnsmasq

	echo
	sudo sed -i 's/HOOKS=(base udev autodetect microcode modconf kms keyboard keymap consolefont block filesystems fsck)/HOOKS=(base udev autodetect modconf block filesystems fsck)/' /etc/mkinitcpio.conf

	echo
	sudo mkinitcpio -P

	echo
	yay -Sc --noconfirm

	PKG_ALIASES=$(cat <<'EOF'
alias q='yay -Ss'
alias u='yay -Syu --noconfirm'
alias i='yay -S --noconfirm'
alias c='yay -Sc --noconfirm'
alias d='yay -Rns'
EOF
)

elif [[ "$HOSTNAME" == *"VOID"* ]]; then
	echo "Detected Void Linux system ($HOSTNAME)"

	sudo sed -i "/^GETTY_ARGS=/c\GETTY_ARGS=\" --noclear --autologin $(whoami)\"" /etc/runit/runsvdir/current/agetty-ttyS0/conf
	echo
	cat /etc/runit/runsvdir/current/agetty-ttyS0/conf

	# Ignore unnecessary packages
	sudo tee /etc/xbps.d/ignore.conf > /dev/null <<EOF
ignorepkg=linux-firmware-amd
ignorepkg=linux-firmware-nvidia
ignorepkg=linux-firmware-intel
ignorepkg=linux-firmware-network
ignorepkg=linux-firmware-broadcom
ignorepkg=wpa_supplicant
ignorepkg=linux6.12
ignorepkg=linux-headers
ignorepkg=btrfs-progs
EOF

	# Removing uneeded things
	sudo xbps-remove -Ry linux-firmware-{amd,nvidia,intel,network,broadcom} \
		wpa_supplicant linux6.12 linux-headers

	sudo touch /etc/sv/agetty-tty{2,3,4,5,6}/down

	sudo rm /var/service/agetty-tty{2,3,4,5,6}
	echo

	echo "Updating the System"
	echo
	# Updating the system
	sudo xbps-install -S
	sudo xbps-install -uy xbps
	sudo xbps-install -Suy

	# Installing some packages
	sudo xbps-install -Sy git go bat lsd yt-dlp fzf curl tmux fontconfig \
		htop ufetch xclip xdotool base-devel libXft-devel font-awesome xterm \
		libxkbcommon-tools ffmpeg aria2 rtmpdump ImageMagick chafa tree time \
		dconf ddcutil rrdtool noto-fonts-{cjk,emoji,ttf} linux-lts alsa-utils \
		linux-lts-headers terminus-font vim chrony nodejs clang llvm python3 \
		libXinerama-devel harfbuzz-devel dnsmasq
	echo

	sudo xbps-reconfigure -fa
	echo
	sudo rm /boot/vmlinuz-6.12.*
	sudo rm /boot/config-6.12.*
	echo
	echo
	# Clean up the grub config file
	sudo tee /etc/default/grub > /dev/null <<EOF
#
# Configuration file for GRUB.
#
GRUB_DEFAULT=0
GRUB_TIMEOUT=0
GRUB_DISTRIBUTOR="Void"
GRUB_CMDLINE_LINUX_DEFAULT="console=ttyS0,115200"
EOF
	echo
	sudo grub-mkconfig -o /boot/grub/grub.cfg
	echo
	sudo rm /var/cache/xbps/*

	echo
	# Dracut
	sudo tee /etc/dracut.conf.d/omit.conf > /dev/null <<EOF
	omit_dracutmodules+=" resume kernel-modules-extra crypt hwdb nvdimm usrmount terminfo shell-interpreter i18n btrfs qemu "
EOF
	sudo dracut -f
	echo

	sudo chmod -x /etc/grub.d/30_os-prober
	echo

	sudo grub-mkconfig -o /boot/grub/grub.cfg
	echo

	sudo vkpurge rm all
	echo

	PKG_ALIASES=$(cat <<'EOF'
alias q='xbps-query -Rs'
alias u='sudo xbps-install -Suy'
alias i='sudo xbps-install -S'
alias c='sudo xbps-remove -oy && sudo xbps-remove -Oy'
alias d='sudo xbps-remove -R'
alias dmesg='dmesg --color=always'
EOF
)

elif [[ "$HOSTNAME" == *"DEB"* ]]; then
	echo "Detected Debian system ($HOSTNAME)"

	printf "[Service]\nExecStart=\nExecStart=-/sbin/agetty --autologin %s --keep-baud 115200,38400,9600 %%I \$TERM\n" "$(whoami)" | sudo systemctl edit serial-getty@ttyS0.service --stdin
	echo
	cat /etc/systemd/system/serial-getty@ttyS0.service.d/override.conf
	echo

	sudo apt update && sudo apt -y upgrade
	sudo apt install -y git golang bat lsd yt-dlp fzf tmux fontconfig \
		htop fastfetch xclip xdotool build-essential libxft-dev curl xterm \
		fonts-font-awesome libxkbcommon-dev ffmpeg aria2 rtmpdump tree time \
		libpulse0 imagemagick chafa dconf-cli ddcutil rrdtool alsa-utils npm \
		fonts-noto-cjk fonts-noto-color-emoji fonts-terminus python3 clang \
		llvm dnsmasq

	PKG_ALIASES=$(cat <<'EOF'
alias q='apt search'
alias u='sudo apt update && sudo apt -y upgrade'
alias i='sudo apt install -y'
alias c='sudo apt autoremove -y && sudo apt clean'
alias d='sudo apt remove --purge -y'
EOF
)

elif [[ "$HOSTNAME" == *"ALPINE"* ]]; then
	echo "Detected Alpine Linux system ($HOSTNAME)"

	sudo tee /etc/default/grub > /dev/null <<EOF
GRUB_TIMEOUT=0
GRUB_DISABLE_SUBMENU=y
GRUB_DISABLE_RECOVERY=true
GRUB_CMDLINE_LINUX_DEFAULT="console=ttyS0,115200 modules=sd-mod,usb-storage,ext4 rootfstype=ext4"
EOF
	echo
	sudo grub-mkconfig -o /boot/grub/grub.cfg
	echo

	sudo apk update && sudo apk upgrade
	echo
	sudo apk add git bash go bat lsd yt-dlp fzf tmux fontconfig curl fastfetch \
		xclip xdotool build-base ffmpeg aria2 rtmpdump pulseaudio imagemagick \
		chafa dconf ddcutil rrdtool font-noto-cjk font-noto-emoji tree time \
		alsa-utils dnsmasq clang llvm python3 htop bash-completion agetty vim
	echo

	sudo tee /usr/sbin/autologin > /dev/null <<EOF
#!/bin/sh
exec login -f sh
EOF
	echo
	sudo chmod +x /usr/sbin/autologin

	sudo tee /etc/inittab > /dev/null <<EOF
::sysinit:/sbin/openrc sysinit
::sysinit:/sbin/openrc boot
::wait:/sbin/openrc default

tty1::respawn:/sbin/getty 38400 tty1
ttyS0::respawn:/sbin/getty -L 115200 -n -l /usr/sbin/autologin ttyS0 vt100

::ctrlaltdel:/sbin/reboot
::shutdown:/sbin/openrc shutdown
EOF
	echo

	sudo sed -i 's|^root:x:0:0:root:/root:/bin/sh$|root:x:0:0:root:/root:/bin/bash|' /etc/passwd
	sudo sed -i 's|^sh:x:1000:1000:sh:/home/sh:/bin/sh$|sh:x:1000:1000:sh:/home/sh:/bin/bash|' /etc/passwd
	echo

	PKG_ALIASES=$(cat <<'EOF'
alias q='apk search'
alias u='sudo apk update && sudo apk upgrade'
alias i='sudo apk add'
alias c='sudo apk cache clean'
alias d='sudo apk del'
EOF
)

# Before all you need to login as root and install (bash, sudo)
# After that change the shell (chsh -s bash username).
# And also add the user to the sudo file.

elif [[ "$HOSTNAME" == *"FREE_BSD"* ]]; then
	echo "Detected FreeBSD system ($HOSTNAME)"

	echo
	sudo pkg update && sudo pkg upgrade
	echo
	sudo pkg install -y bash bash-completion git go bat lsd yt-dlp fzf tmux \
		fontconfig htop fastfetch xclip xdotool libXft curl xterm font-awesome \
		libXkbcommon ffmpeg aria2 tree Imagemagick7 chafa rrdtool alsa-utils \
		npm noto-extra noto-emoji terminus-font python3 llvm dnsmasq vim curl

	echo

	sudo tee /boot/loader.conf > /dev/null <<EOF
autoboot_delay="-1"
boot_multicons="YES"
boot_serial="YES"
comconsole_speed="115200"
console="comconsole,vidconsole"
EOF

	echo
	sudo tee -a /etc/gettytab > /dev/null <<'EOF'
al|Autologin over serial:\
       :al=sh:tc=3wire.9600:
EOF

	echo
	sudo tee /etc/ttys > /dev/null <<EOF
	ttyv0   "/usr/libexec/getty Pc"     xterm   onifexists secure
	ttyu0   "/usr/libexec/getty al"     vt100   onifconsole secure
EOF

	echo
	sudo chsh -s bash sh
	echo

	PKG_ALIASES=$(cat <<'EOF'
[[ $PS1 && -f /usr/local/share/bash-completion/bash_completion.sh ]] && \
		source /usr/local/share/bash-completion/bash_completion.sh

alias q='pkg search'
alias u='sudo pkg update && sudo pkg upgrade'
alias i='sudo pkg install -y'
alias c='sudo pkg clean -y'
EOF
)

else
	echo "Unknown hostname ($HOSTNAME). Please name it ARCH* or VOID*."
	exit 1
fi

cat > ~/.bash_profile <<EOF
[ -f $HOME/.bashrc ] && . $HOME/.bashrc

if [ -t 0 ]; then
	if command -v resize >/dev/null; then
		eval "$(resize)"
	fi
fi
EOF

cat > ~/.bashrc <<EOF
[[ \$- != *i* ]] && return

GRC_ALIASES=true
[[ -s "/etc/profile.d/grc.sh" ]] && source /etc/grc.sh

bind 'set completion-ignore-case on'

export TERM=xterm-256color
export MANPAGER="less -R --use-color -Dd+r -Du+b"
export MANROFFOPT="-P -c"
export EDITOR='vim'
export PATH="\$HOME/.local/bin:\$PATH"
export GOPATH="\$HOME/.local/go"
export MAKEFLAGS="-j2"

$PKG_ALIASES

alias ls='lsd'
alias l='lsd -alh'
alias lh='lsd -hl'
alias ll='lsd -a'
alias s='source ~/.bashrc'
alias cat='bat --style=plain --theme=GitHub'
alias p='sudo poweroff'
alias r='sudo reboot'
alias mi='sudo make install'
alias mc='make clean'
alias lb='lsblk'
alias htop='htop -t'
alias patch='patch -p1 <'
alias grep='grep -i --color=auto'
alias gc='git clone --depth=1'
alias gs='git status'
alias gm='git commit -m'
alias ga='git add .'
alias gr='git restore'
alias gp='git push'
alias gl='git log'
alias gf='git diff'
alias yl='yt-dlp -F'
alias y='yt-dlp'
alias ya='yt-dlp -f 140'
alias yb='yt-dlp -f "bestvideo[ext=mp4]+bestaudio[ext=m4a]/best[ext=mp4]/best" --merge-output-format mp4'
alias yt='yt-dlp --skip-download --write-thumbnail'

function parse_git_branch() {
	BRANCH=\$(git branch 2>/dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/\1/')
	if [ -n "\$BRANCH" ]; then
		STAT=\$(parse_git_dirty)
		echo "[\${BRANCH}\${STAT}]"
	fi
}

function parse_git_dirty() {
	status=\$(git status 2>&1)
	bits=''
	echo "\$status" | grep -q "renamed:" && bits=">\$bits"
	echo "\$status" | grep -q "Your branch is ahead of" && bits="*\$bits"
	echo "\$status" | grep -q "new file:" && bits="+\$bits"
	echo "\$status" | grep -q "Untracked files" && bits="?\$bits"
	echo "\$status" | grep -q "deleted:" && bits="x\$bits"
	echo "\$status" | grep -q "modified:" && bits="!\$bits"
	[ -n "\$bits" ] && echo " \$bits"
}

export PS1="[\[\e[36m\]\h \w\[\e[m\]\[\e[35m\]\$(parse_git_branch)\[\e[m\]] "
EOF

cd /tmp/
git clone --depth=1 https://github.com/garabik/grc.git && cd grc/
sudo ./install.sh && sudo cp /etc/profile.d/grc.sh /etc/
cd

cat > ~/.vimrc <<EOF
syntax on
set nu
set rnu
set noexpandtab
set tabstop=4
set softtabstop=4
set shiftwidth=4
set incsearch
set hlsearch
set nohlsearch
set cursorline
set scrolloff=10
set mouse=a
set encoding=utf-8
set t_Co=256
set timeoutlen=50
set signcolumn=yes
set lazyredraw
set ignorecase
set cb=unnamedplus
set autoindent
set ai
set colorcolumn=80
set updatetime=300
set nobackup
set nowritebackup
set noswapfile
set undofile
set undodir=/tmp
set smartcase
set list
set listchars=nbsp:¬,tab:»·,trail:·,extends:>

hi CursorLine term=bold cterm=bold ctermbg=238
hi CursorLineNr term=none cterm=none ctermbg=238 ctermfg=1
hi CursorColumn ctermbg=238 ctermfg=230
hi SignColumn ctermbg=none cterm=bold term=bold
hi Search ctermfg=0 ctermbg=9 cterm=underline term=underline
hi VertSplit ctermbg=0 ctermfg=0
hi Visual ctermbg=14 ctermfg=232

vnoremap <C-c> "+y          " Copy to clipboard in visual mode
map <C-v> "+v               " Paste from clipboard

let mapleader ="!"
nnoremap <C-n> :tabnew<Space>
nnoremap <C-k> :vertical resize -2<CR>
nnoremap <C-l> :vertical resize +2<CR>
nnoremap <TAB> :bnext<CR>
nnoremap <C-f> :Files<CR>
nnoremap <C-s> :S<CR>
nnoremap <C-d> :Tags<CR>
nnoremap <C-v> :vs<CR>
nnoremap <C-c> :ba<CR>
nnoremap <C-t> :bel vert term<CR>
nnoremap <C-p> :MarkdownPreview<CR>
nnoremap <C-x> :NERDTreeToggle<CR>
nnoremap <C-g> :0Gclog<CR>
nnoremap J :bprev<CR>
nnoremap M :bnext<CR>
nnoremap L <C-u>
nnoremap K <C-d>
nnoremap <silent> <C-o> :let _s=@/<Bar>:%s/\s\+$//e<Bar>:let @/=_s<Bar>:nohl<CR>
nnoremap <C-o> :G<CR>
nnoremap <C-d> :bd<CR>

noremap m l
noremap j h
noremap k j
noremap l k

if !filereadable(system('echo -n "${XDG_CONFIG_HOME:-$HOME/.vim}/autoload/plug.vim"'))
	echo "Downloading junegunn/vim-plug to manage plugins..."
	silent !mkdir -p ${XDG_CONFIG_HOME:-$HOME/.vim}/autoload/
	silent !curl "https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim" > ${XDG_CONFIG_HOME:-$HOME/.vim}/autoload/plug.vim
	autocmd VimEnter * PlugInstall
endif

call plug#begin(system('echo -n "${XDG_CONFIG_HOME:-$HOME/.vim}/plugged"'))
Plug 'junegunn/fzf'
Plug 'junegunn/fzf.vim'
Plug 'dense-analysis/ale'
Plug 'jiangmiao/auto-pairs'
Plug 'junegunn/vim-easy-align'
Plug 'tpope/vim-surround'
Plug 'plasticboy/vim-markdown'
call plug#end()

autocmd FileType gitdiff noremap m l
autocmd FileType gitdiff noremap j h
autocmd FileType gitdiff noremap k j
autocmd FileType gitdiff noremap l k

let g:fzf_preview_window = ['right:50%', 'ctrl-/']

command! S call fzf#vim#grep(
	  \   'rg --vimgrep --hidden --smart-case '.shellescape(<q-args>),
	  \   1,
	  \   fzf#vim#with_preview(),
	  \   0)
EOF

cat > ~/.tmux.conf <<EOF
bind r source-file ~/.tmux.conf

set -g mouse on
set -s escape-time 0

set -g default-terminal "screen-256color"
set -g status-bg gray
set -g status-fg blue

bind k select-pane -L
bind l select-pane -R
bind j select-pane -D
bind m select-pane -U

set -g status-justify left
set -g status-bg default
set -g status-fg colour12
set -g status-interval 2
set -g status-position bottom
set -g status-bg colour234
set -g status-fg colour137
set -g status-left ''
set -g status-right '#[fg=colour233,bg=colour241,bold] %d/%m #[fg=colour233,bg=colour245,bold] %H:%M:%S '
set -g status-right-length 50
set -g status-left-length 20

setw -g window-status-format " #F#I:#W#F "
setw -g window-status-current-format " #F#I:#W#F "
setw -g window-status-format "#[fg=magenta]#[bg=black] #I #[bg=cyan]#[fg=colour8] #W "
setw -g window-status-current-format "#[bg=brightmagenta]#[fg=colour8] #I #[fg=colour8]#[bg=colour14] #W "
setw -g window-status-current-format ' #I#[fg=colour250]:#[fg=colour255]#W#[fg=colour50]#F '
setw -g window-status-format ' #I#[fg=colour237]:#[fg=colour250]#W#[fg=colour244]#F '
EOF

echo "DNS Setup"

sudo tee /etc/resolv.conf > /dev/null <<EOF
	nameserver 1.1.1.1
	nameserver 8.8.8.8
	nameserver 9.9.9.9
	nameserver 208.67.222.222
EOF

source ~/.bashrc

echo
echo "Setup complete for $HOSTNAME. You can reboot now, or the system will automatically reboot in 30 seconds."
sleep 30
sudo reboot
