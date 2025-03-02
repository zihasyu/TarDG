#!/bin/bash

# 获取当前目录下的所有文件夹
folders=$(ls -d */)

# 遍历每个文件夹
for folder in $folders; do
    # 去掉末尾的斜杠
    folder_name=${folder%/}
    
    # 创建 tar 包（不压缩）
    tar -cf "${folder_name}.tar" "$folder_name"
    
    echo "已打包: ${folder_name}.tar"
done