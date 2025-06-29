#!/bin/bash

# Basic QSPI Flash Write Script (with size check)
# Usage: ./simple_qspi_write.sh /dev/mtdX file_to_write

if [ $# -ne 2 ]; then
    echo "Usage: $0 <mtd_device> <file_to_write>"
    exit 1
fi

mtd_device=$1
file_to_write=$2

# 檢查檔案是否存在
if [ ! -f "$file_to_write" ]; then
    echo "Error: File not found: $file_to_write"
    exit 1
fi

# 取得檔案大小（bytes）
file_size=$(stat -c %s "$file_to_write")

# 取得 Flash Partition 大小（bytes）
partition_size=$(cat /proc/mtd | grep "$(basename $mtd_device)" | awk '{print "0x"$2}')

if [ -z "$partition_size" ]; then
    echo "Error: Failed to get partition size for $mtd_device"
    exit 1
fi

partition_size=$((partition_size))  # 轉換成十進位

# 檢查檔案是否超過 Partition 大小
if [ "$file_size" -gt "$partition_size" ]; then
    echo "Error: File size ($file_size bytes) is larger than partition size ($partition_size bytes)"
    exit 1
fi

# 擦除 Flash Partition
flash_erase "$mtd_device" 0 0

# 寫入檔案到 Flash
mtd_debug write "$mtd_device" 0 "$file_size" "$file_to_write"

echo "Flash write completed."
