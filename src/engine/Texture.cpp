/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include <gli/load.hpp>
#include "Texture.hpp"
#include "Log.hpp"

CTexture::CTexture(const std::string& filename)
   : texture(GL_TEXTURE_2D)
{
   Log::msg("loading texture from ", filename);
   gli::texture texture = gli::load(filename);
   if(texture.empty())
      throw std::runtime_error{"failed to load texture"};

   gli::gl GL;
   bool valid = (GL.translate(texture.target()) == GL_TEXTURE_2D);
   valid = (1 == texture.layers()) && valid;
   valid = (1 == texture.faces()) && valid;
   if (!valid)
      throw std::runtime_error{"unsupported texture format"};

   const GLint levels = static_cast<GLint>(texture.levels()) - 1;
   const gli::gl::format format = GL.translate(texture.format());
   const glm::tvec3<GLsizei> dimensions(texture.dimensions());

   bind();
   gl(glTexParameteri, mTarget, GL_TEXTURE_BASE_LEVEL, 0);
   gl(glTexParameteri, mTarget, GL_TEXTURE_MAX_LEVEL, levels);
   gl(glTexParameteri, mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   gl(glTexParameteri, mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

   gl(glTexParameteri, mTarget, GL_TEXTURE_SWIZZLE_R, format.Swizzle[0]);
   gl(glTexParameteri, mTarget, GL_TEXTURE_SWIZZLE_G, format.Swizzle[1]);
   gl(glTexParameteri, mTarget, GL_TEXTURE_SWIZZLE_B, format.Swizzle[2]);
   gl(glTexParameteri, mTarget, GL_TEXTURE_SWIZZLE_A, format.Swizzle[3]);

   gl(glTexStorage2D, mTarget, levels, format.Internal, dimensions.x, dimensions.y);
   for(int l = 0; l < levels; ++l)
   {
      assert(gli::TARGET_2D == texture.target());
      const glm::tvec3<GLsizei> subImageDimensions(texture.dimensions(l));
      if(gli::is_compressed(texture.format()))
         gl(glCompressedTexSubImage2D,
            mTarget, l, 0, 0, subImageDimensions.x, subImageDimensions.y,
            format.Internal, static_cast<GLsizei>(texture.size(l)),
            texture.data(0, 0, l));
      else
         gl(glTexSubImage2D,
            mTarget, l, 0, 0, subImageDimensions.x, subImageDimensions.y,
            format.External, format.Type, texture.data(0, 0, l));
   }
   unbind();
}
