

#include <stdio.h>
#include <stdlib.h>
#include "FileIO.h"

void getParameters(struct parameters *params){
    
    
    
    FILE *file = fopen("runSimulation.txt", "r");
    fscanf(file, "%i", &(params->lambda));
    fscanf(file, "%i", &(params->mu));
    fscanf(file, "%i", &(params->M));
    fscanf(file, "%i", &(params->numberOfArrivalsToSimulate));
    fclose(file);
    
    return;
    
    
}