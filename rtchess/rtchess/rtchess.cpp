#include <iostream>
#include "Scene.h"

using namespace std;

void printHelp() {
	cout << "Usage:\n\trtchess model\n\t\tmodel\t.OBJ model file name" << endl;
}

int main(int argc, char** argv)
{
	// For now the model file to be loaded is specifed as 1. parameter
	// TODO - exceptions
	if(argc < 2) {
		cerr << "Bad parameters!\n";
		printHelp();
		exit(1);
	}

	string modelFile = string(argv[1]);
	// DEBUG
	cout << "Model file name: " << modelFile << endl;

	// Create scene
	Scene scene;

	// Load model
	scene.loadModel(modelFile);

	// Set camera
	scene.setCamera(Vector<double>(0.0, 0.0, 10.0), Vector<double>(1.0, 1.0, -1.0), 640, 480);

	// DEBUG - testing vector operations
	Vector<double> v1(2.0, 3.5, 7.8);
	Vector<double> v2(3.0, 7.1, 0.2);

	Vector<double> v3;
	v3 = v1 + v2;

	//cout << "v3 = (" << v3.x_ << ", " << v3.y_ << ", " << v3.z_ << ")" << endl;
	cout << v3 << endl;

	return 0;
}