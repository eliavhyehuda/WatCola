#include "MPRNG.h"

#include "bottlingplant.h"
#include "nameserver.h"
#include "printer.h"
#include "truck.h"
#include "vendingmachine.h"

extern MPRNG ran;

// The truck first gets a vending machines list
// It then keeps taking shipments from the bottling plant
// It stops for coffee and then delivers the shipment to the vending machines in its list
void Truck::main(){
	VendingMachine **machinesList = nameServer.getMachineList();

	for (;;){

		//Check if plant closed and try to get shipment
		try{
			plant.getShipment(cargo);
		} catch (BottlingPlant::Shutdown){
			break;
		}
		
		// get total battles and print
		int total = 0;
		for (int i=0; i<4; i++){
			total += cargo[i];
		}
		printer.print(Printer::Kind::Truck, 'P', total);

		// Go get a Coffee
		yield(ran(1,10));

		// For each vending machine first take the next vending machine in line and start delivery
		for (unsigned int i=0; i<numVendingMachines; i++){
			lastMachine = (lastMachine + 1) % numVendingMachines;
			unsigned int *machineInventory = machinesList[lastMachine]->inventory();
			printer.print(Printer::Kind::Truck, 'd', lastMachine, total);

			// put bottles in the machinesInventory
			for (int j=0; j<4; j++){
				unsigned int stock = maxStockPerFlavour - machineInventory[j];
				if (stock > cargo[j]){
					stock = cargo[j];
				}
				cargo[j] = cargo[j] - stock;
				machineInventory[j] = machineInventory[j] + stock;
				total = total - stock;
			}

			// check for unfilled inventory
			int unfilled = 0;
			for (int j=0; j<4; j++){
				unfilled += maxStockPerFlavour - machineInventory[j];
			}
			if (unfilled > 0){
				printer.print(Printer::Kind::Truck, 'U', lastMachine, unfilled);
			}

			// print that you finished delivery and call restocked()
			printer.print(Printer::Kind::Truck, 'D', lastMachine, total);
			machinesList[lastMachine]->restocked();

			// If truck is out of bottles then break
			if (total <=0){
				break;
			}
		}
	}
}

// The truck constructor does the setup, does a start print and creates its cargo
Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) 
		: printer(prt), nameServer(nameServer), plant(plant), numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour) {
	printer.print(Printer::Kind::Truck, 'S');
	cargo = new unsigned int[4];
}

// The truck destroctor just deletes its cargo and prints that its shutting down
Truck::~Truck(){
	delete[] cargo;
	printer.print(Printer::Kind::Truck, 'F');
}