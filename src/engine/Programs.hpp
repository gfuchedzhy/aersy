/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_PROGRAMS_HPP
#define ENGINE_PROGRAMS_HPP

#include <glcxx/program.hpp>
#include <glcxx/uniform_input.hpp>
#include <glcxx/texture_input.hpp>

namespace programs
{
   using namespace glcxx;

   using regular = program<
      attrib_input<cts("aPos"), attrib<glm::tvec3, float, float, 1>>,
      uniform_input<cts("uModel"), uniform<glm::tmat4x4>>,
      uniform_input<cts("uViewProj"), uniform<glm::tmat4x4>>>;

   using regular_col = derive_program<regular, uniform_input<cts("uColor"), uniform<glm::tvec3>, tag::fragment>>;

   using regular_tex = derive_program<
      regular,
      attrib_input<cts("aUV"), attrib<glm::tvec2>>,
      texture_input<cts("uTexture")>>;

   using shaded = program<
      attrib_input<cts("aPos"), attrib<glm::tvec3, float, float, 1>>,
      attrib_input<cts("aNorm"), attrib<glm::tvec3>>,
      uniform_input<cts("uModel"), uniform<glm::tmat4x4>>,
      uniform_input<cts("uViewProj"), uniform<glm::tmat4x4>>,
      uniform_input<cts("uAmbient"), uniform<glm::tvec3>, tag::fragment>,
      uniform_input<cts("uDiffuse"), uniform<glm::tvec3>, tag::fragment>,
      uniform_input<cts("uSpecular"), uniform<glm::tvec3>, tag::fragment>,
      uniform_input<cts("uShininess"), uniform<glm::tvec1>, tag::fragment>,
      uniform_input<cts("uSunDir"), uniform<glm::tvec3>, tag::vertfrag>,
      uniform_input<cts("uEye"), uniform<glm::tvec3>, tag::vertex>>;

   using shaded_col = derive_program<shaded, uniform_input<cts("uColor"), uniform<glm::tvec3>, tag::fragment>>;

   using shaded_tex = derive_program<
      shaded,
      attrib_input<cts("aUV"), attrib<glm::tvec2>>,
      texture_input<cts("uTexture")>>;

   using shaded_tex_nmap = derive_program<
      shaded_tex,
      attrib_input<cts("aTan"), attrib<glm::tvec3>>,
      texture_input<cts("uNormalMap"), 1>>;

   using billboard = program<
      attrib_input<cts("aPos"), attrib<glm::tvec3, float, float, 1>>,
      uniform_input<cts("uViewProj"), uniform<glm::tmat4x4>>,
      uniform_input<cts("uPos"), uniform<glm::tvec3>>,
      uniform_input<cts("uSize"), uniform<glm::tvec2>>,
      uniform_input<cts("uEyePos"), uniform<glm::tvec3>>,
      uniform_input<cts("uUp"), uniform<glm::tvec3>>>;

   using billboard_tex = derive_program<
      billboard,
      attrib_input<cts("aUV"), attrib<glm::tvec2>>,
      texture_input<cts("uTexture")>>;

   using billboard_tex_sprite = derive_program<
      billboard_tex,
      uniform_input<cts("uAtlasSize"), uniform<glm::tvec2, int>>,
      uniform_input<cts("uAtlasPos"), uniform<glm::tvec2, int>>>;

   using billboard_hb = derive_program<
      billboard,
      uniform_input<cts("uValue"), uniform<glm::tvec1>, tag::fragment>>;

   using particlesys = program<
      attrib_input<cts("aPos"), attrib<glm::tvec3>>,
      uniform_input<cts("uViewProj"), uniform<glm::tmat4x4>>,
      uniform_input<cts("uSize"), uniform<glm::tvec2>, tag::geometry>,
      uniform_input<cts("uPerspectiveScale"), uniform<glm::tvec2>, tag::geometry>>;

   using particlesys_tex = derive_program<particlesys, texture_input<cts("uTexture")>>;

   using particlesys_tex_sprite = derive_program<
      particlesys_tex,
      // aTime.x is age of particle, aTime.y is age of death
      attrib_input<cts("aTime"), attrib<glm::tvec2>>,
      // for optimization purposes aSpeed.xyz is unit speed directon, aSpeed.w is
      // scalar speed value
      attrib_input<cts("aSpeed"), attrib<glm::tvec4>>,
      uniform_input<cts("uAtlasSize"), uniform<glm::tvec2, int, int>, tag::geometry>>;

   using particlesys_tex_sprite_flame = particlesys_tex_sprite;
}

#endif // ENGINE_PROGRAMS_HPP
