/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_TEXTURE_HPP
#define ENGINE_TEXTURE_HPP

#include <string>
#include <memory>
#include <glcxx/texture.hpp>
#include "Log.hpp"

/// @brief texture object
class CTexture : public glcxx::texture
{
      /// @brief true if has alpha channel
      bool mHasAlpha = false;

   public:
      /// @brief reads texture from file, uploads to gpu
      CTexture(const std::string& filename);

      /// @brief return true if has alpha channel
      bool hasAlpha() const
      {
         return mHasAlpha;
      }
};

/// @brief texture ptr
using tTexturePtr = std::shared_ptr<CTexture>;

/// @brief make texture
inline tTexturePtr make_texture(const std::string& filename)
{
   return std::make_shared<CTexture>(filename);
}

#endif // ENGINE_TEXTURE_HPP
