#!/bin/bash

# Storing screenshots temporarily
SCREENSHOT_DIR="/tmp/shots"

SERVER_USER="sh"
SERVER_IP="192.168.1.15"
SERVER_DIR="/media/Shots"

mkdir -p "$SCREENSHOT_DIR"

upload_screenshots() {

	# Upload all screenshots
	scp "$SCREENSHOT_DIR"/*.png "$SERVER_USER@$SERVER_IP:$SERVER_DIR"

	# Check if the upload was successful
	if [ $? -eq 0 ]; then
		rm "$SCREENSHOT_DIR"/*.png
	fi
}

# Start taking screenshots every 10 seconds
(
while true; do

	FILENAME=$(date '+%Y-%m-%d_%H-%M-%S').png

		# Take the screenshot
		scrot "$SCREENSHOT_DIR/$FILENAME"

		# Check if number of screenshots exceeds 4000
		FILE_COUNT=$(ls -1q "$SCREENSHOT_DIR"/*.png | wc -l)

		if [ "$FILE_COUNT" -gt 4000 ]; then
			upload_screenshots
		fi

		sleep 10
	done
	) &
