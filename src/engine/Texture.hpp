/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_TEXTURE_HPP
#define ENGINE_TEXTURE_HPP

#include <glcxx/src/texture.hpp>
#include <string>
#include <memory>

/// @brief texture object
class CTexture : public texture
{
   public:
      /// @brief reads texture from file, uploads to gpu
      CTexture(const std::string& filename);
};

/// @brief make texture
inline texture_ptr make_texture(const std::string& filename)
{
   return std::make_shared<CTexture>(filename);
}

#endif // ENGINE_TEXTURE_HPP
