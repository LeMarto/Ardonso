#include "Arduino.h"
#include "IntValueHolder.h"

IntValueHolder::IntValueHolder()
{
	_value = 0;
	_oldValue = 0;
}

void IntValueHolder::SetName(String name)
{
	_name = name;
}

void IntValueHolder::SetDebug(bool debug)
{
	_debug = debug;
}

void IntValueHolder::Update(int value)
{
	_oldValue = _value;
	_value = value;
}

bool IntValueHolder::Changed()
{
	if (_oldValue == _value)
		return false;
	else
	{
		if (_debug)
		{
			Serial.print(_name);
			Serial.print(": ");
			Serial.print(_oldValue);
			Serial.print(" -> ");
			Serial.println(_value);
		}
		return true;
	}
}