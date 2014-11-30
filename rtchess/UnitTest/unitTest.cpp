/*
	This file implements simple unit test framework.
*/

// C++ headers
#include <iostream>

// Project headers
#include "Vector3d.h"
#include "Camera.h"
#include "Shape.h"
#include "Ray.h"

using namespace std;

// Comparison of two floating point values. Consideres EPSILON tolerance
// to prevent data type (float, double) imprecision.
bool eq(double a, double b, double EPSILON = 1e-6) 
{
	return fabs(a - b) < EPSILON;
}

class Test
{
public:
	Test(string name, void(* testPtr)()): 
		name_(name), testPtr_(testPtr) { run(); }
	
	~Test() { }

	void run()
	{ 
		Test::total = 0;
		Test::passed = 0;
		cout << "=== Running test '" << name_ << "' ===" << endl;
		(*testPtr_)();
		cout << (!(Test::total - Test::passed) ? "[PASS] " : "[FAIL] ") 
			 << Test::passed << "/" << Test::total << " tests passed" << endl << endl;
	}	

	static void fail(unsigned& testNum, string& msg)
	{
		cerr << "fail (" << testNum << "): " << msg << endl;
	}

	static void assertTrue(bool res, string& msg) 
	{
		Test::total++;
		if(res) Test::passed++;
		else Test::fail(Test::total, msg);
	}

	static unsigned total;
	static unsigned passed;

private:
	string name_;
	void (* testPtr_)(); 	
};

unsigned Test::total;
unsigned Test::passed;


///////////////////////////////////////////////////////////////////////////
////	VECTOR.H

void testVector() 
{	
	// -- test 1 --	
	Vector3d v1(1.2, 3.5, -7.1);
	Vector3d v2(0.8, -7.5, -2.8);
	Vector3d v3 = -v1 + v2;	

	Test::assertTrue(eq(v3.x_, -0.4) && eq(v3.y_, -11.0) && eq(v3.z_, 4.3), 
		string("Vector3d v3 = -v1 + v2;"));	

	// -- test 2 --	
	Test::assertTrue((-v1 + v2) == v3, 
		string("Wrong implementation of operator=="));	

	// -- test 3 --	
	Test::assertTrue((v1 * v2) == Vector3d(v1.x_ * v2.x_, v1.y_ * v2.y_, v1.z_ * v2.z_), 
		string("Wrong implementation of operator* for Vector3d"));	

	// -- test 3 --	
	Test::assertTrue((v1 * -3.5) == Vector3d(v1.x_ * -3.5, v1.y_ * -3.5, v1.z_ * -3.5), 
		string("Wrong implementation of operator* for double"));	

	Test::assertTrue((-3.5 * v1) == Vector3d(v1.x_ * -3.5, v1.y_ * -3.5, v1.z_ * -3.5), 
		string("Wrong implementation of operator* for double"));
}

///////////////////////////////////////////////////////////////////////////
////	CAMERA.H
void testCamera()
{
	// -- test 1 --
	Camera c(Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 0.0), 200, 200, 90);
	double pxStep = c.getPxStep();	

	Test::assertTrue(eq(pxStep, 0.01), string("eq(pxStep, 0.01)"));
}

///////////////////////////////////////////////////////////////////////////
////	Sphere
void testSphere()
{
	Material mat(Vector3d(0.5, 0.5, 0.5), 0.0, 0.0, 0.0, 4.0);
	Shape::Intersection isectInfo;

	// -- test 1 --	
	Sphere s1(Point(0.0, 0.0, 0.0), 5.0, mat);
	Ray r1(Point(0.0, 0.0, 10.0), Vector3d(0.0, 0.0, -1.0));		

	Test::assertTrue(s1.intersects(r1, isectInfo), string("should intersect"));	
	Test::assertTrue(isectInfo.isect == Point(0.0, 0.0, 5.0), string("wrong intersection point"));					
	Test::assertTrue(isectInfo.normal == Vector3d(0.0, 0.0, 1.0).normalize(), string("wrong normal vector"));	

	// -- test 2 --	
	Sphere s2(Point(0.0, 0.0, 0.0), 5.0, mat);
	Ray r2(Point(1.0, 1.0, 1.0), Vector3d(1.0, 1.0, 1.0));		

	Test::assertTrue(s2.intersects(r2, isectInfo), string("should intersect"));	
	Test::assertTrue(isectInfo.isect == Vector3d(5.0 / sqrt(3.0)), string("wrong intersection point"));					
	Test::assertTrue(isectInfo.normal == Vector3d(1.0, 1.0, 1.0).normalize(), string("wrong normal vector"));	
}	

int main(int argc, char** argv) 
{
	// -- TEST Vector.h --
	Test("Vector", testVector);	

	// -- TEST Camera.h --
	Test("Camera", testCamera);	
	
	// -- TEST Sphere --
	Test("Sphere", testSphere);	
}
