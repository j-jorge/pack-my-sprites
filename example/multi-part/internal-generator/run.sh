#!/bin/sh

auto-pack-my-sprites --css --plist --spritepos --spine \
                     --output multi --strip-paths \
                     --margin 1 --size 256x256 \
                     images/b*.png \
                     --bleeding \
                     images/[^b]*.png
