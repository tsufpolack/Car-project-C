#ifndef SHOP_H
#define SHOP_H

#include "CarModel.h"
#include "worker.h"

typedef struct Shop {
    int shop_id;
    char* location;
    CarModel** inventory; // Dynamic array of pointers to CarModel
    int num_inventory; // Logical size of the inventory
    int max_inventory; // Physical size of the inventory
    double sales;
    Worker** workers; // Dynamic array of pointers to Worker
    int num_workers; // Logical size for workers
    int max_workers; // Physical size for workers
} Shop;

// Function declarations
void init_shop(Shop* shop, int id, const char* location);
void update_location_to_shop(Shop* shop, const char* new_location);
void add_to_inventory(Shop* shop, CarModel* model);
void remove_from_inventory(Shop* shop, int modelIndex);
void update_sales(Shop* shop, double sales_amount);
void add_worker_to_shop(Shop* shop, Worker* worker);
int  remove_worker_from_shop(Shop* shop, int workerID);
void display_shop_info(Shop* shop);
void free_shop(Shop* shop); 
#endif // SHOP_H

