#ifndef NAMESERVER_H
#define NAMESERVER_H

#include "printer.h"
#include "vendingmachine.h"



_Task NameServer {
	Printer	&printer;
	unsigned int numVendingMachines;
	unsigned int numStudents;
	unsigned int registerNumber;
	unsigned int *studentsAssignmentList;
	VendingMachine **machines;

    void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif