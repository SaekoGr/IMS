ims: ims.cpp Person.cpp Person.hpp StartDay.cpp StartDay.hpp
	g++ -std=c++11 -o ims ims.cpp Person.cpp Person.hpp StartDay.cpp StartDay.hpp -lsimlib

run: ims
	./ims

all: ims
	
build: ims

exp: ims
	./ims --people 250000 --buses 370 --cars 60000 --transportRatio 25
	./ims --people 250000 --buses 500 --cars 60000 --transportRatio 50
	./ims --people 250000 --buses 800 --cars 0 --transportRatio 100
	./ims --people 250000 --buses 370 --cars 100000 --transportRatio 25

exp1: ims
	./ims --people 250000 --buses 370 --cars 60000 --transportRatio 25

exp2: ims
	./ims --people 250000 --buses 500 --cars 60000 --transportRatio 50

exp3: ims
	./ims --people 250000 --buses 800 --cars 0 --transportRatio 100

exp4: ims
	./ims --people 250000 --buses 370 --cars 100000 --transportRatio 25
