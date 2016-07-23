/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_MATERIAL_HPP
#define ENGINE_MATERIAL_HPP

#include "Texture.hpp"
#include <memory>

class aiMaterial;

/// @brief material properties
struct SMaterial
{
      /// @brief diffuse color
      glm::vec3 mDiffuse;

      /// @brief ambient color
      glm::vec3 mAmbient;
      
      /// @brief specular color
      glm::vec3 mSpecular;

      /// @brief shininess coefficient
      float mShininess = 1.f;

      /// @brief diffuse texture
      texture_ptr mDiffuseMap;

      /// @brief normal map texture
      texture_ptr mNormalMap;

      /// @brief constructor
      SMaterial(const aiMaterial& material, const std::string& textureDirectory);
};

#endif
