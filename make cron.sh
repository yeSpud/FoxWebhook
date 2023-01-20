#!/bin/bash
echo "Removing old executable (if exits)"
rm cron
echo "Building new executable"
cmake -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - Unix Makefiles" .
cmake --build . --target cron -- -j 1
