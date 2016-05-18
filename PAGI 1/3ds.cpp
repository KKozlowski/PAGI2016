#include "main.h"
#include "3ds.h"
#include <assert.h>

int sonda[50000] = {0};

SceneLoader::SceneLoader(){
	file = NULL;
}

bool SceneLoader::Import3DS(Scene *scene, char *path){
	file = fopen(path, "rb");

	if(file == NULL) { //Czy znaleziono
		return false;
	}

	Chunk root = NextChunkInfo();

	if (root.ID != ROOT){ //Czy to plik 3DS.
		return false;
	}

	NextChunk(scene, &root);

	scene->ComputeNormals();

	return true;
}


SceneLoader::~SceneLoader(){
	if (file) {
		fclose(file);
	}
}

void SceneLoader::NextChunk(Scene *scene, Chunk *overchunk){
	Chunk current = { 0 };
	
	while (overchunk->progress < overchunk->length){

		current = NextChunkInfo();

		switch (current.ID)
		{
		case SCENEINFO:	{	
			Chunk temp = NextChunkInfo();
			printSTRINT("SCENEVERSION", temp.length);

			//Nic nas tu nie interesuje
			temp.progress += ignore( temp.length - temp.progress);
			current.progress += temp.progress;

			// Przechodzimy dalej
			break;
		}
		case MATERIAL:				
			printSTRINT("==MATERIAL==", current.length);
			scene->materials.push_back(Material());
			NextMaterial(scene, &current);

			break;

		case OBJECT:
			printSTRINT("==OBJECT==", current.length);
			scene->objects.push_back(Object3DS());
			current.progress += NextString(scene->objects[scene->objects.size() - 1].name);

			NextObject(scene, &(scene->objects[scene->objects.size() - 1]), &current);
			break;

		default: 
			current.progress += ignore(current.length - current.progress);
			break;
		}
		overchunk->progress += current.progress;
	}
}


void SceneLoader::NextObject(Scene *scene, Object3DS *objectt, Chunk *overchunk){
	Chunk temp = {0};

	// Czytamy kolejne podchunki a¿ skoñczy siê ten, który podano w argmencie.
	while (overchunk->progress < overchunk->length)
	{
		temp = NextChunkInfo();

		switch (temp.ID)
		{
		case MESH_HEADER:
			printSTRINT("MESH_START", temp.length);
			//Przechodzimy dalej
			break;

		case OBJECT_VERTICES:	
			printSTRINT("VERTICES", temp.length);
			objectt->ReadVertices(&temp, file);
			break;

		case OBJECT_FACES:		
			printSTRINT("TRIANGLES", temp.length);
			objectt->ReadVertexIndexes(&temp, file);
			break;

		case OBJECT_MATERIAL: {	
			printSTRINT("OBJECT_MATERIAL", temp.length);
			char objectMaterialName[255] = { 0 }; 	
			temp.progress += NextString(objectMaterialName);

			objectt->AssignMaterialByName(scene, objectMaterialName);

			temp.progress += ignore(temp.length - temp.progress); //Reszta danych o materiale nas nie interesuje.
			break;
		}
		case OBJECT_UV:	
			printSTRINT("OBJECT_UV", temp.length);
			objectt->ReadUVCoordinates(&temp, file);
			break;

		default:  
			temp.progress += ignore(temp.length - temp.progress);
			break;
		}

		overchunk->progress += temp.progress;
	}
}

void SceneLoader::NextMaterial(Scene *scene, Chunk *overchunk){
	Chunk current = {0};

	while (overchunk->progress < overchunk->length){
		current = NextChunkInfo();

		Material *mat = &(scene->materials[scene->materials.size() - 1]);

		switch (current.ID)
		{
		case MAT_NAME:
			printSTRINT("MAT_NAME", current.length);
			RestBin(mat->name, current);
			break;

		case DIFFUSE_COLOR: {	
			printSTRINT("DIFFUSE", current.length);
			Chunk temp = NextChunkInfo();
			RestBin(mat->color, temp); //Trzy bajty z wartoœciamin 0-255m RGB.

			current.progress += temp.progress;
			break;
		}
		
		case TEXTURE_MAP:
			printSTRINT("TEXTURE_MAP", current.length);
			break;		

		case MAP_FILENAME:
			printSTRINT("MAP_FILENAME", current.length);
			RestBin(mat->filePath, current);
			break;
		
		default:  
			current.progress += ignore(current.length - current.progress);
			break;
		}
		overchunk->progress += current.progress;
	}
}

Chunk SceneLoader::NextChunkInfo() {
	Chunk chunk;
	chunk.progress = fread(&chunk.ID, 1, 2, file);
	chunk.progress += fread(&chunk.length, 1, 4, file);
	printSTRINT("CHUNK", chunk.length);
	return chunk;
}

int SceneLoader::NextString(char *buff){
	int index = 0;

	fread(buff, 1, 1, file);

	while (*(buff + index++) != NULL) {
		fread(buff + index, 1, 1, file);
	}

	return (int)strlen(buff) + 1;
}

int SceneLoader::NextBin(void *buff, int length) {
	return fread(buff, 1, length, file);
}

void SceneLoader::RestBin(void *buff, Chunk &current) {
	current.progress += NextBin(buff, current.length - current.progress);
}

size_t SceneLoader::ignore(int length) {
	return NextBin(sonda, length);
}