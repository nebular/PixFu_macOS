//
//  Ball.hpp
//
//  All this learnt from onelonecoder.com masterclass on Ball physics
//
//  Created by rodo on 11/01/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "err_typecheck_invalid_operands"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma once

// Flag No Time Info
#define NOTIME -1
// car decceleration constant TODO move to sFeatures
#define TERRAINFRICTION 0.005
// height irregularities gradient we can ignore and drive through unaffected
#define RIDEHEIGHT_SEAMLESS 5
// 0.1

#define FAKE_BALL_ID 9999

#include "Drawable.hpp"
#include "World.hpp"
#include "BallWorld.hpp"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/fast_square_root.hpp"

namespace Pix {


	typedef enum eOverlaps {
		OVERLAPS, OVERLAPS_OUTER, NO_OVERLAPS
	} Overlaps_t;

	class Ball : public WorldObject {

		static constexpr unsigned CLASSID = 2;

		// BallWorld manages all ball instances and will access private properties here
		// to optimize perrformance

		friend class BallWorld;

//		friend class Orbit;

		std::string TAG;

		// Multiple simulation updates with small time steps permit more accurate physics
		// and realistic results at the expense of CPU time of course
		// TODO: research which values to use as jdavidx example uses hundreds of balls but
		// TODO: we are an order of magnitude below, at the most around 30 objects and very spaced

		static constexpr int SIMULATIONUPDATES = 2; // 4

		// Multiple collision trees require more steps to resolve. Normally we would
		// continue simulation until the object has no simulation time left for this
		// epoch, however this is risky as the system may never find stability, so we
		// can clamp it here

		static constexpr int MAXSIMULATIONSTEPS = 3; // 15

	public:

		/** whether this is a static object (so wont collide with another static object) */
		const bool ISSTATIC;

	protected:

		// Circuit has the tight collision detection loops
		// so it is cool to have private access to vars, mostly to position.

		static constexpr float FEATURES_SCRATCHING_NEW = 0.6;
		static constexpr float FEATURES_CLIMB_LIMIT = 0.3;        // TODO
		static constexpr float FEATURES_FALL_LIMIT = 0.3;         // TODO move to features
		static constexpr float ACCELERATION_EARTH = -9.8f * 100;  // i found a 9.8 - ish value that makes sense so let´s keep it like this :)

		// Threshold indicating stability of object
		static constexpr float STABLE = 0.001;

		glm::vec3 mPosition = {0, 0, 0};        // ball position in world coordinates
		glm::vec3 mRotation = {0, 0, 0};        // ball rotation
		glm::vec3 mSpeed = {0, 0, 0};            // ball speed
		glm::vec3 mAcceleration = {0, 0, 0};    // ball acceleration

		// extensions
		static float stfBaseScale;                // A Base scale for all balls in the simulation
		static float stfHeightScale;            // heights pixel height

		float fOuterRadius = 0;                    // outer radius to predict next collisions
		float fMassMultiplier = 1.0;            // multiply ball mass (game powerups)
		float fRadiusMultiplier = 1.0;            // multiply ball radius (game powerups)

		float fHeightTerrain = 0.0;                // target height (gravity effect)

		glm::vec2 fAngleTerrain = {0, 0};         // terrain angle at corners

		float fPenalty = 1.0;                    // penalty in speed percent imposed by terrain irregularities

		bool bFlying = false;                    // whether the ball is currently "flying"
		bool bReverse = false;                    // reverse gear flag
		bool bForward = false;                    // forward gear flag
		bool bDisabled = false;                    // disable the player (will not be updated & behave as ghost) (debug)

		// Internal Simulation vars
		glm::vec3 origPos;

		// simulation time remaining for current iteration
		float fSimTimeRemaining;

		Ball(const WorldConfig_t &planetConfig, float radi, float mass, glm::vec3 position, glm::vec3 speed);

		// internal loop function to commit simulation steps
		void commitSimulation();

		// process Height effects (height calcs separated from 2D calcs)
		void processGravity(World *world, float fTime);

	public:

		static void setBaseScale(float scale);

		static void setHeightScale(float scale);

		Ball(const WorldConfig_t &planetConfig, ObjectMeta_t meta, ObjectLocation_t location, bool isStatic = false, int overrideId = -1);

		/**
		 * ball normalized position is used by the camera
		 * @return ball position, normalized
		 */
		glm::vec3 &pos() override;        // ball position

		/**
		 * Ball rotation
		 * @return rotation in radians
		 */
		glm::vec3 &rot() override;        // ball 3d rotation

		/**
		 * Ball radius
		 * @return Ball radius in world units
		 */
		float radius() override;        // ball radius

		/**
		 * Ball outer radius (collision detection
		 * @return radius in world units
		 */

		float outerRadius();            // outer radius (collision prediction)

		/**
		 * Ball calculated angle from the speed vector
		 * @return angle in radians
		 */

		float angle();

		/**
		 * Ball speed modulus
		 * @return Speed modulus
		 */

		float speed();

		/**
		 * Ball velocity vector
		 * @return The velocity vector
		 */

		glm::vec3 velocity();

		/**
		 * Ball acceleration vector
		 * @return The acceleration vector
		 */

		glm::vec3 acceleration();

		/**
		 * Ball Mass
		 * @return Mass
		 */

		float mass();                // ball mass

	protected:

		/**
		 * Whether the ball is flying
		 * @return whether
		 */

		bool isFlying();                    // whether ball is flying

		/**
		 * Mass multiplier (generic game powerups)
		 * @param massMultiplier Mass multiplier
		 */

		void setMassMultiplier(float massMultiplier);

		/**
		 * Radius multiplier (generic game powerups)
		 * @param radiusMultiplier Radius scalar multiplier
		 */

		void setRadiusMultiplier(float radiusMultiplier);

		/**
		 * Distance to another ball
		 * @param target TArget ball
		 * @return distance in world units
		 */

		float distance(Ball *target);

		/**
		 * Whether a point is inside a ball
		 * @param point Point to test
		 * @return Whether
		 */

		bool isPointInBall(glm::vec3 point);

		/**
		 * Whether a ball intersects another
		 * @param b2 Another ball
		 * @param outerRadius Check this ball's outer radius
		 * @return Whether balls intersect
		 */

		bool intersects(Ball *b2, bool outerRadius);

		// whether a ball overlaps this one
		/**
		 * Check a ball overlaps this one
		 * @param b2 Another ball
		 * @return Overlap code: No overlap, overlaps, overlaps in outer radius
		 */
		Overlaps_t overlaps(Ball *b2);

		// convenience displacement required to avoid an overlap
		glm::vec3 calculateOverlapDisplacement(Ball *target, bool outer = false);

		// make a collision ball (used to model crashes against walls & terrain)

		/**
		 * Make a collision ball using this one as reference
		 * @param radi The new ball radius
		 * @param position The new ball position
		 * @return A bespoke collision ball, ready to crash !
		 */

		Ball *makeCollisionBall(float radi, glm::vec3 position);

		/**
		 * This gets called when this ball collides with another, and receives the
		 * new speed vector. The default implementation just writes the new speed vector, but derived
		 * classes can do different things with the calculated new speed and maybe elaborate the collision.
		 * @param otherBall The ball you have collided with
		 * @param fElapsedTime time
		 * @param newSpeedVector The new speed vector
		 */

		virtual void onCollision(Ball *otherBall, glm::vec3 newSpeedVector, float fElapsedTime);

		/**
		 * This gets called whenever there is a collision on the outer radius so a derived class can
		 * maybe implement logic to avoid the collision
		 * @param other The ball that collided on the outer radius
		 */

		virtual void onFutureCollision(Ball *other);

		/**
		 * Disables a ball (stops physics)
		 * @param disabled Whether to disable / enable
		 */
		void disable(bool disabled);

		/**
		 * Process ball tick. Applies speed changes according to acceleration, and position changes
		 * according to speed.  Derived classes may override this to implement a more elaborated
		 * physics scheme
		 * @param world World
		 * @param fTime Frame time
		 */

		virtual void process(World *world, float fTime = NOTIME) override;

		// process Height effects (height calcs separated from 2D calcs)
		Ball *processHeights(World *world, float fTime = NOTIME);

	};

// INLINE IMPLEMENTATION BELOW THIS POINT

	inline glm::vec3 &Ball::pos() { return mPosition; }                        // ball world position
	inline glm::vec3 &Ball::rot() { return mRotation; }                              // ball world position

	inline float Ball::mass() { return CONFIG.mass * fMassMultiplier; }                    		// ball final mass
	inline float Ball::radius() { return CONFIG.radius * fRadiusMultiplier + fRadiusAnimator * CONFIG.radius; } // ball final radius
	inline float Ball::outerRadius() { return fOuterRadius * fRadiusMultiplier + fRadiusAnimator * CONFIG.radius; }

	inline float Ball::angle() { return mRotation.y; }                              // ball angle (heading)
	inline float Ball::speed() {
		return glm::fastSqrt(mSpeed.x * mSpeed.x + mSpeed.z * mSpeed.z);
	}   // TODO fabs             		// ball speed
	inline glm::vec3 Ball::velocity() { return mSpeed; }

	inline glm::vec3 Ball::acceleration() { return mAcceleration; }

	inline bool Ball::isFlying() { return bFlying; }                                // whether ball is flying

	inline void Ball::setMassMultiplier(float massMultiplier) { fMassMultiplier = massMultiplier; }

	inline void Ball::setRadiusMultiplier(float radiusMultiplier) { fRadiusMultiplier = radiusMultiplier * stfBaseScale; }

	// distance to another ball
	inline float Ball::distance(Ball *target) {
		return glm::fastSqrt(
				(mPosition.x - target->mPosition.x) * (mPosition.x - target->mPosition.x)
				+ (mPosition.z - target->mPosition.z) * (mPosition.z - target->mPosition.z));
	}

	inline void Ball::setBaseScale(float scale) { stfBaseScale = scale; }

	inline void Ball::setHeightScale(float scale) { stfHeightScale = scale; }

	class LinearDelayer {

		float fTarget;
		float fCurrent;
		float fIncrement;

	public:
		LinearDelayer(float increment);

		void setTarget(float target, bool force);

		float get();

		float tick(float fElapsedTime);

		bool isStable();
	};


	inline LinearDelayer::LinearDelayer(float increment) { fIncrement = increment; }

	inline void LinearDelayer::setTarget(float target, bool force = false) {
		fTarget = target;
		if (force)
			fCurrent = target;
	}

	inline float LinearDelayer::get() { return fCurrent; }

	inline bool LinearDelayer::isStable() { return fCurrent == fTarget; }
}
#pragma clang diagnostic pop
