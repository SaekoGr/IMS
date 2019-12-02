#ifndef IMS_START_DAY_HPP
#define IMS_START_DAY_HPP

#include <simlib.h>
#include <stdio.h>

class StartDay : public Event
{
    public:
        StartDay(unsigned long long int buses, unsigned long long int people, unsigned long long int cars, float ratio);

        void Behavior();

    private:
        unsigned long long int buses;
        unsigned long long int people;
        unsigned long long int cars;
        float ratio;
        
        bool buses_generated = false;
};

#endif