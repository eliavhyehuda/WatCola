#ifndef GROUPOFF_H
#define GROUPOFF_H

#include "printer.h"
#include "watcard.h"

using namespace std;

_Task Groupoff {
	Printer &printer;
	unsigned int numStudents;
	unsigned int sodaCost;
	unsigned int groupoffDelay;
	unsigned int numberOfCalls;
	bool done;
	WATCard::FWATCard ** futuresArray;

    void main();
  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
    WATCard::FWATCard giftCard();
};

#endif