.PHONY: build
build: ims

.PHONY: ims
ims:
	mkdir -p cmake-build-release
	cd cmake-build-release && cmake -D CMAKE_BUILD_TYPE=Release ..
	cmake --build cmake-build-release

all: ims
	./cmake-build-release/ims

exp: ims
	./cmake-build-release/ims --people 250000 --buses 1300 --cars 30000 --transportRatio 15
	./cmake-build-release/ims --people 250000 --buses 1300 --cars 30000 --transportRatio 70
	./cmake-build-release/ims --people 250000 --buses 2000 --cars 0 --transportRatio 100
	./cmake-build-release/ims --people 250000 --buses 1300 --cars 60000 --transportRatio 10

exp1: ims 
	./cmake-build-release/ims --people 250000 --buses 1300 --cars 30000 --transportRatio 15

exp2: ims 
	./cmake-build-release/ims --people 250000 --buses 1300 --cars 30000 --transportRatio 70
	
exp3: ims
	./cmake-build-release/ims --people 250000 --buses 500 --cars 0 --transportRatio 100

exp4: ims
	./cmake-build-release/ims --people 250000 --buses 1300 --cars 60000 --transportRatio 10