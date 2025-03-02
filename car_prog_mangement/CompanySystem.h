#ifndef COMPANYSYSTEM_H
#define COMPANYSYSTEM_H

#include "files_handling.h"
#include "Company.h"
#include "DistributionNetwork.h"
#include "Factory.h"
#include "Shop.h"
#include "CarModel.h"
#include "Worker.h"
#include "utils.h"
#include <stdio.h>
#include <string.h> // For strcmp in sorting by name
#include <stdlib.h> // For qsort
#define MAX_LEN 100
#define MAX_NET_LEN 10

typedef enum {
    UPLOAD_SYS_FROM_FILE = 1,
    SAVE_SYS_TO_FILE,
    DISPLAY_COMPANY,
    DISPLAY_DISTRIBUTION_NETWORKS,
    DISPLAY_SHOPS,
    DISPLAY_FACTORIES,
    DISPLAY_CAR_MODELS,
    ADD_WORKER,
    ADD_SHOP,
    ADD_FACTORY,
    ADD_CAR_MODEL,
    ADD_COMPONENT,
    ADD_MODEL_TO_INVENTORY,
    CHANGE_CEO,
    SORT_WORKERS_BY_NAME,
    SORT_WORKERS_BY_ID,
    SORT_WORKERS_BY_JOB_DETAILS,
    SEARCH_WORKER_BY_NAME,
    SEARCH_WORKER_BY_ID,
    SEARCH_WORKER_BY_JOB_DETAIL,
    MOVE_WORKER,
    FIRE_WORKER,
    EXIT
} MenuOptions;


// Macro to display menu options
#define DISPLAY_MENU() \
    printf("1.  Upload System Data\n" \
           "2.  Save System Data\n" \
           "3.  Display Company\n" \
           "4.  Display Distribution Networks\n" \
           "5.  Display Shops\n" \
           "6.  Display Factories\n" \
           "7.  Display Car Models\n" \
           "8.  Add Worker\n" \
           "9.  Add Shop\n" \
           "10. Add Factory\n" \
           "11. Add Car Model\n" \
           "12. Add Component\n" \
           "13. Add Model to Inventory\n" \
           "14. Change CEO\n" \
           "15. Sort Workers by Name\n" \
           "16. Sort Workers by ID\n" \
           "17. Sort Workers by Job Details\n" \
           "18. Search Worker by Name\n" \
           "19. Search Worker by ID\n" \
           "20. Search Worker by Job Detail\n" \
           "21. Move Worker from Factory/Shop to Factory/Shop\n" \
           "22. Fire Worker\n" \
           "23. Exit\n")

// Function prototypes

// Upload system data from file
void upload_system_data(Company** company);
// write system data to file
void write_system_data(Company* company);

// Start the company system
Company* start_company_system();
void run_company_system(Company** company);

// Display functions
void display_company(Company* company);
void display_distribution_networks(Company* company);
void display_shops(Company* company);
void display_factories(Company* company);
void display_car_models(Company* company);


// Add new entities
void change_CEO_to_sys(Company* company);
void add_worker_to_sys(Company* company); // Adds a new worker based on user input
void add_shop_to_sys(Company* company); // Adds a new shop based on user input
void add_factory_to_sys(Company* company); // Adds a new factory based on user input
void add_car_model_to_sys(Company* company); // Adds a new car model based on user input
void add_component_to_sys(Company* company);
void add_model_to_inventory_sys(Company* company);

// Sorting workers
void sort_workers_by_name(Company* company);
void sort_workers_by_id(Company* company);
void sort_workers_by_job_details(Company* company);

// Searching workers
void search_worker_by_name(Company* company);
void search_worker_by_id(Company* company);
void search_worker_by_job_detail(Company* company);

// move worker from factory/shop to factory/shop
void move_worker_factory_shop(Company* company);

// Fire worker from the company
void fire_worker_from_sys(Company* company);

#endif // COMPANYSYSTEM_H

