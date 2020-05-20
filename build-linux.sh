#!/bin/bash

# ==============================================================================
# Opens a docker hosted terminal with all the required libraries to
# build for Linux and runs the make command
#
# Executable can be found in the bin/ directory
# ==============================================================================

docker run -v $(pwd):/mnt/BuildProject --rm didstopia/sdl2 \
    sh -c 'cd /mnt/BuildProject && make -j4'
