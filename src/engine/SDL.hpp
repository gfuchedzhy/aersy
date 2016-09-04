/*
 * Copyright 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_SDL_HPP
#define ENGINE_SDL_HPP

#include <SDL.h>
#include <mutex>

/// @brief wrap SDL initialization in thread safe way
class CSDL
{
      static std::mutex mutex;
      static unsigned initNumber;

      Uint32 mFlags;

   public:
      CSDL(Uint32 flags);
      ~CSDL();
};

#endif
