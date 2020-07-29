include ../config.make

export BUILD_DIR := $(shell readlink -f ../build$(BUILD)/aie)

${BUILD_DIR}/.dir:
	mkdir -p $(dir $@)
	touch $@

# This should build a libLLVM-10git.so which we can link against easily.
build: ${BUILD_DIR}/.dir
	echo `pwd`; ./config.sh ${BUILD_DIR} .; cd ${BUILD_DIR}; ninja

test:
	cd ${BUILD_DIR}; ninja check-aie