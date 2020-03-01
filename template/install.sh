if [[ -d PixFuTemplate ]]; then

	rm -rf /Library/Developer/Xcode/Templates/PixFuTemplate               
	cp -aRf PixFuTemplate /Library/Developer/Xcode/Templates/

else
	exit 1
fi
