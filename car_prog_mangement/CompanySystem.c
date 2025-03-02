#define _CRT_SECURE_NO_WARNINGS
#include "CompanySystem.h"

// Initialize the company system
Company* start_company_system()
{
    CleanScreen();
    printf("=========================================\n");
    printf("Welcome to the company management system!\n");
    printf("=========================================\n");
    int choice = 0;
    Company* company = NULL;
    printf("Please select your choice:\n");
    printf("1. New Company\n");
    printf("2. Load Company From File\n");
    printf("3. Exit\n");
    // Get user choice
    safe_int_scanf(&choice);
    CleanScreen();
    clean_buffer();
    while (1)
    {
        switch (choice) {
        case 1:
            // Create a new company
            company = (Company*)safe_malloc(sizeof(Company));
            printf("Creating a new company...\n");
            printf("Enter the company name: ");
            char* name = get_safe_input();
            init_company(company, name);
            printf("Company %s created successfully.\n", name);
            printf("=========================================\n\n");
            printf("Please Add a CEO to the company\n");
            // Add a CEO to the company
            while (1) {
                add_worker_to_sys(company);
                if (company->num_workers > 0)
                    break;
                printf("pree Enter to continue...");
                clean_buffer();
                CleanScreen();
                printf("Please Add a CEO to the company\n");
            }
            change_CEO(company, company->workers[0]);
            return company;
        case 2:
            // Load company from file
            upload_system_data(&company);
            return company;
        case 3:
            // Exit the program
            return company;
        default:
            printf("Invalid choice. Please try again.\n");
			break;
        }
    }
}

// handle the main menu options
void handleMenu(MenuOptions choice, Company** company) {
    switch (choice) {
    case UPLOAD_SYS_FROM_FILE:
        free_company(*company);
        upload_system_data(company);
        break;
    case SAVE_SYS_TO_FILE:
		write_system_data(*company);
		break;
    case DISPLAY_COMPANY:
        display_company(*company);
        break;
    case DISPLAY_DISTRIBUTION_NETWORKS:
        display_distribution_networks(*company);
        break;
    case DISPLAY_SHOPS:
        display_shops(*company);
        break;
    case DISPLAY_FACTORIES:
        display_factories(*company);
        break;
    case DISPLAY_CAR_MODELS:
        display_car_models(*company);
        break;
    case ADD_WORKER:
        add_worker_to_sys(*company);
        break;
    case ADD_SHOP:
        add_shop_to_sys(*company);
        break;
    case ADD_FACTORY:
        add_factory_to_sys(*company);
        break;
    case ADD_CAR_MODEL:
        add_car_model_to_sys(*company);
        break;
    case ADD_COMPONENT:
        add_component_to_sys(*company);
        break;
    case ADD_MODEL_TO_INVENTORY:
		add_model_to_inventory_sys(*company);
		break;
    case CHANGE_CEO:
        change_CEO_to_sys(*company);
        break;
    case SORT_WORKERS_BY_NAME:
        sort_workers_by_name(*company);
        break;
    case SORT_WORKERS_BY_ID:
        sort_workers_by_id(*company);
        break;
    case SORT_WORKERS_BY_JOB_DETAILS:
        sort_workers_by_job_details(*company);
        break;
    case SEARCH_WORKER_BY_NAME:
        search_worker_by_name(*company);
        break;
    case SEARCH_WORKER_BY_ID:
        search_worker_by_id(*company);
        break;
    case SEARCH_WORKER_BY_JOB_DETAIL:
        search_worker_by_job_detail(*company);
        break;
    case MOVE_WORKER:
        move_worker_factory_shop(*company);
        break;
    case FIRE_WORKER:
        fire_worker_from_sys(*company);
        break;
    case EXIT:
        free_company(*company);
        return;  // Exiting the function early
    default:
        printf("Invalid choice. Please try again.\n");
    }
    printf("Press Enter to continue...");
    clean_buffer();
}

// Main menu for the company system
void run_company_system(Company** company){
    int choice = 0;
    while (1) {
        CleanScreen();
        printf("=========================================\n");
        printf("Welcome to the company management system Menu!\n");
        printf("=========================================\n");
        DISPLAY_MENU();
        printf("Please select your choice: \n");
        safe_int_scanf(&choice);
        CleanScreen();
        clean_buffer();
        handleMenu(choice, company);

        if(choice == EXIT)
		{
			break;
		}
    }
}

// load company data
void upload_system_data(Company** company)
{
    CleanScreen();
    printf("For bin file enter 1, for txt file enter 2: ");
    int choice;
    safe_int_scanf(&choice);
    clean_buffer();
    printf("Enter the file name(No Spaces, Max %d chars): ", MAX_LEN);
    char* filename;
    FILE* file;

    filename = get_safe_input();
    filename = (char*)safe_realloc(filename, strlen(filename) + 5);

    if (choice == 1) {
        file = fopen(strcat(filename, ".bin"), "rb");
        if (file != NULL)
        {
			*company = load_company_info_bin(file);
            fclose(file);
        }
		else
			printf("Error opening file.\n");
    }
    else if (choice == 2) {
        file = fopen(strcat(filename, ".txt"), "r");
        if (file != NULL)
        {
            *company = load_company_info_txt(file);
            fclose(file);
        }
        else
            printf("Error opening file.\n");
    }
    else {
        printf("Invalid choice .\n");
    }
    SAFE_FREE(filename);
}

// upload company data
void write_system_data(Company* company) {
    CleanScreen();
    printf("For bin file enter 1, for txt file enter 2: ");
    int choice;
    safe_int_scanf(&choice);
    clean_buffer();
    printf("Enter the file name(No Spaces, Max %d chars): ", MAX_LEN);
    char* filename;

    filename = get_safe_input();
    int name_len = (int)strlen(filename) + 5;
    filename = (char*)safe_realloc(filename, name_len);
    FILE* file;
    CleanScreen();
    if (choice == 1) {
        file = fopen(strcat(filename, ".bin"), "wb");
        if (file != NULL)
        {
            save_company_info_bin(file, company);
            fclose(file);
        }
        else
            printf("Error opening file.\n");
	}
	else if (choice == 2) {
        file = fopen(strcat(filename, ".txt"), "w");
        if (file != NULL)
        {
			save_company_info_txt(file, company);
            fclose(file);
        }
		else
			printf("Error opening file.\n");
	}
	else {
		printf("Invalid choice File not saved.\n");
	}
	SAFE_FREE(filename);
}


// Display basic company information
void display_company(Company* company) {
    if (company) {
        CleanScreen();
        display_company_info(company);
    }
}

// Display distribution networks information
void display_distribution_networks(Company* company) {
    if (company) {
        // Assuming there are functions to display distribution networks
        // Display information for south and north distribution networks
        printf("=========================================\n");
        printf("South Distribution Network\n");
        display_network_info(&company->south);
        printf("=========================================\n");
        printf("North Distribution Network\n");
        display_network_info(&company->north);
        printf("=========================================\n");
    }
}

// Display all shops in the company
void display_shops(Company* company) {
    int i = 0;
    if (company) {
        ShopFactoryNode* current = company->head_shops_and_factories;
        while (current != NULL) {
            if (current->is_shop) {
                Shop* shop = (Shop*)current->shop_or_factory;
                display_shop_info(shop);
                i++;
            }
            current = current->next;
        }
    }
    if(i == 0)
	{
		printf("No shops available.\n");
	}
}

// Display all factories in the company
void display_factories(Company* company) {
    int i = 0;
    if (company) {
        ShopFactoryNode* current = company->head_shops_and_factories;
        while (current != NULL) {
            if (!current->is_shop) {
                Factory* factory = (Factory*)current->shop_or_factory;
                display_factory_info(factory);
                i++;
            }
            current = current->next;
        }
    }
    if(i == 0)
    {
        printf("No factories available.\n");
    }
}

// Display all car models produced by the company's factories
void display_car_models(Company* company) {
    if (company) {
        ShopFactoryNode* current = company->head_shops_and_factories;
        while (current != NULL) {
            if (!current->is_shop) {
                Factory* factory = (Factory*)current->shop_or_factory;
                for (int i = 0; i < factory->num_models; i++) {
                    display_model_info(&factory->produced_models[i]);
                }
            }
            current = current->next;
        }
    }
}

// Display all workers of the company, including those in shops, factories, and the CEO
void display_workers(Company* company) {
    if (company) {
        // Display CEO information
        display_worker_info(company->CEO);

        // Display workers in shops and factories
        for (int i = 0; i < company->num_workers; i++) {
            display_worker_info(company->workers[i]);
        }
    }
}

// Change the CEO of the company
void change_CEO_to_sys(Company* company) {
    int worker_id;
    printf("===== Setting new CEO =====\n");
    printf("Enter worker's id: ");
    safe_int_scanf(&worker_id);
    clean_buffer();
    // Assuming there is a mechanism to iterate through the company's workers
    for (int i = 0; i < company->num_workers; i++) {
        if (company->workers[i]->worker_id == worker_id) {
            // Found the worker with the matching ID
            change_CEO(company, company->workers[i]);
            printf("The new CEO has been set successfully to %s\n", company->workers[i]->worker_name);
            return;
        }
    }

    // If the worker is not found
    printf("Worker with ID %d not found.\n", worker_id);
}

// Add a new worker to the company
void add_worker_to_sys(Company* company) {
    char* name, *position, *department;
    int salary;
    int factory_id;
    Worker* worker;
    printf("=====add new worker=====\n");
    printf("Enter worker's name: ");
    name = get_safe_input();

    printf("Enter worker's position: ");
    position = get_safe_input();

    printf("Enter worker's department: ");
    department = get_safe_input();

    printf("Enter worker's salary(will be mult by 1000): ");
    safe_int_scanf(&salary);

    clean_buffer();


    printf("Enter worker's factory/Shop ID (for mangement enter 0): ");
    safe_int_scanf(&factory_id);
    clean_buffer();

    worker = add_worker(company, name, position, salary*1000, department, factory_id);
    // Check if the worker was added successfully
    if (worker != NULL) {
        CleanScreen();
        printf("Worker added successfully.\n");
        printf("=========================\n");
        display_worker_info(company->workers[company->num_workers - 1]);
        printf("=========================\n");
    }
}

// Add a new model to a factory
void add_car_model_to_sys(Company* company) {
    int factory_id, year, numInitialComponents;
    char* modelName, *manufacturer;
    int price;
    printf("======Add new model======\n");

    display_factories(company);
    printf("Enter factory ID to add the model to: ");
    safe_int_scanf(&factory_id);
    clean_buffer();

    ShopFactoryNode* node = find_factory_shop_by_id(company, factory_id);
    // Check if the factory exists
    if(node == NULL)
    {
        printf("Invalid factory ID.\n");

		return;
	}
    // Check if the node is a shop
    if(node->is_shop)
	{
		printf("Invalid factory ID.\n");
		return;
	}
    // Get the factory
    Factory* factory = (Factory*)node->shop_or_factory;
    if (factory == NULL) {
        printf("Factory ID %d not found.\n", factory_id);
        return;
    }

    // Get the model details
    printf("Enter model name: ");
    modelName = get_safe_input();

    printf("Enter model year: ");
    safe_int_scanf(&year);
    clean_buffer();

    printf("Enter manufacturer: ");
    manufacturer = get_safe_input();

    printf("Enter price: ");
    safe_int_scanf(&price);
    clean_buffer();

    printf("Enter number of initial components: ");
    safe_int_scanf(&numInitialComponents);
    clean_buffer();

    // Get the initial components
    CarComponent** components = (CarComponent**)safe_malloc(numInitialComponents * sizeof(CarComponent*));

    int componentsNotFound = 0;
    for (int i = 0; i < numInitialComponents; i++) {
        int compId;
        printf("Enter component ID %d: ", i + 1);
        safe_int_scanf(&compId);
        clean_buffer();
        CarComponent* component = find_component_by_id(factory, compId);
        if (component == NULL) {
            printf("Component ID %d not found.\n", compId);
            printf("=========================\n");
            componentsNotFound++;
        }
        else if(components != NULL) {
            components[i] = component;
        }
    }

    // Check if all components were found
    if (componentsNotFound > 0) {
        printf("Missing %d components. Cannot add model.\n", componentsNotFound);
        free(components);
        printf("=========================\n");
        printf("Model not added.\n");
        return;
    }

    // Add the model to the factory
    add_new_model(factory, modelName, year, manufacturer, price, numInitialComponents, components, numInitialComponents);
    printf("Model %s added successfully.\n", modelName);
    free(components);
    printf("=========================\n");
}

// Add a model to a shop's inventory
void add_model_to_inventory_sys(Company* company)
{
    DistributionNetwork* network;
    printf("====================================\n");
    printf("Add a model to a shop's inventory\n");
    printf("====================================\n");
    printf("Enter Distributor ID (South or North): ");
    char* distributorID;
    // Get the distributor south or north
    while(1){
		distributorID = get_safe_input();
		if(strcmp(distributorID, "south") == 0 || strcmp(distributorID, "north") == 0)
		{
			break;
		}
		else
		{
			printf("Invalid distributor ID. Please enter south or north.\n");
			printf("Enter Distributor ID (South or North): ");
		}
	}
    if (strcmp(distributorID, "south") == 0) {
        network = &company->south;
    }
	else {
		network = &company->north;
	}
	printf("Enter Shop ID: ");
	int shopID = -1, not_found = 1;
    // Get the shop ID
    while (not_found && shopID != 0)
    {
        safe_int_scanf(&shopID);
        clean_buffer();
        for(int i = 0; i < network->shops_count; i++)
		{
            if (network->destination_shops[i]->shop_id == shopID)
            {
                not_found = 0;
            }
		}
        if(not_found)
        {
            printf("Shop ID not found. Please enter a valid Shop ID(enter 0 to exit): ");
		}

    }
    if(shopID == 0)
	{
		return;
	}
    // display all models in the network
    printf("Available models in the network:\n");
    for (int i = 0; i < network->factories_count; i++) {
		Factory* factory = network->source_factories[i];
		for (int j = 0; j < factory->num_models; j++) {
			printf("%d %d Model Name: %s\n", i, j, factory->produced_models[j].model_name);
		}
	}
    // Get the factory and model index
    printf("Enter model choice to add to the shop's inventory(enter -1, -1 to exit): ");
    int factoryIndex, modelIndex;
    while (1)
    {
        safe_int_scanf(&factoryIndex);
        safe_int_scanf(&modelIndex);
        clean_buffer();
        if (factoryIndex == -1 && modelIndex == -1)
            return;
		if(factoryIndex >= 0 && factoryIndex < network->factories_count && modelIndex >= 0 && modelIndex < network->source_factories[factoryIndex]->num_models)
		{
			break;
		}
		else
		{
			printf("Invalid model choice. Please enter a valid model choice: ");
		}
    }
    // Add the model to the shop's inventory
    CarModel* model = &(network->source_factories[factoryIndex]->produced_models[modelIndex]);
    add_to_inventory(network->destination_shops[shopID], model);
    CleanScreen();
    printf("====================================\n");
    printf("Model added to shop's inventory successfully.\n");
    display_model_info(model);
    printf("====================================\n");
}

// Add a new factory to the company
void add_factory_to_sys(Company* company) {
    char* location;  // Buffer for location input
    int production_capacity;
    char* network;  // Buffer for network input
    Factory* fact;

    // Prompting user for factory details
    printf("=====Add new factory====\n");
    printf("Enter factory location: ");
    location = get_safe_input();

    printf("Enter production capacity: ");
    safe_int_scanf(&production_capacity);
    clean_buffer();

    printf("Enter distribution network (south or north): ");
    network = get_safe_input();

    // Adding the factory to the company
    while (1)
    {
        if (strcmp("south", network) == 0)
        {
            fact = add_factory(company, location, production_capacity, network);
            break;
        }
        else if(strcmp("north", network) == 0)
        {
            fact = add_factory(company, location, production_capacity, network);
            break;
        }
        else
        {
            printf("Invalid network. Please enter south or north.\n");
			printf("Enter distribution network (south or north): ");
			network = get_safe_input();
        }
    }

    // Display the factory information
    CleanScreen();
    printf("========================\n");
    printf("Factory added successfully.\n");
    display_factory_info(fact);
    printf("========================\n");
}

// Add a new shop to the company
void add_shop_to_sys(Company* company) {
    char* location;  // Buffer for location input
    char* network;  // Buffer for network input
    Shop* shop;

    // Prompting user for factory details
    printf("=====Add new shop====\n");
    printf("Enter shop location: ");
    location = get_safe_input();
    printf("Enter distribution network (south or north): ");
    network = get_safe_input();


    // Adding the shop to the company
    while (1)
    {
        if (strcmp("south", network) == 0)
        {
            shop = add_shop(company, location, network);
            break;
        }
        else if (strcmp("north", network) == 0)
        {
            shop = add_shop(company, location, network);
            break;
        }
        else
        {
            printf("Invalid network. Please enter south or north.\n");
            printf("Enter distribution network (south or north): ");
            network = get_safe_input();
        }
    }

    // Display the shop information
    CleanScreen();
    printf("========================\n");
    printf("Shop added successfully.\n");
    display_shop_info(shop);
    printf("========================\n");
}

// Add a new component to a factory
void add_component_to_sys(Company* company) {
    if (!company || !company->head_shops_and_factories) {
        printf("No factories available to add components.\n");
        return;
    }

    // Temporary storage for user input
    char* componentName;
    char* componentType;
    int componentCost;
    int factoryId;

    // Get user input for the new component
    printf("===== Add new component =====\n");
    printf("Enter component name: ");
    componentName = get_safe_input();
    printf("Enter component type: ");
    componentType = get_safe_input();
    printf("Enter component cost: ");
    safe_int_scanf(&componentCost);
    display_factories(company);
    printf("Enter factory ID to add the component: ");
    safe_int_scanf(&factoryId);
    clean_buffer();

    // Find the factory by ID
    ShopFactoryNode* node = find_factory_shop_by_id(company, factoryId);
    if (node == NULL || node->is_shop) {
        printf("Factory with ID %d not found.\n", factoryId);
        return;
    }
    Factory* targetFactory = (Factory*)node->shop_or_factory;

    // Add the new component to the factory
    add_new_component(targetFactory, targetFactory->num_components + 1, componentName, componentType, componentCost);

    printf("Component '%s' added to factory ID %d.\n", componentName, factoryId);
}

// Comparison function for sorting by name
int compare_by_name(const void* a, const void* b) {
    const Worker* workerA = *(const Worker**)a;
    const Worker* workerB = *(const Worker**)b;
    return strcmp(workerA->worker_name, workerB->worker_name);
}

// Comparison function for sorting by worker ID
int compare_by_id(const void* a, const void* b) {
    const Worker* workerA = *(const Worker**)a;
    const Worker* workerB = *(const Worker**)b;
    return (workerA->worker_id - workerB->worker_id);
}

// Comparison function for sorting by job details (department and then position)
int compare_by_job_details(const void* a, const void* b) {
    const Worker* workerA = *(const Worker**)a;
    const Worker* workerB = *(const Worker**)b;
    // First compare by department
    int departmentCompare = strcmp(workerA->job_details.department, workerB->job_details.department);
    if (departmentCompare == 0) {
        // If departments are the same, then compare by position
        return strcmp(workerA->job_details.position, workerB->job_details.position);
    }
    return departmentCompare;
}

// Display all workers in the company after sorting by name
void sort_workers_by_name(Company* company) {
    qsort(company->workers, company->num_workers, sizeof(Worker*), compare_by_name);
    printf("Workers sorted by name\n");
    display_workers_company(company);
}

// Display all workers in the company after sorting by ID
void sort_workers_by_id(Company* company) {
    qsort(company->workers, company->num_workers, sizeof(Worker*), compare_by_id);
    printf("Workers sorted by ID\n");
    display_workers_company(company);
}

// Display all workers in the company after sorting by job details
void sort_workers_by_job_details(Company* company) {
    qsort(company->workers, company->num_workers, sizeof(Worker*), compare_by_job_details);
    printf("Workers sorted by Job Details\n");
    display_workers_company(company);
}

// Adapted for bsearch: search by name
int compare_by_name_for_search(const void* key, const void* element) {
    const char* keyName = (const char*)key;
    const Worker* workerElement = *(const Worker* const*)element;
    return strcmp(keyName, workerElement->worker_name);
}

// Adapted for bsearch: search by id
int compare_by_id_for_search(const void* key, const void* element) {
    const int* keyId = (const int*)key;
    const Worker* workerElement = *(const Worker* const*)element;
    return (*keyId - workerElement->worker_id);
}

// Adapted for bsearch: search by job details
int compare_by_job_details_for_search(const void* key, const void* element) {
    const JobDetails* keyDetails = (const JobDetails*)key;
    const Worker* workerElement = *(const Worker* const*)element;
    int departmentCompare = strcmp(keyDetails->department, workerElement->job_details.department);
    if (departmentCompare == 0) {
        return strcmp(keyDetails->position, workerElement->job_details.position);
    }
    return departmentCompare;
}

// Searching for a worker by name and printing details
void search_worker_by_name(Company* company) {
    char* name; // Assuming a name won't exceed 255 characters
    printf("Enter the worker's name: ");
    
    name = get_safe_input(); // Read the name
    Worker* found = (Worker*)bsearch(name, company->workers, company->num_workers, sizeof(Worker*), compare_by_name_for_search);
    if (found) {
        printf("Worker found:\nName: %s\nID: %d\nDepartment: %s\nPosition: %s\nSalary: %.2f\nFactory ID: %d\n",
            found->worker_name, found->worker_id, found->job_details.department, found->job_details.position, found->salary, found->factory_id);
    }
    else {
        printf("Worker not found.\n");
    }
    SAFE_FREE(name);
}

// Searching for a worker by ID and printing details
void search_worker_by_id(Company* company) {
    int id;
    printf("Enter the worker's ID: ");
    safe_int_scanf(&id); // Read the ID
    clean_buffer();
    // Search for the worker by ID
    Worker* found = (Worker*)bsearch(&id, company->workers, company->num_workers, sizeof(Worker*), compare_by_id_for_search);
    if (found) {
        printf("Worker found:\nName: %s\nID: %d\nDepartment: %s\nPosition: %s\nSalary: %.2f\nFactory ID: %d\n",
            found->worker_name, found->worker_id, found->job_details.department, found->job_details.position, found->salary, found->factory_id);
    }
    else {
        printf("Worker not found.\n");
    }
}

// Searching for a worker by department and position and printing details
void search_worker_by_job_detail(Company* company) {
    JobDetails jobDetails;
    char* position, *department; // Assuming the strings won't exceed 255 characters
    printf("Enter the worker's department: ");
    department = get_safe_input(); // Read the department
    printf("Enter the worker's position: ");
    position = get_safe_input(); // Read the position

    jobDetails.position = position;
    jobDetails.department = department;

    Worker* found = (Worker*)bsearch(&jobDetails, company->workers, company->num_workers, sizeof(Worker*), compare_by_job_details_for_search);
    if (found) {
        printf("Worker found:\nName: %s\nID: %d\nDepartment: %s\nPosition: %s\nSalary: %.2f\nFactory ID: %d\n",
            found->worker_name, found->worker_id, found->job_details.department, found->job_details.position, found->salary, found->factory_id);
    }
    else {
        printf("Worker not found.\n");
    }
}

// Move a worker to a different factory or shop
void move_worker_factory_shop(Company* company) {
    int worker_id = 0, factory_shop_id;
	printf("===== Moving a worker =====\n");
	printf("Enter worker's id: ");
    get_safe_input(&worker_id);

	// Assuming there is a mechanism to iterate through the company's workers
	for(int i = 0; i < company->num_workers; i++) {
		if (company->workers[i]->worker_id == worker_id) {
			// Found the worker with the matching ID
			printf("Enter factory/shop ID to move the worker to: ");
			safe_int_scanf(&factory_shop_id);
            clean_buffer();
			move_worker(company, company->workers[i], factory_shop_id);
			printf("Worker %s has been moved successfully.\n", company->workers[i]->worker_name);
			return;
		}
	}

	// If the worker is not found
	printf("Worker with ID %d not found.\n", worker_id);
}

// Fire a worker from the company
void fire_worker_from_sys(Company* company) {
	int worker_id;
	printf("===== Firing a worker =====\n");
	printf("Enter worker's id: ");
	safe_int_scanf(&worker_id);
    clean_buffer();

	// Assuming there is a mechanism to iterate through the company's workers
	for (int i = 0; i < company->num_workers; i++) {
		if (company->workers[i]->worker_id == worker_id) {
			// Found the worker with the matching ID
            char * name = safe_strdup(company->workers[i]->worker_name);
			fire_worker(company, company->workers[i]->worker_id);
			printf("Worker %s has been fired successfully.\n", name);
			SAFE_FREE(name);
			return;
		}
	}

	// If the worker is not found
	printf("Worker with ID %d not found.\n", worker_id);
}
