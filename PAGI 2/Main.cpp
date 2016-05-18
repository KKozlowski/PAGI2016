#include "main.h"										
#include "3ds.h"										

#define FILE_NAME  "test.3ds"

Vector3 const Vector3::zero = Vector3(0, 0, 0);
Vector3 const Vector3::one = Vector3(1, 1, 1);

RECT  windowRekt;		
HDC   deviceContext;	

UINT textures[128] = { 0 };						

Scene scene;						

GLuint frameBuffer;
GLuint texColorBuffer;

bool drawLines = false;
bool  lightingEnabled = true;				

void printINT(int value) {
	TCHAR buf[100];
	int len = sprintf(buf, _T("%d\n"), value);
	OutputDebugString(buf);
}

void printSTRINT(char * s, int value) {
	TCHAR buf[100];
	int len = 0;
	len = sprintf(buf, _T("%s "), s);
	sprintf(buf + len, _T("%d\n"), value);
	OutputDebugString(buf);
}

bool CreateTexture(LPTSTR filePath, GLuint &textureID)  
{
	HBITMAP handle;                                          
	BITMAP bitmap;                                            

	glGenTextures(1, &textureID);               
	handle = (HBITMAP) LoadImage(GetModuleHandle(NULL), filePath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (!handle)         //Czy siê uda³o
		return false;    

	GetObject(handle, sizeof(bitmap), &bitmap);                           

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);                           

																	
	glBindTexture(GL_TEXTURE_2D, textureID);                         
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.bmWidth, bitmap.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap.bmBits);

	DeleteObject(handle); 

	return true;          
}

/////////////////
/////
/////	RZECZY ZWI¥ZANE Z SAMYM GDI
/////
/////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow) {
	

	HWND hWnd = GenerateWindow("PAG 1", SCREEN_WIDTH, SCREEN_HEIGHT, hInstance);
	if (hWnd == NULL) return 0;

	InitializeWindow(hWnd);

	glewInit();
	createFramebuffer();

	return (int)MainLoop();
}

HWND GenerateWindow(LPSTR strWindowName, int width, int height, HINSTANCE hInstance){
	HWND hWnd;
	WNDCLASS wndclass;
	memset(&wndclass, 0, sizeof(WNDCLASS));	

	wndclass.lpfnWndProc = WindowsMessageHandler;		
	wndclass.hInstance = hInstance;						
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszClassName = "Window";					

	RegisterClass(&wndclass);							
							
	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN; 

	RECT rekt;
	rekt.left = 0;						
	rekt.right = width;					
	rekt.top = 0;						
	rekt.bottom = height;				

	AdjustWindowRect(&rekt, dwStyle, false);	
	hWnd = CreateWindow("Window", strWindowName, dwStyle, 0, 0,
		rekt.right - rekt.left, rekt.bottom - rekt.top,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) return NULL;							

	ShowWindow(hWnd, SW_SHOWNORMAL);				
	UpdateWindow(hWnd);								

	SetFocus(hWnd);									

	return hWnd;
}

void InitializeWindow(HWND hWnd) {
	GetClientRect(hWnd, &windowRekt);				
	deviceContext = GetDC(hWnd);					

	SetupPixelFormat(deviceContext);

	HGLRC renderingContext = wglCreateContext(deviceContext);	
	wglMakeCurrent(deviceContext, renderingContext);			

	glEnable(GL_TEXTURE_2D);					
	glEnable(GL_DEPTH_TEST);					

	Resize(windowRekt.right, windowRekt.bottom);

	SceneLoader loader;

	loader.Import3DS(&scene, FILE_NAME);

	// IMPORTOWANIE TEKSTUR
	for (int i = 0; i < scene.materials.size(); i++) {				// Pêtla po wszystkich materia³ach w scenie
		if (strlen(scene.materials[i].filePath) > 0) {				// Czy materia³ zawiera nazwê pliku tekstury
			CreateTexture(scene.materials[i].filePath, textures[i]); // Przypisujemy info o teksturze do pola texturex[i] (referencja)			
		}

		scene.materials[i].texureId = i;			 // Ustawiamy materia³owi ID tekstury
	}

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

}

void Resize(int width, int height) {
	if (height == 0) height = 1;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.f, 10000.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool SetupPixelFormat(HDC hdc){
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	int pixelformat;

	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;					
	pfd.cColorBits = COLOR_DEPTH;					
	pfd.cDepthBits = COLOR_DEPTH;					
	
	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE || SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
		return false;

	return true;					
}

WPARAM MainLoop(){
	MSG msg;
	
	while(true)	{											
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT)			
				break;
            TranslateMessage(&msg);			
            DispatchMessage(&msg);			
        }
		else							
			Display();
	}

	return(msg.wParam);			
}

/////////////////
/////
/////	KONIEC GDI
/////
/////////////////

float posY = 3.f;
float posZ = 40.f;
float rotationX = 0.0f;
float rotationSpeed = 0.8f;

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glClearColor(0.5, 1, 1, 1);
	glLoadIdentity();									

	// origin, target, up
	gluLookAt(
		0, posY, posZ,
		0, posY, 0,
		0, 1, 0);

	glRotatef(rotationX, 0, 1.0f, 0);
	rotationX += rotationSpeed;		

	// RYSOWANIE
	for (int i = 0; i < scene.objects.size(); i++) {
		scene.objects[i].Draw();
	}

	SwapBuffers(deviceContext);	
}


LRESULT CALLBACK WindowsMessageHandler(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_SIZE) {			
		Resize(LOWORD(lParam), HIWORD(lParam));  
		GetClientRect(hWnd, &windowRekt);
	}
	else if (msg == WM_KEYDOWN){
		switch (wParam) {						
		case 0x53: //S
			posY -= 0.4f;
			break;

		case 0x57: //W
			posY += 0.4f;
			break;
		case VK_DOWN:
			posZ += 0.8f;
			break;
		case VK_UP:
			posZ -= 0.8f;
			break;
		case VK_SPACE:
			drawLines = !drawLines;
			break;
		case VK_RETURN:
			lightingEnabled = !lightingEnabled;

			if (lightingEnabled) {
				glEnable(GL_LIGHTING);
			}
			else {
				glDisable(GL_LIGHTING);
			}
			break;
		case VK_ESCAPE:							
			PostQuitMessage(0);					
			break;
		}
	}
	else if (msg == WM_CLOSE) {
		PostQuitMessage(0);
	} else return DefWindowProc(hWnd, msg, wParam, lParam);

	return 0;
}

void Object3DS::ReadVertices(Chunk *overchunk, FILE *file)
{
	// Czytamy liczbê wierzcho³ków
	overchunk->progress += fread(&(this->vertexCount), 1, 2, file);

	this->vertices = new Vector3[this->vertexCount];

	// Czytamy dane wierzcho³ków bajt po bajcie
	overchunk->progress += fread(this->vertices, 1, overchunk->length - overchunk->progress, file);

	//Zmieniamy uk³ad odniesienia z 3DS Maksowego na taki u¿ywany przez resztê œwiata.
	for (int i = 0; i < this->vertexCount; i++)
	{
		float fTempY = this->vertices[i].y;
		this->vertices[i].y = this->vertices[i].z;
		this->vertices[i].z = -fTempY;
	}
}


void Object3DS::ReadVertexIndexes(Chunk *overchunk, FILE *file) {
	overchunk->progress += fread(&triangleCount, 1, 2, file); //Ile trójk¹tów

	this->triangles = new Triangle[triangleCount];

	unsigned short index = 0;
	for (int i = 0; i < triangleCount; i++){
		for (int j = 0; j <= 3; j++){
			overchunk->progress += fread(&index, 1, sizeof(index), file);

			if (j !=3) { //Je¿eli to nie flaga widocznosci
				this->triangles[i].vertexIndexes[j] = index;
			}
		}
	}
}

void Object3DS::ReadUVCoordinates(Chunk *overchunk, FILE *file)
{
	overchunk->progress += fread(&this->uvCount, 1, 2, file); //Ile wspó³rzêdnych

	this->uvCoords = new Vector2[this->uvCount];
	memset(this->uvCoords, 0, uvCount*sizeof(Vector2 *)); //PóŸniej sprawdzimy czy cokolwiek tu wczytaliœmy przyrównuj¹c do zera.

	overchunk->progress += fread(this->uvCoords, 1, overchunk->length - overchunk->progress, file);
}

void Object3DS::AssignMaterialByName(Scene * scene, char *  objectMaterialName){
	//Szukamy materia³u o pasuj¹cej nazwie
	for (int i = 0; i < scene->materials.size(); i++) {
		if (strcmp(objectMaterialName, scene->materials[i].name) == 0) { //czy nazwa pasuje
			if (strlen(scene->materials[i].filePath) > 0) //czy materia³ na pewno ma info o pliku
				this->hasTexture = true;
			else
				this->hasTexture = false;

			this->materialId = i;

			break;
		}
	}
}

void Object3DS::Draw(){
	if (this->hasTexture) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[this->materialId]);
	}
	else {
		glDisable(GL_TEXTURE_2D);
	}

	glColor3ub(255, 255, 255);

	glBegin(GL_TRIANGLES);
	for (int j = 0; j < triangleCount; j++){
		for (int v = 0; v < 3; v++){
			int index = triangles[j].vertexIndexes[v];
			glNormal3f(normals[index].x, normals[index].y, normals[index].z);

			if (hasTexture) {
				if (uvCoords != 0) { //czy tablica koordynatów nie jest wyzerowana (czy coœ wczytano)
					glTexCoord2f(uvCoords[index].x, uvCoords[index].y);
				}
			}
			else {
				if (scene.materials.size() && materialId >= 0) 
				{
					BYTE *pColor = scene.materials[materialId].color;
					glColor3ub(pColor[0], pColor[1], pColor[2]);
				}
			}

			glVertex3f(vertices[index].x, vertices[index].y, vertices[index].z);
		}
	}

	glEnd();


	if (drawLines == true) {
		glDisable(GL_TEXTURE_2D);
		glColor3ub(255, 255, 255);
		glLineWidth(3);

		glBegin(GL_LINE_STRIP);

		for (int j = 0; j < triangleCount; j++) {
			for (int whichVertex = 0; whichVertex < 3; whichVertex++) {
				int index = triangles[j].vertexIndexes[whichVertex];
				glVertex3f(vertices[index].x, vertices[index].y, vertices[index].z);
			}
		}

		glEnd();
	}
}

void Scene::ComputeNormals()
{
	Vector3 one, two, norm, triangle[3];

	if (this->objects.size() <= 0) //Czy w ogóle s¹ jakieœ obiekty
		return;

	for (int k = 0; k < this->objects.size(); k++){

		Object3DS *ob = &(this->objects[k]);

		Vector3 *ns = new Vector3[ob->triangleCount];
		Vector3 *temp = new Vector3[ob->triangleCount];
		ob->normals = new Vector3[ob->vertexCount];

		for (int i = 0; i < ob->triangleCount; i++){
			triangle[0] = ob->vertices [ ob->triangles[i].vertexIndexes[0] ];
			triangle[1] = ob->vertices [ ob->triangles[i].vertexIndexes[1] ];
			triangle[2] = ob->vertices [ ob->triangles[i].vertexIndexes[2] ];

			one = triangle[0] - triangle[2];	//Dwie prawêdzie trójk¹ta
			two = triangle[2] - triangle[1];	

			norm = one.Cross(two);	//Normalna powierzchni.
			temp[i] = norm;				
			norm.Normalize();			

			ns[i] = norm;
		}

		//Normalna wierzcho³ka to œrednia normalnych trójk¹tów, które go zawieraj¹

		for (int i = 0; i < ob->vertexCount; i++) {
			int tri_count = 0;
			Vector3 sum = Vector3::zero;
			for (int j = 0; j < ob->triangleCount; j++)	{										
				if (ob->triangles[j].containsVert(i)){
					sum = sum + ns[j];
					tri_count++;			
				}
			}

			ob->normals[i] = sum / float(-tri_count);
		}

		delete[] temp;
		delete[] ns;
	}
}




//https://open.gl/framebuffers
void createFramebuffer() {
	
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// Create texture to hold color buffer
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_HEIGHT, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	// Create Renderbuffer Object to hold depth and stencil buffers
	GLuint rboDepthStencil;
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);
}