#include "Person.hpp"

using namespace std;

static Store* AllBuses;

Facility Stop;
Queue Q[BUS_STOPS];
int cnt = 0;
unsigned long long int all_people = 0;
double bus_interval;
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
        if(Random() > ratio){
            //printf("I have car and I will use it\n");
            this->rideCar();
            this->carEmission = this->distanceTravelled * CAR_EMISSION;
            //printf("I travelled %d km with my car and produced %.2f kg of CO2 emmissions\n", this->distanceTravelled, (this->carEmission/1000.0));
            // succesfully traveled the distance
            //all_people++;
            cnt++;
            return;
        }
    } // does not own a car
    
    // either has car and uses public transport or doesn't own a car
    this->stopNum = generateBusStopNumber();

    // going to the bus stop
    Wait(Normal(300, 60));

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
        // average time for 1 km
        Wait(Normal(97,20));
        this->distanceTravelled++;
        goto ride_again;
    }
    
}


void calculateHasCarRatio(unsigned long long int people, unsigned long long int cars, float ratio){
    availableCars = cars;
    ratio = ratio;

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
    //printf("%lf\n", hasCarRatio);
}

Bus::Bus(){
    this->capacity = BUS_CAPACITY;
    this->on_board = 0;
    this->distanceTravelled = 0;
}

void Bus::Behavior(){
    //printf("%d\n", cnt++);
    int to_disembark;
    double start_time = Time;
    //printf("\n\nFree %d\tUsed %d\n\n", AllBuses->Free(), AllBuses->Used());
    //printf("\n\nI am bus generated at %f\n", Time);
    
    // bus goes to the first stop from the depot
    Wait(Normal(500, 100));
    this->distanceTravelled += Normal(2, 0.6);
    
    for(int i = 1; i < BUS_STOPS; i++){
        // last stop before going to depo
        //printf("I come to stop NO. %d\n", i);
        //printf("Length of Q[%d] is %d\n", i, Q[i].Length());
        
        //printf("DISEMBARKING\n");
        to_disembark = disembarkingPeople(this->on_board);
        //printf("%d people are disembarking\n", to_disembark);
        // people are disembarrking
        for(int j = 0; j < to_disembark; j++){
            // people are getting off
            Wait(Normal(2.5, 0.5));
            this->on_board--;
            
        }

        //printf("BOARDING NOW\n");
        // passangers can board
        boarding:
        

        // check for more passangers that fit into the bus
        if((!(Q[i].Empty())) && this->capacity > this->on_board){
            // somebody is getting on
            this->on_board++;
            this->transportedPassangers++;
            // he got on succesfully
            Entity *tmp = Q[i].GetFirst();
            Wait(Normal(2.5, 0.5));
            tmp->Activate();
            all_people++;
            cnt++;
            //cout << all_people << "\n";
            goto boarding;
        }

        Wait(Normal(84, 10));
        this->distanceTravelled += Normal(0.55, 0.1);
        //printf("There are currently %d people onborad\n", this->on_board);
        //printf("===============\n");
    }
   
    //printf("LAST STOP: EVERYONE DISEMBARK\n");
    while(this->on_board > 0){
        Wait(Normal(2.5, 0.5));
        this->on_board--;
    }

    // Simulates movement to the i-th bus stop
    //Wait()

    // Going back to depo
    Wait(Normal(500, 100));
    this->distanceTravelled += Normal(2, 0.6);

    double total_time = Time - start_time;
    Leave(*AllBuses, 1);
    this->busEmission = this->distanceTravelled * BUS_EMISSION;
    printf("Bus finishing my ride at %f, transported %d people, %f, produced %.2f of kg emissions\n\n\n", Time, this->transportedPassangers, this->distanceTravelled,(this->busEmission/1000.0));
    //all_people += this->transportedPassangers;
}

int disembarkingPeople(unsigned int onBoard){
    if(onBoard == 0){
        return 0;
    }
    int rnd = rand() % onBoard;
    return rnd;
}

void calculateBusInterval(unsigned long long int buses){
    bus_interval = (DAY_END - 7200)/buses;
}

void activateBusGenerator(unsigned long long int buses){
    // if there are no buses, generator is not needed
    calculateBusInterval(buses);

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
    
    //printf("AllBus je je to %d\n", AllBuses->Capacity());

    while(Time < 76400){
        Enter(*AllBuses, 1);
        (new Bus())->Activate();
        Wait(Exponential(bus_interval));
    }
    //printf("Generating %llu. th person\n", this->people);
    // new person appears every 60 seconds
    
}