#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin || { echo "Failed to create bin directory"; exit 1; }

# Change to src directory
cd src || { echo "Failed to enter src directory"; exit 1; }

# Initialize files array
files=()

# Collect all .cpp files (modify this pattern if needed)
for file in *.cpp; do
    if [ -f "$file" ]; then
        files+=("$file")
    fi
done

# Check if any source files were found
if [ ${#files[@]} -eq 0 ]; then
    echo "No source files found in src directory!"
    exit 1
fi

echo "Compiling files: ${files[@]}"

# Compile the project
g++ -Wall -Wextra "${files[@]}" -o ../bin/final-project || {
    echo "Compilation failed!"
    exit 1
}

# Run the program
cd ../bin || { echo "Failed to enter bin directory"; exit 1; }

echo "Running program..."
echo "-------------------"
./final-project