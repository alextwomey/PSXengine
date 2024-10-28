#!/bin/bash

# Prompt for XML file (uncomment if you want to ask for filename input)
# read -p "Enter XML file to make: " file

echo "making ./cuesheet.xml"

# Run the program
./mkpsxiso ./cuesheet.xml

# Pause (optional)
read -p "Press Enter to continue..."
