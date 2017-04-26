#ifndef PRINTER_H
#define PRINTER_H


_Monitor Printer {
  unsigned int students;
  unsigned int machines;
  unsigned int couriers;
  unsigned int numValues;
  bool bufferFilled;

  char * bufferState;
  int * bufferValue1;
  int * bufferValue2;

  void flushPrint();
  void print(unsigned int id, char state, int value1, int value2);

  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
};


#endif