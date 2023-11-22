#!/bin/bash

grep -v "#" $HOME/dotfiles/emoji_list | dmenu | awk '{print $1}' | tr -d '\n' | xclip -selection clipboard

pgrep -x dunst >/dev/null && notify-send "$(xclip -o -selection clipboard) Copied!"
