/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_PROGRAMS_HPP
#define ENGINE_PROGRAMS_HPP

#include <glcxx/program.hpp>
#include <glcxx/uniform.hpp>
#include <glcxx/texture_input.hpp>

namespace programs
{
   using namespace glcxx;

   using regular = program<
      vao_input<cts("aPos"), glm::vec3>,
      uniform<cts("uModel"), glm::mat4>,
      uniform<cts("uViewProj"), glm::mat4>>;

   using regular_col = derive_program<
      regular,
      uniform<cts("uColor"), glm::vec3, tag::fragment>>;

   using regular_tex = derive_program<
      regular,
      vao_input<cts("aUV"), glm::vec2>,
      texture_input<cts("uTexture")>>;

   using shaded = program<
      vao_input<cts("aPos"), glm::vec3>,
      vao_input<cts("aNorm"), glm::vec3>,
      uniform<cts("uModel"), glm::mat4>,
      uniform<cts("uViewProj"), glm::mat4>,
      uniform<cts("uAmbient"), glm::vec3, tag::fragment>,
      uniform<cts("uDiffuse"), glm::vec3, tag::fragment>,
      uniform<cts("uSpecular"), glm::vec3, tag::fragment>,
      uniform<cts("uShininess"), float, tag::fragment>,
      uniform<cts("uSunDir"), glm::vec3, tag::vertfrag>,
      uniform<cts("uEye"), glm::vec3, tag::vertex>>;

   using shaded_col = derive_program<
      shaded,
      uniform<cts("uColor"), glm::vec3, tag::fragment>>;

   using shaded_tex = derive_program<
      shaded,
      vao_input<cts("aUV"), glm::vec2>,
      texture_input<cts("uTexture")>>;

   using shaded_tex_nmap = derive_program<
      shaded_tex,
      vao_input<cts("aTan"), glm::vec3>,
      texture_input<cts("uNormalMap"), 1>>;

   using billboard = program<
      vao_input<cts("aPos"), glm::vec3>,
      uniform<cts("uViewProj"), glm::mat4>,
      uniform<cts("uPos"), glm::vec3>,
      uniform<cts("uSize"), glm::vec2>,
      uniform<cts("uEyePos"), glm::vec3>,
      uniform<cts("uUp"), glm::vec3>>;

   using billboard_tex = derive_program<
      billboard,
      vao_input<cts("aUV"), glm::vec2>,
      texture_input<cts("uTexture")>>;

   using billboard_tex_sprite = derive_program<
      billboard_tex,
      uniform<cts("uAtlasSize"), glm::vec2, tag::vertex, glm::ivec2>,
      uniform<cts("uAtlasPos"),  glm::vec2, tag::vertex, glm::ivec2>>;

   using billboard_hb = derive_program<
      billboard,
      uniform<cts("uValue"), float, tag::fragment>>;

   using particlesys = program<
      vao_input<cts("aPos"), glm::vec3>,
      uniform<cts("uViewProj"), glm::mat4>,
      uniform<cts("uSize"), glm::vec2, tag::geometry>,
      uniform<cts("uPerspectiveScale"), glm::vec2, tag::geometry>>;

   using particlesys_tex = derive_program<
      particlesys,
      texture_input<cts("uTexture")>>;

   using particlesys_tex_sprite = derive_program<
      particlesys_tex,
      // aTime.x is age of particle, aTime.y is age of death
      vao_input<cts("aTime"), glm::vec2>,
      // for optimization purposes aSpeed.xyz is unit speed directon, aSpeed.w is
      // scalar speed value
      vao_input<cts("aSpeed"), glm::vec4>,
      uniform<cts("uAtlasSize"), glm::ivec2, tag::geometry>>;

   using particlesys_tex_sprite_flame = particlesys_tex_sprite;
}

#endif // ENGINE_PROGRAMS_HPP
