#!/bin/bash

# Ducky Snake - Run Script
# This script compiles and runs the ducky-snake game

echo "🦆 Compiling Ducky Snake..."
echo "================================"

# Compile the game
g++ -o ducky-snake ducky.cpp

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "✅ Compilation successful!"
    echo ""
    echo "🎮 Starting Ducky Snake..."
    echo "================================"
    echo ""
    
    # Run the game
    ./ducky-snake
else
    echo "❌ Compilation failed!"
    echo "Please check for errors above."
    exit 1
fi
