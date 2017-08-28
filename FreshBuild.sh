#!/usr/bin/env bash

mkdir build_dir
cd build_dir
cmake ..
open .
cp -R ../libs .
