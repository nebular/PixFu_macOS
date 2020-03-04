if [[ -d template ]]; then 
	NAME=../releases/${PRODUCT_NAME}-${PLATFORM_DISPLAY_NAME}-${MARKETING_VERSION}-${CONFIGURATION}.tgz
	echo "Releasing $NAME"
	rm -f ${NAME}
	cd template
	rm -rf .DS_Store
	tar zcvf ${NAME} *
else
	echo "Cannot find template"
	exit 1
fi
