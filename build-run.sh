#!/bin/bash
mkdir -p bin
cd src

for file in *; do
    if [ -f "$file" ]; then
        files+="$file "
    fi
done

g++ $files -o ../bin/final-project

cd ../bin/
./final-project