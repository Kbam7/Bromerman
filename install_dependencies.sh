#!/usr/bin/env bash

brew install cmake                  #CMAKE
brew install pkg-config             #PKG-CONFIG

brew install glfw3                   #GLFW3
brew link --overwrite glfw3

brew install sdl2                   #SDL2
brew link --overwrite sdl2

brew install sdl2_image             #SDL2_image
brew link --overwrite sdl2_image

brew install sdl2_ttf               #SDL2_ttf
brew link --overwrite sdl2_ttf

brew install sdl2_mixer             #SDL2_mixer
brew link --overwrite sdl2_mixer

#brew install hg                                         #HG - For pulling from BitBucket
#hg clone https://bitbucket.org/cegui/cegui              #CEGUI
#hg clone https://bitbucket.org/cegui/cegui-dependencies #CEGUI Dependencies

brew install glew                   #GLEW
brew link --overwrite glew

brew install glm                    #GLM
brew link --overwrite glm

#sh Build.sh                        #Run CMake