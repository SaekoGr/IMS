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

// create new person
Person::Person(unsigned long long int personalNumber){
    this->personalNumber = personalNumber;
    this->needTransportTime = Time;
}

void Person::Behavior(){  
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
            // finishes here
            return;
        }
    } 
    // uses public transport
    
    // randomly generates bust stop
    this->stopNum = generateBusStopNumber();
    // goes the the assigned bus stop
    Wait(Normal(300, 60));

    // number of people who want to use the bus
    wantToUseBus++;
    // stands in queue at the given bus stop
    Q[this->stopNum].Insert(this);
    Passivate();
}

/**
 * Return the generated bus stop
 */
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
    printf("USED THE CAR %llu\n", usingCar);
    printf("WANTED TO USE THE BUS %llu\n", wantToUseBus);
    printf("ACTUALLY USED THE BUS %llu\n", gotToUseBus);

    
    for(int i = 0; i < BUS_STOPS; i++){
        leftAtBusStop += Q[i].Length();
    }
    printf("PEOPLE LEFT AT ANY BUS STOPS: %llu\n", leftAtBusStop);
    printf("\n");
}

// calculates has car ratio : cars / people
void calculateHasCarRatio(unsigned long long int people, unsigned long long int cars, float ratio){
    availableCars = cars;
    carRatio = ratio;
    

    if(people == 0){
        hasCarRatio = 0.0;
    }
    else{
        double temporaryRatio;
        temporaryRatio = ((double) cars / (double) people);
        if(temporaryRatio >= 1.0){
            hasCarRatio = 1.0;
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
    
    // bus goes to the first stop from the depot
    Wait(Normal(500, 100));
    this->distanceTravelled += Normal(2, 0.6);
    
    for(int i = 0; i < BUS_STOPS; i++){
        // last stop before going to depot
        
        // random number of people are disembarking at any given stop
        to_disembark = disembarkingPeople(this->on_board);
        // people are disembarrking
        for(int j = 0; j < to_disembark; j++){
            // people are getting off
            Wait(Normal(2.5, 0.5));
            this->on_board--;
        }

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
            // another one can board
            goto boarding;
        }

        // bus moves to the next bus stop
        Wait(Normal(84, 10));
        this->distanceTravelled += Normal(0.55, 0.1);
    }
   
    // last stop, everyone disembarks
    while(this->on_board > 0){
        Wait(Normal(2.5, 0.5));
        this->on_board--;
    }

    // Going back to depo
    Wait(Normal(500, 100));
    this->distanceTravelled += Normal(2, 0.6);

    // bus has finished
    Leave(*AllBuses, 1);
    // bus producted this amount of emissions
    this->busEmission = this->distanceTravelled * Normal(BUS_EMISSION, 10);

    // calculating bus effectivity
    double busEff = (((double) this->transportedPassangers/((double) BUS_STOPS*BUS_CAPACITY))) * 100.0;

    // stats
    (*BusEmissions)(this->busEmission);
    (*BusDistance)(this->distanceTravelled);
    (*BusEffectivity)(busEff);

    // add to the sum of bus emissions
    allBusEmission += this->busEmission;
}

// calculate how many people have to disembark
int disembarkingPeople(unsigned int onBoard){
    if(onBoard == 0){
        return 0;
    }
    int rnd = rand() % onBoard;
    return rnd;
}

// calculates the bus interval for generating new buses
void calculateBusInterval(unsigned long long int buses){
    bus_interval = (DAY_END - 7200)/buses;
}

void activateBusGenerator(unsigned long long int buses){
    // if there are no buses, generator is not needed
    calculateBusInterval(buses);

    // if no buses are inputet, bus generator is not even activated
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
    // calculates buses until it is not the end of the day - the reserve
    while(Time < 76400){
        Enter(*AllBuses, 1);
        (new Bus())->Activate();
        Wait(Exponential(bus_interval));
    }
}