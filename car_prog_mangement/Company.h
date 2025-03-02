#ifndef COMPANY_H
#define COMPANY_H

#include "Shop.h"
#include "Factory.h"
#include "Worker.h"
#include "DistributionNetwork.h"

typedef struct ShopFactoryNode {
    void*                       shop_or_factory; // Pointer to either a Shop or a Factory
    int                         is_shop;
    struct ShopFactoryNode*     next;
} ShopFactoryNode;

typedef struct Company {
    char*               company_name;
    ShopFactoryNode*    head_shops_and_factories; // Linked list of shops and factories
    Worker*              CEO;
    int                 num_shops_and_fact; // Total number of shops and factories
    Worker**            workers; // Array of workers
    int                 num_workers;
    int                 max_workers;
    int 			    id_counter;
    DistributionNetwork south, north; // Distribution networks
} Company;


void                init_company(Company* company, const char* name);
void                change_CEO(Company* company, Worker* ceo);
Shop*               add_shop(Company* company, const char* location, char* network);
Factory*            add_factory(Company* company, const char* location, int production_capacity, char* network);
void                add_shop_factory_node(Company* company, void* shop_or_factory, int is_shop);
void                remove_shop(Company* company, int shopID, const char* network);
void                remove_factory(Company* company, int factoryID, const char* network);
Worker*             add_worker(Company* company, const char* name, const char* position, double salary, const char* department, int factory_id);
ShopFactoryNode*    find_factory_shop_by_id(Company* company, int factory_id);
CarModel* 		    find_model_by_name(Company* company, const char* model_name);
int                 find_worker_by_id(Company* company, int worker_id);
void                free_company(Company* company);
void                move_worker(Company* company, int worker_id, int factory_shop_id);
void 	            fire_worker(Company* company, int worker_id);
void                display_workers_company(Company* company);

#endif // COMPANY_H

