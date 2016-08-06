/*
 * Copyright 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "Context.hpp"
#include <glcxx/capabilities.hpp>

namespace
{
   // glsl version and macros to declare and access in/out variables between
   // shaders
   static const char glslCommonDecl[] = R"(
#ifdef VERTEX
#  define DECL_VERT_GEOM(type, name) out type vg##name
#elif defined GEOMETRY
#  define DECL_VERT_GEOM(type, name) in type vg##name[]
#else
#  define DECL_VERT_GEOM(type, name) void
#endif

#ifdef GEOMETRY
#  define DECL_GEOM_FRAG(type, name) out type gf##name
#elif defined FRAGMENT
#  define DECL_GEOM_FRAG(type, name) in type gf##name
#else
#  define DECL_GEOM_FRAG(type, name) void
#endif

#ifdef VERTEX
#  define DECL_VERT_FRAG(type, name) out type vf##name
#elif defined FRAGMENT
#  define DECL_VERT_FRAG(type, name) in type vf##name
#else
#  define DECL_VERT_FRAG(type, name) void
#endif

#ifdef VERTEX
#  ifdef HAS_GEOMETRY
#    define out(name) vg##name
#  else
#   define out(name) vf##name
#  endif
#elif defined GEOMETRY
#  define in(name) vg##name
#  define out(name) gf##name
#elif defined FRAGMENT
#  ifdef HAS_GEOMETRY
#    define in(name) gf##name
#  else
#    define in(name) vf##name
#  endif
#endif
#line 0
)";
}

CContext::CContext()
   : mRenderer("#version 330", glslCommonDecl)
{
   // setup default flags
   glcxx_gl(glEnable, GL_DEPTH_TEST);
   glcxx_gl(glEnable, GL_CULL_FACE);
   glcxx::enable_blending_guard::set_default_factors();
}
