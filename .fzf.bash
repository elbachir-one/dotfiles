# Setup fzf
# ---------
if [[ ! "$PATH" == */home/elbachir/.fzf/bin* ]]; then
  PATH="${PATH:+${PATH}:}/home/elbachir/.fzf/bin"
fi

eval "$(fzf --bash)"
