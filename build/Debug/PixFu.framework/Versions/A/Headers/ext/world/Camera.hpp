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

	/** Default camera vectors */
	static constexpr glm::vec3 DEF_UPVECTOR = glm::vec3(0.0f, 1.0f, 0.0f);
	static constexpr glm::vec3 DEF_FRONTVECTOR = glm::vec3(0.0f, 0.0f, 1.0f);

	/** Default camera values */
	static constexpr float DEF_YAW = 0;
	static constexpr float DEF_PITCH = 0; // M_PI;
	static constexpr float DEF_MOUSE_SENS = 0.1f;
	static constexpr float DEF_ZOOM = 45.0f;


	/**
	 * The camera configurator
	 */

	typedef struct sCameraConfig {

		/** Initial Position (normalized, non world !!) */
		const glm::vec3 position = {0, 0, 0};
		/** INitial Yaw in radians */
		const float yaw = DEF_YAW;
		/** INitial Pitch in radians */
		const float pitch = DEF_PITCH;
		/** INitial roll in radians */
		const float roll = 0.0F;
		/** The camera UP vectpr */
		const glm::vec3 upVector = DEF_UPVECTOR;

		/** Enable smooth mode */
		const bool smooth = true;
		/** Enable terget mode */
		const bool target = false;
		/** delay for position follow */
		const float lerpPosition = 10;
		/** delay for angle follow */
		const float lerpAngle = 15;
		/** delay for distant mode follow TODO */
		const float lerpDistance = 5;

	} CameraConfig_t;

	static constexpr CameraConfig_t CAM_TOPVIEW = { {1,1, 0.700},  M_PI/2, -M_PI/2,0};
	static constexpr CameraConfig_t CAM_TOPPERS = { {0.9,0.9, 0.13},  M_PI/2, -1.15F, 0};
	static constexpr CameraConfig_t CAM_FOLLOW = { {0,0,0}, DEF_YAW, DEF_PITCH, 0, DEF_UPVECTOR, true, true };
	static constexpr CameraConfig_t CAM_INITIAL = { {0,0,0}, DEF_YAW, DEF_PITCH, 0, DEF_UPVECTOR, true, false };

	class Camera {

		static constexpr float STEP = 0.0015f, VSTEP = 0.05;

		/** Default camera values */
		static constexpr float DEF_HEIGHT = 0.2f;

		const CameraConfig_t *CONFIG;

		// Camera Attributes

		/** Current camera position */
		glm::vec3 mPosition;
		/** Current camera Front Vector */
		glm::vec3 mFrontVector;
		/** Current camera Up Vector */
		glm::vec3 mUpVector;
		/** Current camera Right Vector */
		glm::vec3 mRightVector;
		/** Optimizzation: current view matrix */
		glm::mat4 mCurrentViewMatrix;
		/** Optimizzation: current inverse view matrix */
		glm::mat4 mCurrentInvViewMatrix;
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

		bool bAnimateConfigChange = false;
		/** Enable target mode */
		bool bTargetMode = false;
		/** follow distance */
		long lDistantMode = 0;

		/** Target camera angle (easing) */
		float fTargetYaw;
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
		float fTargetDistance = fPlayerDistanceFar;

	public:

		/**
		 * Constructs the camera
		 * @param configuration The configuration struct
		 */

		Camera(const CameraConfig_t &configuration = CAM_INITIAL);

		/**
		 * Update the camera
		 */

		void update(float fElapsedTime);

		/**
		 * Returns the view matrix calculated using Euler Angles and the LookAt Matrix
		 * @return The View Matrix (4x4)
		 */

		glm::mat4& getViewMatrix();
		glm::mat4& getInvViewMatrix();

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

		/**
		 * Enable or disable target mode
		 */
		void toggleTargetMode();
		
		void setConfig(const CameraConfig_t &configuration, bool animate);

		glm::vec3 get3dMouse(glm::mat4& matProj, float xnorm, float ynorm);

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

	inline glm::mat4& Camera::getViewMatrix() {
		return mCurrentViewMatrix;
	}

	inline glm::mat4& Camera::getInvViewMatrix() {
		return mCurrentInvViewMatrix;
	}

	inline void Camera::follow(WorldObject *target) {
		glm::vec3 tpos = target->pos();
		mTargetPosition.x = tpos.x / 1000;
		mTargetPosition.y = tpos.y / 1000;
		mTargetPosition.z = tpos.z / 1000;
		fTargetYaw = (float) M_PI / 2.0F - target->rot().y; //M_PI/2.0f - target->rot().y;
		fTargetYaw = -target->rot().y;

		//		fTargetAngle = (float) -M_PI / 2.0f - target->rot().y;
	}

	inline void Camera::setConfig(const CameraConfig_t &configuration, bool animate = false) {
		CONFIG = &configuration;

		mUpVector=configuration.upVector;

		bSmooth = configuration.smooth;
		bTargetMode = configuration.target;
		bAnimateConfigChange = !bTargetMode && animate;

		if (!animate) {
			mPosition=mInterpolatedPosition=configuration.position;
			fYaw = configuration.yaw;
			fPitch = configuration.pitch;
			fRoll = configuration.roll;
		} else {
			// animateconfigchange will se the values stored in the configuration
			// to LERP there
		}
		updateCameraVectors();
	}

/**
	 * Enables target mode
	 * @param enable  Whether
	 */

	inline void Camera::setTargetMode(bool enable=true) {
		bTargetMode = enable;
		bAnimateConfigChange = false;
	}

	inline void Camera::toggleTargetMode() { bTargetMode = !bTargetMode; }

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
