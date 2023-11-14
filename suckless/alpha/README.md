# Alpha - A Suckless Terminal Fork

## Overview

Alpha is a fork of the suckless terminal (st). This terminal emulator is designed to be lightweight, efficient, and, as the name suggests, "suck less." The customization includes various patches, a bright color scheme, and default keybindings.

## Features

### Patches Included

- **Alpha:** Custom patch for enhanced functionality.
- **Clipboard:** Clipboard support for easy copy-pasting.
- **Boxdraw:** Improved box drawing characters.
- **Scroll:** Enables scrolling functionality.
- **Mouse Scroll:** Mouse scrolling support.
- **Anysize:** Allows resizing the terminal to any size.
- **Copy URL:** Easily copy URLs with a keybinding.
- **Font 2:** Additional font options for customization.

### Colors

Alpha comes with a bright and visually appealing color scheme.

### Keybindings

The keybindings are set to defaults for a seamless out-of-the-box experience.

## Installation

To install Alpha, follow these simple steps:

```bash
git clone https://github.com/elbachir-one/alpha
cd alpha/
make
sudo make clean install
```
## Fonts

If you wish to use the same fonts, clone my dotfiles and copy the content of the fonts directory to your preferred fonts directory.

## Additional Information

## Dependencies

Before installing Alpha, make sure you have the following dependencies installed on your system:

- **make**: The GNU build tool.
- **gcc**: The GNU Compiler Collection.

### Libraries

- **libX11**: X Window System protocol client library.
- **libXft**: X FreeType interface library.

### Notes

The actual dependencies can vary based on your system configuration and the specific features you've enabled in your build. Refer to the [config.h](./config.h) file for detailed information.

alpha out.
