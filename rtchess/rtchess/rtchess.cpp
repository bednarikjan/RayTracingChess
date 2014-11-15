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
	scene.setCamera(Vector(0.0, 0.0, 10.0), Vector(1.0, 1.0, -1.0), 640, 480);

	// 

	return 0;
}