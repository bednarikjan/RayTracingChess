#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

class Exception
{
public:
	Exception(void) { }
	~Exception(void) { }
};

class BadParamsException: public Exception
{

};

#endif