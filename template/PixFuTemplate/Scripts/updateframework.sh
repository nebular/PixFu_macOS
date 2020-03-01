if [[ -d Pixfu.framework ]]; then

	rm -rf PixFu.framework
	cp -aR ../../PixFu/build/Debug/PixFu.framework .

fi
