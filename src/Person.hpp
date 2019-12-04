#ifndef IMS_PERSON_HPP
#define IMS_PERSON_HPP

#include <simlib.h>
#include <stdio.h>

#define USES_PUBLIC_TRANSPORT 0.25
#define MAX_DISTANCE 60
#define RIDING_CAR 0.90
#define CAR_EMISSION 135
#define BUS_CAPACITY 7
#define BUS_STOPS 3

void calculateHasCarRatio(unsigned long long int people, unsigned long long int cars);
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

        unsigned int distanceTravelled = 0;

        double carEmission = 0;
};

class Bus : public Process{
    public:
        

        Bus();

        void Behavior();

    private:
        int capacity;

        int on_board;

        
};

class BusGenerator: public Event{
    public:

        BusGenerator(unsigned long long int buses);

        void Behavior();

        unsigned long long int buses;

    private:
        Store* busStore;
};

#endif