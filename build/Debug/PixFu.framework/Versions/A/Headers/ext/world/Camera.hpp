//
//  Camera.hpp
//  PixFu World Extensoin
//
//  An abstract camera class that processes input and calculates the corresponding Euler Angles,
//	Vectors and Matrices for use in OpenGL
//
//  I learnt everything from https://learnopengl.com/Getting-started/Camera
//
//  Created by rodo on 19/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "err_typecheck_invalid_operands"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"

#include "Keyboard.hpp"
#include "WorldMeta.hpp"

#include <vector>
#include <cmath>

namespace Pix {

	/**
	 * Defines several possible options for camera movement. Used as abstraction to stay away from
	 * window-system specific input methods
	 */

	typedef enum CameraMovement {
		CM_FORWARD,
		CM_BACKWARD,
		CM_LEFT,
		CM_RIGHT,
		CM_UP,
		CM_DOWN
	} CameraMovement_t;

	/**
	 * Camera Control Modes (for the interaction routine)
	 */

	typedef enum sCameraKeyControlMode {
		MOVE, ADJUST_POSITION, ADJUST_ANGLES, ADJUST_PLAYER_POSITION, ADJUST_PLAYER_ANGLES
	} CameraKeyControlMode_t;


	class World;

	class WorldObject;

	class Camera {

		/** Default camera vectors */
		static constexpr glm::vec3 DEF_UPVECTOR = glm::vec3(0.0f, 1.0f, 0.0f);
		static constexpr glm::vec3 DEF_FRONTVECTOR = glm::vec3(0.0f, 0.0f, 1.0f);

		static constexpr float STEP = 0.0015f, VSTEP = 0.05;

		/** Default camera values */
		static constexpr float DEF_YAW = 0;
		static constexpr float DEF_PITCH = 0; // M_PI;
		static constexpr float DEF_HEIGHT = 0.2f;
		static constexpr float DEF_MOUSE_SENS = 0.1f;
		static constexpr float DEF_ZOOM = 45.0f;

		/** The world up vector in use */
		const glm::vec3 UPVECTOR;

		// Camera Attributes

		/** Current camera position */
		glm::vec3 mPosition;
		/** Current camera Front Vector */
		glm::vec3 mFrontVector;
		/** Current camera Up Vector */
		glm::vec3 mUpVector;
		/** Current camera Right Vector */
		glm::vec3 mRightVector;

		// Euler Angles

		/** Current Yaw */
		float fYaw;
		/** Current Pitch */
		float fPitch;
		/** Current Roll */
		float fRoll;

		// target mode / Smooth mode

		/** Smooth targeting mode */
		bool bSmooth = true;
		/** Smooth Mode constant */
		const float SMOOTHLERP;

		/** Enable target mode */
		bool bTargetMode = false;
		/** follow distance */
		long lDistantMode = 0;
		/** Target camera angle (easing) */
		float fTargetAngle;
		/** Target position and interpolated position */
		glm::vec3 mTargetPosition, mInterpolatedPosition;

		// Camera options
		float mMouseSensitivity;
		float mMouseZoom;

		/** Current camera mode (for interaction) */
		CameraKeyControlMode_t mCameraMode;

		// camera player focus. Adjust the camera position when following a player / object

		/** Vertical distance to target */
		float fPlayerDistanceUp = 0.1f;
		/** Horizontal distance to target */
		float fPlayerDistanceFar = 0.3f; // 0.05;
		/** Camera pitch for target mode */
		float fPlayerPitch = -0.05f;

		const float DEFAULT_DISTANCE_FAR = 0.3f;
		float fTargetDistance = fPlayerDistanceFar;
		const float DISTANCELERP;

	public:

		/**
		 * Constructs the camera
		 * @param initialPosition Initial position
		 * @param initialYaw Initial Yaw
		 * @param initialPitch Initial pitch
		 * @param upVector Up Vector
		 * @param smooth Smooth mode
		 * @param smoothLerp Smooth mode constantt
		 * @param distanceLerp Distance mode constant (not finished)
		 */

		Camera(
				glm::vec3 initialPosition = glm::vec3(0.0f, DEF_HEIGHT, 0.0f),
				float initialYaw = DEF_YAW,
				float initialPitch = DEF_PITCH,
				glm::vec3 upVector = DEF_UPVECTOR,
				bool smooth = true,
				float smoothLerp = 15,
				float distanceLerp = 5
		);

		/**
		 * Update the camera
		 */

		void update(float fElapsedTime);

		/**
		 * Returns the view matrix calculated using Euler Angles and the LookAt Matrix
		 * @return The View Matrix (4x4)
		 */

		glm::mat4 getViewMatrix();

		/**
		 * Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		 * @param xoffset The X coordinate
		 * @param yoffset The Y coordinate
		 * @param constrainPitch Whether to constrain pitch range
		 */

		void inputMouse(float xoffset, float yoffset, bool constrainPitch = true);

		/**
		 * Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		 */
		void inputMouseWheel(float yoffset);

		/**
		 *	Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera
		 *	defined ENUM (to abstract it from windowing systems)
		 */

		void inputMovement(CameraMovement_t direction, float speed = STEP, float fElapsedTime = 1);

		/**
		 * Processes input received from a keyboard-like input system. Expects the pressed status of
		 * each direction, plus a mode to set the arrwos meaning: move camera, adjust position,
		 * adjust pitch/yaw
		 * @param mode a CameraControlMode constant to select the operating mode
		 * @param up Whether UP is pressed
		 * @param down Whether DOWN is pressed
		 * @param left Whether LEFT is pressed
		 * @param right Whether RIGHT is pressed
		 */

		void inputKey(CameraKeyControlMode_t mode, bool up, bool down, bool left, bool right, float percent, float fElapsedTime);

		/**
		 * Steps a yaw
		 * @param deltaRads radians
		 */

		void stepYaw(float deltaRads);

		/**
		 * Steps a pitch
		 * @param deltaRads radians
		 */

		void stepPitch(float deltaRads);

		/**
		 * Steps a roll
		 * @param deltaRads radians
		 */

		void stepRoll(float deltaRads);

		// getters & getters

		glm::vec3 getPosition();
		glm::vec3 getFrontVector(float size);

		/**
		 * Set camera X NORMALIZED position
		 * @param xpos normalized X
		 */
		void setX(float xpos);

		/**
		 * Set camera Z NORMALIZED position
		 * @param ypos normalized Y
		 */
		void setZ(float ypos);

		/**
		 * Set camera height (NORMALIZED)
		 * @param height height
		 */

		void setHeight(float height);

		/**
		 * @return camera pitch in radians
		 */

		float getPitch();

		/**
		 * @return camera yaw in radians
		 */

		float getYaw();

		/**
		 * @return camera roll in radians
		 */

		float getRoll();

		/**
		 * Set camera pitch
		 * @param rads Pitch in radians
		 */

		void setPitch(float rads);

		/**
		 * Set camera roll
		 * @param rads Toll in radians
		 */

		void setRoll(float rads);

		/**
		 * Sets camera yaw
		 * @param rads radians
		 */
		void setYaw(float rads);

		/**
		 * Follow a target
		 * @param target Target to follow
		 */

		void follow(WorldObject *target);

		/**
		 * Enable or disable target mode
		 * @param enable whether
		 */
		void setTargetMode(bool enable);

	private:

		// Calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors();

	};


// main getters / setters

	inline glm::vec3 Camera::getPosition() { return mPosition * 1000.0f; }
	inline glm::vec3 Camera::getFrontVector(float size) { return mFrontVector * size; }

	inline float Camera::getPitch() { return fPitch; }

	inline float Camera::getYaw() { return fYaw; }

	inline float Camera::getRoll() { return fRoll; }

	inline void Camera::setPitch(float rads) { fPitch = rads; }

	inline void Camera::setRoll(float rads) { fRoll = rads; }

	inline void Camera::setYaw(float rads) { fYaw = rads; }

	inline void Camera::setHeight(float hnormalized) { mPosition.y = hnormalized; }

	inline void Camera::setX(float xnormalized) { mPosition.x = xnormalized; }

	inline void Camera::setZ(float znormalized) { mPosition.z = znormalized; }

	inline void Camera::stepPitch(float deltaRads) {
		fPitch += deltaRads;
		updateCameraVectors();
	}

	inline void Camera::stepRoll(float deltaRads) {
		fRoll += deltaRads;
		updateCameraVectors();
	}

	inline void Camera::stepYaw(float deltaRads) {
		fYaw += deltaRads;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix

	inline glm::mat4 Camera::getViewMatrix() {
		return glm::lookAt(mPosition, mPosition + mFrontVector, mUpVector);
	}

	inline void Camera::follow(WorldObject *target) {
		glm::vec3 tpos = target->pos();
		mTargetPosition.x = tpos.x / 1000;
		mTargetPosition.y = tpos.y / 1000;
		mTargetPosition.z = tpos.z / 1000;
		fTargetAngle = (float) M_PI / 2.0F - target->rot().y; //M_PI/2.0f - target->rot().y;
		fTargetAngle = -target->rot().y;

		//		fTargetAngle = (float) -M_PI / 2.0f - target->rot().y;
	}

	/**
	 * Enables target mode
	 * @param enable  Whether
	 */

	inline void Camera::setTargetMode(bool enable) { bTargetMode = enable; }


	class Light {

		glm::vec3 mPosition;
		glm::vec3 mColour;

	public:

		Light(glm::vec3 position, glm::vec3 colour);

		glm::vec3 &position();

		void setPosition(glm::vec3 position);

		glm::vec3 getColour();

		void setColour(glm::vec3 colour);


	};

	inline Light::Light(glm::vec3 position, glm::vec3 colour) {
		mPosition = position;
		mColour = colour;
	}

	inline glm::vec3 &Light::position() { return mPosition; }

	inline void Light::setPosition(glm::vec3 position) { mPosition = position; }

	inline glm::vec3 Light::getColour() { return mColour; }

	inline void Light::setColour(glm::vec3 colour) { mColour = colour; }

}

#pragma clang diagnostic pop
