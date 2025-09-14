#!/usr/bin/env bash

# Taking screenshot and copy to clipboard, save, or upload to imgur with scrot and slop.
# Options: Fullscreen / Section / Area
# Post-actions: Copy to clipboard / Save / Upload to Imgur

IMG_PATH="$HOME/Images/Shots/"
TIME=2000

mkdir -p "$IMG_PATH"

prog="1. Fullscreen
2. Section (click on window)
3. Area (select area with mouse)"
cmd=$(echo -e "$prog" | dmenu -p 'ScreenShot')

file="$(date '+ScreenShot-%Y-%m-%d-@%H-%M-%S.png')"
cd "$IMG_PATH" || exit 1

case ${cmd%% *} in
	1.*)
		scrot -d 1 -q 100 "$file" && \
		notify-send -u low -t "$TIME" 'scrot' 'Full Screen Shot saved'
		;;
	2.*)
		scrot -d 1 -s -q 100 "$file" && \
		notify-send -u low -t "$TIME" 'scrot' 'Section Screen Shot saved'
		;;
	3.*)
		geometry=$(slop -f "%x,%y,%w,%h")
		if [ -z "$geometry" ]; then
			notify-send -u critical -t "$TIME" 'Error' 'Area selection canceled'
			exit 1
		fi
		scrot -a "$geometry" -q 100 "$file" && \
		notify-send -u low -t "$TIME" 'scrot' 'Area Screen Shot saved'
		;;
	*)
		notify-send -u critical -t "$TIME" 'Error' 'Invalid selection'
		exit 1
		;;
esac

action=$(echo -e "Copy to clipboard\nSave to Dist directory\nUpload to Imgur" | dmenu -p 'Screenshot Action')

if [[ "$action" == "Copy to clipboard" ]]; then
	xclip -selection clipboard -t image/png -i "$IMG_PATH$file"
	rm "$IMG_PATH$file"
	notify-send -u low -t "$TIME" 'Clipboard' 'Screenshot copied to clipboard and removed the file'

elif [[ "$action" == "Save to Dist directory" ]]; then
	notify-send -u low -t "$TIME" 'Saved' "Screenshot saved in $IMG_PATH"

elif [[ "$action" == "Upload to Imgur" ]]; then
	link=$(imgur "$IMG_PATH$file" | tail -n1)

	if [[ -n "$link" ]]; then
		printf "%s" "$link" | xclip -selection clipboard
		rm "$IMG_PATH$file"
		notify-send -u low -t "$TIME" 'Imgur' "Uploaded & link copied:\n$link"
	else
		notify-send -u critical -t "$TIME" 'Imgur Error' 'Failed to upload screenshot'
	fi
else
	notify-send -u critical -t "$TIME" 'Action Error' 'No valid post-screenshot action selected'
fi
