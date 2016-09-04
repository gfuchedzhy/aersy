/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "App.hpp"

#include <glcxx/capabilities.hpp>
#include "Texture.hpp"
#include "Log.hpp"

namespace
{
   std::uniform_real_distribution<float> distr(0.8f, 1.05f);
   std::uniform_real_distribution<float> distr2(-1, 1);
   const float horizonDistance = 30e3;
}

CApp::CApp()
   : CEngine(1280, 960)
   , mSky(make_texture("sky.png"), horizonDistance)
   , mTerrain(make_texture("ground.jpg"))
{
   mTerrain.scale({0.3*horizonDistance, 0.3*horizonDistance, 1.f});
   mTerrain.radius(horizonDistance);

   mAircraft.pos({0, 0, 1.5e3});

   mCamera.perspective(60, mAspect, 1, 4e4);
   mTargetCameraEyeDistance = 35;
   mCamera.eyeDistance(mTargetCameraEyeDistance);
   mTargetCameraPitch = 15;
   mCamera.pitch(mTargetCameraPitch);

   auto starTexture = make_texture("star-sprite.png");
   for (auto& s: mStars)
   {
      s.atlasSize({4, 4});
      s.texture(starTexture);
      s.size({15, 15});
      s.pos({1e3*distr2(random_gen), 1e3*distr2(random_gen), 1.5e3 * distr(random_gen)});
   }
   mWindow.show();
}

/// @brief return damped value
inline float damp(float value, float timeDelta, float targetValue, float dampCoef = 0.8f)
{
   return targetValue + (value - targetValue) * std::max(0.f, 1.f - timeDelta/dampCoef);
}

/// @brief animates value returned by get in given range on given keypresses
template<typename Get, typename Set>
inline bool animate(Get get, Set set,
                    float timeDelta, float speed,
                    bool isMinKeyPressed, bool isMaxKeyPressed,
                    float minVal, float maxVal)
{
   if (isMinKeyPressed)
      set(std::max(minVal, get() - timeDelta*speed));
   else if (isMaxKeyPressed)
      set(std::min(maxVal, get() + timeDelta*speed));
   else
      return false;
   return true;
}

void CApp::update(float timeDelta)
{
   mAircraft.update(timeDelta);
   if (mCameraFollowsAircraft)
      mCamera.lookAt(mAircraft.pos());

   const auto& pos = mCamera.eye();
   mSky.pos({pos.x, pos.y, mSky.pos().z});
   mTerrain.center({pos.x, pos.y});

   for (auto& s : mStars)
   {
      // @todo redo this with bounding boxes
      if (glm::distance(s.pos(), mAircraft.pos()) < 20)
      {
         Log::msg("bonus collected");
         mAircraft.repair();
         s.pos({1e3*distr2(random_gen), 1e3*distr2(random_gen), 1.5e3 * distr(random_gen)});
      }
      s.update(timeDelta);
   }

   const auto kbdState = SDL_GetKeyboardState(nullptr);

   constexpr auto inf = std::numeric_limits<float>::infinity();
   if (mIsCameraControl)
   {
      animate([this]{return mTargetCameraPitch;},
              [this](float val){mTargetCameraPitch = val;},
              timeDelta, 30.f, kbdState[SDL_SCANCODE_DOWN], kbdState[SDL_SCANCODE_UP], -inf, inf);
      animate([this]{return mTargetCameraEyeDistance;},
              [this](float val){mTargetCameraEyeDistance = val;},
              timeDelta, 30.f, kbdState[SDL_SCANCODE_KP_PLUS], kbdState[SDL_SCANCODE_KP_MINUS], 10.f, 500.f);
   }
   mCamera.pitch(damp(mCamera.pitch(), timeDelta, mTargetCameraPitch, 1.f));
   mCamera.eyeDistance(damp(mCamera.eyeDistance(), timeDelta, mTargetCameraEyeDistance, 1.f));

   if (mIsCameraControl)
   {
      animate([this]{return mRelativeCameraOrientation;},
              [this](float val){mRelativeCameraOrientation = val;},
              timeDelta, 60.f, kbdState[SDL_SCANCODE_LEFT], kbdState[SDL_SCANCODE_RIGHT], -inf, inf);
   }
   mCamera.orientation(damp(mCamera.orientation(), timeDelta, mRelativeCameraOrientation + mAircraft.yaw(), 1.f));

   if (mIsCameraControl || !animate([this]{return mAircraft.roll();},
                                    [this](float val){mAircraft.roll(val);},
                                    timeDelta, 30.f, kbdState[SDL_SCANCODE_LEFT], kbdState[SDL_SCANCODE_RIGHT], -60.f, 60.f))
   {
      mAircraft.roll(damp(mAircraft.roll(), timeDelta, 0));
   }
   const float yawSpeed = 0.5f * mAircraft.roll();
   mAircraft.yaw(mAircraft.yaw() - yawSpeed*timeDelta);

   if(0 != mAircraft.speed())
   {
      if (!animate([this]{return mAircraft.speed();},
                   [this](float val){mAircraft.speed(val);},
                   timeDelta, 20.f, false, kbdState[SDL_SCANCODE_SPACE], 80.f, 180.f))
         mAircraft.speed(damp(mAircraft.speed(), timeDelta, 80));
   }

   if (mIsCameraControl || !animate([this]{return mAircraft.pitch();},
                                    [this](float val){ mAircraft.pitch(val);},
                                    timeDelta, 20.f, kbdState[SDL_SCANCODE_UP], kbdState[SDL_SCANCODE_DOWN], -inf, inf))
   {
      mAircraft.pitch(damp(mAircraft.pitch() - 360*int(round(mAircraft.pitch()/360)), timeDelta, 0, 1));
   }

   static const glm::vec3 sunDir = {-0.577, -0.577, 0.577};
   /// @todo make automatic uniforms in renderer to remove this code
   {  auto& p = mContext.getProgram<cts("regular_col")>();
      p.set<cts("uViewProj")>(mCamera.viewProj()); }
   {  auto& p = mContext.getProgram<cts("regular_tex")>();
      p.set<cts("uViewProj")>(mCamera.viewProj()); }
   {  auto& p = mContext.getProgram<cts("shaded_col")>();
      p.set<cts("uViewProj")>(mCamera.viewProj());
      p.set<cts("uSunDir")>(sunDir);
      p.set<cts("uEye")>(mCamera.eye()); }
   {  auto& p = mContext.getProgram<cts("shaded_tex")>();
      p.set<cts("uViewProj")>(mCamera.viewProj());
      p.set<cts("uSunDir")>(sunDir);
      p.set<cts("uEye")>(mCamera.eye()); }
   {  auto& p = mContext.getProgram<cts("shaded_tex_nmap")>();
      p.set<cts("uViewProj")>(mCamera.viewProj());
      p.set<cts("uSunDir")>(sunDir);
      p.set<cts("uEye")>(mCamera.eye()); }
   {  auto& p = mContext.getProgram<cts("billboard_tex")>();
      p.set<cts("uViewProj")>(mCamera.viewProj());
      p.set<cts("uUp")>({0, 0, 1});
      p.set<cts("uEyePos")>(mCamera.eye()); }
   {  auto& p = mContext.getProgram<cts("billboard_tex_sprite")>();
      p.set<cts("uViewProj")>(mCamera.viewProj());
      p.set<cts("uUp")>({0, 0, 1});
      p.set<cts("uEyePos")>(mCamera.eye()); }
   {  auto& p = mContext.getProgram<cts("billboard_hb")>();
      p.set<cts("uViewProj")>(mCamera.viewProj());
      p.set<cts("uUp")>(mCamera.up());
      p.set<cts("uEyePos")>(mCamera.eye()); }
   {  auto& p = mContext.getProgram<cts("particlesys_tex_sprite_flame")>();
      p.set<cts("uViewProj")>(mCamera.viewProj());
      p.set<cts("uPerspectiveScale")>(mCamera.perspectiveScale()); }
}

void CApp::onKeyPressed(SDL_Keycode keyCode)
{
   switch (keyCode)
   {
      case SDLK_1:
         mCameraFollowsAircraft = !mCameraFollowsAircraft;
         Log::msg("camera aircraft following turned ", Log::SOnOff(mCameraFollowsAircraft));
         break;
      case SDLK_2:
         mContext.mDrawHealthBars = !mContext.mDrawHealthBars;
         Log::msg("health bars turned ", Log::SOnOff(mContext.mDrawHealthBars));
         break;
      case SDLK_3:
         mContext.mDrawNormals = !mContext.mDrawNormals;
         Log::msg("normals turned ", Log::SOnOff(mContext.mDrawNormals));
         break;
      case SDLK_TAB:
         mIsCameraControl = !mIsCameraControl;
         Log::msg("camera control turned ", Log::SOnOff(mIsCameraControl));
         break;
      case SDLK_RETURN:
         Log::msg(mAircraft.launchRocket() ? "rocket launched" : "out of rockets");
         break;
      case SDLK_d:
         mAircraft.randomDamage();
         Log::msg("apply random damage to aircraft");
         break;
      default:
         break;
   }
}

void CApp::draw() const
{
   mTerrain.draw(mContext);
   mSky.draw(mContext);

   {
      glcxx::enable_blending_guard lock;
      for (const auto& s : mStars)
         s.draw(mContext);
   }

   mAircraft.draw(mContext);
}
