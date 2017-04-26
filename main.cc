#include <iostream>

#include "MPRNG.h"

#include "bank.h"
#include "bottlingplant.h"
#include "config.h"
#include "groupoff.h"
#include "nameserver.h"
#include "parent.h"
#include "printer.h"
#include "student.h"
#include "vendingmachine.h"
#include "watcardoffice.h"

using namespace std;

MPRNG ran;

void uMain::main() {
	// get random seed which will replaced if seed is given
	int seed = getpid();

	// read programParams into this
	ConfigParms programParams;

	// case 3: read given seed and read given file
	// case 2: read given file
	// case 1: read defult file "config.txt"
	// defult: usage error
	switch (argc){
		case 3:
			try{
				seed = stoi(argv[2]);
				if (seed <= 0){
					cout << "Error: seed must be a positive number" << endl;
	                cout << "Usage: ./soda [ config-file [ Seed ]]" << endl;
	                return;
				}
			} catch (...){
				cout << "Error: seed must be a positive number" << endl;
                cout << "Usage: ./soda [ config-file [ Seed ]]" << endl;
                return;
			}
		case 2:
			try{
				processConfigFile( argv[1], programParams );
				break;
			} catch (...){
				cout << "Error: File reading problems" << endl;
                cout << "Usage: ./soda [ config-file [ Seed ]]" << endl;
                return;
			}
		case 1:
			try{
				processConfigFile( "config.txt", programParams );
				break;
			} catch (...){
				cout << "Error: Defult file reading problems" << endl;
                cout << "Usage: ./soda [ config-file [ Seed ]]" << endl;
                return;
			}
		default:
			cout << "Usage: ./soda [ config-file [ Seed ]]" << endl;
            return;
	}

	ran.set_seed(seed);

	// start printer
	Printer printer(programParams.numStudents, programParams.numVendingMachines, programParams.numCouriers);
	
	// start bank
	Bank bank(programParams.numStudents);
	
	// start parent
	Parent parent(printer, bank, programParams.numStudents, programParams.parentalDelay);
	
	// start watcardoffice
	WATCardOffice watCardOffice(printer, bank, programParams.numCouriers);
	
	// start groupoff
	Groupoff groupoff (printer, programParams.numStudents, programParams.sodaCost, programParams.groupoffDelay);
	
	// start nameserver
	NameServer nameServer (printer, programParams.numVendingMachines, programParams.numStudents);

	// create vendingMachines array to store machines and start vendingMachines
	VendingMachine *vendingMachines[programParams.numVendingMachines];
	for (unsigned int i=0; i< programParams.numVendingMachines; i++){
		vendingMachines[i] = new VendingMachine(printer, nameServer, i, programParams.sodaCost, programParams.maxStockPerFlavour);
	}

	// allocate bottlingPlant so we can make sure its deleted before machines
	BottlingPlant *bottlingPlant = new BottlingPlant(printer, nameServer, programParams.numVendingMachines, programParams.maxShippedPerFlavour, programParams.maxStockPerFlavour, programParams.timeBetweenShipments);

	// create students array to store machines and start students
	Student *students[programParams.numStudents];
	for (unsigned int i=0; i< programParams.numStudents; i++){
		students[i] = new Student(printer, nameServer, watCardOffice, groupoff, i, programParams.maxPurchases);
	}

	// delete student tasks when they are completed
	for (unsigned int i=0; i< programParams.numStudents; i++){
		delete students[i];
	}

	// delete bottlingPlant before vendingMachines
	delete bottlingPlant;

	// delete vendingMachine tasks
	for (unsigned int i=0; i< programParams.numVendingMachines; i++){
		delete vendingMachines[i];
	}
	// All other tasks will be deleted when uMain finishes
}