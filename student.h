#ifndef STUDENT_H
#define STUDENT_H

#include "groupoff.h"
#include "nameserver.h"
#include "printer.h"
#include "watcardoffice.h"


_Task Student {
	Printer &printer;
	NameServer &nameServer;
	WATCardOffice &watCardOffice;
	Groupoff &groupoff;
	unsigned int id;
	unsigned int maxPurchases;
	unsigned int numBottlesToBuy;
	VendingMachine::Flavours favoriteFlavour;
	WATCard::FWATCard watcard;
	WATCard::FWATCard giftcard;

    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases );
    ~Student();
};

#endif