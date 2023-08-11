#xrandr --output LVDS1 --primary --mode 1366x768 --pos 0x312 --rotate normal --output HDMI1 --mode 1920x1080 --pos 1366x0 --rotate normal &
#mpd &
#picom &
#clipmenud &
#dunst &
#slstatus &
#./.fehbg &
while true; do
	dwm >/dev/null 2>&1
done
#
exec dwm
