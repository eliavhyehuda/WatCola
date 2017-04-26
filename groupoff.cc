#include "groupoff.h"
#include "printer.h"
#include "watcard.h"

// groupoff will make giftcard for each student once after waiting on groupoff delay
void Groupoff::main(){
	for (unsigned int i=0; i<numStudents; i++){
		_Accept(~Groupoff){
			break;
		} _Else {
			yield(groupoffDelay);
			printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
			WATCard *watCard = new WATCard();
			watCard->deposit(sodaCost);
			futuresArray[i]->delivery(watCard);
		}
	}
}


// groupoff constructor will print start message and setup a futuresArray list which store the giftcard futures
Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
		: printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
	printer.print(Printer::Kind::Groupoff, 'S');
	// make futures array to store giftcard futures
	futuresArray = new WATCard::FWATCard *[numStudents];
	for (unsigned int i=0; i<numStudents; i++){
		futuresArray[i] = new WATCard::FWATCard();
	}
	numberOfCalls = 0;
}

// destructor will delete future array elements and the future array and finally print the shutdown message
Groupoff::~Groupoff(){
	for (unsigned int i=0; i<numStudents; i++){
		delete futuresArray[i];
	}
	delete futuresArray;
	printer.print(Printer::Kind::Groupoff, 'F');
}

// for each call return the futures in order
WATCard::FWATCard Groupoff::giftCard(){
	numberOfCalls++;
	return *futuresArray[numberOfCalls - 1];
}

