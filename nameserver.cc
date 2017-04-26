#include "nameserver.h"
#include "printer.h"
#include "vendingmachine.h"

// when in registering mode except VMregister, when in operations mode except operations getMachine, getMachineList
void NameServer::main(){
	for (;;){
		_Accept(~NameServer){
			break;
		} or _When (registerNumber < numVendingMachines) _Accept(VMregister){
			// do nothing
		} or _When (registerNumber == numVendingMachines) _Accept(getMachine, getMachineList){
			// do nothing
		}
	}
}

// constructor creates a student assignment list to store students vending machine assignment and a machines list to store the machines
NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents )
		: printer(prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {
	printer.print(Printer::Kind::NameServer, 'S');
	studentsAssignmentList = new unsigned int [numStudents];
	machines = new VendingMachine* [numVendingMachines];
	for (unsigned int i=0; i<numStudents; i++){
		studentsAssignmentList[i] = i % numVendingMachines;
	}
	registerNumber = 0;
}

// destructor will delete the studentsAssignments array and the machinesList array
NameServer::~NameServer(){
	delete machines;
	delete studentsAssignmentList;
	printer.print(Printer::Kind::NameServer, 'F');
}

// set machines based on their id into machines list
void NameServer::VMregister( VendingMachine *vendingmachine ){
	printer.print(Printer::Kind::NameServer, 'R', vendingmachine->getId());
	machines[vendingmachine->getId()] = vendingmachine;
	registerNumber++;
}

// getMachine will give a student the next machine
VendingMachine *NameServer::getMachine( unsigned int id ){
	VendingMachine* myMachine = machines[studentsAssignmentList[id]];
	printer.print(Printer::Kind::NameServer, 'N', id, studentsAssignmentList[id]);
	studentsAssignmentList[id] = (studentsAssignmentList[id] + 1) % numVendingMachines;
	return myMachine;
}

// getMachineList will just return the list of machines
VendingMachine **NameServer::getMachineList(){
	return machines;
}