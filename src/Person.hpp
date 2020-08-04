#ifndef IMS_PERSON_HPP
#define IMS_PERSON_HPP

#include <simlib.h>
#include <stdio.h>
#include <iostream>

#define MAX_DISTANCE 42
#define RIDING_CAR 0.90
#define CAR_EMISSION 135
#define BUS_EMISSION 822
#define BUS_CAPACITY 42
#define BUS_STOPS 28


// the 24 hour-cycle
#define DAY_START 0
#define DAY_END 86400

void output_stats();
void initialiaze_day();
void calculateHasCarRatio(unsigned long long int people, unsigned long long int cars, float ratio);
void calculateBusInterval(unsigned long long int buses);
void activateBusGenerator(unsigned long long int buses);
unsigned int generateBusStopNumber();
int disembarkingPeople(unsigned int onBoard);

static double hasCarRatio;
static double availableCars;


class Person : public Process{
    public:
        Person(unsigned long long int personalNumber);

        void Behavior();

        void calculateHasCarRatio(unsigned long long int people, unsigned long long int cars);

        void rideCar();

        
    private:
        unsigned long long int personalNumber;

        unsigned int stopNum;

        double needTransportTime;

        double distanceTravelled = 0;

        double carEmission = 0;
};

class Bus : public Process{
    public:
        
        Bus();

        void Behavior();

    private:
        int capacity;

        int on_board;

        double busEmission = 0;

        double distanceTravelled = 0;

        unsigned long long int transportedPassangers = 0;

};

class BusGenerator: public Process{
    public:

        BusGenerator(unsigned long long int buses);

        void Behavior();

        unsigned long long int buses;

    private:
        Store* busStore;
};

#endif