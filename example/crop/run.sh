#!/bin/sh

../../build/debug/apps/auto-pack-my-sprites images/*.png --size=550x250 \
                                            --format=spine --output=spine
../../build/debug/apps/auto-pack-my-sprites images/*.png --size=550x250 \
                                            --format=plist --output=plist
../../build/debug/apps/auto-pack-my-sprites images/*.png --size=550x250 \
                                            --format=spritepos --output=spritepos
../../build/debug/apps/auto-pack-my-sprites images/*.png --size=550x250 \
                                            --format=css --output=css
