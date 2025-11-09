#!/bin/bash

cd build-xcode
cmake .. -GXcode
open markymark.xcodeproj
