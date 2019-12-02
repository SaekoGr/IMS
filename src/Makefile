.PHONY: build
build: ims

.PHONY: ims
ims:
	mkdir -p cmake-build-release
	cd cmake-build-release && cmake -D CMAKE_BUILD_TYPE=Release ..
	cmake --build cmake-build-release

all: ims
	./cmake-build-release/ims
	
	