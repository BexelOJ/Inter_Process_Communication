#!/bin/bash

//-------------------------------------------
// Rename ipc_2020610* → ipc_202610*
//-------------------------------------------

for item in ipc_2020610*; do

    # Skip if nothing matches
    [ -e "$item" ] || continue

    new_name="${item/ipc_2020610/ipc_202610}"

    echo "Renaming:"
    echo "  $item"
    echo "    → $new_name"

    mv -- "$item" "$new_name"

done

echo
echo "Rename completed."