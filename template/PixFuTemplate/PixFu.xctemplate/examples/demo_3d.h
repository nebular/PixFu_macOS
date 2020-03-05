/**
 *  demo_3d.h
 *  PixFu engine
 *
 *  @author Rodolfo Lopez Pintor
 *  @copyright  © 2020 Nebular Streams. All rights reserved.
 *
 *  A simple world that descends from World class. 
 *
 * This is a barebones world with a camera and methods to add objects. Objects do not have any physics.
 * Use this world as your base class when you want to implement custom physics.
 *
 *
 * ALso Check out demo_3d_balls for a world that descends from BallWorld, a world with ball physics.
 */

#pragma once

#include "PixFuWorld.hpp"

/**
 First of all, you must extend the World extension and configure your world.
 */

class Demo3dWorld:public Pix::World {

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

	// our terrain configuration object. You can add several terrains at different coordinates
	// in that case the coordinates should be adjacent (there are no voids between worlds). This
	// might be allowed in the future.
	
	inline static const Pix::TerrainConfig_t TERRAINCONFIG = {
		
		// Terrain Name. Maps to the assets folder /levels/<name>/ and determines
		// the terrain texture (PNG), the terraim mesh (Wavefront OBJ) and heightmap (PNG)
		// inside that folder

		"cheeseland",
		
		// terrain placement in world coordinated (mainly for multi-terrain).
		// REMEMBER YOUR TERRAIN MODEL MUST HAVE THE ORIGIN AT THE TOP LEFT (0,0) - no negative vertexes !
		// This is required to ease queries to the heightmap (coordinates will exactly match)
		{0,0},
		
		// height (Y) model scale. As our Heightmap texture is normalized, we need to know the maximum height (+Y)
		// on the loaded model, so we can translate the heighmap normalized value into world coordinates.
		// MIND THAT NO SCALING IS PERFORMED ON THE MODEL, this is the opposite, we learn about the loaded model
		// height to connect it with our separate heightmap, that we use for example to stick objects to the ground.
		0.1,

		// render a wireframe grid over the texture. You can also access that canvas and draw additional stuff.
		true
		
	};
	
	public:
	
		Demo3dWorld() : World(WORLDCONFIG) {

			// Add our terrain. Not neccessary to get a pointer to it, but in this
			// case we want to access some properties from the terrain (the size)

			Pix::Terrain *t = add(TERRAINCONFIG);

			int w = t->xPixels() , h = t->zPixels();

			// Add some stuff. If you specify "true" as the last parameter (setHeight), the object height will
			// be set to the terrain height (looking it up on the heightmap)
			
			for (int x=0; x<w; x+=200) {
				
				for (int z=0; z<h; z+=200) {

					// add objects to the world. Objects use absolute world coordinates, so if we have
					// another terrain at, ie. (1000,0), an object in the second terriain will have x > 1000

					// objectproperties struct is the inmutable struct common to all objects
					// of this class, and contains a lot of properties like mass, radius, inital speed & acceleration,
					// intrinsic animation ...  The second struct "objectLocation" supplies the desired initial
					// object position and rotation.
					
					add(
						Pix::ObjectProperties_t {
							"tree",						// object class, maps to /assets/objects/<name>/
							(3.0f + random() % 50 )		// object radius
						},
						Pix::ObjectLocation_t  {
							{ x, 0, z },				// position in world coordinates
							{0,0,0}						// initial rotation
						},
						true							// set object height to terrain height
					);
					
				}
			}
		}
};

class Demo3d : public Pix::Fu {
		
		Demo3dWorld *mWorld;

	public:

	Demo3d():Fu("3d demo",{ Pix::FONT_1943 }) {
		
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
		
		mWorld = new Demo3dWorld();
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

		// Select mode with ALT/CMD
		Pix::CameraKeyControlMode_t mode =
				Pix::Keyboard::isHeld(Pix::Keys::ALT) ? Pix::ADJUST_ANGLES :
				Pix::Keyboard::isHeld(Pix::Keys::COMMAND) ? Pix::ADJUST_POSITION : Pix::MOVE;

		float speed = Pix::Keyboard::isHeld(Pix::Keys::SHIFT) ? 4.0 : 1.0;

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

		canvas()->drawString(0,10,Pix::SF("CAM x %f y %f z %f",
										  mWorld->camera()->getPosition().x,
										  mWorld->camera()->getPosition().y,
										  mWorld->camera()->getPosition().z),
							 Pix::Colors::COLOR_3, 3);
		return true;

	}
	
};

