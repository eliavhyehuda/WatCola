#include <iostream>

#include "printer.h"

using namespace std;

// The flush method just prints whatever data is there in the buffers and cleans the buffers
void Printer::flushPrint(){
    for (unsigned int i=0; i<numValues; i++){
        if (bufferState[i] != '-'){
            cout << bufferState[i];
        }
        if (bufferValue1[i] != -1){
            cout << bufferValue1[i];
        }
        if (bufferValue2[i] != -1){
            cout << "," << bufferValue2[i];
        }
        bufferState[i] = '-';
        bufferValue1[i] = -1;
        bufferValue2[i] = -1;
        cout << '\t';
    }
    cout << endl;
    bufferFilled = false;
}

// The main print method handles the finish and the normal state print (more info inside)
void Printer::print(unsigned int id, char state, int value1, int value2){
    // if the finish state is sent then clean the buffer and set all values to ... and print the finish line
    // else if your buffer is occupide at the given state then flush your buffer
    if (state == 'F'){
        if (bufferFilled){
            flushPrint();
        }
        
        for (unsigned int i=0; i<numValues; i++){
            if (i == id){
                cout << "F" << '\t';
            }else {
                cout << "..." << '\t';
            }
        }
        cout << endl;

    } else if (bufferState[id] != '-'){
        flushPrint();
    }

    // if we are not given the Finish state then add info normally to the buffer (assuming buffer was flushed if needed)
    if (state != 'F'){
        bufferState[id] = state;
        bufferValue1[id] = value1;
        bufferValue2[id] = value2;
        bufferFilled = true;
    }
}


// The constructor does the setup, creates 3 buffers for data storage and prints the initial print
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ){
    // setup values
    students = numStudents;
    machines = numVendingMachines;
    couriers = numCouriers;
    numValues = 6 + students + machines + couriers;
    
    // create buffers
    bufferState = new char[numValues];
    bufferValue1 = new int[numValues];
    bufferValue2 = new int[numValues];
    
    // print initialization output
    cout << "Parent" << '\t';
    cout << "Gropoff" << '\t';
    cout << "WATOff" << '\t';
    cout << "Names" << '\t';
    cout << "Truck" << '\t';
    cout << "Plant" << '\t';
    for (unsigned int i=0; i<students; i++){
        cout << "Stud" << i << '\t';
    }
    for (unsigned int i=0; i<machines; i++){
        cout << "Mach" << i << '\t';
    }
    for (unsigned int i=0; i<couriers; i++){
        cout << "Cour" << i << '\t';
    }
    cout << endl;
    for (unsigned int i=0; i<numValues; i++){
        cout << "*******" << '\t';
    }
    cout << endl;
    
    // setup clean buffers
    for (unsigned int i=0; i<numValues; i++){
        bufferState[i] = '-';
        bufferValue1[i] = -1;
        bufferValue2[i] = -1;
    }
    bufferFilled = false;
}

// The destructor just deleted the 3 buffers
Printer::~Printer(){
    // print test code
    if (bufferFilled){
        cout << "ERROR: BUFFER SHOULD BE EMPTY AT THE END OF EXECUTION AS LAST TASK SHOULD BE F TO FINISH" << endl;
    }
    cout << "***********************" <<endl;
    delete bufferState;
    delete bufferValue1;
    delete bufferValue2;
}

// All the folloing print varients call the final print varient by sending the info they have to it
void Printer::print( Kind kind, char state ){
    print(kind, 0, state, -1, -1);
}
void Printer::print( Kind kind, char state, int value1 ){
    print(kind, 0, state, value1, -1);
}
void Printer::print( Kind kind, char state, int value1, int value2 ){
    print(kind, 0, state, value1, value2);
}
void Printer::print( Kind kind, unsigned int lid, char state ){
    print(kind, lid, state, -1, -1);
}
void Printer::print( Kind kind, unsigned int lid, char state, int value1 ){
    print(kind, lid, state, value1, -1);
}

// This final print varient will take all prints and based on the given info will call the correct kind
void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ){
    unsigned int printerOffset = 0;
    if (kind == Student){
        printerOffset = 6;
    }else if (kind == Vending){
        printerOffset = 6 + students;
    }else if (kind == Courier){
        printerOffset = 6 + students + machines;
    }else {
        printerOffset = (unsigned int) kind;
    }

    print(printerOffset + lid, state, value1, value2);
}



