#include "Company.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize a company
void init_company(Company* company, const char* name) {
    company->company_name = safe_strdup(name);
    company->head_shops_and_factories = NULL;
    company->num_shops_and_fact = 0;
    company->workers = NULL;
    company->num_workers = 0;
    company->max_workers = 0;
    company->id_counter = 0;
    

    // Initialize the distribution networks as empty
    init_distribution_network(&company->south, 1, "South Network", NULL, 0, 0, NULL, 0, 0, 0);
    init_distribution_network(&company->north, 2, "North Network", NULL, 0, 0, NULL, 0, 0, 0);
}

void add_shop_factory_node(Company* company, void* shop_or_factory, int is_shop)
{
	ShopFactoryNode* newNode = (ShopFactoryNode*)safe_malloc(sizeof(ShopFactoryNode));
	newNode->shop_or_factory = shop_or_factory;
	newNode->is_shop = is_shop;
	newNode->next = company->head_shops_and_factories;
	company->head_shops_and_factories = newNode;
	company->num_shops_and_fact++;
}

// Add a shop to the company
Shop* add_shop(Company* company, const char* location, char* network) {
    Shop* newShop = (Shop*)safe_malloc(sizeof(Shop));
    init_shop(newShop, company->num_shops_and_fact + 1, location);

    // Add shop to the appropriate network
    if (network == NULL) {}
    else if (strcmp(network, "south") == 0) {
        add_destination_shop(&company->south, newShop);
    }
    else if (strcmp(network, "north") == 0) {
        add_destination_shop(&company->north, newShop);
    }
    else {
        printf("Invalid network specified. Shop not added to any network.\n");
    }

    ShopFactoryNode* newNode = (ShopFactoryNode*)safe_malloc(sizeof(ShopFactoryNode));
    newNode->shop_or_factory = newShop;
    newNode->next = company->head_shops_and_factories;
    newNode->is_shop = 1;
    company->head_shops_and_factories = newNode;
    company->num_shops_and_fact++;

    return newShop;
}

// Add a factory to the company
Factory* add_factory(Company* company, const char* location, int production_capacity, char* network) {
    Factory* newFactory = (Factory*)safe_malloc(sizeof(Factory));
    init_factory(newFactory, company->num_shops_and_fact + 1, location, production_capacity);

    // Add factory to the appropriate network
    if (network == NULL) {}
    else if (strcmp(network, "south") == 0) {
        add_source_factory(&company->south, newFactory);
    }
    else if (strcmp(network, "north") == 0) {
        add_source_factory(&company->north, newFactory);
    }
    else {
        printf("Invalid network specified. Factory not added to any network.\n");
    }

    ShopFactoryNode* newNode = (ShopFactoryNode*)safe_malloc(sizeof(ShopFactoryNode));
    newNode->shop_or_factory = newFactory;
    newNode->next = company->head_shops_and_factories;
    newNode->is_shop = 0;
    company->head_shops_and_factories = newNode;
    company->num_shops_and_fact++;
    return newFactory;

}

// Freeing the company resources
void free_company(Company* company) {
    printf("%s",company->company_name);
    SAFE_FREE(company->company_name);

    ShopFactoryNode* current = company->head_shops_and_factories;
    while (current != NULL) {
        ShopFactoryNode* temp = current;
        current = current->next;

        if (temp->is_shop) {
            free_shop((Shop*)temp->shop_or_factory);
        }
        else {
            free_factory((Factory*)temp->shop_or_factory);
        }

        SAFE_FREE(temp);
    }

    // Assuming direct array for workers; if dynamic, iterate and free each worker
    for (int i = 0; i < company->num_workers; i++) {
        free_worker(company->workers[i]);
    }
    SAFE_FREE(company->workers);
}

// Helper function to free a shop or factory node
static void free_shop_factory_node_and_remove_from_network(ShopFactoryNode* node, Company* company, const char* network) {
    if (!node) return;

    // Check the is_shop flag to determine if it's a shop or factory
    if (node->is_shop) {
        // It's a shop, remove from network and free
        Shop* shop = (Shop*)node->shop_or_factory;
        if (strcmp(network, "south") == 0) {
            remove_destination_shop(&company->south, shop->shop_id);
        }
        else if (strcmp(network, "north") == 0) {
            remove_destination_shop(&company->north, shop->shop_id);
        }
        free_shop(shop);
    }
    else {
        // It's a factory, remove from network and free
        Factory* factory = (Factory*)node->shop_or_factory;
        if (strcmp(network, "south") == 0) {
            remove_source_factory(&company->south, factory->factory_id);
        }
        else if (strcmp(network, "north") == 0) {
            remove_source_factory(&company->north, factory->factory_id);
        }
        free_factory(factory);
    }
    free(node);
}

// Updated remove_shop function with is_shop check
void remove_shop(Company* company, int shopID, const char* network) {
    ShopFactoryNode* current = company->head_shops_and_factories, * prev = NULL;

    while (current != NULL) {
        if (current->is_shop) {
            Shop* shop = (Shop*)current->shop_or_factory;
            if (shop->shop_id == shopID) {
                if (prev == NULL) {
                    company->head_shops_and_factories = current->next;
                }
                else {
                    prev->next = current->next;
                }
                free_shop_factory_node_and_remove_from_network(current, company, network);
                (company->num_shops_and_fact)--;
                break;
            }
        }
        prev = current;
        current = current->next;
    }
}

// Updated remove_factory function with is_shop check
void remove_factory(Company* company, int factoryID, const char* network) {
    ShopFactoryNode* current = company->head_shops_and_factories, * prev = NULL;

    while (current != NULL) {
        if (!current->is_shop) {
            Factory* factory = (Factory*)current->shop_or_factory;
            if (factory->factory_id == factoryID) {
                if (prev == NULL) {
                    company->head_shops_and_factories = current->next;
                }
                else {
                    prev->next = current->next;
                }
                free_shop_factory_node_and_remove_from_network(current, company, network);
                (company->num_shops_and_fact)--;
                break;
            }
        }
        prev = current;
        current = current->next;
    }
}

void display_company_info(const Company* company) {
    printf("==============================\n");
    printf("========display company=======\n");
    printf("==============================\n");
    printf("Company: %s\n", company->company_name);
    printf("CEO: %s\n", company->CEO->worker_name);
    printf("Shops and Factories:\n");

    ShopFactoryNode* current = company->head_shops_and_factories;
    while (current != NULL) {
        if (current->is_shop) {
            Shop* shop = (Shop*)current->shop_or_factory;
            display_shop_info(shop);
        }
        else {
            Factory* factory = (Factory*)current->shop_or_factory;
            display_factory_info(factory);
        }
        current = current->next;
    }

    // display info about the distribution networks if needed
    printf("\nSouth Network:\n");
    display_network_info(&company->south);
    printf("\nNorth Network:\n");
    display_network_info(&company->north);
}

Worker* add_worker(Company* company, const char* name, const char* position, double salary, const char* department, int factory_id) {
    if (factory_id > company->num_shops_and_fact)
    {
        printf("No such factory/shop ID worker is not added\n");
        return NULL;
    }
    Worker* newWorker = (Worker*)safe_malloc(sizeof(Worker));
    init_worker(newWorker, ++(company->id_counter), name, position, salary, department, factory_id);

    // Add the new worker to the company's dynamic array of workers
    add_element_to_array((void**)&company->workers, sizeof(Worker*), &company->num_workers, &company->max_workers, &newWorker);
    if(factory_id == 0)
	{
        printf("Worker Added to Management\n");
        return newWorker;
	}
    // Attempt to add the worker to the specified shop or factory
    ShopFactoryNode* current = company->head_shops_and_factories;
    while (current != NULL) {
        if (current->is_shop && ((Shop*)current->shop_or_factory)->shop_id == factory_id) {
            // Add worker to the shop
            add_worker_to_shop((Shop*)current->shop_or_factory, newWorker);
            break; // Worker added, exit loop
        }
        else if (!current->is_shop && ((Factory*)current->shop_or_factory)->factory_id == factory_id) {
            // Add worker to the factory
            add_worker_to_fact((Factory*)current->shop_or_factory, newWorker);
            break; // Worker added, exit loop
        }
        current = current->next;
    }
    return newWorker;
}

void change_CEO(Company* company, Worker* ceo) {
    company->CEO = ceo;
}

ShopFactoryNode* find_factory_shop_by_id(Company* company, int factory_id) {
    ShopFactoryNode* current = company->head_shops_and_factories;
    while (current != NULL) {
        if (!current->is_shop) {
            Factory* factory = (Factory*)current->shop_or_factory;
            if (factory->factory_id == factory_id) {
                return current;
            }
        }
        else
        {
            Shop* shop = (Shop*)current->shop_or_factory;
            if (shop->shop_id == factory_id) {
                return current;
            }
        }
        current = current->next;
    }
    return NULL;
}

int find_worker_by_id(Company * company, int worker_id)
{
	for (int i = 0; i < company->num_workers; i++) 
    {
		if (company->workers[i]->worker_id == worker_id) {
			return i;
		}
	}
	return -1;
}

void fire_worker(Company* company, int worker_id) {
    int worker_index = find_worker_by_id(company, worker_id);
	if (worker_index == -1) {
		printf("Worker with ID %d not found.\n", worker_id);
		return;
	}
    // implement the rest of the function notice that the worker have is shop/ fact id inside the wokre so you dont need to look for it
    Worker* worker = company->workers[worker_index];   
    ShopFactoryNode* current = company->head_shops_and_factories;  
    while (current != NULL) {
		if (current->is_shop && ((Shop*)current->shop_or_factory)->shop_id == worker->factory_id) {
			// Remove worker from the shop
			remove_worker_from_shop((Shop*)current->shop_or_factory, worker_id);
			break; // Worker removed, exit loop
		}
		else if (!current->is_shop && ((Factory*)current->shop_or_factory)->factory_id == worker->factory_id) {
			// Remove worker from the factory
			remove_worker_from_fact((Factory*)current->shop_or_factory, worker_id);
			break; // Worker removed, exit loop
		}
		current = current->next;
	}
    free_worker(worker);
	remove_element_from_array((void**)&company->workers, sizeof(Worker*), &company->num_workers, &company->max_workers, worker_index);
}

void move_worker(Company* company, int worker_id, int factory_shop_id) {
    int worker_index = find_worker_by_id(company, worker_id);
	if (worker_index == -1) {
		printf("Worker with ID %d not found.\n", worker_id);
		return;
	}
	Worker* worker = company->workers[worker_index];
    ShopFactoryNode* src = find_factory_shop_by_id(company, worker->factory_id);
    ShopFactoryNode* dst = find_factory_shop_by_id(company, factory_shop_id);
    
    if (dst == NULL) {
        printf("No such factory shop ID worker is not moved");
        return;
    }

    if (src->is_shop) {
		remove_worker_from_shop((Shop*)src->shop_or_factory, worker_id);
	}
	else {
		remove_worker_from_fact((Factory*)src->shop_or_factory, worker_id);
	}
    if (dst->is_shop) {
		add_worker_to_shop((Shop*)dst->shop_or_factory, worker);
	}
	else {
		add_worker_to_fact((Factory*)dst->shop_or_factory, worker);
	}
}

CarModel* find_model_by_name(Company* company, const char* model_name) {
	ShopFactoryNode* current = company->head_shops_and_factories;
	while (current != NULL) {
		if (!current->is_shop) {
			Factory* factory = (Factory*)current->shop_or_factory;
			for (int i = 0; i < factory->num_models; i++) {
				if (strcmp(factory->produced_models[i].model_name, model_name) == 0) {
					return &factory->produced_models[i];
				}
			}
		}
		else {
			Shop* shop = (Shop*)current->shop_or_factory;
			for (int i = 0; i < shop->num_inventory; i++) {
				if (strcmp(shop->inventory[i]->model_name, model_name) == 0) {
					return shop->inventory[i];
				}
			}
		}
		current = current->next;
	}
	return NULL;
}

void display_workers_company(Company* company) {
	printf("Workers (%d):\n", company->num_workers);
	for (int i = 0; i < company->num_workers; i++) {
        printf("====================================\n");
		display_worker_info(company->workers[i]);
        printf("====================================\n");
	}
}

