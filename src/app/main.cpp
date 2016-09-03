/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "App.hpp"
#include "Log.hpp"

/// @brief entry point
int main()
{
   // catch to properly free all gl resources
   try
   {
      CApp app;
      Log::msg("main loop start");
      app.run();
      Log::msg("main loop end");
   }
   catch(std::exception& e)
   {
      Log::err(e.what());
   }
}
