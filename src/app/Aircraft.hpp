/*
 * Copyright 2015 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP

#include "ComplexRenderable.hpp"
#include "Rect.hpp"

/// @brief simple model of aircraft made out of boxes
class CAircraft : public CComplexRenderable
{
      /// @brief propeller, we need this field separately to apply animation to
      /// it, raw pointer because ownership of objects is private and managed by
      /// CComplexRenderable
      CComplexRenderable* mProp;

      /// @brief healthbar, raw pointer because ownership of objects is private
      /// and managed by CComplexRenderable
      using tHealthBar = std::pair<CHealthBar, IRenderableModel*>;

      /// @brief healthbar list
      std::vector<tHealthBar> mHealthBars;

   public:
      /// @brief constructor
      CAircraft();

      /// @brief randomly decreases healthbars
      void randomDamage();

      /// @brief repair most damaged detail by 50%
      void repair();

      /// @brief update, animates propeller
      void update(float timeDelta);

      /// @brief draw
      void draw(const SContext& context) const override;
};

#endif // AIRCRAFT_HPP
