/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_TERRAIN_HPP
#define ENGINE_TERRAIN_HPP

#include <glcxx/vao.hpp>
#include "Model.hpp"
#include "Texture.hpp"

/// @brief dummy terrain is bunch of rectangles covering area under skydome
class CTerrain : public IRenderableModel
{
      /// @brief texture
      glcxx::texture_ptr mTexture;

      /// @brief vao
      glcxx::vao<std::pair<cts("aPos"), glm::vec3>,
                 std::pair<cts("aUV"),  glm::vec2>> mVAO;

      /// @brief terrain center
      glm::vec2 mCenter;

      /// @brief terrain radius
      float mRadius;

   public:
      /// @brief constructor
      CTerrain(glcxx::texture_ptr tex);

      /// @brief set region where to draw terrain
      void center(const glm::vec2& center) { mCenter = center; }
      void radius(float radius) { mRadius = radius; }

      /// @brief draw
      void draw(const CContext& context) const override;
};

#endif
