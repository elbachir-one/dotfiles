#!/usr/bin/env bash

#
# This script automatically cleans and saves URLs copied to the clipboard into files.
#

LIST_FILE="$HOME/Documents/Bookmarks/bookmarks.txt"
TUBE_FILE="$HOME/Videos/list.txt"
LAST=""

touch "$LIST_FILE" "$TUBE_FILE"

while true; do
	CLIP=$(xclip -o -selection clipboard 2>/dev/null)

	[[ -z "$CLIP" ]] && sleep 1 && continue

	CLIP=$(echo "$CLIP" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')

	if ! [[ "$CLIP" =~ ^https?://[^[:space:]]+$ ]]; then
		sleep 1
		continue
	fi

	if [[ "$CLIP" =~ \.git$ ]] \
		|| { ! [[ "$CLIP" =~ twitch\.tv ]] && [[ "$CLIP" =~ /v?[0-9]+(\.[0-9]+)*$ ]]; } \
		|| [[ "$CLIP" =~ \.zip$ ]] \
		|| [[ "$CLIP" =~ \.gz$ ]] \
		|| [[ "$CLIP" =~ \.tar$ ]] \
		|| [[ "$CLIP" =~ \.iso$ ]] \
		; then
			sleep 1
			continue
	fi

	URL="$CLIP"
	[[ "$URL" == "$LAST" ]] && sleep 1 && continue

	if [[ "$URL" =~ youtube\.com|youtu\.be ]]; then
		CLEANED="${URL%%&*}"
		TARGET_FILE="$TUBE_FILE"
		MSG="YouTube URL Saved"
		if [[ "$CLEANED" =~ youtube\.com/shorts/ ]]; then
			IS_SHORTS=true
			IS_TWITCH=false
			IS_YT_LONG=false
		else
			IS_SHORTS=false
			IS_TWITCH=false
			IS_YT_LONG=true
		fi
	elif [[ "$URL" =~ twitch\.tv ]]; then
		CLEANED="${URL%%[\#\&\?]*}"
		TARGET_FILE="$TUBE_FILE"
		MSG="Twitch URL Saved"
		IS_SHORTS=false
		IS_TWITCH=true
		IS_YT_LONG=false
	else
		CLEANED="${URL%%[\#\&\?]*}"
		TARGET_FILE="$LIST_FILE"
		MSG="URL Saved"
		IS_SHORTS=false
		IS_TWITCH=false
		IS_YT_LONG=false
	fi

	if grep -Fxq "$CLEANED" "$TARGET_FILE"; then
		sleep 1
		continue
	fi

	if [[ "$TARGET_FILE" == "$LIST_FILE" ]]; then
		TODAY=$(date '+%Y-%m-%d')

		LAST_DATE_IN_FILE=$(tac "$LIST_FILE" | grep -m1 -E '^[0-9]{4}-[0-9]{2}-[0-9]{2}$')
		if [[ "$TODAY" != "$LAST_DATE_IN_FILE" ]]; then
			{
				echo ""
				echo "$TODAY"
			} >> "$LIST_FILE"
		fi

		echo "$CLEANED" >> "$LIST_FILE"

	elif [[ "$TARGET_FILE" == "$TUBE_FILE" ]]; then
		tmp=$(mktemp)

		if [[ "$IS_SHORTS" == true ]]; then
			{
				echo "$CLEANED"
				cat "$TUBE_FILE"
			} > "$tmp"

		elif [[ "$IS_YT_LONG" == true ]]; then
			{
				grep -E 'youtube\.com/shorts/' "$TUBE_FILE"
				echo "$CLEANED"
				grep -v -E 'youtube\.com/shorts/' "$TUBE_FILE"
			} > "$tmp"

		elif [[ "$IS_TWITCH" == true ]]; then
			{
				cat "$TUBE_FILE"
				echo "$CLEANED"
			} > "$tmp"
		fi

		mv "$tmp" "$TUBE_FILE"
	fi

	notify-send "$MSG" "$CLEANED"
	LAST="$URL"
	sleep 1
done
