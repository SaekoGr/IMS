/**
 * IMS Project
 * Simulation of middle-size European town and it's daily production of CO2
 * originating from the use of personal vehicles and buses
 * 
 * Sabína Gregušová (xgregu02)
 * Filip Weigel (xweige01)
 * 
 */
#include <stdio.h>
#include <ctype.h>
#include <simlib.h>
#include <iostream>
#include <getopt.h>
#include <string.h>
#include "StartDay.hpp"
#include "Person.hpp"



#define DEFAULT_CARS 60000
#define DEFAULT_BUSES 370
#define DEFAULT_PEOPLE 250000
#define DEFAULT_RATIO 25

using namespace std;

void help(){
    fprintf(stderr, "\nInput arguments are:\n\n");
    fprintf(stderr, "--cars to denote number of cars in a town\n");
    fprintf(stderr, "--buses to denote number of buses in a town\n");
    fprintf(stderr, "--people to denote number of people to transport\n");
    fprintf(stderr, "--transportRatio to denote the percentage of people who own cars, but are willing to use public transport\n\n");
    fprintf(stderr, "Default arguments are 60 cars, 100 people, 10 buses and transport ratio of 25%%\n\n");
}

bool isIntegerNumber(char* one_argument){
    bool result = true;

    for(unsigned int i = 0; i < strlen(one_argument); i++){
        if(!isdigit(one_argument[i])){
            result = false;
        }
    }

    if(result){
        return true;
    }
    else{
        return false;
    }
}

int main(int argc, char *argv[]){
    // 0 to 18,446,744,073,709,551,615
    unsigned long long int buses = DEFAULT_BUSES;
    unsigned long long int people = DEFAULT_PEOPLE;
    unsigned long long int cars = DEFAULT_CARS;
    // range from 0 to 100
    float ratio = DEFAULT_RATIO;

    static const char *shortOpts = "c:b:p:t";
    int opt;
    static const struct option longOpts[] = {
        {"buses", required_argument, nullptr, 'b'},
        {"cars", required_argument, nullptr, 'c'},
        {"transportRatio", required_argument, nullptr, 't'},
        {"people", required_argument, nullptr, 'p'},
        {nullptr, 0, nullptr, 0},
    };

    
    while((opt = getopt_long(argc, argv, shortOpts, longOpts, nullptr)) != -1)
    {
        switch(opt)
        {
            case 'b':
                if(isIntegerNumber(optarg)){
                    buses = strtoul(optarg, nullptr, 10);
                }
                else{
                    fprintf(stderr, "Number of buses must be an integer value\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'c':
                if(isIntegerNumber(optarg)){
                    cars = strtoul(optarg, nullptr, 10);
                }
                else{
                    fprintf(stderr, "Number of cars must be an integer value\n");
                    return EXIT_FAILURE;
                }
                
                break;
            case 't':
                ratio = stof(optarg);
                if((int) ratio < 0 || (int) ratio > 100){
                    fprintf(stderr, "Ratio must be in range 0 - 100\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'p':
                if(isIntegerNumber(optarg)){
                    people = strtoul(optarg, nullptr, 10);
                }
                else{
                    fprintf(stderr, "Number of people must be an integer value\n");
                    return EXIT_FAILURE;
                }
                break;
            case '?':
            default:
                help();
                return EXIT_FAILURE;
        }
    }

    ratio = ratio/100;
    printf("\n===================");
    printf("\nSTART OF SIMULATION\n");
    printf("===================\n\n");
    printf("ARGUMENTS: %llu BUSES, %llu PEOPLE, %llu CARS, %.2f RATIO\n\n", buses, people, cars, ratio);
    
    initialiaze_day();
    Init(DAY_START, DAY_END);
    (new StartDay(buses, people, cars, ratio))->Activate();
    Run();
    output_stats();
    SIMLIB_statistics.Output();

    printf("\n===================");
    printf("\nEND OF SIMULATION\n");
    printf("===================\n\n");

    return EXIT_SUCCESS;
}