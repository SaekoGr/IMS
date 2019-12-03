#include "Person.hpp"

using namespace std;

Store* AllBuses;

Person::Person(){
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
        }
        else{
            //printf("I have car and I will not use it\n");
            //goto use_public_transport;
        }
    } // does not own a car
    else{
        //printf("I do not have car at all\n");

        //use_public_transport:
    }
    //printf("\n");
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

Bus::Bus(Store* busStore){
    this->busStore = busStore;
    
}

void Bus::Behavior(){
    //getBusStore();
    printf("I am bus generated at %f\n", Time);
    //printf("Store has %d\n", AllBuses->Capacity());
    //Enter(*AllBuses, 1);
    
    //Wait(500);
    //printf("I am bus finish my ride at %f\n", Time);
    //Leave(*AllBuses);
}

void getBusStore(){
    printf("Tu to je zas %d\n", AllBuses->Capacity());
}


void activateBusGenerator(unsigned long long int buses){
    //printf("Teraz to je %d\n", buses->Capacity());
    (new BusGenerator(buses))->Activate();
}

BusGenerator::BusGenerator(unsigned long long int buses){
    this->buses = buses;
}

void BusGenerator::Behavior(){
    this->busStore = Store("Bus store", buses);
    printf("Aktivoval som sa a teraz je to %d\n", this->busStore->Capacity());


    while(Time < 86400){
        Enter(*(this->busStore), 1);
        
        printf("Idem generovať autobus\n");
        (new Bus(this->busStore))->Activate();
        Wait(10000);
        
        Leave(*(this->busStore), 1);

    }
    
    //printf("Generating %llu. th person\n", this->people);
    // new person appears every 60 seconds
    //
    
    
    
}