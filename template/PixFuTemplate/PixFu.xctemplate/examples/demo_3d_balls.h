/**
 *  demo_3d_Balls.h
 *  PixFu engine
 *
 *  @author Created by rodo on 02/03/2020.
 *  @copyright  © 2020 rodo. All rights reserved.
 *
 *  A world that descends from BallWorld. This world has ball physics that affect all the
 *  added objects.
 *
 *  This world also suports "Levels". A Level for this map has a name, ie "Battle", and
 *  contains:
 *
 *  - the terrain model 				in assets/levels/<name>/<name>.obj
 *  - the terrain texture			in assets/levels/<name>/<name>.png
 *  - the level data file 			in assets/levels/<name>/<name>.dat
 *		... that contains:
 *  		- an unlimited number of splines	
 *			- an unlimited number of edges
 *			- an unlimited number of objects
 *
 *  - The level data file is generated by WorldMap::Save() 
 *
 * So when instantiating the class, by just providing the level name, all stuff will be loaded.
 */

#pragma once

#include "PixFuWorld.hpp"

enum MODELS {
	TREE_BIG,
	TREE_SMALL,
	VIRUS
};

void initGameObjects() {
	
	// The static database of objects
	// you can store object profiles here and then easily add them to the world
	// by its code

	// a small tree.
	Pix::ObjectDb::insert(
		TREE_SMALL,					// Unique Object ID of your choosing
		Pix::ObjectProperties_t {	// inmutable object properties
			"tree",					// object class, maps to /assets/objects/<name>/
			25.0f,					// object radius
			10000.0f,				// object mass
			0.0,					// elasticity
			0.0f,					// crash efficiency
			Pix::ObjectAerodynamics_t {},
			Pix::ObjectAnimation_t {},			// intrinsic animation
			true,					// static object
			2.0						// draw 2 times bigger (tree is taller so for collision we use normal radius)
		},
		Pix::ObjectLocation_t {}	// initial position if desired
	);

	// a bigger tree.
	Pix::ObjectDb::insert(
		TREE_BIG,
		{ "tree", 75.0f, 1000.0f, 0.0F, 0.0F, {}, {}, true, 2.0 },	// short form
		{ { 500.0,0,500.0} }	// initial position if desired
	);
	
	Pix::ObjectDb::insert(
		VIRUS,
		{
			"virus",
			20,									// radius 10 = ??
			50,									// mass
			0.7,								// elasticity
			1,									// crash eff
			{ 0.95, 1.0 },						// aerodynamics, low resistance to terrain and air
			{ true, 0.1, 0.15, 0.2, 0.1 }		// intrinsic animaton: enable, x, y, z, scale
		}
	);

}

/**
 First of all, you must extend the World extension and configure your world.
 */

class Demo3dBallWorld:public Pix::BallWorld {

	// our World Configuration Object. There are more advanced values
	// in the struct that you can check out, but let´s keep it simple.

	inline static const Pix::WorldConfig_t WORLDCONFIG = {
		{0.4,0.3,0.9},		// background color
		{20000,20000,2000},	// light position
		{0.4,0.4,0.3},		// light color
		Pix::PERSP_FOV70,	// perspective
		TRANSFORM_NONE,		// no objects global transformation
		TRANSFORM_NONE,		// no terrain global transformation
		true,				// enable 3D canvas
		"default"			// enable font
	};
	
	public:
	
		Demo3dBallWorld():BallWorld("cheeseland", WORLDCONFIG) {

			initGameObjects();
			
			// BallWorld initializes the world reading the terrain model, texture and
			// Level Map with splines and initial objects from the assets.

			int w = 1700, h = 1400;

			// Add some stuff. If you specify "true" as the last parameter (setHeight), the object height will
			// be set to the terrain height (looking it up on the heightmap)
			
			for (int x=0; x<w; x+=300) {
				
				for (int z=0; z<h; z+=300) {

					// We use the alternate way to add objects using their OID.
					// The object properties are then retrieved from the database
					// you can supply an additional placement, or if ommited, the placement
					// from the database is used.
					
					add(
						random()%2==0?TREE_SMALL:TREE_BIG,
						Pix::ObjectLocation_t {
							{ x+(random()%50), 0, z+(random()%50) },				// position in world coordinates
							{0.0, 0, 0.0}			// rotation in degrees around x,y,z axis
						},
						true
					);
				}
			}
		}
	
		void addBall() {
			add(
				VIRUS,
				Pix::ObjectLocation_t {
					camera()->getPosition(),					// position
					{0.0F,0.0F,0.0F},							// initial rotation
					camera()->getFrontVector(250+random()%250),	// speed
					{ 0.0F, 0.0F, 0.0F }						// accel
				},
				false
			);
		}
	
	bool init(Pix::Fu *engine) override {

		if (!World::init(engine)) return false;
		canvas()->blank();
		pMap->drawSelf(canvas());
		return true;
	}

	void tick(Pix::Fu *engine, float fElapsedTime) override {
		BallWorld::tick(engine, fElapsedTime);
	}
};

class Demo3dBalls : public Pix::Fu {
		
		Demo3dBallWorld *mWorld;

	public:

	Demo3dBalls() : Fu("3d demo Balls", { Pix::FONT_1943 }) {
		
		// Add the world extension on the constructor
		// Extensions added on the contructor are rendered
		// before the UI layer. This makes sense for extensions
		// that take care of the background themseves. World is
		// one of those, as it is designed to be the back layer.
			
		// The engine will take care from now on of initializing the
		// extension and update it on every frame
		
		// You can keep a pointer to the world object to access the
		// camera and the canvas (a canvas rendered over the texture
		// where you can draw)
		
		// Demo3DWorld is your World class , that you have extended.
		// Protected methods there allow to fill the world with objects.
		
		mWorld = new Demo3dBallWorld();
		addExtension(mWorld);

	}

	bool onUserCreate(bool restarted) override {

		// nothing to do here, but in a real game you would be initializing everything
		// As you see, the world extension is non-intrusive, your main engine class is
		// clean of clutter
		return true;

	}
	
	bool onUserUpdate(float fElapsedTime) override {
				
		mWorld->canvas()->drawString(100,100,"RODOLFO LOPEZ PINTOR", Pix::Colors::PINK, 6);

		if (Pix::Keyboard::isPressed(Pix::Keys::SPACE))
			mWorld->addBall();
			
		// Select mode with ALT/CMD
		Pix::CameraKeyControlMode_t mode =
				Pix::Keyboard::isHeld(Pix::Keys::ALT) ? Pix::ADJUST_ANGLES :
				Pix::Keyboard::isHeld(Pix::Keys::COMMAND) ? Pix::ADJUST_POSITION : Pix::MOVE;

		float speed = Pix::Keyboard::isHeld(Pix::Keys::SHIFT) ? 5.0:1.0;

		mWorld->camera()->inputKey(
			  mode,
			  Pix::Keyboard::isHeld(Pix::Keys::UP),
			  Pix::Keyboard::isHeld(Pix::Keys::DOWN),
			  Pix::Keyboard::isHeld(Pix::Keys::LEFT),
			  Pix::Keyboard::isHeld(Pix::Keys::RIGHT),
			  speed,
			  fElapsedTime);


		// print camera position
		
		canvas()->blank();	// fast clear (memset)

		canvas()->drawString(0,10,Pix::SF("CAM X %f Y %f Z %f",
										  mWorld->camera()->getPosition().x,
										  mWorld->camera()->getPosition().y,
										  mWorld->camera()->getPosition().z),
							 Pix::Colors::COLOR_3, 3);
		canvas()->drawString(0,40,Pix::SF("    p %f y %f r %f",
										  mWorld->camera()->getPitch(),
										  mWorld->camera()->getYaw(),
										  mWorld->camera()->getRoll()),
							 Pix::Colors::COLOR_3, 3);
		return true;

	}
	
};

