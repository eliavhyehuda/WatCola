#include "watcard.h"

// The constroctor just sets the initial ballance
WATCard::WATCard(){
	currentBalance = 0;
}

// The deposit method just adds the amount to the currentBalance
void WATCard::deposit( unsigned int amount ) {
	currentBalance += amount;
}

// The withdraw method just removes the amount from the currentBalance
void WATCard::withdraw( unsigned int amount ) {
	currentBalance -= amount;
}

// The getBalance method just gets the balance from the card
unsigned int WATCard::getBalance(){
	return currentBalance;
}