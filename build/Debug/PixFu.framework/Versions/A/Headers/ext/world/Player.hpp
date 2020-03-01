//
// Created by rodo on 2020-02-20.
//

#pragma once

#include "Ball.hpp"
#include "Meta.hpp"
#include "PlayerFeatures.hpp"

namespace Pix {

	typedef struct sWorldMeta {
		std::string name;
		int xMas, yMax;
	} WorldMeta_t;

	typedef struct sObjectFeatures {
		std::string className;
		sObjectConfig config;
		bool isStatic = true;
	} ObjectFeatures_t;

	class Player : public Ball {

//		friend class Arena;

		const PlayerFeatures *FEATURES;

		float fAcceleration = 0;
		float fCalcDirection = 0;
//		float fSteerAngle = 0;

	public:
		float fSteerAngle = 0;

		Player(World *world, PlayerFeatures_t features);

		void process(World *world, float fTime);

		void steer(float angle, float fElapsedTime);

		void accelerate(float percentage, float fElapsedTime);

		void brake(float percentage, float fElapsedTime);

		void jump();

		float speedPercent();
	};

	// speed percent r/max speed
	inline float Player::speedPercent() { return FEATURES->speedPercent(speed()); }

	class GameObject : public Ball {
	public:
		GameObject(World *world, ObjectFeatures_t features)
				: Ball(world, features.className, features.config.position,
					   features.config.radius, features.config.mass, features.isStatic) {}
	};
}
