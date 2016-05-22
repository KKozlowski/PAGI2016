#include "editor.h"

void Editor::Up()
{
	if (mode == MOVE_VIEW)
		addPos(0, 0.4f, 0);
}

void Editor::Down()
{
	if (mode == MOVE_VIEW)
		addPos(0, -0.4f, 0);
}

void Editor::Forward()
{
	if (mode == MOVE_VIEW)
		addPos(0, 0, -0.8f);
}

void Editor::Backward()
{
	if (mode == MOVE_VIEW)
		addPos(0, 0, 0.8f);
}

void Editor::Left()
{
	if (mode == ROTATE_VIEW)
		addRot(1.5F, 0, 0);
}

void Editor::Right()
{
	if (mode == ROTATE_VIEW)
		addRot(-1.5F, 0, 0);
}
