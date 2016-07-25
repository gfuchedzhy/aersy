/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "App.hpp"
#include "Log.hpp"
#include <glcxx/src/gl.hpp>

/// @brief entry point
int main()
{
   glcxx::set_glerror_cb([](const std::stringstream& func, const char* scope, GLenum err)
                         {
                            Log::err("gl error code ", err, " in ", func.rdbuf(), " at ", scope);
                         });
   // catch and rethrow to properly free all gl resources even in case of
   // exception
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
