/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_SKY_HPP
#define ENGINE_SKY_HPP

#include <vector>
#include <glcxx/vao.hpp>
#include "Model.hpp"
#include "Billboard.hpp"

/// @brief sky consists of 5 textured rects and some clouds todo
class CSky : public IRenderableModel
{
      /// @brief clouds implemented using billboards
      std::vector<CTexturedBillboard> mClouds;

      /// @brief dome texture
      glcxx::texture_ptr mTexture;

      /// @brief vao
      glcxx::indexed_vao<std::pair<cts("aPos"), glm::vec3>,
                         std::pair<cts("aUV"),  glm::vec2>> mVAO;

   public:
      /// @brief constructor
      CSky(glcxx::texture_ptr tex, float horizonDistance);

      /// @brief draw
      void draw(const CContext& context) const override;
};

#endif
