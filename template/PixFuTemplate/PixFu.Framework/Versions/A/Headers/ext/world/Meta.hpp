//
//  GameObjects.h
//  LoneKart
//
//  Created by rodo on 19/01/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#define NOHEIGHT -1
#define NOMASS -1

#include <map>

namespace Pix {
/* todo arena
	typedef struct sObjectPlacement {
		glm::vec3 position;
		float radius;
		int oid;
	} ObjectPlacement_t;
 */

	typedef struct sObjectInfo {
		int oid;
		std::string name;
		float animationSpeed = 10.0;

		float radius = 0.1, mass = 10.0, angle = 0;
		float initialAcceleration = 0;
		float initialHeight = NOHEIGHT;
	} ObjectInfo_t;

// The Static Database of objects, defined in the Meta/ dir
// Provides methods to get Ground Sprites

	class GameObjectsDb {

		static const std::map<int, ObjectInfo_t> MapGroundSprites;
		static const std::map<int, int> MapCircuitSprites;                    // 1000kg mass of the player car

	public:

		static ObjectInfo_t const *getCircuitGroundSprite(int circuitObjectOid);

		static ObjectInfo_t const *getGroundSprite(int groundObjectOid);

	};
}
