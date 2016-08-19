/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "Texture.hpp"
#include <SFML/Graphics/Image.hpp>
#include "Log.hpp"

CTexture::CTexture(const std::string& filename)
   : texture(GL_TEXTURE_2D)
{
   Log::msg("loading texture from ", filename);
   sf::Image img;
   if (!img.loadFromFile(filename))
      throw std::runtime_error{"failed to load texture"};
   auto size = img.getSize();
   bind();
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)img.getPixelsPtr());
   glGenerateMipmap(_target);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   unbind();
}
