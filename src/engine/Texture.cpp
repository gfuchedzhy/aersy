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
   img.read(std::string("aersy-res/") + filename);
   auto size = img.size();
   Magick::Blob blob;
   img.write(&blob, "RGBA");
   bind();
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width(), size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
   glGenerateMipmap(_target);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   unbind();
}
