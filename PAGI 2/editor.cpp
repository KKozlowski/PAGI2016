#include "editor.h"

float moveStep = 0.4f;
float rotStep = 0.4f;

void Editor::Up()
{
	if (mode == MOVE_VIEW)
		addPos(0, 0.4f, 0);
	if (mode == MOVE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.position.y += moveStep;
	}

	if (mode == ROTATE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.rotation.x -= rotStep;
	}
}

void Editor::Down()
{
	if (mode == MOVE_VIEW)
		addPos(0, -0.4f, 0);
	if (mode == MOVE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.position.y -= moveStep;
	}
	if (mode == ROTATE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.rotation.x += rotStep;
	}
}

void Editor::Forward()
{
	if (mode == MOVE_VIEW)
		addPos(0, 0, -0.8f);
	if (mode == MOVE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.position.z -= moveStep;
	}

	if (mode == ROTATE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.rotation.z += rotStep;
	}
}

void Editor::Backward()
{
	if (mode == MOVE_VIEW)
		addPos(0, 0, 0.8f);
	if (mode == MOVE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.position.z += moveStep;
	}

	if (mode == ROTATE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.rotation.z -= rotStep;
	}
}

void Editor::Left()
{
	if (mode == ROTATE_VIEW)
		addRot(1.5F, 0, 0);
	if (mode == MOVE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.position.x -= moveStep;
	}

	if (mode == ROTATE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.rotation.y -= rotStep;
	}
}

void Editor::Right()
{
	if (mode == ROTATE_VIEW)
		addRot(-1.5F, 0, 0);
	if (mode == MOVE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.position.x += moveStep;
	}
	if (mode == ROTATE_OBJECT) {
		if (Object3DS::selected != NULL)
			Object3DS::selected->transform.rotation.y += rotStep;
	}
}
