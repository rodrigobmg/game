#include "ObjectHidden.h"

 
ObjectHidden::ObjectHidden(int X, int Y, int W, int H)
{
	x = (float) X;
	y = (float) Y;
	width = W;
	height = H;
	type = eID::OBJECT_HIDDEN;
	id = 0; // set sau
	Health = 1;
}

ObjectHidden::~ObjectHidden()
{
}

void ObjectHidden::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;
}

void ObjectHidden::Update(DWORD dt, vector<LPOBJECT>* coObjects)
{
}

void ObjectHidden::Render(Camera * camera)
{
	
	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);
}
