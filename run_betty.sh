#!/bin/bash

# Run Betty checks on all .c files in the current directory
for file in *.c; do
    betty "$file"
done

echo "betty check completed."