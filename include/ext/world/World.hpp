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

namespace rgl {

//////////////////////////////////////////////////////////////////////////////////////////
	glm::mat4 createTransformationMatrix(glm::vec3 translation, float rxrads, float ryrads, float rzrads,
						   float scale, bool flipX, bool flipY, bool flipZ);

	// Base World class
	class World : public PixFuExtension {

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

		virtual bool init(PixFu *engine);

		virtual void tick(PixFu *engine, float fElapsedTime);

		void add(TerrainConfig_t terrainConfig);

		void add(WorldObject *object);

		template<typename Func>
		void iterateObjects(Func callback) {
			for (ObjectCluster *cluster:vObjects) {
				std::for_each(cluster->vInstances.begin(), cluster->vInstances.end(), callback);
			}
		}


	public:

		const Perspective_t PERSPECTIVE;
		const WorldConfig_t CONFIG;

		static constexpr Perspective_t PERSP_FOV90_LOW = {90, 0.005, 0.1, 0.25};
		static constexpr Perspective_t PERSP_FOV90_MID = {90, 0.005, 100.0, 0.25};
		static constexpr Perspective_t PERSP_FOV90_FAR = {90, 0.5, 1000.0, 0.25};
		static constexpr Perspective_t PERSP_FOV60_LOW = {90, 0.005, 0.1, 0.25};
		static constexpr Perspective_t PERSP_FOV60_MID = {70, 0.005, 1000.0, 0.25};
		static constexpr Perspective_t PERSP_FOV60_FAR = {60, 0.5, 1000.0, 0.25};

		
		static constexpr Transformation_t TRANSFORM_NONE = {};
		static constexpr Transformation_t TRANSFORM_FLIPX = {
			{0,0,0},				// global translation
			{0,0,0},				// global rotation
			1.0,					// global scale
			true, false, false		// global xyz flip
		};
		static constexpr Transformation_t TRANSFORM_FLIPXY = {
			{0,0,0},				// global translation
			{0,0,0},				// global rotation
			1.0,					// global scale
			true, true, false		// global xyz flip
		};
		static constexpr Transformation_t TRANSFORM_FLIPY = {
			{0,0,0},				// global translation
			{0,0,0},				// global rotation
			1.0,					// global scale
			false, true, false		// global xyz flip
		};
		
		static constexpr Transformation_t TRANSFORM_FLIPZ = {
			{0,0,0},				// global translation
			{0,0,0},				// global rotation
			1.0,					// global scale
			false, false, true		// global xyz flip
		};

		static constexpr Transformation_t TRANSFORM_FLIPZ_ROT = {
			{0,0,0},				// global translation
			{0,M_PI/2,0},			// global rotation
			1.0,					// global scale
			false, false, 0		// global xyz flip
		};

		World(WorldConfig_t config, Perspective_t perspective = PERSP_FOV90_LOW);

		virtual ~World();

		// get camera
		Camera *camera();

		// query heightmap
		float getHeight(glm::vec3 &posWorld);

		// get 3d canvas
		Canvas2D *canvas(glm::vec3 &posWorld);

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

}

#pragma clang diagnostic pop
