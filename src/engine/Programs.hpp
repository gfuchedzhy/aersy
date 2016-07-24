/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_PROGRAMS_HPP
#define ENGINE_PROGRAMS_HPP

#include <glcxx/src/program.hpp>
#include <glcxx/src/uniform_input.hpp>
#include <glcxx/src/texture_input.hpp>

template<typename TName>
const char* programSrc();

#define inp_by_name(name) decltype(progInputDef(cts(name){}))

auto progInputDef(cts("regular")) -> std::tuple<
   attrib_input<cts("aPos"), attrib<glm::tvec3, float, float, 1>>,
   uniform_input<cts("uModel"), uniform<glm::tmat4x4>>,
   uniform_input<cts("uViewProj"), uniform<glm::tmat4x4>>>;

auto progInputDef(cts("regular-col")) -> ct::tuple_append<inp_by_name("regular"),
   uniform_input<cts("uColor"), uniform<glm::tvec3>, tag::fragment>>;

auto progInputDef(cts("regular-tex")) -> ct::tuple_append<inp_by_name("regular"),
   attrib_input<cts("aUV"), attrib<glm::tvec2>>,
   texture_input<cts("uTexture")>>;

auto progInputDef(cts("shaded")) -> std::tuple<
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

auto progInputDef(cts("shaded-col")) -> ct::tuple_append<inp_by_name("shaded"),
   uniform_input<cts("uColor"), uniform<glm::tvec3>, tag::fragment>>;

auto progInputDef(cts("shaded-tex")) -> ct::tuple_append<inp_by_name("shaded"),
   attrib_input<cts("aUV"), attrib<glm::tvec2>>,
   texture_input<cts("uTexture")>>;

auto progInputDef(cts("shaded-tex-nmap")) -> ct::tuple_append<inp_by_name("shaded-tex"),
   attrib_input<cts("aTan"), attrib<glm::tvec3>>,
   texture_input<cts("uNormalMap"), 1>>;

auto progInputDef(cts("billboard")) -> std::tuple<
   attrib_input<cts("aPos"), attrib<glm::tvec3, float, float, 1>>,
   uniform_input<cts("uViewProj"), uniform<glm::tmat4x4>>,
   uniform_input<cts("uPos"), uniform<glm::tvec3>>,
   uniform_input<cts("uSize"), uniform<glm::tvec2>>,
   uniform_input<cts("uEyePos"), uniform<glm::tvec3>>,
   uniform_input<cts("uUp"), uniform<glm::tvec3>>>;

auto progInputDef(cts("billboard-tex")) -> ct::tuple_append<inp_by_name("billboard"),
   attrib_input<cts("aUV"), attrib<glm::tvec2>>,
   texture_input<cts("uTexture")>>;

auto progInputDef(cts("billboard-tex-sprite")) -> ct::tuple_append<inp_by_name("billboard-tex"),
   uniform_input<cts("uAtlasSize"), uniform<glm::tvec2, int>>,
   uniform_input<cts("uAtlasPos"), uniform<glm::tvec2, int>>>;

auto progInputDef(cts("billboard-hb")) -> ct::tuple_append<inp_by_name("billboard"),
   uniform_input<cts("uValue"), uniform<glm::tvec1>, tag::fragment>>;

auto progInputDef(cts("particlesys")) -> std::tuple<
   attrib_input<cts("aPos"), attrib<glm::tvec3>>,
   uniform_input<cts("uViewProj"), uniform<glm::tmat4x4>>,
   uniform_input<cts("uSize"), uniform<glm::tvec2>, tag::geometry>,
   uniform_input<cts("uPerspectiveScale"), uniform<glm::tvec2>, tag::geometry>>;

auto progInputDef(cts("particlesys-tex")) -> ct::tuple_append<inp_by_name("particlesys"),
   texture_input<cts("uTexture")>>;

auto progInputDef(cts("particlesys-tex-sprite")) -> ct::tuple_append<inp_by_name("particlesys-tex"),
   // aTime.x is age of particle, aTime.y is age of death
   attrib_input<cts("aTime"), attrib<glm::tvec2>>,
   // for optimization purposes aSpeed.xyz is unit speed directon, aSpeed.w is
   // scalar speed value
   attrib_input<cts("aSpeed"), attrib<glm::tvec4>>,
   uniform_input<cts("uAtlasSize"), uniform<glm::tvec2, int, int>, tag::geometry>>;

auto progInputDef(cts("particlesys-tex-sprite-flame")) -> inp_by_name("particlesys-tex-sprite");

#endif // ENGINE_PROGRAMS_HPP
