/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_SKY_HPP
#define ENGINE_SKY_HPP

#include "Model.hpp"
#include "Billboard.hpp"
#include <glcxx/src/vao.hpp>
#include <vector>

/// @brief sky consists of 5 textured rects and some clouds todo
class CSky : public IRenderableModel
{
      /// @brief clouds implemented using billboards
      std::vector<CTexturedBillboard> mClouds;

      /// @brief dome texture
      glcxx::texture_ptr mTexture;

      /// @brief vao
      glcxx::indexed_vao<glcxx::tpair<cts("aPos"), glm::vec3>,
                         glcxx::tpair<cts("aUV"),  glm::vec2>> mVAO;

   public:
      /// @brief constructor
      CSky(glcxx::texture_ptr tex, float horizonDistance);

      /// @brief draw
      void draw(const CContext& context) const override;
};

#endif
