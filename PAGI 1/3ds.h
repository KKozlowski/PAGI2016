#ifndef _3DSS
#define _3DSS

#define ROOT       0x4D4D							
	#define SCENEINFO    0x3D3D							

		#define MATERIAL			0xAFFF		
			#define MAT_NAME        0xA000				// Nazwa materia³u
			#define DIFFUSE_COLOR   0xA020				// Kolor materia³u
			#define TEXTURE_MAP     0xA200				// Nag³ówek nowego materia³u
				#define MAP_FILENAME	0xA300			// Œcie¿ka do tekstury

		#define OBJECT						0x4000		
			#define MESH_HEADER				0x4100				
				#define OBJECT_VERTICES     0x4110			
				#define OBJECT_FACES		0x4120			
					#define OBJECT_MATERIAL	0x4130		
				#define OBJECT_UV			0x4140		


struct Chunk
{
	unsigned short int ID;				
	unsigned int length;					// d³ugoœæ
	size_t progress;						// bajty przeczytane do tej pory
};

class SceneLoader
{
public:
	SceneLoader();			
	~SceneLoader();

	bool Import3DS(Scene *pModel, char *strFileName);

private:
	int NextString(char *);
	int NextBin(void *buff, int length);
	void RestBin(void *buff, Chunk &current);
	Chunk NextChunkInfo();
	void NextChunk(Scene *scene, Chunk *);
	void NextObject(Scene *scene, Object3DS *pObject, Chunk *);
	void NextMaterial(Scene *scene, Chunk *);

	size_t ignore(int length);

	FILE *file;
};

#endif

/*
0x4D4D // Main Chunk
|- 0x3D3D // 3D Editor Chunk
|  |- 0x4000 // Object Block
|  |  |- 0x4100 // Triangular Mesh
|  |  |  |- 0x4110 // Vertices List
|  |  |  |- 0x4120 // Faces Description
|  |  |  |  |- 0x4130 // Faces Material
|  |  |  |  |- 0x4150 // Smoothing Group List
|  |  |  |- 0x4140 // Mapping Coordinates List
|  |  |  |- 0x4160 // Local Coordinates System
|  |  |- 0x4600 // Light
|  |  |  |- 0x4610 // Spotlight
|  |  |- 0x4700 // Camera
|  |- 0xAFFF // Material Block
|     |- 0xA000 // Material Name
|     |- 0xA010 // Ambient Color
|     |- 0xA020 // Diffuse Color
|     |- 0xA030 // Specular Color
|     |- 0xA200 // Texture Map 1
|     |  |- 0xA300 // Mapping Filename
|     |  |- 0xA351 // Mapping Parameters
|     |- 0xA230 // Bump Map
|     |  |- 0xA300 // Mapping Filename
|     |  |- 0xA351 // Mapping Parameters
|     |- 0xA220 // Reflection Map
|        |- 0xA300 // Mapping Filename
|        |- 0xA351 // Mapping Parameters
|- 0xB000 // Keyframer Chunk
|- 0xB002 // Mesh Information Block
|   |- 0xB010 // Object Name
|   |- 0xB013 // Object Pivot Point
|   |- 0xB020 // Position Track
|   |- 0xB021 // Rotation Track
|   |- 0xB022 // Scale Track
|   |-0 xB030 // Hierarchy Position
|- 0xB007 // Spot Light Information Block
|- 0xB008 // Frames (Start and End)

-- //pl.wikipedia.org/wiki/3ds  */