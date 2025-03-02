#define _CRT_SECURE_NO_WARNINGS
#include "CarModel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize car model
void init_car_model(CarModel* model, const char* name, int year, const char* manufacturer, double price, int volume, CarComponent** components, int num_components) {
    strncpy(model->model_name, name, 29);
    model->model_name[29] = '\0'; // Ensure null termination
    model->model_year = year;
    model->manufacturer = safe_strdup(manufacturer);
    model->price = price;
    model->production_volume = volume;
    model->num_components = num_components;
    model->components = (CarComponent*)safe_malloc(num_components * sizeof(CarComponent*));
    for (int i = 0; i < num_components; ++i) {
        model->components[i] = components[i];
    }
}

// Update model name
void update_model_name(CarModel* model, const char* new_name) {
    strncpy(model->model_name, new_name, 29);
    model->model_name[strlen(new_name)] = '\0';
}

// Update model year
void update_model_year(CarModel* model, int new_year) {
    model->model_year = new_year;
}

// Update manufacturer
void update_manufacturer(CarModel* model, const char* new_manufacturer) {
    free(model->manufacturer); // Free the old manufacturer string
    model->manufacturer = safe_strdup(new_manufacturer);
}

// Update price
void update_price(CarModel* model, double new_price) {
    model->price = new_price;
}

// Add a component to the model
void add_component(CarModel* model, CarComponent* component) {
    model->num_components++;
    model->components = safe_realloc(model->components, model->num_components * sizeof(CarComponent*));
    model->components[model->num_components - 1] = component;
}

// Remove a component from the model
void remove_component(CarModel* model, int componentIndex) {
    if (componentIndex < 0 || componentIndex >= model->num_components) {
        printf("Invalid component index.\n");
        return;
    }

    for (int i = componentIndex; i < model->num_components - 1; ++i) {
        model->components[i] = model->components[i + 1];
    }

    model->num_components--;
    model->components = safe_realloc(model->components, model->num_components * sizeof(CarComponent*)); // Resize the array
}

// Calculate production cost
int calculate_production_cost(CarModel* model) {
    double total_cost = 0.0;
    for (int i = 0; i < model->num_components; ++i) {
        total_cost += model->components[i]->cost;
    }
    return (int)total_cost; // The function returns an int, so cast the total cost
}

// Display model information
void display_model_info(CarModel* model) {
    printf("Model Name: %s\n", model->model_name);
    printf("Model Year: %d\n", model->model_year);
    printf("Manufacturer: %s\n", model->manufacturer);
    printf("Price: %.2f\n", model->price);
    printf("Production Volume: %d\n", model->production_volume);
    printf("Number of Components: %d\n", model->num_components);
    for (int i = 0; i < model->num_components; ++i) {
        printf("\tComponent %d - Name: %s, Type: %s, Cost: %.2f\n",
            i + 1, model->components[i]->name, model->components[i]->type, model->components[i]->cost);
    }
}


// Free memory allocated for car model
void free_car_model(CarModel* model) {
    if (model != NULL) {
        SAFE_FREE(model->manufacturer);
        for (int i = 0; i < model->num_components; ++i) {
            free_car_component(model->components[i]);
            model->components[i] = NULL;
        }
        SAFE_FREE(model->components); // Free the array of pointers itself
        model->components = NULL;
    }
}
