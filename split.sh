#!/bin/bash

input_file="Thunderbird.log"
split_dir="split_files"

pwdpath=$(pwd) 
output_dir="$pwdpath/split_files"
# Step 1: Split the large file into smaller chunks
mkdir -p "$split_dir"
split -l 16777216 "$input_file" "$output_dir/part_"

# Clean up
# rm -r "$split_dir"