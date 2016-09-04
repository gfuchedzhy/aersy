/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP

#include <chrono>
#include "GLWindow.hpp"
#include "Context.hpp"
#include "Utils.hpp"

/// @brief engine class
class CEngine
{
   public:
      /// @brief constructor
      CEngine(const size_t width, const size_t height);

      /// @brief destructor
      virtual ~CEngine() = default;

      /// @brief draw
      virtual void update(float timeDelta) = 0;

      /// @brief key presses handler
      virtual void onKeyPressed(SDL_Keycode keyCode) = 0;

      /// @brief draw
      virtual void draw() const = 0;

      /// @brief main loop
      virtual void run();

   protected:
      /// @brief window object
      CGLWindow mWindow;

      /// @brief drawing context
      CContext mContext;

      /// @brief aspect ratio
      float mAspect;

      /// @brief current absolute time
      std::chrono::steady_clock::time_point mTime;

      /// @brief struct for fps counting
      struct
      {
            float mTime = 0.f;
            unsigned int mFrameNumber = 0;
      } mFPS;
};

#endif // ENGINE_ENGINE_HPP
