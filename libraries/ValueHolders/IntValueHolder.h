#ifndef IntValueHolder_h
#define IntValueHolder_h

#include "Arduino.h"

class IntValueHolder
{
  public:
    IntValueHolder();
    void SetName(String);
	void SetDebug(bool);
    void Update(int);
	bool Changed(void);
  private:
    int _value;
	int _oldValue;
	String _name;
	bool _debug;
};

#endif