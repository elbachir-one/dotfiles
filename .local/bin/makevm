#!/usr/bin/env bash

set -euo pipefail

# ======================================================================== #
# ====== VM Creator / Launcher with Serial Console + NAT (Defaults) ====== #
# ======================================================================== #


# =============================================================================
# Copyright (c) 2025 El Bachir (ALPHA)  https://elbachir.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# =============================================================================

# Defaults
DEFAULT_RAM=2048
DEFAULT_DISK="10G"
DEFAULT_CPUS=2
VM_DIR_BASE="$HOME/VMs"
OVMF_CODE="/usr/share/OVMF/x64/OVMF_CODE.fd"

# Help
show_usage() {
	cat <<EOF
Usage: makevm [OPTIONS]

Options:
  -l             List all installed VMs
  -s VM_NAME     Start a specific VM
  -s             Prompt you to select from the list of installed VMs
  -c             Create / install a new VM
  -h             Show this help message
EOF
exit 0
}

# List Installed VM's
list_vms() {
	if [ ! -d "$VM_DIR_BASE" ]; then
		echo "No VMs found in $VM_DIR_BASE."
		return
	fi

	echo "Installed VMs in $VM_DIR_BASE:"
	shopt -s nullglob
	for vmdir in "$VM_DIR_BASE"/*/; do
		vmname=$(basename "$vmdir")
		disk="$vmdir/$vmname.qcow2"
		if [ -f "$disk" ]; then
			echo "  - $vmname"
		fi
	done
	shopt -u nullglob
}

# Start VM
boot_vm() {
	local vmname="$1"
	local vmdir="$VM_DIR_BASE/$vmname"
	local disk="$vmdir/${vmname}.qcow2"
	local ovmf_vars="$vmdir/OVMF_VARS.fd"

	if [ ! -f "$disk" ]; then
		echo "VM '$vmname' not found in $vmdir!"
		exit 1
	fi

	[ ! -f "$ovmf_vars" ] && cp -n /usr/share/OVMF/x64/OVMF_VARS.fd "$ovmf_vars"

	echo "Starting VM '$vmname' with ${DEFAULT_RAM} MB RAM and ${DEFAULT_CPUS} CPU cores..."
	qemu-system-x86_64 \
		-enable-kvm \
		-m "$DEFAULT_RAM" \
		-smp cores="$DEFAULT_CPUS",threads=1,sockets=1 \
		-cpu host \
		-drive if=pflash,format=raw,readonly=on,file="$OVMF_CODE" \
		-drive if=pflash,format=raw,file="$ovmf_vars" \
		-drive file="$disk",if=virtio \
		-boot c \
		-netdev user,id=net0,hostfwd=tcp::2222-:22 \
		-device virtio-net-pci,netdev=net0 \
		-nographic \
		-serial mon:stdio \
		-name "$vmname"
	}

# Create / Install
create_vm() {
	echo "=== Creating a new VM ==="
	read -rp "VM Name: " vmname
	[ -z "$vmname" ] && { echo "VM name cannot be empty!"; exit 1; }

	local vmdir="$VM_DIR_BASE/$vmname"
	local disk="$vmdir/${vmname}.qcow2"
	local ovmf_vars="$vmdir/OVMF_VARS.fd"

	if [ -d "$vmdir" ]; then
		echo "VM '$vmname' already exists!"
		while true; do
			read -rp "VM Already Exists. Overwrite it? [y/N] " answer
			case "$answer" in
				[Yy]* )
					echo "Overwriting existing VM..."
					rm -rf "$vmdir"
					mkdir -p "$vmdir"
					break
					;;
				[Nn]*|"" )
					local suffix=2
					while [ -d "$VM_DIR_BASE/${vmname}${suffix}" ]; do
						((suffix++))
					done
					vmname="${vmname}${suffix}"
					vmdir="$VM_DIR_BASE/$vmname"
					disk="$vmdir/${vmname}.qcow2"
					ovmf_vars="$vmdir/OVMF_VARS.fd"
					echo "Creating VM with new name: $vmname"
					mkdir -p "$vmdir"
					break
					;;
				* ) echo "Please answer y or n." ;;
			esac
		done
	else
		mkdir -p "$vmdir"
	fi

	local iso_dir="$HOME/Downloads/ISOs"
	echo "Available ISOs in $iso_dir:"
	shopt -s nullglob
	local isos=("$iso_dir"/*.iso)
	shopt -u nullglob

	if [ ${#isos[@]} -eq 0 ]; then
		echo "No ISO files found in $iso_dir!"
		exit 1
	fi

	local iso_names=()
	for iso in "${isos[@]}"; do
		iso_names+=("$(basename "$iso")")
	done

	echo
	select choice in "${iso_names[@]}"; do
		if [ -n "$choice" ]; then
			iso_path="$iso_dir/$choice"
			break
		else
			echo "Invalid choice. Try again."
		fi
	done

	read -rp "Disk size [${DEFAULT_DISK}]: " disksize
	read -rp "RAM in MB [${DEFAULT_RAM}]: " ram
	read -rp "CPU cores [${DEFAULT_CPUS}]: " cpus

	disksize=${disksize:-$DEFAULT_DISK}
	ram=${ram:-$DEFAULT_RAM}
	cpus=${cpus:-$DEFAULT_CPUS}

	[[ "$ram" =~ ^[0-9]+$ ]] || ram=$DEFAULT_RAM
	[[ "$cpus" =~ ^[0-9]+$ ]] || cpus=$DEFAULT_CPUS

	[ ! -f "$disk" ] && qemu-img create -f qcow2 "$disk" "$disksize"
	[ ! -f "$ovmf_vars" ] && cp -n /usr/share/OVMF/x64/OVMF_VARS.fd "$ovmf_vars"

	echo "Starting VM '$vmname' with installation ISO..."
	qemu-system-x86_64 \
		-enable-kvm \
		-m "$ram" \
		-smp cores="$cpus",threads=1,sockets=1 \
		-cpu host \
		-drive if=pflash,format=raw,readonly=on,file="$OVMF_CODE" \
		-drive if=pflash,format=raw,file="$ovmf_vars" \
		-drive file="$disk",if=virtio \
		-cdrom "$iso_path" \
		-boot d \
		-netdev user,id=net0,hostfwd=tcp::2222-:22 \
		-device virtio-net-pci,netdev=net0 \
		-nodefaults \
		-nographic \
		-serial mon:stdio \
		-name "$vmname"
	}

# Main
if [ $# -eq 0 ]; then
	show_usage
fi

# Help Flags
case "$1" in
	-l)
		list_vms
		;;
	-c)
		create_vm
		;;
	-h)
		show_usage
		;;
	-s)
		if [ -n "${2-}" ]; then
			boot_vm "$2"
		else
			shopt -s nullglob
			vms=("$VM_DIR_BASE"/*/)
			shopt -u nullglob
			if [ ${#vms[@]} -eq 0 ]; then
				echo "No VMs found."
				exit 1
			fi

			vmnames=()
			for vmdir in "${vms[@]}"; do
				vmnames+=("$(basename "$vmdir")")
			done

			echo "Select a VM to start:"
			select vmname in "${vmnames[@]}"; do
				if [ -n "$vmname" ]; then
					boot_vm "$vmname"
					break
				fi
			done
		fi
		;;
	*)
		show_usage
		;;
esac
