/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_ANIMATIONOBJECT_HPP
#define ENGINE_ANIMATIONOBJECT_HPP

class CAnimationObject
{
   protected:
      /// @brief number of frames in current object
      unsigned int mFrameNumber = 16;

      /// @brief local time measured in frames
      float mCurrentFrame = 0.f;

      /// @brief number of completed animation cycles
      unsigned int mCycles = 0;

   public:
      /// @brief set total frame number
      void frameNumber(unsigned int frameNumber) { mFrameNumber = frameNumber; }

      /// @brief return number of completed cycles
      unsigned int cycles() const { return mCycles; }

      /// @brief reset animation
      void reset()
      {
         mCurrentFrame = 0.f;
         mCycles = 0;
      }

      /// @brief update current frame
      void update(float timeDelta)
      {
         static int fps = 30;
         mCurrentFrame += fps*timeDelta;
         const unsigned int cyclesPassed = int(mCurrentFrame/mFrameNumber);
         mCycles += cyclesPassed;
         mCurrentFrame -= mFrameNumber*cyclesPassed;
      }
};

class CAtlasedAnimationObject : protected CAnimationObject
{
   protected:
      /// @brief number of frames in atlas by each axis
      glm::ivec2 mAtlasSize = {4, 4};

      /// @brief current atlas position
      glm::ivec2 mAtlasPos;

      /// @brief texture
      texture_ptr mTexture;

   public:
      using CAnimationObject::cycles;

      /// @brief set size of the atlas
      void atlasSize(glm::ivec2 atlasSize)
      {
         mAtlasSize = atlasSize;
         mFrameNumber = atlasSize.x*atlasSize.y;
      }

      /// @brief set texture
      void texture(texture_ptr tex) { mTexture = std::move(tex); }

      /// @brief reset animation
      void reset()
      {
         CAnimationObject::reset();
         mAtlasPos = {0, 0};
      }

      /// @brief update current atlas pos
      void update(float timeDelta)
      {
         CAnimationObject::update(timeDelta);
         mAtlasPos = {int(mCurrentFrame) % mAtlasSize.x, mCurrentFrame/mAtlasSize.x};
      }
};

#endif
