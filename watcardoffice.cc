#include "MPRNG.h"

#include "bank.h"
#include "printer.h"
#include "watcard.h"
#include "watcardoffice.h"

extern MPRNG ran;

// setup for Args
Args::Args(unsigned int sid, unsigned int amount, WATCard *watCard)
		: sid(sid), amount(amount), watCard(watCard) { }


// The courier waits on the destroctor
// While waiting it requests jobs
void WATCardOffice::Courier::main(){
	for (;;){
		_Accept(~Courier){
			break;
		} _Else {
			Job *job = office.requestWork();

			// deal with a finish message
			if (job == NULL){
				break;
			}

			// print start progress and start the withdrawing and depositing proccess
			printer.print(Printer::Kind::Courier, cId, 't', job->args.sid, job->args.amount);
			bank.withdraw(job->args.sid, job->args.amount);
			job->args.watCard->deposit(job->args.amount);
			printer.print(Printer::Kind::Courier, cId, 'T', job->args.sid, job->args.amount);

			// By random chance or deliver watCard to student or lose the card
			if (ran(1,6) != 1){
				job->result.delivery(job->args.watCard);
			}else{
				delete job->args.watCard;
				job->result.exception(new WATCardOffice::Lost);
			}

			// delete the job once its done
			delete job;
		}
	}
}

// The constuctor just does a setup and prints the start message
WATCardOffice::Courier::Courier(Printer &printer, Bank &bank, WATCardOffice &office, unsigned int cId)
		:printer(printer), bank(bank), office(office), cId(cId) {
	printer.print(Printer::Kind::Courier, cId, 'S');
}

// The destroctor just prints the shutdown message
WATCardOffice::Courier::~Courier(){
	printer.print(Printer::Kind::Courier, cId, 'F');
}

// The watcardoffice will accept the destroctor
// Or a new job request if it has jobs available
// or a create or transfer request
void WATCardOffice::main(){
	for (;;){
		_Accept(~WATCardOffice) {
			//shutdown things??
			officeClosing = true;
			while (deleteCount > 0){
				_Accept(requestWork) { 
					// do nothing
				}
			}
			break;
		} or _When (!jobRequests.empty()) _Accept (requestWork) {
			jobRequests.pop();
		} or _Accept (create, transfer) {
			// do nothing
		}
	}
}

// The constroctor just does a setup and creates the corriers array
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers )
		: printer(prt), bank(bank) ,numCouriers(numCouriers) {

	printer.print(Printer::Kind::WATCardOffice, 'S');
	couriers = new Courier*[numCouriers];
	for (unsigned int i = 0; i < numCouriers; i++){
		couriers[i] = new Courier(printer, bank, *this, i);
	}
	deleteCount = numCouriers;
	officeClosing = false;
}

// The destroctor deletes the corrier array and prints the shutdown message
WATCardOffice::~WATCardOffice(){
	for (unsigned int i=0; i < numCouriers; i++){
		delete couriers[i];
	}
	delete couriers;
	printer.print(Printer::Kind::WATCardOffice, 'F');
}

// The create method create a watcard for a student and gives it to it
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ){
	printer.print(Printer::Kind::WATCardOffice, 'C', sid, amount);
	Args args(sid, amount, new WATCard());
	Job *job = new Job(args);
	jobRequests.push(job);
	return job->result;
}

// the transfer method transfers money to the watcard provided
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ){
	printer.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
	Args args(sid, amount, card);
	Job *job = new Job(args);
	jobRequests.push(job);
	return job->result;
}

// The requestwork method just return the next job in the queue and pops it
// If it gets the closing signal it sends a message to the corriers
WATCardOffice::Job *WATCardOffice::requestWork() {
	if (officeClosing){
		deleteCount = deleteCount-1;
		return NULL;
	}

	printer.print(Printer::Kind::WATCardOffice, 'W');
	return jobRequests.front();
	// maybe can just do pop()??
}

