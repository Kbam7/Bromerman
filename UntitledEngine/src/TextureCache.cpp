#include "../include/TextureCache.h"
#include "../include/ImageLoader.h"
#include <iostream>

namespace UntitledEngine {

    TextureCache::TextureCache()
    {
    }

    TextureCache::~TextureCache()
    {
    }

    GLTexture TextureCache::getTexture(std::string texturePath) {

        //lookup the texture and see if its in the map
        auto mit = _textureMap.find(texturePath);

        //check if its not in the map
        if (mit == _textureMap.end()) {
            //Load the texture
            GLTexture newTexture = ImageLoader::loadPNG(texturePath);

            //Insert it into the map
            _textureMap.insert(make_pair(texturePath, newTexture));

            return newTexture;
        }
        return mit->second;
    }

}