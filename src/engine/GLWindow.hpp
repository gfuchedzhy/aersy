/*
 * Copyright 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_GLWINDOW_HPP
#define ENGINE_GLWINDOW_HPP

#include "SDL.hpp"
#include <memory>
#include <iostream>

class CGLWindow
{
      template<typename T, void (*DeleterFunc)(T*)>
      struct deleter
      {
            void operator()(T* arg) const
            {
               DeleterFunc(arg);
            }
      };

      /// @brief SDL initialization holder
      CSDL mSDL;

      /// @brief window object
      std::unique_ptr<SDL_Window, deleter<SDL_Window, SDL_DestroyWindow>>
         mWindow;

      /// @brief opengl context
      std::unique_ptr<std::remove_pointer_t<SDL_GLContext>, deleter<std::remove_pointer_t<SDL_GLContext>, SDL_GL_DeleteContext>>
         mContext;

   public:
      CGLWindow(const char* title, int x, int y, int w, int h, Uint32 flags);

      void show(bool show = true)
      {
         (show ? SDL_ShowWindow : SDL_HideWindow)(mWindow.get());
      }

      void swap()
      {
         SDL_GL_SwapWindow(mWindow.get());
      }
};

#endif
