#ifndef WATCARDOFFICE_H
#define WATCARDOFFICE_H

#include "bank.h"
#include "printer.h"
#include "watcard.h"

#include <queue>

using namespace std;

struct Args {
	unsigned int sid;
	unsigned int amount;
	WATCard * watCard;

	Args(unsigned int sid, unsigned int amount, WATCard *watCard);
};



_Task WATCardOffice {
	Printer &printer;
	Bank &bank;
	unsigned int numCouriers;

    struct Job {                              // marshalled arguments and return future
        Args args;                            // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;             // return future
        Job( Args args ) : args( args ) {}
    };

    _Task Courier {
    	Printer &printer;
    	Bank &bank;
    	WATCardOffice &office;
    	unsigned int cId;
    	void main();
      public:
      	Courier(Printer &printer, Bank &bank, WATCardOffice &office, unsigned int cId);
      	~Courier();
    };                    // communicates with bank
    
    Courier **couriers;
    queue<Job *> jobRequests;
    int deleteCount;
	bool officeClosing;
    void main();
  public:
    _Event Lost {};                           // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif