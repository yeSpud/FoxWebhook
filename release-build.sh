#!/bin/bash
cmake -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - Unix Makefiles" .
cmake --build . --target FoxWebhook -- -j 1
