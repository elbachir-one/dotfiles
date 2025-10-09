#!/usr/bin/env bash

#
# Opening websites in Chromium app mode using dmenu with icons.
#

options=(
"Knowledge HUB|https://elbachir.org"
"DWM Key Bindings|https://elbachir.org/src/dwm_keybindings.html"
"Void Linux|https://voidlinux.org"
"ChatGPT AI|https://chatgpt.com"
"Discord|https://discord.com/channels/@me"
"Chess|https://chess.com"
"YouTube|https://youtube.com"
"Gmail|https://mail.google.com"
"Reddit|https://reddit.com"
"Wikipedia|https://wikipedia.org"
"Monkey Type|https://monkeytype.com"
"Grok AI|https://grok.com"
"GitHub|https://github.com"
)

menu=$(printf "%s\n" "${options[@]}" | cut -d'|' -f1)
choice=$(echo "$menu" | dmenu -i -p "Open sites as apps in Chromium:")
[[ -z "$choice" ]] && exit
url=$(printf "%s\n" "${options[@]}" | grep "^$choice|" | cut -d'|' -f2)
chromium --app="$url"
