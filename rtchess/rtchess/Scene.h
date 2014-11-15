#include <string>
#include "Camera.h"
#include "Vector.h"

using namespace std;

class Scene
{
public:
	//! Constructor
	Scene(void);
	
	//! Destructor
	~Scene(void);

	//! Loads model from file
	void loadModel(string fileName);

	//! Sets cameraposition and projection screen size
	void setCamera(Vector<double> position, Vector<double> direction, unsigned screenWidth, unsigned screenHeight);

private:
	Camera camera;

};

