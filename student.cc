#include "MPRNG.h"

#include "groupoff.h"
#include "nameserver.h"
#include "printer.h"
#include "student.h"
#include "watcard.h"
#include "watcardoffice.h"
#include "vendingmachine.h"

extern MPRNG ran;

// student will attempt to by each of its bottle using the giftcard and then using the watcard where each case would have its own exceptions
void Student::main() {

	// get the students vending machine assignment and print it
	VendingMachine *vMachine = nameServer.getMachine(id);
	printer.print(Printer::Kind::Student, id, 'V', vMachine->getId());

	// for each bottle yield for a randome time
	for (unsigned int i = 0; i < numBottlesToBuy; ++i) {
		yield(ran(1,10));
		// attempt to buy a bottle until you have a succesful buy
		for (;;){
			// attempt to buy bottle using giftcard
			// if out of stuck get a new vMachine and print it
			// if our of funds do nothing because giftcard is only one time use
			try{
				vMachine->buy(favoriteFlavour, *(giftcard()));
				printer.print(Printer::Kind::Student, id, 'G', giftcard()->getBalance());
				break;
			} catch (VendingMachine::Stock) {
				vMachine = nameServer.getMachine(id);
				printer.print(Printer::Kind::Student, id, 'V', vMachine->getId());
				continue;
			} catch (VendingMachine::Funds) {
				// do nothing for giftcard
			}

			// attemp to but bottle using watcard
			// if out of stuck get a new vMachine and print it
			// if out of funds requiest fonds from watcardoffice
			// if watcard lost then request watcardoffice for a new watcard
			try {
				vMachine->buy(favoriteFlavour, *(watcard()));
				printer.print(Printer::Kind::Student, id, 'B', watcard()->getBalance());
				break;
			} catch (VendingMachine::Stock) {
				vMachine = nameServer.getMachine(id);
				printer.print(Printer::Kind::Student, id, 'V', vMachine->getId());
			} catch (VendingMachine::Funds) {
				watcard = watCardOffice.transfer(id, 5 + vMachine->cost(), watcard());
				continue;
			} catch (WATCardOffice::Lost) {
				watcard = watCardOffice.create(id, 5);
				printer.print(Printer::Kind::Student, id, 'L');
				continue;
			}
		}
	}
}


// The constructor does the setup and generates a random number of bottle pick and a randome fvoriteFlavour pick and then requests for a watcard and a giftcard
Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases )
		: printer(prt), nameServer(nameServer), watCardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) { 
	numBottlesToBuy = ran(1, maxPurchases);
	favoriteFlavour = VendingMachine::Flavours(ran(0, 3));
	printer.print(Printer::Kind::Student, id, 'S', (unsigned int) favoriteFlavour, numBottlesToBuy);
	watcard = watCardOffice.create(id, 5);
	giftcard = groupoff.giftCard();
}

// The destructor deletes the giftcard and the watcard if its not lost already and prints that its done 
Student::~Student(){
	delete giftcard();
	try {
		delete watcard();
	} catch (WATCardOffice::Lost){
		// do nothing
	}
	printer.print(Printer::Kind::Student, id, 'F');
}
