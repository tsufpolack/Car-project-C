#include "Factory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_factory(Factory* factory, int id, const char* location, int capacity) {
    factory->factory_id = id;
    factory->location = safe_strdup(location);
    factory->production_capacity = capacity;
    factory->workers = NULL; // Initialize as NULL before any workers are added
    factory->num_workers = 0;
    factory->max_workers = 0; // No allocated space yet
    factory->max_components = 0;
    factory->max_models = 0;
    factory->num_components = 0;
    factory->num_models = 0;
    factory->produced_models = NULL;
    factory->produced_components = NULL;
    factory->workers = NULL;
}

void update_location_fact(Factory* factory, const char* new_location) {
    free(factory->location); // Free the old location string
    factory->location = safe_strdup(new_location); // Allocate and copy the new location
}

void update_production_capacity(Factory* factory, int new_capacity) {
    factory->production_capacity = new_capacity;
}

void add_new_model(Factory* factory, const char* name, int year, const char* manufacturer, double price, int volume, CarComponent** initialComponents, int numInitialComponents) {
    CarModel newModel; // Temporarily hold the new model
    // Initialize the new model with provided components
    init_car_model(&newModel, name, year, manufacturer, price, volume, initialComponents, numInitialComponents);

    // Add the new model to the factory's models array
    add_element_to_array((void**)&factory->produced_models, sizeof(CarModel), &factory->num_models, &factory->max_models, &newModel);
}



void add_new_component(Factory* factory, int component_id, const char* name, const char* type, double cost) {
    CarComponent* newComponent = (CarComponent*)safe_malloc(sizeof(CarComponent));
    // Initialize the new component
    init_car_component(newComponent, component_id, name, type, cost);

    display_component_info(newComponent);
    // Add the new component to the factory's components array
    add_element_to_array((void**)(& factory->produced_components), sizeof(CarComponent), &factory->num_components, &factory->max_components, newComponent);
}



void add_worker_to_fact(Factory* factory, Worker* worker) {
    if (factory->num_workers == factory->max_workers) { // Need to resize
        factory->max_workers = factory->max_workers == 0 ? 1 : factory->max_workers * 2;
        factory->workers = safe_realloc(factory->workers, factory->max_workers * sizeof(Worker*));
    }
    factory->workers[factory->num_workers++] = worker;
}

int remove_worker_from_fact(Factory* factory, int workerID) {
    int index;
    for (index = 0; index < factory->num_workers; index++) {
        if (factory->workers[index]->worker_id == workerID) {
            // Shift workers down to fill the gap left by the removed worker
            ARRAY_REMOVE_SHIFT(factory->workers, index, factory->num_workers);
            factory->num_workers--; // Decrease the logical size of the workers array
            return 1; // Found and removed the worker, so break out of the loop
        }
    }
    return 0; // Worker not found
}


void display_factory_info(Factory* factory) {
    printf("Factory ID: %d\nLocation: %s\nProduction Capacity: %d\n",
        factory->factory_id, factory->location, factory->production_capacity);

    // Displaying Car Models
    printf("\nProduced Models (%d):\n", factory->num_models);
    for (int i = 0; i < factory->num_models; i++) {
        // Assuming factory->produced_models is an array of CarModel objects
        display_model_info(&factory->produced_models[i]);
        printf("\n");
    }

    // Displaying Car Components
    printf("\nProduced Components (%d):\n", factory->num_components);
    for (int i = 0; i < factory->num_components; i++) {
        // Assuming factory->produced_components is an array of CarComponent objects
        display_component_info(&factory->produced_components[i]);
        printf("\n");
    }

    // Displaying Workers
    printf("\nWorkers (%d):\n", factory->num_workers);
    for (int i = 0; i < factory->num_workers; i++) {
        display_worker_info(factory->workers[i]);
        printf("\n");
    }
}

void free_factory(Factory* factory) {
    if (factory != NULL) {
        SAFE_FREE(factory->location);
        for (int i = 0; i < factory->num_models; ++i) {
            free_car_model(&factory->produced_models[i]);
        }
        SAFE_FREE(factory->produced_models);
        factory->produced_models = NULL;
           factory->num_models = 0;

        for (int i = 0; i < factory->num_components; ++i) {
            free_car_component(&factory->produced_components[i]);
        }
        SAFE_FREE(factory->produced_components);
        factory->produced_components = NULL;
        factory->num_components = 0;

        SAFE_FREE(factory->workers); // Free the array of worker pointers
    }
}

CarComponent* find_component_by_id(Factory* factory, int component_id) {
    for (int i = 0; i < factory->num_components; i++) {
        if (factory->produced_components[i].component_id == component_id) {
            return &factory->produced_components[i];
        }
    }
    return NULL;
}

