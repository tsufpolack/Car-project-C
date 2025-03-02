#ifndef CARCOMPONENT_H
#define CARCOMPONENT_H
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Definition of the CarComponent structure
typedef struct CarComponent {
    int component_id; 
    char* name;
    char* type;
    double cost;
} CarComponent;

// Function declarations
void            init_car_component(CarComponent* component, int id, const char* name, const char* type, double cost);
void            update_component_name(CarComponent* component, char* new_name);
void            update_component_type(CarComponent* component, char* new_type);
void            update_cost(CarComponent* component, double new_cost);
void            display_component_info(CarComponent* component);
void            free_car_component(CarComponent* component);

#endif // CARCOMPONENT_H

