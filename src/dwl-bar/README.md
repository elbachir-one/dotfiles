# dwl-bar
dwm-like bar for dwl

I believe dwl-bar provides a more dwm-like experience out of the box than other bars like somebar.

## Dependencies
I'm not sure what the package names will be for your distrobution, so just make sure these are generally what you have.
 + make
 + pango
 + cairo
 + wayland
 + wayland-protocols

## Compile
Compile with `make`, install with `make install`, uninstall `make uninstall`.

## Configuration
Like most suckless-like software, configuration is done through `src/config.def.h` modify it to your heart's content. dwl-bar is compatible with [someblocks](https://sr.ht/~raphi/someblocks/) for status.

If you want dwl-bar to control dwl (via mouse gestures primarily) you may want to apply the [ipc](https://github.com/MadcowOG/dwl-bar/wiki/ipc) patch. 
**However, do note that you will also need to apply the [ipc](https://github.com/djpohly/dwl/wiki/ipc) patch for dwl.**

For more information about patching see the [hacking](https://suckless.org/hacking/) article from suckless and the [wiki](https://github.com/MadcowOG/dwl-bar/wiki) for available patches.

## Thanks
Thanks to [raphi](https://sr.ht/~raphi/) for [somebar](https://sr.ht/~raphi/somebar/) this project borrows from it. 
The ipc protocol is also just the ipc patch in [somebar's `contrib/`](https://git.sr.ht/~raphi/somebar/tree/master/item/contrib/ipc.patch).
Thanks to the swaybar contriutors and [Drew DeVault](https://github.com/ddevault) for [swaybar](https://github.com/swaywm/sway/tree/master/swaybar), I basically did a lot of this project with it open in browser.
