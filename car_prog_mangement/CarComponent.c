#define _CRT_SECURE_NO_WARNINGS
#include "CarComponent.h"
#include "utils.h" // Include the utilities for safe memory operations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_component_name(CarComponent* component, const char* new_name) {
    char* temp = safe_strdup(new_name); // Use safe_strdup to duplicate the new name
    free(component->name); // Free the old name
    component->name = temp; // Assign the new name
}

void update_component_type(CarComponent* component, const char* new_type) {
    char* temp = safe_strdup(new_type); // Duplicate the new type similarly
    free(component->type); // Free the old type
    component->type = temp; // Assign the new type
}

void update_cost(CarComponent* component, double new_cost) {
    component->cost = new_cost;
}

void display_component_info(CarComponent* component) {
    printf("Component ID: %d\n", component->component_id);
    printf("Name: %s\n", component->name);
    printf("Type: %s\n", component->type);
    printf("Cost: %.2f\n", component->cost);
}

void init_car_component(CarComponent* component, int id, const char* name, const char* type, double cost) {
    component->component_id = id;
    component->name = safe_strdup(name); // Allocate and copy
    component->type = safe_strdup(type);
    component->cost = cost;
}

void free_car_component(CarComponent* component) {
    if (component != NULL) {
        if (component->name != NULL)
            SAFE_FREE(component->name);
        if (component->type != NULL)
            SAFE_FREE(component->type);
        component->type = NULL;
        component->name = NULL;
    }
}
