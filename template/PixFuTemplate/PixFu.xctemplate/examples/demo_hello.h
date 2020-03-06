/**
 *  demo_hello.h
 *  PixFu engine
 *
 *  @author Rodolfo Lopez Pintor
 *  @copyright  © 2020 Nebular Streams. All rights reserved.
 *
 *  A simple hello world. We extend the engine class and draw a string and a couple circles.
 *
 */

#include <cmath>
#include <iostream>
#include "PixFu.hpp"

class DemoHello : public Pix::Fu {
	
public:
		
	DemoHello():Pix::Fu("hello PixFu", { Pix::FONT_BISASAM_STROKE }) {}

	bool onUserUpdate(float fElapsedTime) override {
		
		float s = sinf(METRONOME), c = cosf(METRONOME);
		canvas()->clear(Pix::Pixel(255*s,255*c,255*s,255));
		canvas()->fillCircle(screenWidth()/2, screenHeight()/2, screenWidth()/2*s, Pix::Pixel(0,0,255,255));
		canvas()->drawString(150,250 + 50 *s,"PixFu engine 1.0", Pix::Pixel(0,255,0,255), 3);
		canvas()->fillCircle(Pix::Mouse::x(), Pix::Mouse::y(), 30, Pix::Mouse::isHeld(0)? Pix::Pixel(255,0,0,255):Pix::Pixel(255,255,255,255));

		return true;
	}
	
};

