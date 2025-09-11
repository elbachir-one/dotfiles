#!/bin/bash

# This scipte allow to boot external usb/drive using qemu legacy/uefi
# Usage:
#   ./bootx.sh -l <device>    # For legacy BIOS mode
#   ./bootx.sh -u <device>    # For UEFI mode using OVMF
#
# Example:
#   ./bootx.sh -l sda         # Launch QEMU in legacy mode on /dev/sda
#   ./bootx.sh -u sdb         # Launch QEMU in UEFI mode on /dev/sdb

# Check if exactly two arguments are provided.
if [ "$#" -ne 2 ]; then
	echo "Usage: $0 [-l|-u] <device>"
	exit 1
fi

# Extract options and device from command-line arguments.
MODE="$1"
DEVICE_ARG="$2"

# Prepend /dev/ if it's not already part of the device.
if [[ "$DEVICE_ARG" != /dev/* ]]; then
	DEVICE="/dev/$DEVICE_ARG"
else
	DEVICE="$DEVICE_ARG"
fi

# Optional: Check if the provided device exists.
if [ ! -e "$DEVICE" ]; then
	echo "Error: Device '$DEVICE' does not exist."
	exit 1
fi

# Decide on the QEMU command based on the mode.
case "$MODE" in
	-l)
		# Legacy BIOS mode
		CMD="sudo qemu-system-x86_64 -m 2G $DEVICE"
		;;
	-u)
		# UEFI mode: using OVMF firmware
		CMD="sudo qemu-system-x86_64 --bios /usr/share/edk2-ovmf/x64/OVMF.fd -m 2G $DEVICE"
		;;
	*)
		echo "Invalid mode. Use -l for legacy or -u for UEFI."
		exit 1
		;;
esac

# Execute the QEMU command.
echo "Running: $CMD"
$CMD
