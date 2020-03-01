//
//  ObjectLoader.hpp
//  PixEngine
//
//  Created by rodo on 19/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include <vector>
#include <string>

namespace objl {
	class Loader;
}

namespace Pix {

	class ObjLoader {

		objl::Loader *pLoader;

	public:

		ObjLoader(std::string name);

		float *vertices(int mesh = 0);

		unsigned verticesCount(int mesh = 0);

		unsigned *indices(int mesh = 0);

		unsigned indicesCount(int mesh = 0);

		unsigned meshCount();
	};

}
