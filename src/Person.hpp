#ifndef IMS_PERSON_HPP
#define IMS_PERSON_HPP

#include <simlib.h>
#include <stdio.h>

#define USES_PUBLIC_TRANSPORT 0.25
#define MAX_DISTANCE 60
#define RIDING_CAR 0.90
#define CAR_EMISSION 600

void calculateHasCarRatio(unsigned long long int people, unsigned long long int cars);

static double hasCarRatio;

static double availableCars;

class Person : public Process{
    public:
        Person();

        void Behavior();

        void calculateHasCarRatio(unsigned long long int people, unsigned long long int cars);

        void rideCar();
    private:
        double needTransportTime;

        unsigned int distanceTravelled = 0;

        double carEmission = 0;
};

class Bus : public Process{
    public:
        void createBusStore(unsigned long long int buses);

        Bus(unsigned long long int buses);

        void Behavior();

        Store* AllBuses;
};

#endif