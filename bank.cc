#include "bank.h"

// create student accounts for each student and initialize them to 0
Bank::Bank( unsigned int numStudents ){
	studentAccounts = new unsigned int [numStudents];
	for (unsigned int i=0; i<numStudents; i++){
		studentAccounts[i] = 0;
	}
}

// delete student accounts
Bank::~Bank(){
	delete studentAccounts;
}

// add amount to student acount
void Bank::deposit( unsigned int id, unsigned int amount ){
	studentAccounts[id] += amount;
}

// remove amount from student account if you have enought money or wait for money
void Bank::withdraw( unsigned int id, unsigned int amount ){
	while(studentAccounts[id] < amount){
		_Accept(deposit);
	}
	studentAccounts[id] -= amount;
}