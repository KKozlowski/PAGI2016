#pragma once

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
class TransformInfo;

void printINT(int value);

void printSTRINT(char * s, int value);

void printFLOAT(float value);

void addPos(float x, float y, float z);
void addRot(float x, float y, float z);

class Color {
public:
	BYTE r;
	BYTE g;
	BYTE b;

	Color() {
		r = g = b = 0;
	}

	Color(BYTE red, BYTE green, BYTE blue) {
		r = red;
		g = green;
		b = blue;
	}

	Color(vector< unsigned char > vec) {
		r = vec[0];
		g = vec[1];
		b = vec[2];
	}

	bool operator<(const Color &two) const {
		return r < two.r && g < two.g && b < two.b;
	}

	bool operator==(const Color &two) const {
		return r == two.r && g == two.g && b == two.b;
	}
};

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

	Vector3 operator *(const float & v) const {
		return Vector3(x * v, y * v, z * v);
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

	float Dot(Vector3 vec)
	{
		return x*vec.x + y*vec.y + z*vec.z;
	}

	static Vector3 Cross(Vector3 one, Vector3 two){
		return one.Cross(two);
	}

	string to_string() {
		std::ostringstream ss;
		ss << "Vector3 + { " << x << ", " << y << ", " << z << " }";
		return ss.str();
	}

	static Vector3 const zero;
	static Vector3 const one;

	static Vector3 ToEuler(double x, double y, double z, double angle)
	{
		Vector3 result;
		double s = sin(angle);
		double c = cos(angle);
		double t = 1 - c;

		if ((x*y*t + z*s) > 0.998)
		{
			result.x = 2 * atan2(x*sin(angle / 2), cos(angle / 2));
			result.y = 3.14159 / 2;
			result.z = 0;
			return result;
		}
		if ((x*y*t + z*s) < -0.998)
		{
			result.x = -2 * atan2(x*sin(angle / 2), cos(angle / 2));
			result.y = -3.14159 / 2;
			result.z = 0;
			return result;
		}

		result.x = atan2(y * s - x * z * t, 1 - (y*y + z*z) * t);
		result.y = asin(x * y * t + z * s);
		result.z = atan2(x * s - y * z * t, 1 - (x*x + z*z) * t);
		return result;
	}

	static Vector3 ToEuler(float *q) {
		return ToEuler(q[0], q[1], q[2], q[3]);
	}
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

class TransformInfo {
public:
	Vector3 position; //przeksztalcenia usera;
	Vector3 offset_deprecated; //local position;
	Vector3 rotation; //local rotation - you modify it
	Vector3 pivot;
	int16_t parent = -1;
	vector<int16_t> *children = new vector<int16_t>();

	Vector3 self_position_without_rotation() const
	{
		return pivot + position;
	}
};

class Object3DS {
public:
	char name[255];

	bool hasTexture;
	int  materialId = -1;

	int  vertexCount;		
	int  triangleCount;			
	int  uvCount;	

	bool hidden = false;

	Vector3  *vertices;		// Tablica werteksów obiektu
	Vector3  *normals;		// Tablica normalnych obiektu
	Vector2  *uvCoords;		// Tablica wspó³rzêdnych UV
	Triangle *triangles;	// Trójk¹ty obiektu

	void ReadVertices(Chunk *, FILE *);
	void ReadVertexIndexes(Chunk *, FILE *);
	void ReadUVCoordinates(Chunk *, FILE *);
	void AssignMaterialByName(Scene *scene, char *name);
	void Draw();

	void DrawColor();

	Color color;
	void AssignColor();
	TransformInfo transform;
	vector<int16_t> *children = new vector<int16_t>();
	

	static Object3DS *selected;
};



struct Scene {
	vector<Material> materials;	
	vector<Object3DS> objects;	
	void ComputeNormals();

	Object3DS *selectObjectByColor(Color c);
	Object3DS *getObjectByName(string s);
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

void ApplyTransformations2(TransformInfo &actor);

void printSTR(string str);
void printINT(int value);
void printSTR(char * s);
void printSTRINT(char * s, int value);

void createFramebuffer();

