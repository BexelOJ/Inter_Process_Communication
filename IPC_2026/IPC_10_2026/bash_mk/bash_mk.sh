#!/bin/bash

#!/bin/bash

#-------------------------------------------
# Configuration
#-------------------------------------------

BASE_DIR="/home/exin/Workspace/IPC/IPC_2026/IPC_10_2026"

cd "$BASE_DIR" || exit 1

#-------------------------------------------
# Create directory for each file
# and move the file into it
#-------------------------------------------

for file in *; do

    # Skip directories
    if [ -d "$file" ]; then
        continue
    fi

    # Get filename without extension
    filename="${file%.*}"

    echo "-------------------------------------------"
    echo "File : $file"
    echo "Dir  : $filename"

    # Create directory
    mkdir -p "$filename"

    # Move file
    mv "$file" "$filename/"

done

echo "-------------------------------------------"
echo "Done."