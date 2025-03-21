#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <directory>"
  exit 1
fi

find "$1" -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +
