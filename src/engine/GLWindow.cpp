/*
 * Copyright 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "GLWindow.hpp"
#include <glcxx/gl.hpp>

CGLWindow::CGLWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
   : mSDL(SDL_INIT_VIDEO)
{
   mWindow.reset(SDL_CreateWindow(title, x, y, w, h, flags | SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN));
   if (!mWindow)
      throw std::runtime_error(SDL_GetError());

   SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

   mContext.reset(SDL_GL_CreateContext(mWindow.get()));
   if (!mContext)
      throw std::runtime_error(SDL_GetError());

   SDL_GL_MakeCurrent(mWindow.get(), mContext.get());

   if (!glcxx_load_gl(SDL_GL_GetProcAddress))
      throw std::runtime_error("gl failed to load");

   if (SDL_GL_SetSwapInterval(-1))
      SDL_GL_SetSwapInterval(1);
}
