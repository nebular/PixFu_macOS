if [[ -d Pixfu.framework ]]; then

	rm -rf PixFu.framework
	cp -aR ../../PixFu/build/${CONFIGURATION}/PixFu.framework .

fi
