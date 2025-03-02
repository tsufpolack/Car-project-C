#include "files_handling.h"


// Helper function to encode a character into a 5-bit representation
uint8_t encode_char(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';  // 'a' becomes 0, 'b' becomes 1, ..., 'z' becomes 25
    }
    // Optionally handle uppercase and spaces
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    if (c == ' ') {
        return 26;  // Represent space as 26
    }
    return 27;  // Using 27 to represent any other character as '?'
}

// Helper function to decode a 5-bit value into a character
char decode_char(uint8_t encoded) {
    if (encoded < 26) {
        return 'a' + encoded;  // Convert 0-25 back to 'a'-'z'
    }
    if (encoded == 26) {
        return ' ';  // Decode 26 back to space
    }
    return '?';  // Use '?' to represent any unrecognized encoding
}

// Function to encode a name into a 5-bit per character representation
void encode_name(const char* name, uint8_t* encoded_name, int name_length, int* bit_count) {
    *bit_count = name_length * 5;  // 5 bits per character
    memset(encoded_name, 0, (*bit_count + 7) / 8);  // Clear the buffer

    for (int i = 0, bit_index = 0; i < name_length; ++i, bit_index += 5) {
        uint8_t encoded_char = encode_char(name[i]);
        int byte_index = bit_index / 8;
        int bit_offset = bit_index % 8;
        encoded_name[byte_index] |= encoded_char << bit_offset;
        if (bit_offset > 3) {  // Handle spillover to the next byte
            encoded_name[byte_index + 1] |= encoded_char >> (8 - bit_offset);
        }
    }
}

// Function to decode a name from a 5-bit per character representation
void decode_name(const uint8_t* encoded_name, char* decoded_name, int name_length) {
    for (int i = 0, bit_index = 0; i < name_length; ++i, bit_index += 5) {
        int byte_index = bit_index / 8;
        int bit_offset = bit_index % 8;
        uint8_t bits = (encoded_name[byte_index] >> bit_offset) | (encoded_name[byte_index + 1] << (8 - bit_offset));
        decoded_name[i] = decode_char(bits & 0x1F);  // Mask the lowest 5 bits
    }
    decoded_name[name_length] = '\0';  // Null terminate the string
}

// Helper functions to write/read bits are also assumed
void save_worker_info_compressed(const Worker* worker, FILE* file) {
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    uint8_t name_length = (uint8_t)strlen(worker->worker_name);
    int bit_count;
    uint8_t* encoded_name = malloc((name_length * 5 + 7) / 8);
    if (!encoded_name) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }
    encode_name(worker->worker_name, encoded_name, name_length, &bit_count);

    // Write the name length and encoded name
    fwrite(&name_length, sizeof(uint8_t), 1, file);
    fwrite(encoded_name, sizeof(uint8_t), (bit_count + 7) / 8, file);

    // Write the worker ID, salary, and factory ID
    uint16_t worker_id = (uint16_t)worker->worker_id;
    fwrite(&worker_id, sizeof(worker_id), 1, file);

    uint16_t salary_compressed = (uint16_t)(worker->salary / 1000);
    fwrite(&salary_compressed, sizeof(salary_compressed), 1, file);

    uint8_t factory_id = (uint8_t)worker->factory_id;
    fwrite(&factory_id, sizeof(factory_id), 1, file);
    
    // write the job details
    save_string_bin(worker->job_details.position, file);
    save_string_bin(worker->job_details.department, file);


    free(encoded_name);
}

// Load worker info from a compressed binary file
Worker* load_worker_info_compressed(FILE* file) {

    uint8_t name_length;
    fread(&name_length, sizeof(uint8_t), 1, file);

    int bit_count = name_length * 5;
    uint8_t* encoded_name = malloc((bit_count + 7) / 8);
    if (!encoded_name) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }
    fread(encoded_name, sizeof(uint8_t), (bit_count + 7) / 8, file);

    char* decoded_name = malloc(name_length + 1);
    if (!decoded_name) {
        perror("Memory allocation failed");
        free(encoded_name);
        fclose(file);
        return NULL;
    }
    decode_name(encoded_name, decoded_name, name_length);

    uint16_t worker_id;
    fread(&worker_id, sizeof(worker_id), 1, file);

    uint16_t salary_compressed;
    fread(&salary_compressed, sizeof(salary_compressed), 1, file);

    uint8_t factory_id;
    fread(&factory_id, sizeof(factory_id), 1, file);

    // read the job details
    char* position = load_string_bin(file);
    char* department = load_string_bin(file);

    // Make the first letter of word in name uppercase
    for(int i = 0; i < name_length; i++)
	{
		if(decoded_name[i] == ' ' && i + 1 < name_length)
		{
			decoded_name[i + 1] = decoded_name[i + 1] - 'a' + 'A';
		}
	}
    Worker* worker = safe_malloc(sizeof(Worker));
    init_worker(worker, worker_id, decoded_name, position, salary_compressed * 1000, department, factory_id);


    free(encoded_name);
    return worker;
}

// Save a worker into a text file
void save_worker_info_text_file(const Worker* worker, FILE* file) {
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "Worker Name: %d %s\n", (int)strlen(worker->worker_name), worker->worker_name);
    fprintf(file, "Worker ID: %d\n", worker->worker_id);
    fprintf(file, "Position: %d %s\n", (int)strlen(worker->job_details.position), worker->job_details.position);
    fprintf(file, "Salary: %.2f\n", worker->salary);
    fprintf(file, "Department: %d %s\n", (int)strlen(worker->job_details.department), worker->job_details.department);
    fprintf(file, "Factory ID: %d\n", worker->factory_id);

}

// Load a worker from a text file
Worker* load_worker_info_text_file(FILE* file) {

    // Temporary storage to read string data from file
    char* name, *position, *department;
    float salary;
    int id, factoryId, len, returned_value_for_err;
    
    // Attempt to read the worker data from the file
    returned_value_for_err = fscanf(file, "Worker Name: %d", &len);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
		return NULL;
	}
    name = (char*)safe_malloc((len + 1)*sizeof(char));
    returned_value_for_err = fscanf(file, " %[^\n]\n", name);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    name[len] = '\0';
    returned_value_for_err = fscanf(file, "Worker ID: %d\n", &id);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    returned_value_for_err = fscanf(file, "Position: %d", &len);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    position = (char*)safe_malloc((len + 1)*sizeof(char));
    returned_value_for_err = fscanf(file, " %[^\n]\n", position);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    position[len] = '\0';
    returned_value_for_err = fscanf(file, "Salary: %f\n", &salary);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    returned_value_for_err = fscanf(file, "Department: %d", &len);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    department = (char*)safe_malloc((len + 1)*sizeof(char));
    returned_value_for_err = fscanf(file, " %[^\n]\n", department);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    department[len] = '\0';
    returned_value_for_err = fscanf(file, "Factory ID: %d\n", &factoryId);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }

    // Assigning read values to worker struct with memory allocation for strings
    Worker* worker = safe_malloc(sizeof(Worker));
    init_worker(worker, id, name, position, salary, department, factoryId);

    return worker;
}

// Save a car model to a binary file
void save_model_info_bin(CarModel* model, FILE* file) {
    if (file == NULL || model == NULL) {
        perror("File or model pointer is NULL");
        return;
    }

    // Write the basic model information
    fwrite(model->model_name, sizeof(char), 30, file);  // Model name fixed at 30 chars
    fwrite(&model->model_year, sizeof(int), 1, file);  // Model year
    int manufacturer_len = (int)strlen(model->manufacturer) + 1;
    fwrite(&manufacturer_len, sizeof(int), 1, file);  // Length of manufacturer name including null terminator
    fwrite(model->manufacturer, sizeof(char), manufacturer_len, file);  // Manufacturer name
    fwrite(&model->price, sizeof(double), 1, file);  // Price
    fwrite(&model->production_volume, sizeof(int), 1, file);  // Production volume

    // Write the number of components
    fwrite(&model->num_components, sizeof(int), 1, file);

    // Write only the component_id of each CarComponent
    for (int i = 0; i < model->num_components; ++i) {
        if (model->components[i] != NULL) {
            fwrite(&model->components[i]->component_id, sizeof(int), 1, file);
        }
    }
}

// Load a car model from a binary file
CarModel* load_model_info_bin(FILE* file, struct Factory* fact)
{
    CarModel* model = safe_malloc(sizeof(CarModel));
    // Read model name
    fread(model->model_name, sizeof(char), 30, file);

    // Read model year
    fread(&model->model_year, sizeof(int), 1, file);

    // Read manufacturer name
    int manufacturer_len;
    fread(&manufacturer_len, sizeof(int), 1, file);
    model->manufacturer = safe_malloc(manufacturer_len);
    fread(model->manufacturer, sizeof(char), manufacturer_len, file);

    // Read price and production volume
    fread(&model->price, sizeof(double), 1, file);
    fread(&model->production_volume, sizeof(int), 1, file);

    // Read number of components
    fread(&model->num_components, sizeof(int), 1, file);
    model->components = safe_malloc(model->num_components * sizeof(CarComponent*));

    // Read each component by its ID and link it
    for (int i = 0; i < model->num_components; i++) {
        int comp_id;
        fread(&comp_id, sizeof(int), 1, file);
        model->components[i] = find_component_by_id(fact, comp_id);
        if (model->components[i] == NULL) {
            printf("Component with ID %d not found in the factory\n", comp_id);
        }
    }

    return model;
}

// Save a car model to a text file
void save_model_info_txt(CarModel* model, FILE* file) {
    // Save basic model information
    fprintf(file, "Model Name: %s\n", model->model_name);
    fprintf(file, "Model Year: %d\n", model->model_year);
    fprintf(file, "Manufacturer: %d %s\n", (int)strlen(model->manufacturer), model->manufacturer);
    fprintf(file, "Price: %.2f\n", model->price);
    fprintf(file, "Production Volume: %d\n", model->production_volume);
    fprintf(file, "Number of Components: %d\n", model->num_components);

    // Save component IDs
    for (int i = 0; i < model->num_components; i++) {
        if (model->components[i] != NULL) {
            fprintf(file, "Component ID %d: %d\n", i + 1, model->components[i]->component_id);
        }
    }
}

// Load a car model from a text file
CarModel* load_model_info_txt(FILE* file, struct Factory* fact) {
    CarModel* model = safe_malloc(sizeof(CarModel));

    char buffer[256];
    int component_id, returned_value_for_err;


    // Read and set model information
    returned_value_for_err = fscanf(file, "Model Name: %[^\n]\n", model->model_name);

    returned_value_for_err = fscanf(file, "Model Year: %d\n", &model->model_year);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    returned_value_for_err = fscanf(file, "Manufacturer: %[^\n]\n", buffer);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    model->manufacturer = safe_strdup(buffer);
    returned_value_for_err = fscanf(file, "Price: %lf\n", &model->price);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    returned_value_for_err = fscanf(file, "Production Volume: %d\n", &model->production_volume);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    returned_value_for_err = fscanf(file, "Number of Components: %d\n", &model->num_components);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }

    // Allocate space for component pointers
    model->components = safe_malloc(model->num_components * sizeof(CarComponent*));
    if (model->components == NULL) {
        perror("Memory allocation failed for components");
        free_car_model(model);
        return NULL;
    }

    // Load component IDs and link components
    int j;
    for (int i = 0; i < model->num_components; i++) {
        returned_value_for_err = fscanf(file, "Component ID %d: %d\n", &j, &component_id);
        if (returned_value_for_err != 2) {
            printf("Failed to read from file");
            return NULL;
        }
        model->components[i] = find_component_by_id(fact, component_id);
        if (model->components[i] == NULL) {
            printf("Component with ID %d not found in factory\n", component_id);
        }
    }

    return model;
}

// Save a distribution network to a binary file
void save_network_info_bin(DistributionNetwork* network, FILE* file)
{
    fwrite(&network->network_id, sizeof(int), 1, file);
    save_string_bin(network->description, file);
    fwrite(&network->factories_count, sizeof(int), 1, file);
    for (int i = 0; i < network->factories_count; i++)
    {
        int factory_id = network->source_factories[i]->factory_id;
        fwrite(&factory_id, sizeof(int), 1, file);
    }
    fwrite(&network->shops_count, sizeof(int), 1, file);
    for (int i = 0; i < network->shops_count; i++)
    {
        int shop_id = network->destination_shops[i]->shop_id;
        fwrite(&shop_id, sizeof(int), 1, file);
    }
}

// Load a distribution network from a binary file
DistributionNetwork load_network_info_bin(Company* company, FILE* file)
{
    int network_id;
    char* description;
    Factory** source_factories = NULL;
    int factories_count, fact_count = 0; // Logical size of source factories array to send to the add element function
    int max_factories = 0; // Physical size of source factories array
    Shop** destination_shops = NULL;
    int shops_count, sh_count = 0; // Logical size of destination shops array
    int max_shops = 0; // Physical size of destination shops array

    fread(&network_id, sizeof(int), 1, file);
    description = load_string_bin(file);
    fread(&factories_count, sizeof(int), 1, file);
    for (int i = 0; i < factories_count; i++)
    {
        int factory_id;
        fread(&factory_id, sizeof(int), 1, file);
        Factory* factory = (Factory*)find_factory_shop_by_id(company, factory_id)->shop_or_factory;
        add_element_to_array((void**)&source_factories, sizeof(Factory*), &fact_count, &max_factories, &factory);
    }
    fread(&shops_count, sizeof(int), 1, file);
    for (int i = 0; i < shops_count; i++)
    {
        int shop_id;
        fread(&shop_id, sizeof(int), 1, file);
        Shop* shop = (Shop*)(find_factory_shop_by_id(company, shop_id)->shop_or_factory);
        add_element_to_array((void**)&destination_shops, sizeof(Shop*), &sh_count, &max_shops, &shop);
    }
    DistributionNetwork network;
    init_distribution_network(&network, network_id, description, source_factories, factories_count, max_factories, destination_shops, shops_count, max_shops);
    return network;
}

// Save a distribution network to a text file
void save_network_info_txt(DistributionNetwork* network, FILE* file)
{
    fprintf(file, "Network ID: %d\n", network->network_id);
    fprintf(file, "Description len: %d\n", (int)strlen(network->description));
    fprintf(file, "Description: %s\n", network->description);
    fprintf(file, "Source Factories Count: %d\n", network->factories_count);
    for (int i = 0; i < network->factories_count; i++)
    {
        fprintf(file, "Factory %d: %d\n", i + 1, network->source_factories[i]->factory_id);
    }
    fprintf(file, "Destination Shops Count: %d\n", network->shops_count);
    for (int i = 0; i < network->shops_count; i++)
    {
        fprintf(file, "Shop %d: %d\n", i + 1, network->destination_shops[i]->shop_id);
    }


}

// Load a distribution network from a text file
DistributionNetwork load_network_info_txt(Company* company, FILE* file)
{
    int network_id, returned_value_for_err;
	char* description;
    int description_len;
	Factory** source_factories = NULL;
	int factories_count; // Logical size of source factories array
	int max_factories = 0; // Physical size of source factories array
	Shop** destination_shops = NULL;
	int shops_count; // Logical size of destination shops array
	int max_shops = 0; // Physical size of destination shops array
    returned_value_for_err = fscanf(file, "Network ID: %d\n", &network_id);

    // Allocate memory for description
    returned_value_for_err = fscanf(file, "Description len: %d\n", &description_len);
    description = (char*)safe_malloc(description_len + 1);
    returned_value_for_err = fscanf(file, "Description: %[^\n]\n", description);
    returned_value_for_err = fscanf(file, "Source Factories Count: %d\n", &factories_count);
    int j = 0;
	for (int i = 0; i < factories_count; i++)
	{
		int factory_id;
        returned_value_for_err = fscanf(file, "Factory %d: %d\n", &j, &factory_id);
        j=0;
		Factory* factory = (Factory*)find_factory_shop_by_id(company, factory_id)->shop_or_factory;
		add_element_to_array((void**)&source_factories, sizeof(Factory*), &j, &max_factories, &factory);
	}
    returned_value_for_err = fscanf(file, "Destination Shops Count: %d\n", &shops_count);
	j = 0;
	for (int i = 0; i < shops_count; i++)
	{
		int shop_id;
        returned_value_for_err = fscanf(file, "Shop %d: %d\n", &j, &shop_id);
        j=0;
		Shop* shop = (Shop*)(find_factory_shop_by_id(company, shop_id)->shop_or_factory);
		add_element_to_array((void**)&destination_shops, sizeof(Shop*), &j, &max_shops, &shop);
	}
	DistributionNetwork network;
	init_distribution_network(&network, network_id, description, source_factories, factories_count, max_factories, destination_shops, shops_count, max_shops);
	return network;
}

// Save a car component to a binary file
void save_component_info_bin(FILE* file, CarComponent* component) {
    fwrite(&component->component_id, sizeof(int), 1, file);
    int name_length = (int)strlen(component->name);
    fwrite(&name_length, sizeof(int), 1, file);
    fwrite(component->name, sizeof(char), name_length, file);
    int type_length = (int)strlen(component->type);
    fwrite(&type_length, sizeof(int), 1, file);
    fwrite(component->type, sizeof(char), type_length, file);
    fwrite(&component->cost, sizeof(double), 1, file);
}

// Load a car component from a binary file
CarComponent* load_component_info_bin(FILE* file) {
    CarComponent* component = (CarComponent*)safe_malloc(sizeof(CarComponent));

    fread(&component->component_id, sizeof(int), 1, file);
    int name_length;
    fread(&name_length, sizeof(int), 1, file);
    component->name = (char*)safe_malloc(name_length + 1);
    fread(component->name, sizeof(char), name_length, file);
    component->name[name_length] = '\0';
    int type_length;
    fread(&type_length, sizeof(int), 1, file);
    component->type = (char*)safe_malloc(type_length + 1);
    fread(component->type, sizeof(char), type_length, file);
    component->type[type_length] = '\0';
    fread(&component->cost, sizeof(double), 1, file);
    return component;
}

// Save a car component to a text file
void save_component_info_txt(FILE* file, CarComponent* component) {
    fprintf(file, "ID: %d\n", component->component_id);
    fprintf(file, "Name: %d %s\n", (int)strlen(component->name), component->name);
    fprintf(file, "Type: %d %s\n", (int)strlen(component->type), component->type);
    fprintf(file, "Cost: %.2f\n", component->cost);
}

// Load a car component from a text file
CarComponent* load_component_info_txt(FILE* file) {
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    // Temporary storage for the string data
    char *name, *type;
    int component_id, len, returned_value_for_err;
    double cost;

    returned_value_for_err = fscanf(file, "ID: %d\n", &component_id);
    if (returned_value_for_err != 1) {
		printf("Failed to read from file");
		return NULL;
	}
    returned_value_for_err = fscanf(file, "Name: %d", &len);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    name = (char*)safe_malloc((len + 1)*sizeof(char));
    returned_value_for_err = fscanf(file, " %[^\n]\n", name);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    name[len] = '\0';
    returned_value_for_err = fscanf(file, "Type: %d", &len);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    type = (char*)safe_malloc((len + 1)*sizeof(char));
    returned_value_for_err = fscanf(file, " %[^\n]\n", type);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    type[len] = '\0';
    returned_value_for_err = fscanf(file, "Cost: %lf\n", &cost);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }


    // Create a new CarComponent structure and initialize it
    CarComponent* component = (CarComponent*)safe_malloc(sizeof(CarComponent));

    init_car_component(component, component_id, name, type, cost);

    return component;
}

// Save a factory to a binary file
void save_factory_info_bin(FILE* file, Factory* factory) {
	fwrite(&factory->factory_id, sizeof(int), 1, file);
    save_string_bin(factory->location, file);
	fwrite(&factory->production_capacity, sizeof(int), 1, file);
    fwrite(&factory->num_components, sizeof(int), 1, file);
	for (int i = 0; i < factory->num_components; i++) {
        save_component_info_bin(file, &factory->produced_components[i]);
	}
    fwrite(&factory->num_models, sizeof(int), 1, file);
    for (int i = 0; i < factory->num_models; i++) {
        save_model_info_bin(&factory->produced_models[i], file);
	}
    fwrite(&factory->num_workers, sizeof(int), 1, file);
	for (int i = 0; i < factory->num_workers; i++) {
		fwrite(&factory->workers[i]->worker_id, sizeof(int), 1, file);
	}

}

// Load a factory from a binary file
Factory* load_factory_info_bin(FILE* file, Company* company) {
	Factory* factory = (Factory*)safe_malloc(sizeof(Factory));
	fread(&factory->factory_id, sizeof(int), 1, file);
	factory->location = load_string_bin(file);
	fread(&factory->production_capacity, sizeof(int), 1, file);
	fread(&factory->num_components, sizeof(int), 1, file);
	factory->max_components = factory->num_components;
	factory->produced_components = (CarComponent*)safe_malloc(factory->num_components * sizeof(CarComponent));
	for (int i = 0; i < factory->num_components; i++) {
		factory->produced_components[i] = *load_component_info_bin(file);
	}
	fread(&factory->num_models, sizeof(int), 1, file);
    factory->max_models = factory->num_models;
	factory->produced_models = (CarModel*)safe_malloc(factory->num_models * sizeof(CarModel));
	for (int i = 0; i < factory->num_models; i++) {
        factory->produced_models[i] = *load_model_info_bin(file, factory);
	}
	fread(&factory->num_workers, sizeof(int), 1, file);
	factory->workers = (Worker**)safe_malloc(factory->num_workers * sizeof(Worker*));
	for (int i = 0; i < factory->num_workers; i++) {
		int worker_id;
		fread(&worker_id, sizeof(int), 1, file);
		factory->workers[i] = company->workers[find_worker_by_id(company, worker_id)];
	}
	return factory;
}

// Save a factory to a text file
void save_factory_info_txt(FILE* file, Factory* factory) {
	fprintf(file, "Factory ID: %d\n", factory->factory_id);
	fprintf(file, "Location: %d %s\n", (int)strlen(factory->location), factory->location);
	fprintf(file, "Production Capacity: %d\n", factory->production_capacity);
	fprintf(file, "Number of Components: %d\n", factory->num_components);
	for (int i = 0; i < factory->num_components; i++) {
		save_component_info_txt(file, &(factory->produced_components[i]));
	}
	fprintf(file, "Number of Models: %d\n", factory->num_models);
	for (int i = 0; i < factory->num_models; i++) {
		save_model_info_txt(&(factory->produced_models[i]), file);
	}
	fprintf(file, "Number of Workers: %d\n", factory->num_workers);
	for (int i = 0; i < factory->num_workers; i++) {
		fprintf(file, "Worker %d: %d\n", i + 1, factory->workers[i]->worker_id);
	}
}

// Load a factory from a text file
Factory* load_factory_info_txt(FILE* file, Company* company) {
	Factory* factory = (Factory*)safe_malloc(sizeof(Factory));
    int loc_len, returned_value_for_err;
    returned_value_for_err = fscanf(file, "Factory ID: %d\n", &factory->factory_id);
    if (returned_value_for_err != 1) {
		printf("Failed to read from file");
		return NULL;
	}
    returned_value_for_err = fscanf(file, "Location: %d", &loc_len);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
	factory->location = (char*)safe_malloc(loc_len + 1);
    returned_value_for_err = fscanf(file, " %[^\n]\n", factory->location);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
	factory->location[loc_len] = '\0';
    returned_value_for_err = fscanf(file, "Production Capacity: %d\n", &factory->production_capacity);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }

    // Read component information
    returned_value_for_err = fscanf(file, "Number of Components: %d\n", &factory->num_components);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
    factory->max_models = factory->num_models;
    factory->max_components = factory->num_components;
	factory->produced_components = (CarComponent*)safe_malloc(factory->num_components * sizeof(CarComponent));
	for (int i = 0; i < factory->num_components; i++) {
        factory->produced_components[i] = *load_component_info_txt(file);
	}
    returned_value_for_err = fscanf(file, "Number of Models: %d\n", &factory->num_models);
    if (returned_value_for_err != 1) {
        printf("Failed to read from file");
        return NULL;
    }
	factory->produced_models = (CarModel*)safe_malloc(factory->num_models * sizeof(CarModel));
	for (int i = 0; i < factory->num_models; i++) {
		factory->produced_models[i] = *load_model_info_txt(file, factory);
	}
    returned_value_for_err = fscanf(file, "Number of Workers: %d\n", &factory->num_workers);
    factory->max_workers = factory->num_workers;
	factory->workers = (Worker**)safe_malloc(factory->num_workers * sizeof(Worker*));
	for (int i = 0; i < factory->num_workers; i++) {
		int worker_id, i_;
        returned_value_for_err = fscanf(file, "Worker %d: %d\n", &i_, &worker_id);
        if (returned_value_for_err != 1) {
            printf("Failed to read from file");
            return NULL;
        }
        int worker_p = find_worker_by_id(company, worker_id);
        Worker* worker = company->workers[worker_p];
		factory->workers[i] = worker;
	}
	return factory;
}

// Save a shop to a binary file
void save_shop_info_bin(FILE* file, Shop* shop) {
    fwrite(&shop->shop_id, sizeof(int), 1, file);
    save_string_bin(shop->location, file);
    fwrite(&shop->num_inventory, sizeof(int), 1, file);
    for (int i = 0; i < shop->num_inventory; i++) {
        int model_name_length = (int)strlen(shop->inventory[i]->model_name);
        fwrite(&model_name_length, sizeof(int), 1, file);
        fwrite(shop->inventory[i]->model_name, sizeof(char), model_name_length, file);
    }
    fwrite(&shop->sales, sizeof(double), 1, file);
    fwrite(&shop->num_workers, sizeof(int), 1, file);
    for (int i = 0; i < shop->num_workers; i++) {
        fwrite(&shop->workers[i]->worker_id, sizeof(int), 1, file);
    }
}

// Load a shop from a binary file
Shop* load_shop_info_bin(FILE* file, Company* company)
{
    Shop* shop = (Shop*)safe_malloc(sizeof(Shop));
    fread(&shop->shop_id, sizeof(int), 1, file);
    shop->location = load_string_bin(file);
    fread(&shop->num_inventory, sizeof(int), 1, file);
    shop->max_inventory = shop->num_inventory;
    shop->inventory = (CarModel**)safe_malloc(shop->num_inventory * sizeof(CarModel*));
    for (int i = 0; i < shop->num_inventory; i++) {
        int model_name_length;
        fread(&model_name_length, sizeof(int), 1, file);
        shop->inventory[i] = find_model_by_name(company, load_string_bin(file));
    }
    fread(&shop->sales, sizeof(double), 1, file);
    fread(&shop->num_workers, sizeof(int), 1, file);
    shop->max_workers = shop->num_workers;
    shop->workers = (Worker**)safe_malloc(shop->num_workers * sizeof(Worker*));
    for (int i = 0; i < shop->num_workers; i++) {
        int worker_id;
        fread(&worker_id, sizeof(int), 1, file);
        shop->workers[i] = company->workers[find_worker_by_id(company, worker_id)];
    }
    return shop;
}

// Save a shop to a text file
void save_shop_info_txt(FILE* file, Shop* shop) {
	fprintf(file, "Shop ID: %d\n", shop->shop_id);
	fprintf(file, "Location: %d %s\n", (int)strlen(shop->location), shop->location);
	fprintf(file, "Number of Inventory: %d\n", shop->num_inventory);
	for (int i = 0; i < shop->num_inventory; i++) {
        int model_name_length = (int)strlen(shop->inventory[i]->model_name);
        fprintf(file, "Model %d: %d %s\n", i + 1, model_name_length, shop->inventory[i]->model_name);
	}
	fprintf(file, "Sales: %.2f\n", shop->sales);
	fprintf(file, "Number of Workers: %d\n", shop->num_workers);
	for (int i = 0; i < shop->num_workers; i++) {
		fprintf(file, "Worker %d: %d\n", i + 1, shop->workers[i]->worker_id);
	}
}

// Load a shop from a text file
Shop* load_shop_info_txt(FILE* file, Company* company) {
	Shop* shop = (Shop*)safe_malloc(sizeof(Shop));
    int loc_len, returned_value_for_err;
    returned_value_for_err = fscanf(file, "Shop ID: %d\n", &shop->shop_id);
    CHECK_VALUE_OF_SCANF(returned_value_for_err);
    returned_value_for_err = fscanf(file, "Location: %d", &loc_len);
    CHECK_VALUE_OF_SCANF(returned_value_for_err);
    shop->location = (char*)safe_malloc(loc_len + 1);
    returned_value_for_err = fscanf(file, " %[^\n]\n", shop->location);
    CHECK_VALUE_OF_SCANF(returned_value_for_err);
    shop->location[loc_len] = '\0';
    returned_value_for_err = fscanf(file, "Number of Inventory: %d\n", &shop->num_inventory);
    CHECK_VALUE_OF_SCANF(returned_value_for_err);
	shop->max_inventory = shop->num_inventory;
	shop->inventory = (CarModel**)safe_malloc(shop->num_inventory * sizeof(CarModel*));
	for (int i = 0; i < shop->num_inventory; i++) {
        int model_name_length;
        returned_value_for_err = fscanf(file, "Model %d: %d", &i, &model_name_length);
        CHECK_VALUE_OF_SCANF(returned_value_for_err);
        char* model_name = (char*)safe_malloc(model_name_length + 1);
        returned_value_for_err = fscanf(file, " %[^\n]\n", model_name);
        CHECK_VALUE_OF_SCANF(returned_value_for_err);
		shop->inventory[i] = find_model_by_name(company, model_name);
	}
    returned_value_for_err = fscanf(file, "Sales: %lf\n", &shop->sales);
    CHECK_VALUE_OF_SCANF(returned_value_for_err);
    returned_value_for_err = fscanf(file, "Number of Workers: %d\n", &shop->num_workers);
    CHECK_VALUE_OF_SCANF(returned_value_for_err);
	shop->max_workers = shop->num_workers;
	shop->workers = (Worker**)safe_malloc(shop->num_workers * sizeof(Worker*));
	for (int i = 0; i < shop->num_workers; i++) {
		int worker_id, i_;
        returned_value_for_err = fscanf(file, "Worker %d: %d\n", &i_, &worker_id);
        CHECK_VALUE_OF_SCANF(returned_value_for_err);
		shop->workers[i] = company->workers[find_worker_by_id(company, worker_id)];
	}
	return shop;
}

// Save a company to a binary file
void save_company_info_bin(FILE* file, Company* company) {
    save_string_bin(company->company_name, file);
    save_worker_info_compressed(company->CEO, file);
	fwrite(&company->num_workers, sizeof(int), 1, file);
    fwrite(&company->id_counter, sizeof(int), 1, file);
	for (int i = 0; i < company->num_workers; i++) {
		save_worker_info_compressed(company->workers[i], file);
	}
    fwrite(&company->num_shops_and_fact, sizeof(int), 1, file);
    ShopFactoryNode* current = company->head_shops_and_factories;
    while(current != NULL) {
		fwrite(&current->is_shop, sizeof(int), 1, file);
		if (current->is_shop) {
			save_shop_info_bin(file, (Shop*)current->shop_or_factory);
		} else {
			save_factory_info_bin(file, (Factory*)current->shop_or_factory);
		}
		current = current->next;
	}
	save_network_info_bin(&company->south, file);
	save_network_info_bin(&company->north, file);
}

//  Load a company from a binary file
Company* load_company_info_bin(FILE* file) {
	Company* company = (Company*)safe_malloc(sizeof(Company));
	company->company_name = load_string_bin(file);
	company->CEO = load_worker_info_compressed(file);
	fread(&company->num_workers, sizeof(int), 1, file);
    fread(&company->id_counter, sizeof(int), 1, file);
    company->max_workers = company->num_workers;
	company->workers = (Worker**)safe_malloc(company->num_workers * sizeof(Worker*));
	for (int i = 0; i < company->num_workers; i++) {
		company->workers[i] = load_worker_info_compressed(file);
	}
    int number_of_shops_facts;
	fread(&number_of_shops_facts, sizeof(int), 1, file);
	company->head_shops_and_factories = NULL;
	for (int i = 0; i < number_of_shops_facts; i++) {
		int is_shop;
		fread(&is_shop, sizeof(int), 1, file);
        add_shop_factory_node(company, is_shop ? (void*)load_shop_info_bin(file, company) : (void*)load_factory_info_bin(file, company), is_shop);
	}
    company->num_shops_and_fact = number_of_shops_facts;
	company->south = load_network_info_bin(company, file);
	company->north = load_network_info_bin(company, file);
	return company;
}

// Save a company to a text file
void save_company_info_txt(FILE* file, Company* company) {
	fprintf(file, "Company Name: %d %s\n", (int)strlen(company->company_name), company->company_name);
	fprintf(file, "Number of Workers: %d\n", company->num_workers);
    fprintf(file, "ID Counter: %d\n", company->id_counter);
	for (int i = 0; i < company->num_workers; i++) {
		save_worker_info_text_file(company->workers[i], file);
	}
	fprintf(file, "Number of Shops and Factories: %d\n", company->num_shops_and_fact);
	ShopFactoryNode* current = company->head_shops_and_factories;
	while (current != NULL) {
		if (current->is_shop) {
            fprintf(file, "Type: Shop\n");
			save_shop_info_txt(file, (Shop*)current->shop_or_factory);
		} else {
            fprintf(file, "Type: Factory\n");
			save_factory_info_txt(file, (Factory*)current->shop_or_factory);
		}
		current = current->next;
	}
	save_worker_info_text_file(company->CEO, file);
	save_network_info_txt(&company->south, file);
	save_network_info_txt(&company->north, file);
}

// Load a company from a text file
Company* load_company_info_txt(FILE* file) {
	Company* company = (Company*)safe_malloc(sizeof(Company));
    int name_len, returned_value_for_err;
    returned_value_for_err = fscanf(file, "Company Name: %d", &name_len);
    CHECK_VALUE_OF_SCANF(returned_value_for_err);
	company->company_name = (char*)safe_malloc((name_len + 1) * sizeof(char));
    returned_value_for_err = fscanf(file, " %[^\n]\n", company->company_name);
    CHECK_VALUE_OF_SCANF(returned_value_for_err);
    company->company_name[name_len] = '\0';
    returned_value_for_err = fscanf(file, "Number of Workers: %d\n", &company->num_workers);
    CHECK_VALUE_OF_SCANF(returned_value_for_err);
    returned_value_for_err = fscanf(file, "ID Counter: %d\n", &company->id_counter);
    CHECK_VALUE_OF_SCANF(returned_value_for_err);
	company->max_workers = company->num_workers;
	company->workers = (Worker**)safe_malloc(company->num_workers * sizeof(Worker*));
	for (int i = 0; i < company->num_workers; i++) {
		company->workers[i] = load_worker_info_text_file(file);
	}
    int num_shops = 0;
    returned_value_for_err = fscanf(file, "Number of Shops and Factories: %d\n", &num_shops);
    CHECK_VALUE_OF_SCANF(returned_value_for_err);
	company->head_shops_and_factories = NULL;
    company->num_shops_and_fact = 0;
	for (int i = 0; i < num_shops; i++) {
		char type[32];
        returned_value_for_err = fscanf(file, "Type: %[^\n]\n", type);
        CHECK_VALUE_OF_SCANF(returned_value_for_err);
        if (strcmp(type, "Shop") == 0) {
			add_shop_factory_node(company, load_shop_info_txt(file, company), 1);
		} else {
			add_shop_factory_node(company, load_factory_info_txt(file, company), 0);
		}
    
	}
	company->CEO = load_worker_info_text_file(file);
	company->south = load_network_info_txt(company, file);
	company->north = load_network_info_txt(company, file);
	return company;
}
