#include "nameserver.h"
#include "printer.h"
#include "vendingmachine.h"
#include "watcard.h"


// The vending machine waits on its destroctor and accepts a buy or an inventory restock
// It doesnt allow a buy or anything to happen while its restovking until restocked is called signalling that its completed
void VendingMachine::main(){
	for (;;){
		_Accept(~VendingMachine){
			break;
		} or _Accept(buy){
			//do nothing
		} or _Accept(inventory){
			printer.print(Printer::Kind::Vending, id, 'r');
			_Accept (restocked){
				// do nothing
			}
		}
	}
}

// The constructor just does a setup and creates the machine flavours and initializes them as 0
// It also registers itself at the nameServer
VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour )
		: printer(prt), nameServer(nameServer), id(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour) {
	printer.print(Printer::Kind::Vending, id, 'S', sodaCost);
	machineFlavours = new unsigned int[4];
	for (int i=0; i<4; i++){
		machineFlavours[i] = 0;
	}
	nameServer.VMregister(this);
}

// The destroctor deletes the machineFlavours array and prints the shutdown message
VendingMachine::~VendingMachine(){
	delete machineFlavours;
	printer.print(Printer::Kind::Vending, id, 'F');
}

// The buy method withdraws money from the students card and removes one bottle from the matching flavor stock
void VendingMachine::buy( Flavours flavour, WATCard &card ){
	if (card.getBalance() < sodaCost){
		uRendezvousAcceptor(); 		// Given by TA so we dont get an additional throw
		_Throw Funds();
	}

	if (machineFlavours[flavour] == 0){
		uRendezvousAcceptor();		// Given by TA so we dont get an additional throw
		_Throw Stock();
	}

	card.withdraw(sodaCost);
	machineFlavours[flavour]--;
	printer.print(Printer::Kind::Vending, id, 'B', flavour, machineFlavours[flavour]);
}

// The inventory call just returns the machineFlavours stock
unsigned int *VendingMachine::inventory(){
	return machineFlavours;
}

// The restocked method just prints the restocked message and is used to signal main
void VendingMachine::restocked(){
	printer.print(Printer::Kind::Vending, id, 'R');
}

// The cost method is used to get the cost of a soda (Nomutex because its a read only)
_Nomutex unsigned int VendingMachine::cost(){
	return sodaCost;
}

// The getId method is used to get the vendingMachines Id number (Nomutex because its a read only)
_Nomutex unsigned int VendingMachine::getId(){
	return id;
}









