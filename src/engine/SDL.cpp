/*
 * Copyright 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */
#include "SDL.hpp"

unsigned CSDL::initNumber = 0;
std::mutex CSDL::mutex;

CSDL::CSDL(Uint32 flags)
   : mFlags(flags)
{
   std::lock_guard<std::mutex> lock(mutex);
   ++initNumber;
   SDL_SetMainReady();
   if(SDL_Init(mFlags))
      throw std::runtime_error(SDL_GetError());
}

CSDL::~CSDL()
{
   std::lock_guard<std::mutex> lock(mutex);
   --initNumber;
   SDL_QuitSubSystem(mFlags);
   if (!initNumber)
      SDL_Quit();
}
