#!/bin/sh

#  installtemplate.sh
#  PixFu
#
#  Created by rodo on 01/03/2020.
#  Copyright © 2020 rodo. All rights reserved.


echo "COJONES $SRCROOT/template/PixFuTemplate"

if [[ -d $SRCROOT/template/PixFuTemplate/PixFu.framework ]]; then
	rm -rf $SRCROOT/template/PixFuTemplate/PixFu.framework
	cp -aRv $BUILD_DIR/Debug/PixFu.Framework $SRCROOT/template/PixFuTemplate/
else
	exit 1
fi
	
