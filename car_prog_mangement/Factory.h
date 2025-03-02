#ifndef FACTORY_H
#define FACTORY_H

#include "CarModel.h"
#include "CarComponent.h"
#include "Worker.h"
#include "utils.h"

typedef struct Factory {
    int factory_id;
    char* location;
    int production_capacity;
    CarModel* produced_models; // Array of CarModel objects
    int num_models; // Logical size for produced_models
    int max_models; // Physical size for produced_models
    CarComponent* produced_components; // Array of CarComponent objects
    int num_components; // Logical size for produced_components
    int max_components; // Physical size for produced_components
    Worker** workers; // Dynamic array of pointers to Worker
    int num_workers; // Logical size for workers
    int max_workers; // Physical size for workers
} Factory;

// Factory initialization
void init_factory(Factory* factory, int id, const char* location, int capacity);

// Update functions
void update_location_fact(Factory* factory, const char* new_location);
void update_production_capacity(Factory* factory, int new_capacity);

// Functions to add new entities
void add_new_model(Factory* factory, const char* name, int year, const char* manufacturer, double price, int volume, CarComponent** initialComponents, int numInitialComponents);
void add_new_component(Factory* factory, int component_id, const char* name, const char* type, double cost);

// Function to manage workers
void add_worker_to_fact(Factory* factory, Worker* worker);
int remove_worker_from_fact(Factory* factory, int workerIndex);

// Display information
void display_factory_info(Factory* factory);

// Find functions
CarComponent* find_component_by_id(Factory* factory, int component_id);

// Free function
void free_factory(Factory* factory);

#endif // FACTORY_H
