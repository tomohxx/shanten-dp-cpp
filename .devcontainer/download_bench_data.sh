#!/bin/bash
URL="https://mahjong.ara.black/etc/shanten/problems.zip"
ZIP_PATH="benches/data/problems.zip"

echo "Downloading $URL..."

if curl -sSL -o "$ZIP_PATH" "$URL"; then
    echo "Download successful. Extracting to benches/data/..."

    unzip -o "$ZIP_PATH" -d benches/data/
    rm -f "$ZIP_PATH"

    echo "Benchmark data setup completed."
else
    echo "Warning: Failed to download benchmark data from $URL. Skipping..."
fi

exit 0
