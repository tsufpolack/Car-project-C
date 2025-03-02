#include "CompanySystem.h"

int main() {
	Company* company;
	company = start_company_system();
	run_company_system(&company);
	CleanScreen();
	printf("Goodbye!\n");
	return 0;
}