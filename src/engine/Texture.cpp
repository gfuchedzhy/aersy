/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "Texture.hpp"
#include "Log.hpp"

#define MAGICKCORE_QUANTUM_DEPTH 8
#define MAGICKCORE_HDRI_ENABLE 0
#include <Magick++.h>

namespace
{
   static struct CInitImageMagick
   {
         CInitImageMagick()
         {
            Magick::InitializeMagick(nullptr);
         }
   } initMagick;
}

CTexture::CTexture(const std::string& filename)
   : texture(GL_TEXTURE_2D)
{
   Log::msg("loading texture from ", filename);
   Magick::Image img;
   Magick::Blob blob;
   img.read(std::string("aersy-res/") + filename);
   mHasAlpha = !img.isOpaque();
   img.write(&blob, mHasAlpha ? "RGBA" : "RGB");
   bind();
   const GLenum format = mHasAlpha ? GL_RGBA : GL_RGB;
   glTexImage2D(GL_TEXTURE_2D, 0, format, img.columns(), img.rows(), 0, format, GL_UNSIGNED_BYTE, blob.data());
   glGenerateMipmap(_target);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   unbind();
}
