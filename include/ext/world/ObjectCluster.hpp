//
//  ObjectCluster.hpp
//  PixEngine
//
//  Created by rodo on 25/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include "LayerVao.hpp"
#include "WorldMeta.hpp"
#include "ObjectShader.hpp"


namespace Pix {

	class World;

	class WorldObject;

	typedef struct sVisible {
		WorldObject *object;
		glm::mat4 transformMatrix;
	} Visible_t;

	class ObjectCluster : public LayerVao {

		friend class World;

		static std::string TAG;

		Texture2D *pTexture;

		bool bInited = false;
		ObjLoader *pLoader;

		std::vector<Visible_t> vVisibles;
		std::vector<Texture2D *> vTextures;
		glm::mat4 mPlacer;
// todo		std::vector<WorldObject *> vInstances;

	public:
		std::vector<WorldObject *> vInstances;

		const WorldConfig_t PLANET;
		const Transformation_t PLACER;
		const std::string NAME;
		World *WORLD;

		ObjectCluster(World *planet, std::string name, Transformation_t initialTransform = Transformation_t());

		virtual ~ObjectCluster();

		void add(WorldObject *object);

		void init();

		void render(ObjectShader *shader);
	};

}
