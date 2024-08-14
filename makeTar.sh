#!/bin/bash
input_dir="../DBset/output"
output_dir="../DBset/tar_files"
mkdir -p "$output_dir"
for dir in "$input_dir"/*; do
  if [ -d "$dir" ]; then
    dirname=$(basename "$dir")
    tar -cf "$output_dir/$dirname.tar" -C "$input_dir" "$dirname"
  fi
done
