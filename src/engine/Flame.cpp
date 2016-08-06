/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "Flame.hpp"
#include <glcxx/capabilities.hpp>
#include "Context.hpp"

CJetFlame::CJetFlame(float radius, float rate, float particleSpeed)
   : tParticleSystem(rate, 0.6*rate)
   , mParticleSpeed(particleSpeed)
{
   size({radius, radius});
   mAtlasSize = {4, 4};
}

void CJetFlame::update(float timeDelta)
{
   tParticleSystem::update(*this, timeDelta);
   mVAO.upload<cts("aPos")>(mPositions.data(), mPositions.size(), GL_STREAM_DRAW);
   mVAO.upload<cts("aSpeed")>(mSpeeds.data(), mSpeeds.size(), GL_STREAM_DRAW);
   mVAO.upload<cts("aTime")>(mTimes.data(), mTimes.size(), GL_STREAM_DRAW);
}

void CJetFlame::draw(const CContext& context) const
{
   if (hasAliveParticles())
   {
      auto& p = context.getProgram<cts("particlesys_tex_sprite_flame")>();
      p.set<cts("uSize")>(mSize);
      p.set<cts("uAtlasSize")>(mAtlasSize);
      p.set<cts("uTexture")>(mTexture);
      glcxx::disable_depth_mask_guard dtLock;
      glcxx::enable_blending_guard bLock(GL_SRC_ALPHA, GL_ONE);
      p.draw_arrays(mVAO, aliveParticleNum(), GL_POINTS);
   }
}
