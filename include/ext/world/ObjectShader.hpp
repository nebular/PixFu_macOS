//
//  TerrainShader.hpp
//  PixEngine
//
//  Created by rodo on 16/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include "Camera.hpp"
#include "Shader.hpp"
#include "glm/mat4x4.hpp"

namespace rgl {

	class Frustum;

	class ObjectShader : public Shader {

		// keey this for frustum calculations
		glm::mat4 mProjectionMatrix;
		
		// we wil evaluate each draw() to check if inside the frustum
		Frustum *mFrustum;

	public:

		ObjectShader(std::string name);

		void bindAttributes();

		void loadShineVariables(float damper, float reflectivity);

		void loadTransformationMatrix(glm::mat4 &matrix);

		void loadLight(Light *light);

		void loadViewMatrix(Camera *camera);

		void loadProjectionMatrix(glm::mat4 &projection);

		Frustum *frustum() { return mFrustum; }
	};
}
