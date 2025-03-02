#ifndef CARMODEL_H
#define CARMODEL_H

#include "CarComponent.h"
#include "utils.h" // For using safe memory functions
struct Factory;
#define MAX_MODEL_NAME 30
typedef struct CarModel {
    char model_name[MAX_MODEL_NAME];
    int model_year;
    char* manufacturer;
    double price;
    CarComponent** components; // Dynamic array of pointers to CarComponent
    int production_volume;
    int num_components; // To keep track of the number of components
} CarModel;

// Function declarations
void        init_car_model(CarModel* model, const char* name, int year, const char* manufacturer, double price, int volume, CarComponent** components, int num_components);
void        update_model_name(CarModel* model, const char* new_name);
void        update_manufacturer(CarModel* model, const char* new_manufacturer);
void        update_model_year(CarModel* model, int new_year);
void        update_price(CarModel* model, double new_price);
void        add_component(CarModel* model, CarComponent* component);
void        remove_component(CarModel* model, int componentIndex);
int         calculate_production_cost(CarModel* model);
void        display_model_info(CarModel* model);

void        free_car_model(CarModel* model);

#endif // CARMODEL_H