/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "Billboard.hpp"
#include "Context.hpp"
#include "Utils.hpp"

namespace
{
   auto& indexBuffer()
   {
      static const GLubyte data[] = {0, 1, 3, 2};
      static auto buffer = glcxx::make_index_buffer(data, size(data), GL_TRIANGLE_STRIP);
      return buffer;
   }

   auto& uvBuffer()
   {
      /// @note flip of Y coordinates, because loaded data are top-row-first, but
      /// opengl expects bottom-row-first
      static const typename glm::vec2 data[] = {
         {0.f, 1.f},
         {1.f, 1.f},
         {1.f, 0.f},
         {0.f, 0.f}
      };
      static auto buffer = glcxx::make_buffer(data, size(data));
      return buffer;
   }

   auto& posBuffer()
   {
      static const glm::vec3 data[] = {
         {-0.5f,-0.5f, 0.f},
         { 0.5f,-0.5f, 0.f},
         { 0.5f, 0.5f, 0.f},
         {-0.5f, 0.5f, 0.f}
      };
      static auto buffer = glcxx::make_buffer(data, size(data));
      return buffer;
   }
}

void CTexturedBillboard::draw(const CContext& context) const
{
   static auto vao = glcxx::make_vao<cts("aPos"), cts("aUV")>(indexBuffer(), posBuffer(), uvBuffer());
   auto& p = context.getProgram<cts("billboard-tex")>();
   p.set<cts("uPos")>(mPos);
   p.set<cts("uSize")>(mSize);
   p.set<cts("uTexture")>(mTexture);
   p.draw_elements(vao);
}

void CAnimatedBillboard::draw(const CContext& context) const
{
   static auto vao = glcxx::make_vao<cts("aPos"), cts("aUV")>(indexBuffer(), posBuffer(), uvBuffer());
   auto& p = context.getProgram<cts("billboard-tex-sprite")>();
   p.set<cts("uPos")>(mPos);
   p.set<cts("uSize")>(mSize);
   p.set<cts("uAtlasSize")>(mAtlasSize);
   p.set<cts("uAtlasPos")>(mAtlasPos);
   p.set<cts("uTexture")>(mTexture);
   p.draw_elements(vao);
}

void CHealthBar::draw(const CContext& context) const
{
   static auto vao = glcxx::make_vao<cts("aPos")>(indexBuffer(), posBuffer());
   auto& p = context.getProgram<cts("billboard-hb")>();
   p.set<cts("uPos")>(mPos);
   p.set<cts("uSize")>(mSize);
   p.set<cts("uValue")>(mValue);

   glcxx::disable_depth_test_guard lock;
   p.draw_elements(vao);
}
