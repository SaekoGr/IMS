#include "StartDay.hpp"
#include "Person.hpp"

using namespace std;

double reg_interval;

StartDay::StartDay(unsigned long long int buses, unsigned long long int people, unsigned long long int cars, float ratio){
    this->people = people;
    this->buses = buses;
    this->cars = cars;
    this->ratio = ratio;
}

// calculates the interval of generating new people 
void calculate_req_interval(unsigned long long int people){
    reg_interval = ((double) (DAY_END - (10500))) / people;
}

void StartDay::Behavior(){
    // generates buses, activates bus generator and the interval
    if(!this->buses_generated){
        calculateHasCarRatio(this->people, this->cars, this->ratio);
        activateBusGenerator(this->buses);
        this->buses_generated = true;
        if(people != 0){
            calculate_req_interval(this->people);
        }
    }
    
    // generates population of n people
    if(this->people > 0){
        (new Person(this->people))->Activate();
        // new person appears every reg interval seconds
        Activate(Time + Exponential(reg_interval));
        this->people--;
    }   
}

