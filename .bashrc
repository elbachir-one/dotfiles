# .bashrc

[[ $- != *i* ]] && return

[ -f "$HOME"/.bash_aliases ] && . "$HOME"/.bash_aliases

GRC_ALIASES=true
[[ -s "/etc/profile.d/grc.sh" ]] && source /etc/grc.sh

echo -e -n "\x1b[\x35 q"

bind 'set completion-ignore-case on'

complete -cf sudo

HISTSIZE=HISTFILESIZE=

# Exports
export TERMINAL='sakura'
export BROWSER='chromium'
export EDITOR='vim'
export PATH="$HOME/.local/bin:$PATH"
export GOPATH="$HOME/.local/go"
export IMGVIEWER='feh'
export MAKEFLAGS="-j3"
export QT_QPA_PLATFORMTHEME="qt5ct"

# FZF
[ -f ~/.fzf.bash ] && source "$HOME"/.fzf.bash

export FZF_DEFAULT_OPTS="
	--color=fg:#908caa,bg:#191724,hl:#ebbcba
	--color=fg+:#e0def4,bg+:#26233a,hl+:#ebbcba
	--color=border:#403d52,header:#31748f,gutter:#191724
	--color=spinner:#f6c177,info:#9ccfd8,separator:#403d52
	--color=pointer:#c4a7e7,marker:#eb6f92,prompt:#908caa"

# Prompt stuff
function parse_git_branch() {
	BRANCH=$(git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/\1/')
	if [ ! "${BRANCH}" == "" ]
	then
		STAT=$(parse_git_dirty)
		echo "[${BRANCH}${STAT}]"
	else
		echo ""
	fi
}

function parse_git_dirty {
	status=$(git status 2>&1 | tee)
	dirty=$(echo -n "${status}" 2> /dev/null | grep "modified:" &> /dev/null; echo "$?")
	untracked=$(echo -n "${status}" 2> /dev/null | grep "Untracked files" &> /dev/null; echo "$?")
	ahead=$(echo -n "${status}" 2> /dev/null | grep "Your branch is ahead of" &> /dev/null; echo "$?")
	newfile=$(echo -n "${status}" 2> /dev/null | grep "new file:" &> /dev/null; echo "$?")
	renamed=$(echo -n "${status}" 2> /dev/null | grep "renamed:" &> /dev/null; echo "$?")
	deleted=$(echo -n "${status}" 2> /dev/null | grep "deleted:" &> /dev/null; echo "$?")
	bits=''
	if [ "${renamed}" == "0" ]; then
		bits=">${bits}"
	fi
	if [ "${ahead}" == "0" ]; then
		bits="*${bits}"
	fi
	if [ "${newfile}" == "0" ]; then
		bits="+${bits}"
	fi
	if [ "${untracked}" == "0" ]; then
		bits="?${bits}"
	fi
	if [ "${deleted}" == "0" ]; then
		bits="x${bits}"
	fi
	if [ "${dirty}" == "0" ]; then
		bits="!${bits}"
	fi
	if [ ! "${bits}" == "" ]; then
		echo " ${bits}"
	else
		echo ""
	fi
}

# Prompt
export PS1="[\[\e[36m\]\h\w\[\e[m\]\[\e[35m\]\`parse_git_branch\`\[\e[m\]] "

# Less Config
export LESS='-R -M -I'
export LESSPROMPT='%{?f%f:}  %{G[Line: %l/%L]}%{M[Col: %c]} (%p%%)'
export LESS_TERMCAP_md=$'\e[01;32m'    # bold green for metadata
export LESS_TERMCAP_me=$'\e[0m'        # reset
export LESS_TERMCAP_us=$'\e[04;35m'    # underline magenta
export LESS_TERMCAP_ue=$'\e[0m'        # reset
export LESS_TERMCAP_so=$'\e[01;44;33m' # standout mode (e.g., highlighted text)
export LESS_TERMCAP_se=$'\e[0m'        # reset

# Yarn
export PATH="$HOME/.yarn/bin:$HOME/.config/yarn/global/node_modules/.bin:$PATH"

gcop () {
	git log \
		--reverse \
		--color=always \
		--format="%C(cyan)%h %C(blue)%ar%C(auto)%d %C(yellow)%s%+b %C(black)%ae" "$@" |
	fzf -i -e +s \
		--reverse \
		--tiebreak=index \
		--no-multi \
		--ansi \
		--preview="echo {} | grep -o '[a-f0-9]\{7\}' | head -1 | xargs git show --color=always" \
		--header="enter: view, C-c: copy hash" \
		--bind "enter:execute:echo {} | grep -o '[a-f0-9]\{7\}' | head -1 | xargs git show | less -R" \
		--bind "ctrl-c:execute-silent:echo {} | grep -o '[a-f0-9]\{7\}' | head -1 | xclip -r -selection clipboard"
}

# Safe Remove
rm() {
	for arg in "$@"; do
		if [[ "$arg" == "*" || "$arg" == *\** || "$arg" == *\?* ]]; then
			echo "You are using a wildcard ($arg) with rm!"
			read -r -p "Are you sure you want to delete all matching files? [y/N]: " confirm
			if [[ "$confirm" != "y" && "$confirm" != "Y" ]]; then
				echo "Aborted."
				return 1
			fi
			break
		fi
	done

	command rm "$@"
}

# Safe Copy
cp_safe() {
	dest="${!#}"
	if [ ! -d "$dest" ]; then
		echo "Destination $dest is not a directory"
		return 1
	fi

	for src in "${@:1:$#-1}"; do
		if [ ! -e "$src" ]; then
			echo "Source $src does not exist, skipping"
			continue
		fi

		base=$(basename "$src")
		dst="$dest/$base"
		n=1

		while [ -e "$dst" ]; do
			name="${base%.*}"
			ext="${base##*.}"
			if [ "$name" = "$ext" ]; then ext=""; else ext=".$ext"; fi
			dst="$dest/${name}_$n$ext"
			((n++))
		done

		cp "$src" "$dst"
		echo "Copied $src → $dst"
	done
}

#figlet -c -f ANSI_Shadow.flf "Void/runit" -t | lolcat
