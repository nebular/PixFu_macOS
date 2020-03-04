/**
 *  demo_sprites.h
 *  PixFu engine
 *
 *  @author Rodolfo Lopez Pintor
 *  @copyright  © 2020 Nebular Streams. All rights reserved.
 *
 *  A demo of the SpriteSheet extension.
 * 
 */

#include <iostream>
#include "PixFu.hpp"
#include "PixFuSprites.hpp"

#pragma once

class DemoSprites : public Pix::Fu {
	
	Pix::SpriteSheet *pSprites;
	Pix::SpriteSheet *pItems;

	float pop = 0;
	int sprites[10]={0};
	
public:
	
	DemoSprites() : Pix::Fu("Hello Sprites!") {}

	bool onUserCreate(bool restarted) override {

		if (restarted) return true;
		
		pSprites = new Pix::SpriteSheet(this, {"sprites/mario-50.png", 16, 22});
			addExtension(pSprites);
			
		pItems = new Pix::SpriteSheet(this, {"sprites/items-spr.png", 10, 10});
			addExtension(pItems);
	
		sprites[0]=pSprites->create(8, 1,1,glm::vec2(200,200), 3, 0.5);
		sprites[1]=pSprites->create(10, 1,1,glm::vec2(500,400), 5, 0.3);
		sprites[2]=pSprites->create(4, 1,1,glm::vec2(100,300), 2, 0.3);
		sprites[3]=pSprites->create(4, 1,1,glm::vec2(100,300), 2, 0.3);
		pSprites->tint(3, Pix::TINT_FULL, Pix::Pixel(200,100,255,255));
		pSprites->tint(2, Pix::TINT_CHROMA, Pix::Pixel(255,255,0,255));
		pSprites->tint(1, Pix::TINT_CHROMA, Pix::Pixel(155,0,43,255));

//		pItems->create(2, 1,1,glm::vec2(20,10), 2);
		
		return true;
	}
	
	bool onUserUpdate(float fElapsedTime) override {
		
		pop += fElapsedTime;
		canvas()->clear(Pix::Pixel(0,0,255*sinf(pop),255));
		canvas()->drawString(200,250 + 25 *sinf(pop),"Spritesheets", Pix::Pixel(0,255,0,255), 6);
		

		for (int i = 0; i<4; i++) {
			pSprites->update(
							 // sprite Id
							 i,
							 // position
							 glm::vec2(200.0+200.0*i, screenHeight() / 2 + 100*cosf(pop+M_PI*i/4)),
							 // sprite code in spritesheet (to change it for animation)
							 0,
							 // scale
							 i==0||i==3?3*sinf(pop)+0.4*(i+1):2,
							 // rotation
							 i==1?pop:i==3?cosf(pop):0
							 );
		}
		return true;
	}
};

