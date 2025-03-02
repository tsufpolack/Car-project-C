#ifndef FILES_HANDEL_H
#define FILES_HANDEL_H
#define _CRT_SECURE_NO_WARNINGS
#include "CarModel.h"
#include "DistributionNetwork.h"
#include "Company.h"
#include "CarComponent.h"
#include "Shop.h"
#include "Factory.h"
#include "Worker.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHECK_VALUE_OF_SCANF(x) if (x < 1) { printf("Error reading from file\n"); return NULL; }

// Function declarations
void save_company_info_bin(FILE* file, Company* company);
Company* load_company_info_bin(FILE* file);
void save_company_info_txt(FILE* file, Company* company);
Company* load_company_info_txt(FILE* file);
void save_worker_info_compressed(const Worker* worker, FILE* file);
Worker* load_worker_info_compressed(FILE* file);
void save_worker_info_text_file(const Worker* worker, FILE* file);
Worker* load_worker_info_text_file(FILE* file);
void save_shop_info_bin(FILE* file, Shop* shop);
Shop* load_shop_info_bin(FILE* file, Company* company);
void save_shop_info_txt(FILE* file, Shop* shop);
Shop* load_shop_info_txt(FILE* file, Company* company);
void save_factory_info_bin(FILE* file, Factory* factory);
Factory* load_factory_info_bin(FILE* file, Company* company);
void save_factory_info_txt(FILE* file, Factory* factory);
Factory* load_factory_info_txt(FILE* file, Company* company);
void save_component_info_bin(FILE* file, CarComponent* component);
CarComponent* load_component_info_bin(FILE* file);
void save_component_info_txt(FILE* file, CarComponent* component);
CarComponent* load_component_info_txt(FILE* file);
void save_model_info_bin(CarModel* model, FILE* file);
CarModel* load_model_info_bin(FILE* file, Factory* fact);
void save_model_info_txt(CarModel* model, FILE* file);
CarModel* load_model_info_txt(FILE* file, Factory* fact);
void save_network_info_bin(DistributionNetwork* network, FILE* file);
DistributionNetwork load_network_info_bin(Company* company, FILE* file);
void save_network_info_txt(DistributionNetwork* network, FILE* file);
DistributionNetwork load_network_info_txt(Company* company, FILE* file);


#endif // !FILES_HANDEL_H

