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
   glcxx::attrib_input<cts("aPos"), glcxx::attrib<glm::tvec3, float, float, 1>>,
   glcxx::uniform_input<cts("uModel"), glcxx::uniform<glm::tmat4x4>>,
   glcxx::uniform_input<cts("uViewProj"), glcxx::uniform<glm::tmat4x4>>>;

auto progInputDef(cts("regular-col")) -> glcxx::ct::tuple_append<inp_by_name("regular"),
   glcxx::uniform_input<cts("uColor"), glcxx::uniform<glm::tvec3>, glcxx::tag::fragment>>;

auto progInputDef(cts("regular-tex")) -> glcxx::ct::tuple_append<inp_by_name("regular"),
   glcxx::attrib_input<cts("aUV"), glcxx::attrib<glm::tvec2>>,
   glcxx::texture_input<cts("uTexture")>>;

auto progInputDef(cts("shaded")) -> std::tuple<
   glcxx::attrib_input<cts("aPos"), glcxx::attrib<glm::tvec3, float, float, 1>>,
   glcxx::attrib_input<cts("aNorm"), glcxx::attrib<glm::tvec3>>,
   glcxx::uniform_input<cts("uModel"), glcxx::uniform<glm::tmat4x4>>,
   glcxx::uniform_input<cts("uViewProj"), glcxx::uniform<glm::tmat4x4>>,
   glcxx::uniform_input<cts("uAmbient"), glcxx::uniform<glm::tvec3>, glcxx::tag::fragment>,
   glcxx::uniform_input<cts("uDiffuse"), glcxx::uniform<glm::tvec3>, glcxx::tag::fragment>,
   glcxx::uniform_input<cts("uSpecular"), glcxx::uniform<glm::tvec3>, glcxx::tag::fragment>,
   glcxx::uniform_input<cts("uShininess"), glcxx::uniform<glm::tvec1>, glcxx::tag::fragment>,
   glcxx::uniform_input<cts("uSunDir"), glcxx::uniform<glm::tvec3>, glcxx::tag::vertfrag>,
   glcxx::uniform_input<cts("uEye"), glcxx::uniform<glm::tvec3>, glcxx::tag::vertex>>;

auto progInputDef(cts("shaded-col")) -> glcxx::ct::tuple_append<inp_by_name("shaded"),
   glcxx::uniform_input<cts("uColor"), glcxx::uniform<glm::tvec3>, glcxx::tag::fragment>>;

auto progInputDef(cts("shaded-tex")) -> glcxx::ct::tuple_append<inp_by_name("shaded"),
   glcxx::attrib_input<cts("aUV"), glcxx::attrib<glm::tvec2>>,
   glcxx::texture_input<cts("uTexture")>>;

auto progInputDef(cts("shaded-tex-nmap")) -> glcxx::ct::tuple_append<inp_by_name("shaded-tex"),
   glcxx::attrib_input<cts("aTan"), glcxx::attrib<glm::tvec3>>,
   glcxx::texture_input<cts("uNormalMap"), 1>>;

auto progInputDef(cts("billboard")) -> std::tuple<
   glcxx::attrib_input<cts("aPos"), glcxx::attrib<glm::tvec3, float, float, 1>>,
   glcxx::uniform_input<cts("uViewProj"), glcxx::uniform<glm::tmat4x4>>,
   glcxx::uniform_input<cts("uPos"), glcxx::uniform<glm::tvec3>>,
   glcxx::uniform_input<cts("uSize"), glcxx::uniform<glm::tvec2>>,
   glcxx::uniform_input<cts("uEyePos"), glcxx::uniform<glm::tvec3>>,
   glcxx::uniform_input<cts("uUp"), glcxx::uniform<glm::tvec3>>>;

auto progInputDef(cts("billboard-tex")) -> glcxx::ct::tuple_append<inp_by_name("billboard"),
   glcxx::attrib_input<cts("aUV"), glcxx::attrib<glm::tvec2>>,
   glcxx::texture_input<cts("uTexture")>>;

auto progInputDef(cts("billboard-tex-sprite")) -> glcxx::ct::tuple_append<inp_by_name("billboard-tex"),
   glcxx::uniform_input<cts("uAtlasSize"), glcxx::uniform<glm::tvec2, int>>,
   glcxx::uniform_input<cts("uAtlasPos"), glcxx::uniform<glm::tvec2, int>>>;

auto progInputDef(cts("billboard-hb")) -> glcxx::ct::tuple_append<inp_by_name("billboard"),
   glcxx::uniform_input<cts("uValue"), glcxx::uniform<glm::tvec1>, glcxx::tag::fragment>>;

auto progInputDef(cts("particlesys")) -> std::tuple<
   glcxx::attrib_input<cts("aPos"), glcxx::attrib<glm::tvec3>>,
   glcxx::uniform_input<cts("uViewProj"), glcxx::uniform<glm::tmat4x4>>,
   glcxx::uniform_input<cts("uSize"), glcxx::uniform<glm::tvec2>, glcxx::tag::geometry>,
   glcxx::uniform_input<cts("uPerspectiveScale"), glcxx::uniform<glm::tvec2>, glcxx::tag::geometry>>;

auto progInputDef(cts("particlesys-tex")) -> glcxx::ct::tuple_append<inp_by_name("particlesys"),
   glcxx::texture_input<cts("uTexture")>>;

auto progInputDef(cts("particlesys-tex-sprite")) -> glcxx::ct::tuple_append<inp_by_name("particlesys-tex"),
   // aTime.x is age of particle, aTime.y is age of death
   glcxx::attrib_input<cts("aTime"), glcxx::attrib<glm::tvec2>>,
   // for optimization purposes aSpeed.xyz is unit speed directon, aSpeed.w is
   // scalar speed value
   glcxx::attrib_input<cts("aSpeed"), glcxx::attrib<glm::tvec4>>,
   glcxx::uniform_input<cts("uAtlasSize"), glcxx::uniform<glm::tvec2, int, int>, glcxx::tag::geometry>>;

auto progInputDef(cts("particlesys-tex-sprite-flame")) -> inp_by_name("particlesys-tex-sprite");

#endif // ENGINE_PROGRAMS_HPP
