/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_CONTEXT_HPP
#define ENGINE_CONTEXT_HPP

#include <glcxx/renderer.hpp>
#include "Programs.hpp"

/// @brief drawing context
class CContext
{
#define named_program(name) std::pair<cts(#name), programs::name>
      /// @brief renderer holding programs
      using tRenderer = glcxx::renderer<named_program(regular_col),
                                        named_program(regular_tex),
                                        named_program(shaded_col),
                                        named_program(shaded_tex),
                                        named_program(shaded_tex_nmap),
                                        named_program(billboard_tex),
                                        named_program(billboard_tex_sprite),
                                        named_program(billboard_hb),
                                        named_program(particlesys_tex_sprite_flame)>;
      // renderer state doesn't change logical const of context
      mutable tRenderer mRenderer;

   public:
      /// @brief constructor
      CContext();

      /// @brief return drawing program
      template<typename TName>
      auto& getProgram() const
      {
         return mRenderer.program<TName>();
      }

      /// additional flags to enable/disable various features
      /// @brief if true draw health bars
      bool mDrawHealthBars  = false;

      /// @brief if true draw debug normals
      bool mDrawNormals  = false;
};

#endif
