/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#include "Aircraft.hpp"

#include <algorithm>
#include <glcxx/capabilities.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Context.hpp"

CAircraft::CAircraft()
   : mFlames{{{0.8, 300, 100}, {0.8, 300, 100}}}
{
   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile("aersy-res/Su-35_SuperFlanker/Su-35_SuperFlanker.obj",
                                            aiProcess_CalcTangentSpace       |
                                            aiProcess_JoinIdenticalVertices  |
                                            aiProcess_SortByPType);
   if (!scene)
      throw std::runtime_error{"couldn't load aircraft model"};

   std::vector<std::shared_ptr<SMaterial>> materials;
   for (size_t m = 0; m < scene->mNumMaterials; ++m)
   {
      const aiMaterial* material = scene->mMaterials[m];
      assert(material);
      materials.push_back(std::make_shared<SMaterial>(*material, "Su-35_SuperFlanker/"));
   }

   for (size_t m = 0; m < scene->mNumMeshes; ++m)
   {
      const aiMesh* const mesh = scene->mMeshes[m];
      assert(mesh);
      mMeshes.emplace_back(*mesh, materials[mesh->mMaterialIndex]);
   }

   auto flameTex = make_texture("flame-sprite.png");
   for(auto& f : mFlames)
      f.texture(flameTex);

   auto explosionTex = make_texture("explosion-sprite.jpg");
   for(auto& r : mRockets)
   {
      r.flameTexture(flameTex);
      r.explosionTexture(explosionTex);
   }

   for (auto& h : mHealthBars)
      h.size({3, 0.5});
}

bool CAircraft::launchRocket()
{
   for (size_t i = 0; i < mRockets.size(); ++i)
   {
      if (!mRockets[i].active())
      {
         const glm::vec3& fwd = forward();
         mRockets[i].launch(pos() + (i%2 ? -3.f : +3.f) * right() - 2.5f*fwd - up(), fwd, mSpeed);
         return true;
      }
   }
   return false;
}

void CAircraft::update(float timeDelta)
{
   if (0 != mSpeed && pos().z <= 0)
   {
      mSpeed = 0;
      for (auto& h : mHealthBars)
         h.value(0);
   }

   const glm::vec3& fwd = forward();

   pos(pos() + fwd*mSpeed*timeDelta);

   // place healthbars
   for (size_t i = 0; i < mHealthBars.size(); ++i)
      mHealthBars[i].pos(pos() + (-5.f + 7*i)*fwd);

   for (auto& r : mRockets)
      r.update(timeDelta);

   // place flames in engines of this model
   const glm::vec3 flamePos = pos() - 5.5f*fwd - 0.15f*up();
   mFlames[0].pos(flamePos + 1.2f*right());
   mFlames[1].pos(flamePos - 1.2f*right());
   for(auto& f : mFlames)
   {
      f.rate(0.05f*mSpeed*mSpeed);
      f.dir(-fwd);
      f.sourceVelocity(mSpeed*fwd);
      f.update(timeDelta);
   }
}

void CAircraft::draw(const CContext& context) const
{
   auto& p = context.getProgram<cts("shaded_tex")>();
   p.set<cts("uModel")>(model());
   auto& p2 = context.getProgram<cts("shaded_tex_nmap")>();
   p2.set<cts("uModel")>(model());

   for (const auto& m: mMeshes)
      m.draw(context);

   for (const auto& f : mFlames)
      f.draw(context);

   for (const auto& r : mRockets)
      r.draw(context);

   if (context.mDrawHealthBars)
   {
      for (const auto& h : mHealthBars)
         h.draw(context);
   }
}

void CAircraft::randomDamage()
{
   std::uniform_real_distribution<float> d(0, 1);
   for (auto& h : mHealthBars)
      h.value(d(random_gen));
}

void CAircraft::repair()
{
   auto it = std::min_element(begin(mHealthBars), end(mHealthBars),
                              [] (const CHealthBar& a, const CHealthBar& b)
                              {
                                 return a.value() < b.value();
                              });
   assert(it != end(mHealthBars));
   it->value(std::min(it->value() + 0.5f, 1.f));
}
