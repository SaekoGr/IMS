#include "Person.hpp"

using namespace std;

static Store* AllBuses;

Facility Stop;
Queue Q[BUS_STOPS];
//Queue Q[BUS_STOPS];

Person::Person(unsigned long long int personalNumber){
    this->personalNumber = personalNumber;
    this->needTransportTime = Time;
}

void Person::Behavior(){
    // chance of owning a car
    
    // owns a car
    // TODO actually submit the cars from the amount... or?
    if((Random() < hasCarRatio) && (availableCars > 0)){
        //printf("I have car at %lf time\n", this->needTransportTime);
        availableCars--;
        // has car and uses it
        if(Random() > USES_PUBLIC_TRANSPORT){
            //printf("I have car and I will use it\n");
            this->rideCar();
            this->carEmission = this->distanceTravelled * CAR_EMISSION;
            //printf("I travelled %d km with my car and produced %.2f kg of CO2 emmissions\n", this->distanceTravelled, (this->carEmission/1000.0));
            // succesfully traveled the distance
            return;
        }
    } // does not own a car
    
    // either has car and uses public transport or doesn't own a car
    this->stopNum = generateBusStopNumber();

    //printf("Standing at the queue: ID %d, STOP %d\n", this->personalNumber, this->stopNum);
    Q[this->stopNum].Insert(this);
    Passivate();

    //printf("Nastúpil som ID %d\n", this->personalNumber);
}

unsigned int generateBusStopNumber(){
    int rnd = rand();
    return (rnd % BUS_STOPS);
}

void Person::rideCar(){
    // Riding car
    ride_again:

    if(Random() < RIDING_CAR || this->distanceTravelled == 0){
        if(this->distanceTravelled >= MAX_DISTANCE){
            return;
        }
        //Wait()
        this->distanceTravelled++;
        goto ride_again;
    }
    
}


void calculateHasCarRatio(unsigned long long int people, unsigned long long int cars){
    availableCars = cars;

    if(people == 0){
        hasCarRatio = 0.0;
    }
    else{
        double temporaryRatio;
        temporaryRatio = ((double) cars / (double) people);
        if(temporaryRatio >= 100){
            hasCarRatio = 100;
        }
        else{
            hasCarRatio = temporaryRatio;
        }
    }
    printf("%lf\n", hasCarRatio);
}

Bus::Bus(){
    this->capacity = BUS_CAPACITY;
    this->on_board = 0;
}

void Bus::Behavior(){
    int to_disembark;
    // bus depart from depo
    Enter(*AllBuses, 1);
    printf("\n\nI am bus generated at %f\n", Time);
    
    Wait(500);
    
    
    for(int i = 1; i < BUS_STOPS; i++){
        // last stop before going to depo
        printf("I come to stop NO. %d\n", i);
        printf("Length of Q[%d] is %d\n", i, Q[i].Length());
        
        printf("DISEMBARKING\n");
        to_disembark = disembarkingPeople(this->on_board);
        printf("%d people are disembarking\n", to_disembark);
        this->on_board = this->on_board - to_disembark; 

        printf("BOARDING NOW\n");
        // passangers can board
        boarding:
        

        // check for more passangers that fit into the bus
        if(!Q[i].Empty() && this->capacity > this->on_board){
            // somebody is getting on
            Wait(5);
            this->on_board++;
            // he got on succesfully
            Entity *tmp = Q[i].GetFirst();
            tmp->Activate();
            goto boarding;
        }
        printf("There are currently %d people onborad\n", this->on_board);
        printf("===============\n");
    }
   
    printf("LAST STOP: EVERYONE DISEMBARK\n");
    this->on_board = 0;

    printf("I have %d people, bus is leaving to depo\n", this->on_board);
    // Simulates movement to the i-th bus stop
    //Wait()


    
    Wait(500);

    printf("I am bus finishing my ride at %f\n\n\n", Time);
    Leave(*AllBuses, 1);
}

int disembarkingPeople(unsigned int onBoard){
    if(onBoard == 0){
        return 0;
    }
    int rnd = rand() % onBoard;
    return rnd;
}

void activateBusGenerator(unsigned long long int buses){
    // if there are no buses, generator is not needed
    if(buses == 0){
        return;
    }
    // create all buses
    AllBuses = new Store("Bus store", buses);
    // activate the bus generator
    (new BusGenerator(buses))->Activate();
}

BusGenerator::BusGenerator(unsigned long long int buses){
    
    this->buses = buses;
}

void BusGenerator::Behavior(){
    
    
    //printf("Used %d\n", AllBuses->Used());
    //printf("Free %d\n", AllBuses->Free());
    //printf("AllBus je je to %d\n", AllBuses->Capacity());

        //printf("Idem generovať autobus\n");
    (new Bus())->Activate();
    Activate(Time + 15000);
        
    
    
    //printf("Generating %llu. th person\n", this->people);
    // new person appears every 60 seconds
    //
    
    
    
}