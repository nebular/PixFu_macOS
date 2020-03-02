//
//  ObjectLoader.hpp
//  PixEngine
//
//  OBJ_Loader (third party) wrapper.
//
//  Created by rodo on 19/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include <vector>
#include <string>

#include "WorldMeta.hpp"

namespace objl {
	class Loader;
}

namespace Pix {

	class ObjLoader {

		objl::Loader *pLoader;

	public:

		/**
		 * Loads and parses a Wavefront OBJ Model
		 * @param name Object name. The model is expected on assets /objects/<name>/<name>.obj
		 */
		ObjLoader(std::string name);

		/**
		 * Loads a model from a static struct
		 * @param object a static object.
		 */

		ObjLoader(const Static3DObject_t *object);

		/**
		 * Return object vertices
		 * @param mesh Mesh number (0 default)
		 * @return The vertices
		 */
		float *vertices(int mesh = 0);

		/**
		 * Return number of vertices in the model
		 * @param mesh Mesh number (0 default)
		 * @return The number of vertices
		 */
		unsigned verticesCount(int mesh = 0);

		/**
		 * Return object indices
		 * @param mesh Mesh number (0 default)
		 * @return The indices
		 */
		unsigned *indices(int mesh = 0);

		/**
		 * Return number of indices in the model
		 * @param mesh Mesh number (0 default)
		 * @return The number of indices
		 */
		unsigned indicesCount(int mesh = 0);

		/**
		 * Return number of meshes in the model
		 * @return Number of meshes
		 */

		unsigned meshCount();
	};

}
