/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "Billboard.hpp"

#include <glcxx/capabilities.hpp>
#include "Context.hpp"
#include "Utils.hpp"

namespace
{
   auto rectVAO()
   {
      struct data_t {
         glm::vec3 pos;
         glm::vec2 uv;
      };
      static data_t data[] = {
         { {-0.5f,-0.5f, 0.f}, {0.f, 1.f} },
         { { 0.5f,-0.5f, 0.f}, {1.f, 1.f} },
         { { 0.5f, 0.5f, 0.f}, {1.f, 0.f} },
         { {-0.5f, 0.5f, 0.f}, {0.f, 0.f} }
      };

      static const GLubyte indices[] = {0, 1, 3, 2};

      static auto buf = glcxx::make_buffer(data);
      glcxx::vao<std::pair<cts("aPos"), glm::vec3>,
                 std::pair<cts("aUV"), glm::vec2>> vao;

      vao.upload_indices(indices, GL_TRIANGLE_STRIP);
      vao.set<cts("aPos")>(buf, &data_t::pos);
      vao.set<cts("aUV")>(buf, &data_t::uv);

      return vao;
   }
}

void CTexturedBillboard::draw(const CContext& context) const
{
   auto& p = context.getProgram<cts("billboard_tex")>();
   p.set<cts("uPos")>(mPos);
   p.set<cts("uSize")>(mSize);
   p.set<cts("uTexture")>(mTexture);
   p.draw_elements(rectVAO());
}

void CAnimatedBillboard::draw(const CContext& context) const
{
   auto& p = context.getProgram<cts("billboard_tex_sprite")>();
   p.set<cts("uPos")>(mPos);
   p.set<cts("uSize")>(mSize);
   p.set<cts("uAtlasSize")>(mAtlasSize);
   p.set<cts("uAtlasPos")>(mAtlasPos);
   p.set<cts("uTexture")>(mTexture);
   p.draw_elements(rectVAO());
}

void CHealthBar::draw(const CContext& context) const
{
   auto& p = context.getProgram<cts("billboard_hb")>();
   p.set<cts("uPos")>(mPos);
   p.set<cts("uSize")>(mSize);
   p.set<cts("uValue")>(mValue);

   glcxx::disable_depth_test_guard lock;
   p.draw_elements(rectVAO());
}
