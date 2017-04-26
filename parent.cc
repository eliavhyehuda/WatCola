#include "MPRNG.h"

#include "bank.h"
#include "parent.h"
#include "printer.h"

extern MPRNG ran;

// parent waits on its destructor and keeps adding a randome amount of money to a randome student
void Parent::main(){
	for (;;){
		_Accept(~Parent) {
			break;
		} _Else {
			int ranMoney = ran(1,3);
			int ranStudent = ran(0, numStudents - 1);

			yield (parentalDelay);

			printer.print(Printer::Kind::Parent, 'D', ranStudent, ranMoney);
			bank.deposit(ranStudent, ranMoney);
		}
	}
}

// The constructor just does the setup and prints the start message
Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay )
		: printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay){
	printer.print(Printer::Kind::Parent, 'S');
}

// the destructor doesn't need to do anything so it just prints the shutdown message
Parent::~Parent(){
	printer.print(Printer::Kind::Parent, 'F');
}
