//
//  StaticObject.hpp
//  PixEngine
//
//  Created by rodo on 02/03/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

namespace Pix {

	typedef struct sStatic3DObject {
		unsigned indicesCount;
		unsigned *indices;
		unsigned verticesCount;
		float *vertices;
	} Static3DObject_t;

	// these are wrong, instead of colors we have to provide the normals !!
	const Static3DObject_t PLANEXY = {
			6,
			(unsigned *) (new unsigned[6]{
					0, 1, 3, // first triangle
					1, 2, 3  // second triangle
			}),
			32,
			(float *) new float[32]{
					// positions          // colors           // texture coords
					1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
					1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
					-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
			}
	};


}

