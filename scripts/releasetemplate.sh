if [[ -d template ]]; then 
	NAME=../releases/${PRODUCT_NAME}-${PLATFORM_DISPLAY_NAME}-${MARKETING_VERSION}.tgz
	echo "Releasing $NAME"
	rm -f ${NAME}
	cd template
	rm -rf .DS_Store
	tar zcvf ../releases/${PRODUCT_NAME}-${PLATFORM_DISPLAY_NAME}-${MARKETING_VERSION}.tgz *
else
	echo "CAnnot find template"
	exit 1
fi
