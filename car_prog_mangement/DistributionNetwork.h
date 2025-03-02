#ifndef DISTRIBUTION_NETWORK_H
#define DISTRIBUTION_NETWORK_H
#include "Factory.h"
#include "Shop.h"
#include <stdlib.h>

// Structure for DistributionNetwork
typedef struct {
    int network_id;
    char* description;
    Factory** source_factories;
    int factories_count; // Logical size of source factories array
    int max_factories; // Physical size of source factories array
    Shop** destination_shops;
    int shops_count; // Logical size of destination shops array
    int max_shops; // Physical size of destination shops array
} DistributionNetwork;

// Function declarations
void init_distribution_network(DistributionNetwork* network, int network_id, char* description, Factory** source_factories, int factories_count, int max_factories, Shop** destination_shops, int shops_count, int max_shops);
void update_description(DistributionNetwork* network, char* new_description);
void add_source_factory(DistributionNetwork* network, Factory* factory);
void remove_source_factory(DistributionNetwork* network, int factoryID);
void add_destination_shop(DistributionNetwork* network, Shop* shop);
void remove_destination_shop(DistributionNetwork* network, int shopID);
void display_network_info(DistributionNetwork* network);
void free_distribution_network(DistributionNetwork* network);

#endif // DISTRIBUTION_NETWORK_H
