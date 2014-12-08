#include <iostream>
#include "Scene.h"
#include "RayTracer.h"
#include "Camera.h"
#include "Shape.h"
#include "Chess.h"

using namespace std;

void printHelp() {
	cout << "Usage: rtchess model output\n\tmodel\t\tmodel file name (.OBJ)\n\toutput\t\toutput file (.PNG)" << endl;
}

int main(int argc, char** argv)
{
	// For now the model file to be loaded is specifed as 1. parameter
	// TODO - exceptions
	if(argc < 3) {
		cerr << "Bad parameters!\n";
		printHelp();
		exit(1);
	}

	string modelFile = string(argv[1]);
	string outputFile = string(argv[2]);			

	// prepare chessboard
	Chess chess(modelFile);

	// Set camera and light				
	// -- or use defaults set by Scene constructor --

	// Create scene and fill it with model
	/*Scene scene(modelFile);	*/
	Scene scene(chess.getModel());	

	// try to adjust camera position
	scene.setCameraLocation(Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 1.0, 0.0).normalize());

	// Render
	scene.render();

	// Save resulting image
	scene.saveImage(outputFile);

	return 0;
}