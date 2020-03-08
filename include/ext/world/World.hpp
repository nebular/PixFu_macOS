// Base class for a 3d world
//
// Provides methods to add terrains and objects
// and has a camera and lighting. This class does not know about
// physics, that is the task of derived classes.
//
// Created by rodo on 2020-02-17.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma once

#include "WorldMeta.hpp"
#include "Terrain.hpp"
#include "ObjectCluster.hpp"

#include <vector>
#include <map>
#include <cmath>

namespace Pix {

//////////////////////////////////////////////////////////////////////////////////////////

	glm::mat4 createTransformationMatrix(glm::vec3 translation, float rxrads, float ryrads, float rzrads,
										 float scale, bool flipX, bool flipY, bool flipZ);

	// Base World class
	class World : public FuExtension {

		static std::string TAG;

		/** Shader for terrain */
		TerrainShader *pShader;
		
		/** Shader for objects */
		ObjectShader *pShaderObjects;

		/** World Light */
		Light *pLight;

		/** World Camera */
		Camera *pCamera;

		/** Object Clusters */
		std::map<std::string, ObjectCluster *> mClusters;

	protected:

		/** The current projection matrix */
		glm::mat4 projectionMatrix;

		/** Object Clusters */
		std::vector<ObjectCluster *> vObjects;
		
		/** Terrains */
		std::vector<Terrain *> vTerrains;

		/**
		 * Intits the extension
		 * @param engine The FU engine
		 */
		virtual bool init(Fu *engine);

		/**
		 * Ticks the extension
		 * @param engine The FU engine
		 * @param fElapsedTime The frame time
		 */
		
		virtual void tick(Fu *engine, float fElapsedTime);

		/**
		 * Adds a terrain to the world
		 * @param terrainConfig The therrain configuration object
		 */

		Terrain *add(TerrainConfig_t terrainConfig);

		/**
		 * Adds a pre-created object to the world
		 * @param object The object to add
		 */
		
		void add(WorldObject *object, bool setHeight = true);

		/**
		 * Creates an object from its metadata and Adds it to the world
		 * @param object The object to add
		 * @param setHeight whether to set object height to terrain height
		 * @return The added object
		 */

		virtual WorldObject *add(ObjectProperties_t object, ObjectLocation_t location, bool setHeight);

		/**
		 * Creates an object from irs OID. Object must have been inserted in the ObjectDb
		 * with that OID. Object is added to the world at the supplied location.
		 * @param oid Object OID as inserted into the ObjecctDb
		 * @param location Object location info
		 * @param setHeight whether to set ground height
		 * @return The object
		 */

		virtual WorldObject *add(int oid, ObjectLocation_t location, bool setHeight);
		
		/**
		 * Creates an object from irs OID. Object must have been inserted in the ObjectDb
		 * with that OID. Initial location from the DB is used.
		 * @param oid Object OID as inserted into the ObjecctDb
		 * @param setHeight whether to set ground height
		 * @return The object
		 */

		virtual WorldObject *add(int oid, bool setHeight);
		
		/**
		 * Iterates all world objects
		 * @param callback The callback
		 */

		template<typename Func>
		void iterateObjects(Func callback) {
			for (ObjectCluster *cluster:vObjects) {
				std::for_each(cluster->vInstances.begin(), cluster->vInstances.end(), callback);
			}
		}

	public:

		/** Do not transform, use vertex data as-is */
		static constexpr Transformation_t TRANSFORM_NONE = {};

		/** Flip X axis */
		static constexpr Transformation_t TRANSFORM_FLIPX = {
				{0, 0, 0},                // global translation
				{0, 0, 0},                // global rotation
				1.0,                    // global scale
				true, false, false        // global xyz flip
		};

		/** Flip X and Y axis */
		static constexpr Transformation_t TRANSFORM_FLIPXY = {
				{0, 0, 0},                // global translation
				{0, 0, 0},                // global rotation
				1.0,                    // global scale
				true, true, false        // global xyz flip
		};

		/** Flip Y axis */
		static constexpr Transformation_t TRANSFORM_FLIPY = {
				{0, 0, 0},                // global translation
				{0, 0, 0},                // global rotation
				1.0,                    // global scale
				false, true, false        // global xyz flip
		};

		/** Flip Z axis */
		static constexpr Transformation_t TRANSFORM_FLIPZ = {
				{0, 0, 0},                // global translation
				{0, 0, 0},                // global rotation
				1.0,                    // global scale
				false, false, true        // global xyz flip
		};

		/** Flip Z axis and rotates PI/2 */
		static constexpr Transformation_t TRANSFORM_FLIPZ_ROT = {
				{0, 0, 0},                // global translation
				{0, M_PI / 2, 0},            // global rotation
				1.0,                    // global scale
				false, false, 0        // global xyz flip
		};

		/** Immutable config */
		const WorldConfig_t CONFIG;

		World(WorldConfig_t& config);
		virtual ~World();

		/**
		 Gets camera
		 @return The world camera
		 */

		Camera *camera();

		/**
		 * Gets the projection matrix in use
		 * @return The projection atix in use
		 */

		glm::mat4 getProjectionMatrix();

		/**
		 * Looks up the terrain height (+Y) for a world position. Height will be adjusted using the height scale
		 * parameter passed on the TerrainConfig object.
		 * @param posWorld Position to check
		 * @return height in world coordinates
		 */

		float getHeight(glm::vec3& posWorld);
		
		/**
		 * Whether there is a terrain at that world coords.
		 * @param posWorld Position to check
		 * @return whether
		 */

		bool hasTerrain(glm::vec3& posWorld);

		/**
		 * Selects an object using raytracing (nehavior is object dependent)
		 *
		 * @param rayDirection Normalized Ray from the camera
		 * @param exclusive Whether to select all objects
		 * @return The object pointed by the ray, if any.
		 */

		WorldObject *select(glm::vec3& rayDirection, bool exclusive = true);

		/**
		 * selects/unselects all objects.
		 */

		void selectAll(bool select=true);

		/**
		 * Gets the 3D canvas. A
		 * @param posWorld Any world position. As we can have several terrains, we need to supply this world coordinates
		 * so the engine knows what terrain canvas to return (each terrain has a 3D canvas)
		 * @return The 3D canvas
		 */

		Canvas2D *canvas(glm::vec3& posWorld);

		/**
		 * Convenience function to return the 3D canvas of the first terrain.
		 * @return The 3D canvas
		 */
		Canvas2D *canvas();

	};

	//
	///////// INLINE IMPLEMENTATION
	//

	inline Camera *World::camera() { return pCamera; }

	inline float World::getHeight(glm::vec3 &posWorld) {

		if (vTerrains.size() == 1)
			return vTerrains[0]->getHeight(posWorld);

		for (Terrain *terrain:vTerrains) {
			if (terrain->contains(posWorld))
				return terrain->getHeight(posWorld);
		}

		return 0;
	}

	inline glm::mat4 World::getProjectionMatrix() {
		return projectionMatrix;
	}

	inline bool World::hasTerrain(glm::vec3 &posWorld) {

		if (vTerrains.size()==1)
			return vTerrains[0]->contains(posWorld);

		for (Terrain *terrain:vTerrains)
			if (terrain->contains(posWorld))
				return true;

		return false;
	}

	inline Canvas2D *World::canvas(glm::vec3 &posWorld) {

		if (vTerrains.size() == 1)
			return vTerrains[0]->canvas();

		for (Terrain *terrain:vTerrains) {
			if (terrain->contains(posWorld))
				return terrain->canvas();
		}

		return nullptr;
	}

	inline Canvas2D *World::canvas() {
		return vTerrains[0]->canvas();
	}

	inline WorldObject *World::add(int oid, ObjectLocation_t location, bool setHeight) {
		const ObjectDbEntry_t *entry = ObjectDb::get(oid);
		return add(entry->first, location, setHeight);
	}

	inline WorldObject *World::add(int oid, bool setHeight) {
		const ObjectDbEntry_t *entry = ObjectDb::get(oid);
		return add(entry->first, entry->second, setHeight);
	}

}

#pragma clang diagnostic pop
