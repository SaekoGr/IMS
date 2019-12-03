#include "StartDay.hpp"
#include "Person.hpp"

using namespace std;


StartDay::StartDay(unsigned long long int buses, unsigned long long int people, unsigned long long int cars, float ratio){
    this->people = people;
    this->buses = buses;
    this->cars = cars;
}

void StartDay::Behavior(){
    // generate buses
    if(!this->buses_generated){
        printf("\nGenereting %llu buses\n\n", this->buses);
        calculateHasCarRatio(this->people, this->cars);
        //Store* store_of_buses = Store("Bus store", buses);
        activateBusGenerator(this->buses);
        this->buses_generated = true;
    }
    // generates population of n people
    if(this->people > 0){
        (new Person())->Activate();
        //printf("Generating %llu. th person\n", this->people);
        // new person appears every 60 seconds
        Activate(Time + Exponential(30));
        this->people--;
    }   
}

