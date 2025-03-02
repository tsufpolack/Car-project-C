#define _CRT_SECURE_NO_WARNINGS
#include "worker.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

// Function implementations

void init_worker(Worker* worker, int id, const char* name, const char* position, double salary, const char* department, int factory_id) {
    worker->worker_id = id;
    worker->worker_name = safe_strdup(name); // Allocate and copy
    worker->job_details.position = safe_strdup(position);
    worker->salary = salary;
    worker->job_details.department = safe_strdup(department);
    worker->factory_id = factory_id;
}


void update_worker_name(Worker* worker, char* new_name) {
    worker->worker_name = safe_realloc(worker->worker_name, strlen(new_name) + 1);
    strcpy(worker->worker_name, new_name);
}

void update_position(Worker* worker, char* new_position) {
    worker->job_details.position = safe_realloc(worker->job_details.position, strlen(new_position) + 1);
    strcpy(worker->job_details.position, new_position);
}

void update_salary(Worker* worker, double new_salary) {
    worker->salary = new_salary;
}

void assign_to_department(Worker* worker, char* new_department) {
    worker->job_details.department = safe_realloc(worker->job_details.department, strlen(new_department) + 1);
    strcpy(worker->job_details.department, new_department);
}

void assign_to_factory(Worker* worker, int new_factory_id) {
    worker->factory_id = new_factory_id;
}

void display_worker_info(Worker* worker) {
    printf("Worker Name: %s\n", worker->worker_name);
    printf("Worker ID: %d\n", worker->worker_id);
    printf("Position: %s\n", worker->job_details.position);
    printf("Salary: %.2f\n", worker->salary);
    printf("Department: %s\n", worker->job_details.department);
    printf("Factory ID: %d\n", worker->factory_id);
}


void free_worker(Worker* worker) {
    if (worker != NULL) {
        SAFE_FREE(worker->worker_name);
        SAFE_FREE(worker->job_details.position);
        SAFE_FREE(worker->job_details.department);
    }
}
