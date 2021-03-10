#!/bin/bash

# ==============================================================================
# Opens a docker hosted terminal with all the required libraries to
# build for Windows 64-bit and runs the make command
#
# Executable can be found in the bin/ directory
# ==============================================================================

docker run -v /$(pwd):/mnt/BuildProject --rm jtymburski/sdl2-cross-make \
    sh -c 'cd /mnt/BuildProject && make -j4 windows'
