#!/bin/bash

DEST=include
rm -rf $DEST
mkdir $DEST $DEST/core $DEST/ext $DEST/input $DEST/support $DEST/items $DEST/glm
mkdir $DEST/ext/world $DEST/ext/sprites

cp -v modules/PixFu/src/core/headers/* $DEST/core/
cp -v modules/PixFu/src/input/headers/* $DEST/input/
cp -v modules/PixFu/src/support/headers/* $DEST/support/
cp -v modules/PixFu/src/items/headers/* $DEST/items/
cp -r modules/PixFu/src/thirdparty/glm/* $DEST/glm/

cp -v modules/PixFu_Extensions/world/core/headers/* $DEST/ext/world/
cp -v modules/PixFu_Extensions/world/geom/* $DEST/ext/world/
cp -v modules/PixFu_Extensions/world/core/worlds/* $DEST/ext/world/

cp -v modules/PixFu_Extensions/sprites/headers/* $DEST/ext/sprites/
