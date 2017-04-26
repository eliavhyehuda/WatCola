#include "MPRNG.h"

#include "bottlingplant.h"
#include "nameserver.h"
#include "printer.h"
#include "truck.h"
#include "vendingmachine.h"

extern MPRNG ran;

// wait for destructor call and keep producing bottles until called
void BottlingPlant::main(){
	Truck truck(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);
	for (;;) {
		_Accept(~BottlingPlant) {
			// tell truck that it can also shut down and wait for it to shut down first
			truckFinished = true;
			_Accept(getShipment);
			break;
		} _Else {
			// yield for timeBetweenShipments
			yield(timeBetweenShipments);

			// produce random bottles for each flavour and keep count of total for a print
			int total = 0;
			for (int i=0; i<4; i++){
				flavoursProduced[i] = ran(0,maxShippedPerFlavour);
				total += flavoursProduced[i];
			}
			printer.print(Printer::Kind::BottlingPlant, 'G', total);

			// wait for truck to take shipment
			_Accept(getShipment);
		}
	}
}

// Start up bottlingPlant, initialize all variables and print start message
BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments )
		: printer(prt), nameServer(nameServer), numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments) {
	printer.print(Printer::Kind::BottlingPlant, 'S');
	truckFinished = false;
}

// destructor shuold only print finish message as no allocation in bottlingPlant
BottlingPlant::~BottlingPlant(){
	printer.print(Printer::Kind::BottlingPlant, 'F');
}

// Put shipment bottles into cargo and if bottling plant is set to shutdown tell truck to also shutdown
void BottlingPlant::getShipment( unsigned int cargo[] ){
	// give signal to truck so it can shutdown
	if (truckFinished){
		uRendezvousAcceptor(); // Given by TA to not throw additional exception
		_Throw Shutdown();
	}

	// put bottles into cargo
	for (int i=0; i<4; i++){
		cargo[i] = flavoursProduced[i];
		flavoursProduced[i] = 0;
	}

	printer.print(Printer::Kind::BottlingPlant, 'P');
}