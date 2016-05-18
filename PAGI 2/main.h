#ifndef MAINE
#define MAINE

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
//#include <GL\glew.h>		
#include <GL\gl.h>	
#include <gl\glu.h>
#include <tchar.h>
#include <sstream>

using namespace std;

#define SCREEN_WIDTH 1280								
#define SCREEN_HEIGHT 720								
#define COLOR_DEPTH 16					

struct Chunk;
class Scene;

void printINT(int value);

void printSTRINT(char * s, int value);

///<summary>
///Przechowuje informacje o punkcie w trójwymiarowym uk³adzie odniesienia.
///Wa¿na sprawa - sk³ada siê tylko z trzech liczb float, wiêc mo¿na wczytywaæ do niego informacje przez fread prosto z pliku 3ds.
///</summary>
class Vector3 
{
public:
	Vector3() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float x, y, z;
	void Normalize() {
		float Magnitude;				

		Magnitude = Mag();				

		x /= Magnitude;			
		y /= Magnitude;			
		z /= Magnitude;			
	}

	float Mag() {
		return(sqrt(x*x + y*y + z*z));
	}

	Vector3 operator /(const float & v) const{
		return Vector3(x / v, y / v, z / v);
	}

	Vector3 operator +(const Vector3 & v) const {
		return Vector3(x +v.x, y +v.y, z +v.z);
	}

	Vector3 operator -(const Vector3 & v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	 Vector3 Cross(Vector3 vec) {
		return Vector3(
			((y * vec.z) - (z * vec.y)),
			((z * vec.x) - (x * vec.z)),
			((x * vec.y) - (y * vec.x))
			);
	}

	static Vector3 Cross(Vector3 one, Vector3 two){
		return one.Cross(two);
	}

	static Vector3 const zero;
	static Vector3 const one;
};

class Vector2 
{
public:
	float x, y;
};

///<summary>Przechowuje indeksy pól w tablicach wierzcho³ków i koordynatów UV, które dotycz¹ danego trójk¹ta.</summary>
class Triangle
{
public:
	int vertexIndexes[3];	
	int uvIndexes[3];		

	bool containsVert(int i) {
		return vertexIndexes[0] == i ||vertexIndexes[1] == i ||vertexIndexes[2] == i;
	}

	bool containsCoord(int i) {
		return uvIndexes[0] == i || uvIndexes[1] == i || uvIndexes[2] == i;
	}
};


///<summary>Najbardziej podstawowe informacje o materiale</summary>
class Material {
public:
	char  name[255];		
	char  filePath[255];	

	BYTE  color[3];			
	int   texureId;
} ;

class Object3DS {
public:
	char name[255];

	bool hasTexture;
	int  materialId = -1;

	int  vertexCount;		
	int  triangleCount;			
	int  uvCount;			

	Vector3  *vertices;		// Tablica werteksów obiektu
	Vector3  *normals;		// Tablica normalnych obiektu
	Vector2  *uvCoords;		// Tablica wspó³rzêdnych UV
	Triangle *triangles;	// Trójk¹ty obiektu

	void ReadVertices(Chunk *, FILE *);
	void ReadVertexIndexes(Chunk *, FILE *);
	void ReadUVCoordinates(Chunk *, FILE *);
	void AssignMaterialByName(Scene *scene, char *name);
	void Draw();
};

struct Scene {
	vector<Material> materials;	
	vector<Object3DS> objects;	
	void ComputeNormals();
};

///<summary>Funkcja "main()" w aplikacjach okienkowych GDI.</summary>
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int);
void Resize(int, int);
HWND GenerateWindow(LPSTR, int, int, HINSTANCE);
void InitializeWindow(HWND);
bool SetupPixelFormat(HDC);

void Display();

LRESULT CALLBACK WindowsMessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

WPARAM MainLoop();

void printINT(int value);
void printSTRINT(char * s, int value);

void createFramebuffer();

#endif 