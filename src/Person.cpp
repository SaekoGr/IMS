#include "Person.hpp"

using namespace std;

Store* AllBuses;
double allCarEmission = 0;
double allBusEmission = 0;
unsigned long long int wantToUseBus = 0;
unsigned long long int gotToUseBus = 0;

double carRatio;
Facility Stop;
Queue Q[BUS_STOPS];
int cnt = 0;
unsigned long long int usingCar = 0;
double bus_interval;

Stat* BusEmissions;
Stat* CarEmissions;
Stat* CarDistance;
Stat* BusDistance;
Stat* BusEffectivity;

//Queue Q[BUS_STOPS];

Person::Person(unsigned long long int personalNumber){
    this->personalNumber = personalNumber;
    this->needTransportTime = Time;
}

void Person::Behavior(){
    // chance of owning a car
    
    // owns a car
    if((Random() < hasCarRatio) && (availableCars > 0)){
        availableCars--;
        // has car and uses it
        if(Random() > carRatio){
            this->rideCar();
            this->carEmission = this->distanceTravelled * Normal(CAR_EMISSION, 5);
            allCarEmission += this->carEmission; 
            (*CarEmissions)(this->carEmission);
            (*CarDistance)(this->distanceTravelled);
            usingCar++;
            //printf("I travelled %d km with my car and produced %.2f kg of CO2 emmissions\n", this->distanceTravelled, (this->carEmission/1000.0));
            return;
        }
    } // does not own a car
    
    // either has car and uses public transport or doesn't own a car
    this->stopNum = generateBusStopNumber();
    //printf("BUS STOP NUM %d\n", this->stopNum);
    // going to the bus stop
    Wait(Normal(300, 60));

    wantToUseBus++;
    //printf("Standing at the queue: ID %d, STOP %d\n", this->personalNumber, this->stopNum);
    Q[this->stopNum].Insert(this);
    Passivate();
}

unsigned int generateBusStopNumber(){
    int rnd = rand();
    return (rnd % (BUS_STOPS));
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
        this->distanceTravelled = this->distanceTravelled + Normal(1, 0.1);
        goto ride_again;
    }
    
}

void initialiaze_day(){
    BusEmissions = new Stat("Emissions from bus in kg");
    CarEmissions = new Stat("Emissions from car in kg");
    CarDistance = new Stat("Distance travelled by car in km");
    BusDistance = new Stat("Distance travelled by bus in km");
    BusEffectivity = new Stat("Bus usage efecticity in %");
}

void output_stats(){
    unsigned long long int leftAtBusStop = 0;

    AllBuses->Output();
    printf("\n");
    CarEmissions->Output();
    CarDistance->Output();
    printf("\n");
    BusEmissions->Output();
    BusDistance->Output();
    BusEffectivity->Output();
    printf("\n");
    printf("\nTOTAL EMISSIONS\n");
    printf("CARS: %f kg\n", allCarEmission/1000.0);
    
    
    printf("BUSES: %f kg\n", allBusEmission/1000.0);
    

    double allEmissions = allBusEmission + allCarEmission;
    printf("SUM: %f kg\n", (allEmissions)/1000);

    printf("\n");
    if(usingCar > 0){
        printf("CAR EMISSION PER PERSON THAT USED CAR %f kg\n", (allCarEmission/usingCar)/1000);
    }
    else{
        printf("CAR EMISSION PER PERSON THAT USED CAR 0.0 kg\n");
    }

    if(gotToUseBus > 0){
        printf("BUS EMISSION PER PERSON THAT USED BUS %f kg\n", (allBusEmission/gotToUseBus)/1000);
    }
    else{
        printf("BUS EMISSION PER PERSON THAT USED BUS 0.0 kg\n");
    }

    if(usingCar + gotToUseBus > 0){
        printf("ALL EMISSION PER PERSON %f kg\n", (allEmissions/(usingCar + gotToUseBus))/1000);
    }
    else{
        printf("ALL EMISSION PER PERSON 0.0 kg\n");
    }
    printf("\n");
    printf("=================\n");
    printf("PASSENGERS DISTRIBUTION\n");
    printf("USED THE CAR %d\n", usingCar);
    printf("WANTED TO USE THE BUS %d\n", wantToUseBus);
    printf("ACTUALLY USED THE BUS %d\n", gotToUseBus);

    
    for(int i = 0; i < BUS_STOPS; i++){
        leftAtBusStop += Q[i].Length();
    }
    printf("PEOPLE LEFT AT ANY BUS STOPS: %d\n", leftAtBusStop);
    printf("\n");
}

void calculateHasCarRatio(unsigned long long int people, unsigned long long int cars, float ratio){
    availableCars = cars;
    carRatio = ratio;

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
}

Bus::Bus(){
    this->capacity = BUS_CAPACITY;
    this->on_board = 0;
    this->distanceTravelled = 0;
}

void Bus::Behavior(){
    int to_disembark;
    double start_time = Time;
    
    // bus goes to the first stop from the depot
    Wait(Normal(500, 100));
    this->distanceTravelled += Normal(2, 0.6);
    
    for(int i = 0; i < BUS_STOPS; i++){
        // last stop before going to depot
        
        //printf("DISEMBARKING\n");
        to_disembark = disembarkingPeople(this->on_board);
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
            gotToUseBus++;
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
    this->busEmission = this->distanceTravelled * Normal(BUS_EMISSION, 10);
    (*BusEmissions)(this->busEmission);
    (*BusDistance)(this->distanceTravelled);
    //printf("TRANS: %d\tMAX: %d\n", this->transportedPassangers, BUS_CAPACITY*BUS_STOPS);
    //printf("EFF U %f\n", (((double) this->transportedPassangers/((double) BUS_STOPS*BUS_CAPACITY))) * 100.0);
    double busEff = (((double) this->transportedPassangers/((double) BUS_STOPS*BUS_CAPACITY))) * 100.0;
    (*BusEffectivity)(busEff);
    allBusEmission += this->busEmission;
    
    //printf("Bus finishing my ride at %f, transported %d people, %f, produced %.2f of kg emissions\n\n\n", Time, this->transportedPassangers, this->distanceTravelled,(this->busEmission/1000.0));
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