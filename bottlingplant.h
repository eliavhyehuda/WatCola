#ifndef BOTTLINGPLANT_H
#define BOTTLINGPLANT_H

#include "nameserver.h"
#include "printer.h"


_Task BottlingPlant {
	Printer &printer;
	NameServer &nameServer;
	bool truckFinished;

	unsigned int numVendingMachines;
	unsigned int maxShippedPerFlavour;
	unsigned int maxStockPerFlavour;
	unsigned int timeBetweenShipments;
	int flavoursProduced[4];
	// ??

    void main();
  public:
    _Event Shutdown {};                       // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );
};

#endif