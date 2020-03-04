
# copy framework assets
cp -f -R -v ${SRCROOT}/PixFu.Framework/Resources/* ${BUILD_DIR}/${CONFIGURATION}/

# copy user assets
cp -f -R -v ${SRCROOT}/${TARGETNAME}/Assets/* ${BUILD_DIR}/${CONFIGURATION}/

