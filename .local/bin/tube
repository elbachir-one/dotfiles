#!/usr/bin/env bash

# Download 1080p YouTube videos (no 2K/4K), log failures to tube.log without duplicates

LOGFILE="tube.log"

if [ -z "$1" ]; then
	echo "Usage: $0 <URL or file_with_urls>"
	exit 1
fi

log_error() {
	local VIDEO_URL="$1"
	if ! grep -Fxq "$VIDEO_URL" "$LOGFILE" 2>/dev/null; then
		echo "$VIDEO_URL" >> "$LOGFILE"
		echo "  ↳ Logged: $VIDEO_URL"
	fi
}

process_video() {
	local VIDEO_URL="$1"

	echo "Processing: $VIDEO_URL"

	if echo "$VIDEO_URL" | grep -qE "youtube\.com|youtu\.be"; then
		if ! yt-dlp -f "137+bestaudio[ext=m4a]/299+bestaudio[ext=m4a]/bestvideo+bestaudio" \
			-o "%(title)s.%(ext)s" --merge-output-format mp4 "$VIDEO_URL" 2>yt_error.log; then
					log_error "$VIDEO_URL"
					echo "  ↳ Error detected — see $LOGFILE"
					grep -E "ERROR|ERROR:" yt_error.log | sed 's/^/  /'
		fi
	else
		if ! yt-dlp "$VIDEO_URL" 2>yt_error.log; then
			log_error "$VIDEO_URL"
			echo "  ↳ Error detected — see $LOGFILE"
			grep -E "ERROR|ERROR:" yt_error.log | sed 's/^/  /'
		fi
	fi

	rm -f yt_error.log
}

if [[ -f "$1" ]]; then
	while IFS= read -r VIDEO_URL; do
		[[ -n "$VIDEO_URL" ]] && process_video "$VIDEO_URL"
	done < "$1"
else
	process_video "$1"
fi
