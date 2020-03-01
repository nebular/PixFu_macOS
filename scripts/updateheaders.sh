#!/bin/bash
cd ${SRCROOT}

DEST=include

if [[ -d $DEST ]]; then
	rm -rf $DEST
	mkdir $DEST $DEST/core $DEST/ext $DEST/input $DEST/support $DEST/items $DEST/glm
	mkdir $DEST/ext/world $DEST/ext/sprites $DEST/arch $DEST/arch/apple

	cp -v scripts/data/*.hpp $DEST/

	cp -v modules/PixFu/src/arch/apple/*.hpp $DEST/arch/apple/
	cp -v modules/PixFu/src/arch/apple/*.h $DEST/arch/apple/
	cp -v modules/PixFu/src/core/headers/* $DEST/core/
	cp -v modules/PixFu/src/input/headers/* $DEST/input/
	cp -v modules/PixFu/src/support/headers/* $DEST/support/
	cp -v modules/PixFu/src/items/headers/* $DEST/items/
	cp -r modules/PixFu/src/thirdparty/glm/* $DEST/glm/

	cp -v modules/PixFu_Extensions/world/core/headers/* $DEST/ext/world/
	cp -v modules/PixFu_Extensions/world/geom/* $DEST/ext/world/
	cp -v modules/PixFu_Extensions/world/worlds/ballworld/headers/* $DEST/ext/world/

	cp -v modules/PixFu_Extensions/sprites/headers/* $DEST/ext/sprites/
else
	exit 1
fi
