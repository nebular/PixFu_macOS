//
//  BallCollisions.hpp
//  All this learnt from onelonecoder.com masterclass on Ball physics
//
//  Created by rodo on 25/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma once

#include "World.hpp"
#include "BallWorldMap.hpp"
#include "LineSegment.hpp"
#include <vector>

namespace Pix {

	class Ball;

	class BallWorld : public World {

		inline const static std::string TAG = "BallWorld";

	protected:

		BallWorldMap_t *pMap = nullptr;

		std::vector<Ball *> vFakeBalls;
		std::vector<std::pair<Ball *, Ball *>> vCollidingPairs;
		std::vector<std::pair<Ball *, Ball *>> vFutureColliders;

		/**
		 * Add Balls to the world
		 */

		WorldObject *add(ObjectProperties_t features, ObjectLocation_t location, bool setHeight) override;

		/**
		 * Creates an object from irs OID. Object must have been inserted in the ObjectDb
		 * with that OID. Object is added to the world at the supplied location.
		 * @param oid Object OID as inserted into the ObjecctDb
		 * @param location Object location info
		 * @param setHeight whether to set ground height
		 * @return The object
		 */

		WorldObject *add(int oid, ObjectLocation_t location, bool setHeight = true) override;
		
		/**
		 * Creates an object from irs OID. Object must have been inserted in the ObjectDb
		 * with that OID. Initial location from the DB is used.
		 * @param oid Object OID as inserted into the ObjecctDb
		 * @param setHeight whether to set ground height
		 * @return The object
		 */

		WorldObject *add(int oid, bool setHeight = true) override;
		
		/**
		 * Processes ball updates and collisions.
		 */

		long processCollisions(float fElapsedTime);

		// process static collisions
		void processStaticCollision(Ball *ball, Ball *target);

		// process dynamic collisions
		void processDynamicCollision(Ball *b1, Ball *b2, float fElapsedTime);

	public:

		BallWorld(const std::string& levelName, WorldConfig_t& config);

		virtual void tick(Pix::Fu *engine, float fElapsedTime) override;

		void load(const std::string& levelName);
	};

	inline WorldObject *BallWorld::add(int oid, ObjectLocation_t location, bool setHeight) {
		return World::add(oid, location, setHeight);
	}

	inline WorldObject *BallWorld::add(int oid, bool setHeight) {
		return World::add(oid, setHeight);
	}

}

#pragma clang diagnostic pop
