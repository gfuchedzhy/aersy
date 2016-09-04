/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "Engine.hpp"
#include "Log.hpp"

CEngine::CEngine(const size_t width, const size_t height)
   : mWindow(APPNAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0)
   , mAspect(width/float(height))
   , mTime(std::chrono::steady_clock::now())
{
}

void CEngine::run()
{
   while (true)
   {
      // handle events
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
         switch(event.type)
         {
            case SDL_QUIT:
               Log::msg("quit event occured");
               return;
            case SDL_KEYDOWN:
               onKeyPressed(event.key.keysym.sym);
               break;
            default:
               break;
         }
      }

      using namespace std::chrono;
      const steady_clock::time_point newTime = steady_clock::now();
      const float timeDelta = duration_cast<duration<float>>(newTime - mTime).count();
      mTime = newTime;

      // fps counting
      mFPS.mTime += timeDelta;
      mFPS.mFrameNumber++;
      if (mFPS.mTime > 5)
      {
         Log::msg("fps: ", mFPS.mFrameNumber/mFPS.mTime);
         mFPS.mFrameNumber = 0;
         mFPS.mTime = 0;
      }

      // update scene
      update(timeDelta);

      // clear the buffers
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // draw whole scene
      draw();

      // end the current frame, swap the front and back buffers
      mWindow.swap();
   }
}
