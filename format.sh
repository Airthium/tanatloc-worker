#!/usr/bin/env bash

CLANG_FORMAT="clang-format -i"

# Define targets
# Exclude dxflib
TARGETS=$(find converters/ -regex ".*\.\(hpp\|cpp\|c\|h\)$" -not -path "converters/dxflib/*")

# Run clang-format
echo -e "$TARGETS";
$CLANG_FORMAT "$TARGETS";
