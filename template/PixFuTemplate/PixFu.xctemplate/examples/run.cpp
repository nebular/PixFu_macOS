//
//  run.cpp
//  PixEngine
//
//  Created by rodo on 16/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "demo_hello.h"
#include "demo_sprites.h"

int main(int argc, const char * argv[]) {
	
	Pix::FuPlatform::init(new Pix::PixFuPlatformApple({true, false}));

	Pix::Fu *engine = new DemoHello();
//	rgl::PixFu *engine = new DemoSprites();
	engine->init(1024, 576);
	engine->start();
	delete engine;
	
	return 0;
}
