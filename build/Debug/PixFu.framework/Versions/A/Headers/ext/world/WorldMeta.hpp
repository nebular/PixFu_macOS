//
//  WorldMeta.hpp
//
//  The structs that hold world metadata
//  PixFu Engine
//
//  Created by rodo on 25/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include <string>
#include <map>
#include "StaticObject.hpp"
#include "Font.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace Pix {

	// convenience to create a transformation matrix
	glm::mat4 createTransformationMatrix(glm::vec3 translation, float rxrads, float ryrads, float rzrads,
										 float scale, bool flipX, bool flipY, bool flipZ);

	/**
	 * Perspective presets (with initial camera height / pitch)
	 */

	typedef struct sPerspective {
		const float FOV;
		const float NEAR_PLANE;
		const float FAR_PLANE;
		const float C_HEIGHT = 0.2;
		const float C_PITCH = 0;
	} Perspective_t;

	static constexpr Perspective_t PERSP_FOV90 = {90, 0.005, 1000.0, 0.25};
	static constexpr Perspective_t PERSP_FOV70 = {70, 0.005, 1000.0, 0.25};

/**
	 * defines an initiali transforation for the world and objects
	 */

	typedef struct sTransformation {

		/** Translation vector */
		glm::vec3 position = {0, 0, 0};

		/** Rotation Vector */
		glm::vec3 rotation = {0, 0, 0};

		/** Scale */
		float scale = 1.0;

		/** Flip around axis */
		bool flipx = false, flipy = false, flipz = false;

		/** Return as transformation matrix */
		inline glm::mat4 toMatrix() const {
			return createTransformationMatrix(
					position,
					rotation.x, rotation.y, rotation.z,
					scale, flipx, flipy, flipz
			);
		}
	} Transformation_t;

	typedef enum eWorldDebug {
		DEBUG_NONE, DEBUG_GRID, DEBUG_COLLISIONS, DEBUG_WIREFRAME
	} WorldDebug_t;

	/**
	 * World configuration object. It is used to instantiate the world class and
	 * contains the root parameters: lighting, background ...
	 */

	typedef const struct sWorldConfig {

		/** global background color */
		const glm::vec3 backgroundColor = {0.8, 0.8, 1};

		/** light position */
		const glm::vec3 lightPosition = {20000, 20000, 2000};

		/** light color */
		const glm::vec3 lightColor = {0.8, 0.8, 0.93};

		/** debug mode */
		const WorldDebug_t debugMode = DEBUG_NONE;

		/** Perspective */
		const Perspective_t perspective = PERSP_FOV70;

		/** the global world objects transform to flip their axis, etc */
		const Transformation_t worldTransform;

		/** the global terrain transform to flip axis, etc */
		const Transformation_t terrainTransform;

		/** whether to create a 3d canvas to draw over the terrain */
		const bool withCanvas = true;

		/** determines which font to use (requires withCanvas) */
		const FontInfo_t withFont = {};

		/** determines which shader to use (assets) */
		const std::string shaderName = "world";

	} WorldConfig_t;

	//
	// Metadata for objects
	//

	/**
	 * A simple intrinsic object animation that will rotate the object around
	 * all axis over time, and scale with a sin function
	 */
	typedef struct sObjectAnimation {
		/** animation is enabled */
		bool enabled = false;
		// delta animations on every axis
		float deltaRotationX = 0, deltaRotationY = 0, deltaRotationZ = 0;
		// sin() animation on scale (0.1 = 10% size variation)
		float scalePulse = 0;
	} ObjectAnimation_t;

	/**
	 * Rough approximation to global resistance as acceleration penalty constant
	 * when object is in the air, air will be chosen, and vice versa
	 */

	typedef const struct sObjectAerodynamics {
		const float terrain = 0.8;
		const float air = 0.95;
		const float air_vertical = 0.9;
	} ObjectAerodynamics_t;

	/**
	 * Packs behavior of object when crawling through terrain
	 */
	typedef struct sObjectTerrain {
		const float SCRATCHING_NEW = 0.6;
		const float CLIMB_LIMIT = 0.3;
		const float FALL_LIMIT = 0.3;
		const float RIDEHEIGHT_SEAMLESS = 5;	// ignore this height difference
	} ObjectTerrainBehavior_t;


	/**
	 *
	 * The static Object properties. This is used to insert an object into the world.
	 * The class ObjectDB can store these structs ("all game objects") then you can also
	 * insert the objects just using the OID (this struct will be retrieved from the DB)
	 *
	 */

	typedef const struct sObjectProperties {

		/** Object class name determines what assets to use */
		const std::string CLASSNAME;

		/** Object Radius in world units */
		const float radius = 1.0;

		/** Object mass in kg */
		const float mass = 1.0;

		/** Elasticity (bounce on fall) */
		const float elasticity = 0.8;

		/** energy loss on crash*/
		const float crashEfficiency = 0.75;

		/** total resistance to air and terrain (rough approximation) */
		const ObjectAerodynamics_t aero = {};

		/** Terrain friction etc */
		const ObjectTerrainBehavior_t terrain = {};

		/** Intrinsic Animation */
		const ObjectAnimation_t animation = {};

		/** Whether object is static */
		const bool ISSTATIC = false;

		/**
		 * Exaggerte draw radius (not collision radius), to approximate non-spherical objects:
		 * For example, if you have a very tall tree, you may set the real radius to that of the trunk
		 * and multiply the drawing scale 5 times. This way, collisions will only happen hen hitting the
		 * trunk.
		 */
		const float drawRadiusMultiplier = 1.0;

	} ObjectProperties_t;

	/**
	 * Used to provide optional initial positions and speeds when adding objets to the world
	 * and also as defaults to store presets in the DB
	 */

	typedef struct sObjectLocation {
		glm::vec3 position = {0, 0, 0};
		glm::vec3 rotation = {0, 0, 0};
		glm::vec3 initialSpeed = {0, 0, 0};
		glm::vec3 initialAcceleration = {0, 0, 0};
	} ObjectLocation_t;

	/**
	 * The ObjectDB maps an OID to a pair <Properties,InitialLocation>. When inserting an
	 * object to the world, you can suply another ObjectLocation to override this one
	 */

	typedef std::pair<ObjectProperties_t, ObjectLocation_t> ObjectDbEntry_t;

	/**
	 * A database of objects, maps an OID to a to a pair <Properties,InitialLocation>
	 */

	class ObjectDb {

		static std::map<int, ObjectDbEntry_t> Database;

	public:

		/*
		//		static const std::map<int, int> MapCircuitSprites;                    // 1000kg mass of the player car
		//		static ObjectFeatures_t  const *getCircuitGroundSprite(int circuitObjectOid);
		*/

		inline static void insert(int code, ObjectProperties_t objectMeta, ObjectLocation_t initialPosition = {}) {
			Database.insert({code, {objectMeta, initialPosition}});
		}


		inline static ObjectDbEntry_t const *get(int oid) {
			return &Database.at(oid);
		}

	};

	/**
	 * Terrain definition. Used to add terrains to the world.
	 */

	typedef struct sTerrainConfig {

		/** determines also where the resources are */
		const std::string name;

		/** world can hold several terrains */
		const glm::vec2 origin = {0, 0};

		/**
		 * sync Heightmap heights with the terrain. Terrain has to be pre-scaled, this is just only
		 * to sync the heightmap to whatever heght scale the terrain was created with.
		 */
		const float scaleHeight = 0.1;

		/** use a provided mesh instead of loading one */
		const Static3DObject_t *staticMesh = nullptr;

	} TerrainConfig_t;


	/**
	 Base class for any object in the world. This is an abstract class.
	 You will extend this class to provide physics to your objects.
	 */

	class World;

	class WorldObjectBase {

		static int instanceCounter;

	protected:

		/** World Configuration */
		const WorldConfig_t &WORLD;

	public:

		const unsigned CLASSID;

		/** Object ID */
		const int ID;

		/** Object Classname (maps to resources) */
		const std::string CLASS;

		inline WorldObjectBase(const WorldConfig_t &worldConfig, std::string objectClass, unsigned classId, int overrideId = -1) :
				WORLD(worldConfig),
				CLASSID(classId),
				ID(overrideId >= 0 ? overrideId : instanceCounter++),
				CLASS(objectClass) {}

		inline virtual ~WorldObjectBase() = default;

		/**
		 * Return object position
		 * @return The object position
		 */
		virtual glm::vec3 &pos() = 0;

		/**
		 * Return object rotation around xyz axis
		 * @return The rotation vector in radians
		 */
		virtual glm::vec3 &rot() = 0;

		/**
		 * Return object radius
		 * @return Radius in World Coordinates
		 */
		virtual float radius() = 0;

		/**
		 * Return object radius
		 * @return Radius in World Coordinates
		 */
		virtual float drawRadius() = 0;

	};

	/**
	 * Convenience class for a static world object without any physics. Will just get rendered
	 * in a static location.
	 */

	class WorldObject : public WorldObjectBase {


	protected:

		float fRadiusAnimator = 0;

		// Object Location
		ObjectLocation_t LOCATION;

	public:

		static constexpr unsigned CLASSID_CODE = 1;

		// Object metadata
		const ObjectProperties_t CONFIG;

		inline WorldObject(const WorldConfig_t &worldConfig, const ObjectProperties_t objectMeta, ObjectLocation_t location,
						   unsigned int classid = CLASSID_CODE, int overrideId = -1) :
				WorldObjectBase(worldConfig, objectMeta.CLASSNAME, classid, overrideId),
				LOCATION(std::move(location)),
				CONFIG(std::move(objectMeta))
				{}

		inline virtual glm::vec3 &pos() override { return LOCATION.position; }

		inline virtual glm::vec3 &rot() override { return LOCATION.rotation; }

		inline virtual float radius() override { return CONFIG.radius; }

		inline virtual float drawRadius() override { return CONFIG.radius * CONFIG.drawRadiusMultiplier / 1000.0F; }

		/** process animations */
		virtual void process(World *world, float fElapsedTime);
	};
}
