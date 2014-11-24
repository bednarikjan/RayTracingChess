/*
	This file implements simple unit test framework.
*/
#include <iostream>
#include "Vector3d.h"
#include "Camera.h"

using namespace std;

const double EPSILON = 1e-5;

// Comparison of two floating point values. Consideres EPSILON tolerance
// to prevent data type (float, double) imprecision.
bool eq(double a, double b) 
{
	return fabs(a - b) < EPSILON;
}

class Test
{
public:
	Test(string name, void(* testPtr)(unsigned& total, unsigned& passed)): 
		name_(name), testPtr_(testPtr), total_(0), passed_(0) { }
	
	~Test() { }

	void run()
	{ 
		cout << "=== Running test '" << name_ << "' ===" << endl;
		(*testPtr_)(total_, passed_);
		cout << (!(total_ - passed_) ? "[PASS] " : "[FAIL] ") 
			 << passed_ << "/" << total_ << " tests passed" << endl << endl;
	}	

	static void fail(unsigned& testNum, string& msg)
	{
		cerr << "fail (test " << testNum << "): " << msg << endl;
	}

private:
	string name_;
	void (* testPtr_)(unsigned& total, unsigned& passed); 
	unsigned total_;
	unsigned passed_;
};

///////////////////////////////////////////////////////////////////////////
////	VECTOR.H

void testVector(unsigned& total, unsigned& passed) 
{	
	// -- test 1 --
	total++;
	Vector3d v1(1.2, 3.5, -7.1);
	Vector3d v2(0.8, -7.5, -2.8);
	Vector3d v3 = -v1 + v2;	

	if(eq(v3.x_, -0.4) && eq(v3.y_, -11.0) && eq(v3.z_, 4.3)) passed++;
	else Test::fail(total, string("Vector3d v3 = -v1 + v2;"));
}

///////////////////////////////////////////////////////////////////////////
////	CAMERA.H
void testCamera(unsigned& total, unsigned& passed)
{
	// -- test 1 --
	total++;
	Camera c(Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 0.0), 200, 200, 90);
	double pxStep = c.getPxStep();	

	if(eq(pxStep, 0.01)) passed++;
	else Test::fail(total, string("eq(pxStep, 0.01)"));

}

int main(int argc, char** argv) 
{
	// -- TEST Vector.h --
	Test tVector("Vector", testVector);
	tVector.run();		

	// -- TEST Camera.h --
	Test tCamera("Camera", testCamera);
	tCamera.run();
	

}
