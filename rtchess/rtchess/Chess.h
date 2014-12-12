#ifndef _CHESS_H_
#define _CHESS_H_

// C++ includes
#include <fstream>
#include <iostream>

// project includes
#include "Model.h"
#include "Material.h"
#include "common.h"

using namespace std;

//! Class implements specific model with chessboard and 32 chess pieces
/*!
Member function load() implements loading of the chess model which is constrained
by the following requirements:

	- Each object of the scene (pieces and chessboard) is saved as a single object 
	(parameter 'o'), and has its own name in the format name_[n_]_{b|w}, where 'b' and 'w'
	stand for black or white and n stands for the number. The names are following:
		
		- chessboard_[b|w]
		- pawn_n_[b|w]
		- rook_n_[b|w]
		- knight_n_[b|w]
		- bishop_n_[b|w]
		- queen_[b|w]
		- king_[b|w]

	- The chessboard is saved as two single object each containing only black
	or white fields.

	- The chessboard is aligned with the x, y plane and the left bottom corner
	(where the black field should start) is positioned at [0, 0, 0].
*/

/*Material DEFAULT_WHITE_FIELD_MATERIAL(Vector3d(0.8, 0.8, 0.8), 0.8, 0.0, 0.0, 4.0);
Material DEFAULT_BLACK_FIELD_MATERIAL(Vector3d(0.1, 0.1, 0.1), 0.5, 0.0, 0.0, 4.0);
Material DEFAULT_WHITE_PIECE_MATERIAL(Vector3d(0.343, 0.492, 0.97), 0.6, 0.0, 0.0, 16.0);
Material DEFAULT_BLACK_PIECE_MATERIAL(Vector3d(0.273, 0.03, 0.03), 0.6, 0.0, 0.0, 16.0);	*/	

Material DEFAULT_WHITE_FIELD_MATERIAL(Vector3d(0.8, 0.8, 0.8), 0.8, 0.0, 0.0, 4.0);
Material DEFAULT_BLACK_FIELD_MATERIAL(Vector3d(0.1, 0.1, 0.1), 0.5, 0.0, 0.0, 4.0);
Material DEFAULT_WHITE_PIECE_MATERIAL(Vector3d(0.88, 0.88, 0.88), 0.6, 0.0, 0.0, 16.0);
Material DEFAULT_BLACK_PIECE_MATERIAL(Vector3d(0.2, 0.2, 0.2), 0.6, 0.0, 0.0, 16.0);	

class ModelChess : public Model
{
public:
	enum chessModelObjects {		
		NO_PIECE = -1,

		PAWN_1_W = 0,	PAWN_2_W,	PAWN_3_W,	PAWN_4_W,	PAWN_5_W,	PAWN_6_W,	PAWN_7_W,	PAWN_8_W,
		ROOK_1_W,		KNIGHT_1_W,	BISHOP_1_W, QUEEN_W,	KING_W,		BISHOP_2_W,	KNIGHT_2_W,	ROOK_2_W,

		PAWN_1_B,		PAWN_2_B,	PAWN_3_B,	PAWN_4_B,	PAWN_5_B,	PAWN_6_B,	PAWN_7_B,	PAWN_8_B,
		ROOK_1_B,		KNIGHT_1_B,	BISHOP_1_B, KING_B,		QUEEN_B,	BISHOP_2_B,	KNIGHT_2_B,	ROOK_2_B,
				
		CHESSBOARD_W, CHESSBOARD_B
	};
	
	static const unsigned CHESS_MODEL_OBJECTS_COUNT;
	static const unsigned CHESS_PIECES_COUNT;
	static const char* modelObjectNames[];	

	//! x,y chess board coordinates of the piece [<0;7>, <0;7>]
	struct chessBoardCoords {	
		chessBoardCoords(int x, int y) : x(x), y(y) { }
		int x;
		int y;		
	};

	ModelChess(string fileName) : fieldWidth(0.0) { 
		objects_ = vector<Object>(ModelChess::CHESS_MODEL_OBJECTS_COUNT, Object());

		matChessboardW = &DEFAULT_WHITE_FIELD_MATERIAL;
		matChessboardB = &DEFAULT_BLACK_FIELD_MATERIAL;
		matPieceW = &DEFAULT_WHITE_PIECE_MATERIAL;
		matPieceB = &DEFAULT_BLACK_PIECE_MATERIAL;

		// load model
		load(fileName);

		// create bounding box for each object
		for(int i = 0; i < (int)objects_.size(); i++) {
			objects_.at(i).createBoundingBox();
		}
	}

	~ModelChess() { }

	//! Loads chess model from the OBJ file with specific format (see class description).
	virtual void load(string fileName);		

	//! Moves the given piece from the given position to the new position in Z plane
	void move(ModelChess::chessModelObjects piece, chessBoardCoords& from, chessBoardCoords& to);

	//! object visibility getter
	bool getVisibility(chessModelObjects piece) { return objects_.at(piece).visible; }

	//! object visibility setter
	void setVisibility(chessModelObjects piece, bool visible) { objects_.at(piece).visible = visible; }

	Material* matChessboardW; // chessboards' white fields material
	Material* matChessboardB; // chessboards' black fields material
	Material* matPieceW;	  // white pieces material
	Material* matPieceB;	  // black pieces material

	void setObjectMaterial(chessModelObjects obj, Material* m) {  
		for(int i = 0; i < (int)objects_.at(obj).shapes.size(); i++)
			objects_.at(obj).shapes.at(i)->mat_ = m;		
	}

private:
	double fieldWidth;
	
	//! Calculates the chessboard field width in loaded model
	double calculateFieldWidth();	
};

const unsigned ModelChess::CHESS_MODEL_OBJECTS_COUNT = 34;
const unsigned ModelChess::CHESS_PIECES_COUNT = 32;
const char* ModelChess::modelObjectNames[] = {	
	"pawn_1_w",	"pawn_2_w",		"pawn_3_w",		"pawn_4_w", "pawn_5_w", "pawn_6_w",		"pawn_7_w",		"pawn_8_w",
	"rook_1_w", "knight_1_w",	"bishop_1_w",	"queen_w",	"king_w",	"bishop_2_w",	"knight_2_w",	"rook_2_w",
	
	"pawn_1_b",	"pawn_2_b",		"pawn_3_b",		"pawn_4_b", "pawn_5_b", "pawn_6_b",		"pawn_7_b",		"pawn_8_b",
	"rook_1_b", "knight_1_b",	"bishop_1_b",	"king_b",	"queen_b",	"bishop_2_b",	"knight_2_b",	"rook_2_b",
	 			
	"chessboard_w", "chessboard_b",
};

inline void ModelChess::move(ModelChess::chessModelObjects piece, chessBoardCoords& from, chessBoardCoords& to)
{
	Vector3d t((to.x - from.x) * fieldWidth, (to.y - from.y) * fieldWidth, 0.0);	
	objects_.at(piece).translate(t);
}

inline void ModelChess::load(string fileName)
{
	//debug
	cout << "Loading model " << fileName << "..." << endl;

	vector<Vector3d> vertices;
	vector<Vector3d> normals;		

	ifstream file(fileName);	
	if(file.fail()) {
		cerr << "ERROR: The file " << fileName << " cannot be opened." << endl;
		exit(1);
	}	

	int modelObject = 0;
	Material* m = NULL;
	string line;
	while(getline(file, line)) {				
		// object 'o name_[n_]_{b|w}'
		if(line[0] == 'o' && isspace(line[1])) {				
			char buf[100];
			sscanf(line.c_str(), "%*s %s", buf);
			string objName(buf);

			// find which object we are reading
			for(int i = 0; i < (int)ModelChess::CHESS_MODEL_OBJECTS_COUNT; i++) {
				if(objName.find(ModelChess::modelObjectNames[i]) != string::npos) {					
					modelObject = i;					
					
					// choose material
					if     (modelObject < (int)(ModelChess::CHESS_MODEL_OBJECTS_COUNT - 2) / 2)	m = matPieceW;
					else if(modelObject < (int)(ModelChess::CHESS_MODEL_OBJECTS_COUNT - 2))		m = matPieceB; 
					else if(modelObject < (int)(ModelChess::CHESS_MODEL_OBJECTS_COUNT - 1))		m = matChessboardW;
					else																	m = matChessboardB;
				}
			}
		}
		// vertex 'v num1 num2 num3' 
		else if(line[0] == 'v' && isspace(line[1])) {
			double x, y, z;					
			sscanf(line.c_str(), "%*s %lf %lf %lf", &x, &y, &z);
			vertices.push_back(Vector3d(x, y, z));

		// normal 'vn num1 num2 num3'
		} else if(line[0] == 'v' && line[1] == 'n' && isspace(line[2])) {
			normals.push_back(Vector3d());
			sscanf(line.c_str(), "%*s %lf %lf %lf", &normals.back().x_, &normals.back().y_, &normals.back().z_);

		// face 'f v1//vn1 v2//vn2 v3//vn3'
		} else if(line[0] == 'f' && isspace(line[1])) {
			unsigned iv1, iv2, iv3;
			unsigned in1, in2, in3;

			sscanf(line.c_str(), "%*s %u//%u %u//%u %u//%u", &iv1, &in1, &iv2, &in2, &iv3, &in3);
			objects_.at(modelObject).shapes.push_back(new Triangle(vertices.at(iv1 - 1), vertices.at(iv2 - 1), vertices.at(iv3 - 1),
										  normals.at(in1 - 1),  normals.at(in2 - 1),  normals.at(in3 - 1), m));
		}
	}	

	// Check if all models were loaded
	for(int i = 0; i < (int)objects_.size(); i++)
		if(objects_.at(i).shapes.size() == 0) {			
			cerr << "ERROR: Object " << ModelChess::modelObjectNames[i] << " missing" << endl;
			exit(1);
		}	

	// estimate chessboard field width
	fieldWidth = calculateFieldWidth();		
}

double ModelChess::calculateFieldWidth()
{	
	double xMin = INFINITY;
	double xMax = -INFINITY;

	for(int i = 0; i < (int)objects_.at(CHESSBOARD_W).shapes.size(); i++) {
		Vector3d x(	static_cast<Triangle *>(objects_.at(CHESSBOARD_W).shapes.at(i))->v0.x_,
					static_cast<Triangle *>(objects_.at(CHESSBOARD_W).shapes.at(i))->v1.x_,
					static_cast<Triangle *>(objects_.at(CHESSBOARD_W).shapes.at(i))->v2.x_);
		double xMinCurrent = x.min();
		double xMaxCurrent = x.max();
		if(xMinCurrent < xMin) xMin = xMinCurrent;
		if(xMaxCurrent > xMax) xMax = xMaxCurrent;
	}
	
	assert(xMin < xMax);

	return (xMax - xMin) / 8.0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//// CHESSS

const int HORIZONTAL_FIELDS = 8;

//! Class implements the chessboard state (configuration)
class Chess
{
public:	
	typedef ModelChess::chessModelObjects chessPieces;				
	
	Chess(string modelFile, string configChessboardFile, string configRTFile) { 
		chessModel = new ModelChess(modelFile);	
		configureChessboard(configChessboardFile);				
	}

	~Chess() { 
		//delete chessModel;  // SEGFAULT!!!
	}	

	ModelChess* getModel() { return chessModel; }

	//! Moves the given piece to the new chessboard position.
	/*! If the piece is not present on the chessboard, it is noop.
	*/
	void move(chessPieces piece, ModelChess::chessBoardCoords to);	

	void setWhitePieceMaterial(Material* m) 
	{
		for(int i = 0; i < 16; i++)
			chessModel->setObjectMaterial((ModelChess::chessModelObjects)i, m);		
	}
	
	void setBlackPieceMaterial(Material* m) 
	{
		for(int i = 16; i < 32; i++)
			chessModel->setObjectMaterial((ModelChess::chessModelObjects)i, m);
	}
	
	void setWhiteFieldMaterial(Material* m) 
	{
		chessModel->setObjectMaterial((ModelChess::chessModelObjects)32, m);
	}	
	
	void setBlackFieldMaterial(Material* m)
	{
		chessModel->setObjectMaterial((ModelChess::chessModelObjects)33, m);
	}

private:
	//vector<ModelChess::chessBoardCoords> pieces;	// pieces' chessboard coordinates [<0;7>, <0;7>]
	chessPieces chessBoard[HORIZONTAL_FIELDS][HORIZONTAL_FIELDS]; // 8x8 chessboard
	ModelChess* chessModel;

	//! Sets all fields of the chessboard to NO_PIECE
	void initChessboard();

	//! Initializes default positions of pieces on chessboard (assumes standard configuration at new game)
	void initPieces();

	//! Returns coordinates of the piece on the chessboard
	/*! @return pieceCoords coordinates of the piece. If not found, {-1, -1} is returned.
	*/
	ModelChess::chessBoardCoords pieceCoords(chessPieces piece);

	//! Loads the configuration file and sets the pieces' positions accordingly
	void configureChessboard(string fileName);

	/*! Converts standard chess coordinates (e.g. F3) to C [y][x] field coords.
		example:
			A1 == [0][0]
			C5 == [4][2]
	*/
	ModelChess::chessBoardCoords coordsChess2Carray(char letter, char number);

	//! Returns the default coordinates of the given piece
	ModelChess::chessBoardCoords pieceDefaultCoords(chessPieces piece);
};

void Chess::move(chessPieces piece, ModelChess::chessBoardCoords to)
{
	/*cout << "want to: x: " << to.x << ", y: " << to.y << endl;
	cout << "at: " << (int)chessBoard[to.y][to.x] << endl;*/

	ModelChess::chessBoardCoords from = pieceCoords(piece);
	
	if(from.x == to.x && from.y == to.y) return;

	// check if there is not some piece placed already
	if(chessBoard[to.y][to.x] == chessPieces::NO_PIECE) {		
		// debug
		/*cout << "from: [" << from.x << ", " << from.y << "]" <<  endl;
		cout << "to:   [" << to.x   << ", " << to.y   << "]" <<  endl;*/

		chessBoard[from.y][from.x] = chessPieces::NO_PIECE;
		chessBoard[to.y][to.x] = piece;

		// move the actual model
		if(from.x != -1 && from.y != -1)
			chessModel->move(piece, from, to);
	}
}

void Chess::initPieces()
{
	// white pieces
	for(int i = 0; i < 2; i++)
			for(int j = 0; j < HORIZONTAL_FIELDS; j++)
				chessBoard[i][j] = (chessPieces)((1 - i) * HORIZONTAL_FIELDS + j);
	// the middle of the chessboard
	for(int i = 2; i < 6; i++)
		for(int j = 0; j < 8; j++)
			chessBoard[i][j] = chessPieces::NO_PIECE;
	// black pieces
	for(int i = 6; i < 8; i++)
		for(int j = 0; j < HORIZONTAL_FIELDS; j++)
			chessBoard[i][j] = (chessPieces)((i - 4) * HORIZONTAL_FIELDS + (HORIZONTAL_FIELDS - 1 - j));

	//// debug print out chessboard
	//for(int i = 0; i < HORIZONTAL_FIELDS; i++) {
	//	for(int j = 0; j < HORIZONTAL_FIELDS; j++) {
	//		cout << (int)chessBoard[i][j] << " ";
	//	}
	//	cout << endl;
	//}
}

ModelChess::chessBoardCoords Chess::pieceCoords(chessPieces piece)
{
	ModelChess::chessBoardCoords coords(-1, -1);

	for(int i = 0; i < HORIZONTAL_FIELDS; i++)
		for(int j = 0; j < HORIZONTAL_FIELDS; j++)
			if(chessBoard[i][j] == piece) {
				coords.x = j;
				coords.y = i;				
			}

	return coords;
}

ModelChess::chessBoardCoords Chess::pieceDefaultCoords(chessPieces piece)
{
	if((int)piece < 8) {
		return ModelChess::chessBoardCoords((int)piece, 1);
	} else if((int)piece < 16) {
		return ModelChess::chessBoardCoords((int)piece - 8, 0);
	} else if((int)piece < 24) {
		return ModelChess::chessBoardCoords(24 - (int)piece - 1, 6);
	} else {
		return ModelChess::chessBoardCoords(32 - (int)piece - 1, 7);
	}
}

void Chess::initChessboard()
{
	for(int i = 0; i < HORIZONTAL_FIELDS; i++)
		for(int j = 0; j < HORIZONTAL_FIELDS; j++)
			chessBoard[i][j] = ModelChess::NO_PIECE;
}

ModelChess::chessBoardCoords Chess::coordsChess2Carray(char letter, char number)
{
	if(!(letter >= 'A' && letter <= 'H' && number >= '1' && number <= '8')) {
		cerr << "ERROR: wrong piece position." << endl;
		return ModelChess::chessBoardCoords(-1, -1);
	}
	
	return ModelChess::chessBoardCoords((int)(letter - 'A'), (int)(number - '1'));
}

void Chess::configureChessboard(string fileName)
{
	initChessboard();
	
	//debug
	cout << "Loading configuration file " << fileName << "..." << endl;		

	ifstream file(fileName);	
	if(file.fail()) {
		cerr << "ERROR: The file " << fileName << " cannot be opened." << endl;
		exit(1);
	}	
			
	string line;
	char pieceBuf[20];
	char positionBuf[5];	
	int lineNum = 0;
	while(getline(file, line)) {
		//cout << "line: " << line << endl;
		lineNum++;
		if(line[0] == '#') continue;	// commentary
		else {
			sscanf(line.c_str(), "%s %s", pieceBuf, positionBuf);
			string piece(pieceBuf);
			string position(positionBuf);

			// find which object we are reading
			for(int i = 0; i < (int)ModelChess::CHESS_PIECES_COUNT; i++) {
				if(piece.find(ModelChess::modelObjectNames[i]) != string::npos) {				
					//cout << "found at " << i << ", line " << lineNum << endl;
					ModelChess::chessModelObjects modelObject = (ModelChess::chessModelObjects)i;
					//cout << "object: " << (int)modelObject << endl;
					ModelChess::chessBoardCoords to = coordsChess2Carray(position[0], position[1]);
					//cout << "to: " << position[0] << position[1] << " = " << "[" << to.y << "][" << to.x << "]" << endl;
					ModelChess::chessBoardCoords from = pieceDefaultCoords(modelObject);
					//cout << "from: " << "[" << from.y << "][" << from.x << "]" << endl;
					chessBoard[to.y][to.x] = modelObject;
					chessModel->move(modelObject, from, to);
				}
			}
		}
		
		pieceBuf[0] = '\0';
		positionBuf[0] = '\0';
	}

	// check which pieces were not set at all and set their visibility to false
	for(int i = 0; i < (int)ModelChess::CHESS_PIECES_COUNT; i++) {
		if(pieceCoords((Chess::chessPieces)i).x == -1)
			chessModel->setVisibility((Chess::chessPieces)i, false);
	}
}

#endif