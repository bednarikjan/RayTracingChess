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
	static const char* modelObjectNames[];	

	//! x,y chess board coordinates of the piece [<0;7>, <0;7>]
	struct chessBoardCoords {	
		chessBoardCoords(int x, int y) : x(x), y(y) { }
		int x;
		int y;		
	};

	ModelChess(string fileName) : fieldWidth(0.0) { 
		objects_ = vector<Object>(ModelChess::CHESS_MODEL_OBJECTS_COUNT, Object());

		matChessboardW = new Material(Vector3d(0.9, 0.9, 0.9), 0.85, 0.0, 0.0, 4.0);
		matChessboardB = new Material(Vector3d(0.1, 0.1, 0.1), 0.85, 0.0, 0.0, 4.0);
		matPieceW = new Material(Vector3d(0.88, 0.88, 0.66), 0.3, 0.0, 0.0, 16.0);
		matPieceB = new Material(Vector3d(0.32, 0.2, 0.01), 0.3, 0.0, 0.0, 16.0);		

		// load model
		load(fileName);

		// create bounding box for each object
		for(int i = 0; i < (int)objects_.size(); i++)
			createBoundingBox(i);
	}

	~ModelChess() { 
		delete matChessboardW;
		delete matChessboardB;
		delete matPieceW;
		delete matPieceB;
	}

	//! Loads chess model from the OBJ file with specific format (see class description).
	virtual void load(string fileName);		

	//! Moves the given piece from the given position to the new position in Z plane
	void move(ModelChess::chessModelObjects piece, chessBoardCoords& from, chessBoardCoords& to);

	Material* matChessboardW; // chessboards' white fields material
	Material* matChessboardB; // chessboards' black fields material
	Material* matPieceW;	  // white pieces material
	Material* matPieceB;	  // black pieces material

private:
	double fieldWidth;
	
	//! Calculates the chessboard field width in loaded model
	double calculateFieldWidth();	
};

const unsigned ModelChess::CHESS_MODEL_OBJECTS_COUNT = 34;
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

	for(int i = 0; i < (int)objects_.at(piece).shapes.size(); i++) {
		static_cast<Triangle *>(objects_.at(piece).shapes.at(i))->v0 += t; 
		static_cast<Triangle *>(objects_.at(piece).shapes.at(i))->v1 += t;
		static_cast<Triangle *>(objects_.at(piece).shapes.at(i))->v2 += t;
	}
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
			unsigned idx = string::npos;
			char buf[100];
			sscanf(line.c_str(), "%*s %s", buf);
			string objName(buf);

			// find which object we are reading
			for(int i = 0; i < (int)ModelChess::CHESS_MODEL_OBJECTS_COUNT; i++) {
				if((idx = objName.find(ModelChess::modelObjectNames[i])) != string::npos) {
					//if(objName.find("_w") != string::npos)
						modelObject = i;
					//else if(objName.find("_b") != string::npos)
						//modelObject = (ModelChess::CHESS_MODEL_OBJECTS / 2 + i);
					
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

const int HORIZONTAL_FIELDS = 8;

//! Class implements the chessboard state (configuration)
class Chess
{
public:	
	typedef ModelChess::chessModelObjects chessPieces;				
	
	Chess(string modelFileName) { 
		chessModel = new ModelChess(modelFileName);	

		// init configuration of chessboard
		initPieces();

		//// debug - try moving some pieces
		//move(chessPieces::QUEEN_W, ModelChess::chessBoardCoords(7, 4));
		//move(chessPieces::PAWN_5_B, ModelChess::chessBoardCoords(3, 5));
		//move(chessPieces::QUEEN_B, ModelChess::chessBoardCoords(0, 5));
	}

	~Chess() { 
		//delete chessModel;  // SEGFAULT!!!
	}	

	ModelChess* getModel() { return chessModel; }

	//! Moves the given piece to the new chessboard position.
	/*! If the piece is not present on the chessboard, it is noop.
	*/
	void move(chessPieces piece, ModelChess::chessBoardCoords to);

private:
	//vector<ModelChess::chessBoardCoords> pieces;	// pieces' chessboard coordinates [<0;7>, <0;7>]
	chessPieces chessBoard[HORIZONTAL_FIELDS][HORIZONTAL_FIELDS]; // 8x8 chessboard
	ModelChess* chessModel;

	//! Initializes default positions of pieces on chessboard (assumes standard configuration at new game)
	void initPieces();

	//! Returns coordinates of the piece on the chessboard
	/*! @return pieceCoords coordinates of the piece. If not found, {-1, -1} is returned.
	*/
	ModelChess::chessBoardCoords pieceCoords(chessPieces piece);
};

void Chess::move(chessPieces piece, ModelChess::chessBoardCoords to)
{
	/*cout << "want to: x: " << to.x << ", y: " << to.y << endl;
	cout << "at: " << (int)chessBoard[to.y][to.x] << endl;*/

	// check if there is not some piece placed already
	if(chessBoard[to.y][to.x] == chessPieces::NO_PIECE) {
		ModelChess::chessBoardCoords from = pieceCoords(piece);

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

#endif