#pragma once
#include "main.h"

using namespace std;

enum editor_mode {
	MOVE_VIEW, ROTATE_VIEW,
	MOVE_OBJECT
};

class Editor {
public:
	editor_mode mode;

	Editor() {
		mode = MOVE_OBJECT;
	};
	void Up();
	void Down();
	void Forward();
	void Backward();
	void Left();
	void Right();
};