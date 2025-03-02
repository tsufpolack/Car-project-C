#include "DistributionNetwork.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

// Initializes a Distribution Network with the given parameters
void init_distribution_network(DistributionNetwork* network, int network_id, char* description, Factory** source_factories, int factories_count, int max_factories, Shop** destination_shops, int shops_count, int max_shops) {
    network->network_id = network_id;
    network->description = safe_strdup(description); // Duplicate the description safely

    // Assign the source factories directly, along with their logical and physical sizes
    network->source_factories = source_factories;
    network->factories_count = factories_count;
    network->max_factories = max_factories;

    // Assign the destination shops directly, along with their logical and physical sizes
    network->destination_shops = destination_shops;
    network->shops_count = shops_count;
    network->max_shops = max_shops;

}

void update_description(DistributionNetwork* network, char* new_description) {
    SAFE_FREE(network->description); // Free the old description safely
    network->description = safe_strdup(new_description); // Duplicate the new description
}

void add_source_factory(DistributionNetwork* network, Factory* factory) {
    add_element_to_array(&(network->source_factories), sizeof(Factory*), &network->factories_count, &network->max_factories, &factory);
}

void add_destination_shop(DistributionNetwork* network, Shop* shop) {
    add_element_to_array(&(network->destination_shops), sizeof(Shop*), &network->shops_count, &network->max_shops, &shop);
}

void remove_destination_shop(DistributionNetwork* network, int shopID) {
    for (int index = 0; index < network->shops_count; index++) {
        if (network->destination_shops[index]->shop_id == shopID) {
            // Found the shop to remove
            ARRAY_REMOVE_SHIFT(network->destination_shops, index, network->shops_count);
            network->shops_count--;
            break; // Exit after removing the shop
        }
    }
}

void remove_source_factory(DistributionNetwork* network, int factoryID) {
    for (int index = 0; index < network->factories_count; index++) {
        if (network->source_factories[index]->factory_id == factoryID) {
            // Found the factory to remove
            ARRAY_REMOVE_SHIFT(network->source_factories, index, network->factories_count);
            network->factories_count--;
            break; // Exit after removing the factory
        }
    }
}


void display_network_info(DistributionNetwork* network) {
    printf("Network ID: %d\nDescription: %s\n", network->network_id, network->description);
    printf("Source Factories Count: %d\nDestination Shops Count: %d\n", network->factories_count, network->shops_count);

    // Display each factory in the network
    printf("\n--- Factories ---\n");
    for (int i = 0; i < network->factories_count; ++i) {
        printf("Factory %d:\n", i + 1);
        display_factory_info(network->source_factories[i]); // Assumes display_factory_info is properly implemented
    }

    // Display each shop in the network
    printf("\n--- Shops ---\n");
    for (int j = 0; j < network->shops_count; ++j) {
        printf("Shop %d:\n", j + 1);
        display_shop_info(network->destination_shops[j]); // Assumes display_shop_info is properly implemented
    }
}

void free_distribution_network(DistributionNetwork* network) {
    SAFE_FREE(network->description);
    SAFE_FREE(network->source_factories);
    SAFE_FREE(network->destination_shops);
}

