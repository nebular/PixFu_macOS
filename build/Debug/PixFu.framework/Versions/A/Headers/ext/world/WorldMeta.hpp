//
//  WorldMeta.hpp
//  The structs that hold world metadata
//  PixEngine
//
//  Created by rodo on 25/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include <string>
#include "StaticObject.hpp"
#include "Font.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace Pix {

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

	/**
	 * World configuration object
	 */
	typedef struct sWorldConfig {

		/** global background color */
		const glm::vec3 backgroundColor = {0.8, 0.8, 1};

		/** light position */
		const glm::vec3 lightPosition = {20000, 20000, 2000};

		/** light color */
		const glm::vec3 lightColor = {0.8, 0.8, 0.93};

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

	/** Initial object properties */
	typedef struct sObjectConfig {
		glm::vec3 position = {0, 0, 0};
		glm::vec3 rotation = {0, 0, 0};
		float radius = 1.0;
		float mass = 1.0;
	} ObjectConfig_t;

	/**
	 * Object definition + initial properties
	 */
	typedef struct sObjectFeatures {

		/** Object class name */
		std::string className;

		/** Initial properties */
		sObjectConfig config;

		/** Whether object is static */
		bool isStatic = true;

	} ObjectFeatures_t;

	/** Terrain Definition */
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

		/** render wireframe grid (if world has a canvas enabled) */
		const bool wireframe = false;

		/** use a provided mesh instead of loading one */
		const Static3DObject_t *staticMesh = nullptr;

	} TerrainConfig_t;


	/**
	 Base class for any object in the world. This is an abstract class.
	 You will extend this class to provide physics to your objects.
	 */

	class WorldObject {

	protected:

		/** World Configuration */
		const WorldConfig_t &PLANET;

	public:

		/** Object Classname (maps to resources) */
		const std::string CLASS;

		inline WorldObject(const WorldConfig_t &worldConfig, std::string objectClass) : PLANET(worldConfig), CLASS(objectClass) {}

		inline virtual ~WorldObject() = default;

		/**
		 * Return object NORMALIZED position (warning)
		 * @return The object normalized position
		 */
		virtual glm::vec3 pos() = 0;

		/**
		 * Return object rotation around xyz axis
		 * @return The rotation vector in radians
		 */
		virtual glm::vec3 rot() = 0;

		/**
		 * Return object radius
		 * @return Radius in World Coordinates
		 */
		virtual float radius() = 0;

	};

	/**
	 * Convenience class for a static world object without any physics. Will just get rendered
	 * in a static location that cannot be changed.
	 */

	class WorldStaticObject : public WorldObject {

		const ObjectFeatures_t CONFIG;

	public:

		inline WorldStaticObject(const WorldConfig_t &worldConfig, const ObjectFeatures_t objectConfig)
				: WorldObject(worldConfig, objectConfig.className), CONFIG(std::move(objectConfig)) {

		}

		inline glm::vec3 pos() override { return CONFIG.config.position / 1000.0f; }

		inline glm::vec3 rot() override { return CONFIG.config.rotation; }

		inline float radius() override { return CONFIG.config.radius; }
	};


}
