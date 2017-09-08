#!/usr/bin/env bash

brew install cmake                  #CMAKE
brew install pkg-config             #PKG-CONFIG

brew install glfw3                  #GLFW3
brew link --overwrite glfw3

brew install glew                   #GLEW
brew link --overwrite glew

brew install glm                    #GLM
brew link --overwrite glm

brew install assimp                 #ASSIMP
brew link --overwrite assimp

sh Build.sh                         #Run CMake