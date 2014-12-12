#include <chrono>
#include <iostream>
#include "Scene.h"
#include "RayTracer.h"
#include "Camera.h"
#include "Shape.h"
#include "Chess.h"
#include "Camera.h"
#include "Vector3d.h"
#include "Light.h"
#include "Shape.h"

using namespace std;

void printHelp() {
	cout << "Usage: rtchess model config_chessboard config_ray_tracer output\n" 
			"\tmodel\t\t\tmodel file name (.OBJ)\n"
			"\tconfig_chessboard\tchessboard configuration file\n"
			"\tconfig_ray_tracer\tray tracer configuration file\n"
			"\toutput\t\t\toutput file (.PNG)" 
		 << endl;
}

//! extracts vector from config file format to Vector3d format
Vector3d extractVector(string value)
{
	int idxFirst = value.find_first_of('[') + 1;
	int idxSecond = value.find_first_of(',', idxFirst) + 1;
	int idxThird = value.find_first_of(',', idxSecond) + 1;
	int idxEnd = value.find_first_of(']', idxThird) + 1;

	double x = atof(value.substr(idxFirst, idxSecond - idxFirst - 1).c_str());
	double y = atof(value.substr(idxSecond, idxThird - idxSecond - 1).c_str());
	double z = atof(value.substr(idxThird, idxThird - idxEnd).c_str());

	return Vector3d(x, y, z);
}

//! Parse ray tracer configuration file
void configureScene(string& configRTFile, Camera& camera, Light& light, int& depth, Vector3d& bgrdColor,
					Material& wPieceMat, Material& bPieceMat, Material& wFieldMat, Material& bFieldMat)
{	
	Vector3d position;
	Vector3d direction;
	int width;
	int height;
	double fov;
	
	//debug
	cout << "Loading configuration file " << configRTFile << "..." << endl;		

	ifstream file(configRTFile);	
	if(file.fail()) {
		cerr << "ERROR: The file " << configRTFile << " cannot be opened." << endl;
		exit(1);
	}	
			
	string line;
	char propertyBuf[50];
	char valueBuf[50];		
	while(getline(file, line)) {
		if(line[0] == '#') continue;	// commentary
		
		sscanf(line.c_str(), "%s %s", propertyBuf, valueBuf);
		string prop(propertyBuf);
		string val(valueBuf);

		if	   (prop.find("camera-position") != string::npos) {
			position = extractVector(val);
			//camera.setPosition(extractVector(val));
		}
		else if(prop.find("direction") != string::npos) {
			direction = extractVector(val);
			//camera.setDirection(extractVector(val));
		}
		else if(prop.find("width") != string::npos)	{
			width = atoi(val.c_str());
			//camera.setResolution(atoi(val.c_str()), camera.getScreenHeight());
		}
		else if(prop.find("height") != string::npos) {
			height = atoi(val.c_str());
			//camera.setResolution(camera.getScreenWidth(), atoi(val.c_str()));
		}
		else if(prop.find("fov") != string::npos)	{
			fov = atoi(val.c_str());
			//camera.setFieldOfView(atoi(val.c_str()));
		}
		else if(prop.find("light-position") != string::npos)			light.center_ = extractVector(val);
		else if(prop.find("depth") != string::npos)						depth = atoi(val.c_str());
		else if(prop.find("bgrd-color") != string::npos)				bgrdColor = extractVector(val);
		else if(prop.find("white-piece-color") != string::npos)			wPieceMat.color = extractVector(val);
		else if(prop.find("white-piece-reflectivity") != string::npos)	wPieceMat.reflection = atof(val.c_str());
		else if(prop.find("white-piece-shininess") != string::npos)		wPieceMat.shininess = atof(val.c_str());
		else if(prop.find("black-piece-color") != string::npos)			bPieceMat.color = extractVector(val);
		else if(prop.find("black-piece-reflectivity") != string::npos)	bPieceMat.reflection = atof(val.c_str());
		else if(prop.find("black-piece-shininess") != string::npos)		bPieceMat.shininess = atof(val.c_str());
		else if(prop.find("white-field-color") != string::npos)			wFieldMat.color = extractVector(val);
		else if(prop.find("white-field-reflectivity") != string::npos)	wFieldMat.reflection = atof(val.c_str());
		else if(prop.find("white-field-shininess") != string::npos)		wFieldMat.shininess = atof(val.c_str());
		else if(prop.find("black-field-color") != string::npos)			bFieldMat.color = extractVector(val);
		else if(prop.find("black-field-reflectivity") != string::npos)	bFieldMat.reflection = atof(val.c_str());
		else if(prop.find("black-field-shininess") != string::npos)		bFieldMat.shininess = atof(val.c_str());
	}

	camera = Camera(position, direction, width, height, fov);
}

int main(int argc, char** argv)
{
	// For now the model file to be loaded is specifed as 1. parameter
	// TODO - exceptions
	if(argc < 5) {
		cerr << "Bad parameters!\n";
		printHelp();
		exit(1);
	}

	string modelFile = string(argv[1]);	
	string configChessboardFile = string(argv[2]);
	string configRTFile = string(argv[3]);
	string outputFile = string(argv[4]);		

	// Prepare chessboard
	Chess chess(modelFile, configChessboardFile, configRTFile);

	// Prepare scene, raytracer, adjust model colors
	Camera camera2;
	Light light2;
	int depth;
	Vector3d bgrdColor;
	Material whitePieceMaterial, blackPieceMaterial, whiteFieldMaterial, blackFieldMaterial;

	configureScene(configRTFile, camera2, light2, depth, bgrdColor, 
		whitePieceMaterial, blackPieceMaterial, whiteFieldMaterial, blackFieldMaterial);

	//debug
	cout << "Camera: " << endl;
	cout << "position: " << camera2.position() << endl;
	cout << "direction: " << camera2.direction() << endl;
	cout << "width: " << camera2.getScreenWidth() << endl;
	cout << "height: " << camera2.getScreenHeight() << endl;
	cout << "fov: " << camera2.getFieldOfView() << endl;

	chess.setWhitePieceMaterial(&whitePieceMaterial);
	chess.setBlackPieceMaterial(&blackPieceMaterial);
	chess.setWhiteFieldMaterial(&whiteFieldMaterial);
	chess.setBlackFieldMaterial(&blackFieldMaterial);

	// Set camera					
	Point position(-6.0, -3.0, 6.0);
	Vector3d direction = Vector3d(4.0, 3.0, -2.0).normalize();
	/*Point position(0.182, 2.459, 1.930);
	Vector3d direction = Vector3d(0.8664, 0.3339, -0.3714).normalize();	*/
	
	double fieldOfView = 45.0;
	int width = 80;
	int height = 60;
	/*int width = 3840;
	int height = 2016;*/

	Camera camera(position, direction, width, height, fieldOfView);

	// Set light
	Material lightMaterial(Vector3d(1.0, 1.0, 1.0), 0.0, 0.0, 0.0, 0.0);
	Light light(Vector3d(-0.05, 8.432, 3.769), 0.0, &lightMaterial);

	// Create scene and fill it with model	
	Scene scene(camera2, light2, chess.getModel());
	scene.setRecursionDepth(depth);
	scene.setBackgroundColor(bgrdColor);

	// debug - measure a time of rendering
	std::chrono::high_resolution_clock::time_point tStart = std::chrono::high_resolution_clock::now();

	// Render
	scene.render();

	// debug - measure a time of rendering
	std::chrono::high_resolution_clock::time_point tEnd = std::chrono::high_resolution_clock::now();
	auto durationMsec = std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart).count();
	std::cout << "Rendering time: " << (durationMsec / 1000.0) << std::endl;

	// Save resulting image
	scene.saveImage(outputFile);

	return 0;
}