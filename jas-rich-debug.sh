#!/bin/bash

# Copyright (c) 2023-2025 Alvin Cheng <eventide1029@gmail.com>

# Where debugging is required for the Jas library, these operations
# must be performed externally through an application dependant on
# the library.

# To allow for quicker debugging, the Jas library's output can be 
# symlinked to the application's dependency directory through this 
# script. This allows for the rapid development and the instant
# changes of the library to take effect without tedious copying
# and pasting of files.

# Usage: ./jas-rich-debug.sh <path-to-application-dependency-directory>

if [ "$#" -ne 1 ]; then
    echo "usage: $0 <path-to-application-dependency-directory>"
    echo "hint: see https://github.com/cheng-alvin/jas for more info"
    
    exit 1
fi

JAS_LIB_DIR=$1
APP_DEP_DIR=$(pwd)/libs/libjas

ln -sf "$JAS_LIB_DIR" "$APP_DEP_DIR"

exit 0