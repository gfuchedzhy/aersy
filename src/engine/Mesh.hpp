/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_MESH_HPP
#define ENGINE_MESH_HPP

#include <glcxx/vao.hpp>
#include <memory>
#include "Renderable.hpp"
#include "Material.hpp"

class aiMesh;

/// @brief geometry mesh that has some material property
class CMesh : public IRenderable
{
      /// @brief material properties
      std::shared_ptr<SMaterial> mMaterial;

      /// @brief vao
      glcxx::vao<std::pair<cts("aPos"),  glm::vec3>,
                 std::pair<cts("aUV"),   glm::vec2>,
                 std::pair<cts("aNorm"), glm::vec3>,
                 std::pair<cts("aTan"),  glm::vec3>> mVAO;

   public:
      /// @brief constructor
      CMesh(const aiMesh& mesh, std::shared_ptr<SMaterial> material);

      /// @brief draw
      void draw(const CContext& context) const override;
};

#endif
