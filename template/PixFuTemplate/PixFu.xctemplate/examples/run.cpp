//
//  run.cpp
//  PixEngine
//
//  Created by rodo on 16/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "demo_hello.h"
#include "demo_sprites.h"
// #include "demo_3d.h"

int main(int argc, const char * argv[]) {
	
	Pix::FuPlatform::init(new Pix::PixFuPlatformApple({true, false}));

	Pix::Fu *engine = new DemoHello();
//	rgl::PixFu *engine = new DemoSprites();
//	rgl::PixFu *engine = new Demo3d();
	engine->init(1024, 576);
	engine->start();
	delete engine;
	
	return 0;
}
