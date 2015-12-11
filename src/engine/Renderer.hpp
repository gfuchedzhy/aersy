/*
 * Copyright 2015 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_RENDERER_HPP
#define ENGINE_RENDERER_HPP

#include "Program.hpp"

/// @brief renderer, implements lazy program creation and program selection
/// @tparam list of program names
template<typename... TProgramName>
class TRenderer
{
      /// @brief disabled stuff
      TRenderer(const TRenderer&) = delete;
      TRenderer& operator=(const TRenderer& other) = delete;

   public:
      TRenderer() = default;

      /// @brief searches given program by name in compile time, creates it if
      /// it is not created, deselects previous program, selects requested one
      /// and returns it, @see @notes for TRenderer::get
      /// @note it is disallowed to cache returned value, as selection of
      /// another program will invalidate it, to discourage caching this
      /// function returns raw pointer
      template<typename TName>
      auto get()
      {
         constexpr int index = ct::TTypeIndexInPack<TName, TProgramName...>::value;
         static_assert(-1 != index, "program name not found");
         auto&& p = std::get<index>(mPrograms);
         if (!p)
            p = make_program(TName{});
         if (p.get() != mCurrent)
         {
            p->select();
            mCurrent = p.get();
         }
         return p.get();
      }

   private:
      /// @brief program list
      std::tuple<decltype(make_program(TProgramName{}))...> mPrograms;

      /// @brief current program ptr
      CProgramObject* mCurrent;
};

#endif // ENGINE_RENDERER_HPP
