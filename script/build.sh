#! /bin/bash

ROOT="$(dirname "$0")"

echo "Relocating script to '$ROOT'..."
cd "$ROOT"


echo "Building the compiler..."
cmake -S .. -B ../bin
cd ../bin

# Agregar 'src/utils/wrapper-functions.c' a la lista de source files
# Compile your program
make PROGRAM_SOURCES="../src/frontend/lexical-analysis/flex-actions.c ../src/utils/wrapper-functions.c"

echo "All done."
