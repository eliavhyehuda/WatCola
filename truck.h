#ifndef TRUCK_H
#define TRUCK_H

#include "nameserver.h"
#include "printer.h"

_Task BottlingPlant;


_Task Truck {
	Printer &printer;
	NameServer &nameServer;
	BottlingPlant &plant;
	unsigned int numVendingMachines;
	unsigned int maxStockPerFlavour;
	unsigned int * cargo;
	unsigned int lastMachine;

    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};

#endif