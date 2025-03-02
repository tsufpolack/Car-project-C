#include "Shop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Assuming utils.h includes necessary utility functions like safe_strdup and SAFE_FREE
#include "utils.h"

void init_shop(Shop* shop, int id, const char* location) {
    shop->shop_id = id;
    shop->location = safe_strdup(location);
    shop->inventory = NULL;
    shop->num_inventory = 0;
    shop->max_inventory = 0;
    shop->sales = 0.0;
    shop->workers = NULL;
    shop->num_workers = 0;
    shop->max_workers = 0;
}

void update_location_to_shop(Shop* shop, const char* new_location) {
    SAFE_FREE(shop->location);
    shop->location = safe_strdup(new_location);
}

void add_to_inventory(Shop* shop, CarModel* model) {
    if (shop->num_inventory == shop->max_inventory) {
        int new_size = (shop->max_inventory == 0) ? 1 : shop->max_inventory * 2;
        shop->inventory = safe_realloc(shop->inventory, new_size * sizeof(CarModel*));
        shop->max_inventory = new_size;
    }
    shop->inventory[shop->num_inventory++] = model;
}

void remove_from_inventory(Shop* shop, int modelIndex) {
    if (modelIndex < 0 || modelIndex >= shop->num_inventory) return;
    ARRAY_REMOVE_SHIFT(shop->inventory, modelIndex, shop->num_inventory);
    shop->num_inventory--;
}

void update_sales(Shop* shop, double sales_amount) {
    shop->sales += sales_amount;
}

void add_worker_to_shop(Shop* shop, Worker* worker) {
    if (shop->num_workers == shop->max_workers) {
        int new_size = (shop->max_workers == 0) ? 1 : shop->max_workers * 2;
        shop->workers = safe_realloc(shop->workers, new_size * sizeof(Worker*));
        shop->max_workers = new_size;
    }
    shop->workers[shop->num_workers++] = worker;
}

void display_shop_info(Shop* shop) {
    printf("Shop ID: %d\nLocation: %s\nSales: $%.2f\n", shop->shop_id, shop->location, shop->sales);
    printf("Inventory (%d models):\n", shop->num_inventory);
    for (int i = 0; i < shop->num_inventory; i++) {
        display_model_info(shop->inventory[i]);
    }
    printf("Workers (%d):\n", shop->num_workers);
    for (int i = 0; i < shop->num_workers; i++) {
        display_worker_info(shop->workers[i]);
    }
}

int remove_worker_from_shop(Shop* shop, int workerID){
    int index;
    for (index = 0; index < shop->num_workers; index++) {
        if (shop->workers[index]->worker_id == workerID) {
            // Shift workers down to fill the gap left by the removed worker
            ARRAY_REMOVE_SHIFT(shop->workers, index, shop->num_workers);
            shop->num_workers--; // Decrease the logical size of the workers array
            return 1; // Found and removed the worker, so break out of the loop
        }
    }
    return 0; // Worker not found
}

void free_shop(Shop* shop) {
    SAFE_FREE(shop->location);
    shop->num_inventory = 0;
    SAFE_FREE(shop->inventory);
    shop->num_workers = 0;
    SAFE_FREE(shop->workers);
}
