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

		TerrainShader *pShader;
		ObjectShader *pShaderObjects;

		glm::mat4 projectionMatrix;

		Light *pLight;
		Camera *pCamera;

		std::map<std::string, ObjectCluster *> mCluesters;

	protected:

		std::vector<ObjectCluster *> vObjects;
		std::vector<Terrain *> vTerrains;
		
		virtual bool init(Fu *engine);

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

		virtual WorldObject *add(ObjectProperties_t object, ObjectLocation_t location, bool setHeight = true);

		/**
		 * Creates an object from irs OID. Object must have been inserted in the ObjectDb
		 * with that OID. Object is added to the world at the supplied location.
		 * @param oid Object OID as inserted into the ObjecctDb
		 * @param location Object location info
		 * @param setHeight whether to set ground height
		 * @return The object
		 */

		virtual WorldObject *add(int oid, ObjectLocation_t location, bool setHeight = true);
		
		/**
		 * Creates an object from irs OID. Object must have been inserted in the ObjectDb
		 * with that OID. Initial location from the DB is used.
		 * @param oid Object OID as inserted into the ObjecctDb
		 * @param setHeight whether to set ground height
		 * @return The object
		 */

		virtual WorldObject *add(int oid, bool setHeight = true);
		
		template<typename Func>
		void iterateObjects(Func callback) {
			for (ObjectCluster *cluster:vObjects) {
				std::for_each(cluster->vInstances.begin(), cluster->vInstances.end(), callback);
			}
		}


	public:

		const WorldConfig_t CONFIG;
		static float METRONOME;

		static constexpr Transformation_t TRANSFORM_NONE = {};
		static constexpr Transformation_t TRANSFORM_FLIPX = {
				{0, 0, 0},                // global translation
				{0, 0, 0},                // global rotation
				1.0,                    // global scale
				true, false, false        // global xyz flip
		};
		static constexpr Transformation_t TRANSFORM_FLIPXY = {
				{0, 0, 0},                // global translation
				{0, 0, 0},                // global rotation
				1.0,                    // global scale
				true, true, false        // global xyz flip
		};
		static constexpr Transformation_t TRANSFORM_FLIPY = {
				{0, 0, 0},                // global translation
				{0, 0, 0},                // global rotation
				1.0,                    // global scale
				false, true, false        // global xyz flip
		};

		static constexpr Transformation_t TRANSFORM_FLIPZ = {
				{0, 0, 0},                // global translation
				{0, 0, 0},                // global rotation
				1.0,                    // global scale
				false, false, true        // global xyz flip
		};

		static constexpr Transformation_t TRANSFORM_FLIPZ_ROT = {
				{0, 0, 0},                // global translation
				{0, M_PI / 2, 0},            // global rotation
				1.0,                    // global scale
				false, false, 0        // global xyz flip
		};

		World(WorldConfig_t config);

		virtual ~World();

		/**
		 Gets camera
		 @return camera
		 */
		Camera *camera();

		/**
		 * Looks up the terrain height (+Y) for a world position. Height will be adjusted using the height scale
		 * parameter passed on the TerrainConfig object.
		 * @param posWorld Position to check
		 * @return height in world coordinates
		 */

		float getHeight(glm::vec3 &posWorld);

		/**
		 * Gets the 3D canvas. A
		 * @param posWorld Any world position. As we can have several terrains, we need to supply this world coordinates
		 * so the engine knows what terrain canvas to return (each terrain has a 3D canvas)
		 * @return The 3D canvas
		 */

		Canvas2D *canvas(glm::vec3 &posWorld);

		/**
		 * Convenience function to return the 3D canvas of the first terrain.
		 * @return The 3D canvas
		 */
		Canvas2D *canvas();

	};

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
