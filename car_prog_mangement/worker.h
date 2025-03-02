#ifndef WORKER_H
#define WORKER_H

// Include the necessary header files
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Define the JobDetails struct
typedef struct {
    char*   position; // Job position
    char*   department; // Department name
} JobDetails;

// Define the Worker struct
typedef struct {
    char*           worker_name;
    int             worker_id;
    double          salary; 
    int             factory_id; 
    JobDetails      job_details;
} Worker;

// Function declarations
void    init_worker(Worker* worker, int id, const char* name, const char* position, double salary, const char* department, int factory_id);
void    update_worker_name(Worker* worker, char* new_name);
void    update_salary(Worker* worker, double new_salary);
void    assign_to_factory(Worker* worker, int new_factory_id);
void    display_worker_info(Worker* worker);\
void    encode_name(const char* name, uint8_t* encoded_name, int name_length, int* bit_count);
void    decode_name(const uint8_t* encoded_name, char* decoded_name, int name_length);
void    free_worker(Worker* worker);

#endif // End of the include guard WORKER_H
