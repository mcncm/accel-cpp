# Accel
A side project to learn about graphics and SDL, (re-)learn a little C++, and
experiment with the fantastic new [Pinephone](https://www.pine64.org/pinephone/).

## SDL code
I didn't know any SDL before starting this, so I started from a core drawn from
TwinklebearDev's excellent [tutorials](https://www.willusher.io/pages/sdl2/).
This is particularly apparent in the earliest commits, and some helper functions
from these tutorials are still present.

## Pinephone setup
I'm running [SwayWM](https://swaywm.org), a window manager for Wayland, under
[PostmarketOS](https://postmarketos.org). This Alpine Linux-based mobile distro
uses musl libc instead of glibc, which has made the cross-compilation journey a
little more interesting.
