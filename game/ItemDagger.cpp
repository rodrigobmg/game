﻿#include "ItemDagger.h"


 
ItemDagger::ItemDagger(float X, float Y)
{
	_texture = new GTexture("Resources\\item\\4.png");
	_sprite = new GSprite(_texture, 0);
	type = eID::ITEMDAGGER;

	this->x = X;
	this->y = Y;
	vy = ITEMDAGGER_GRAVITY;
	TimeDisplayMax = ITEMDAGGER_TIMEDISPLAYMAX;
	TimeDisplayed = 0;
}

void ItemDagger::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + _texture->FrameWidth;
	bottom = y + _texture->FrameHeight /*- 18*/; 
}




void ItemDagger::Update(DWORD dt, vector<LPOBJECT> *listObject)
{ 
	TimeDisplayed += dt;
	if (TimeDisplayed >= TimeDisplayMax)
	{
		isFinish = true;
		return;
	}
	 
	dy = vy * dt;


	vector<LPOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (UINT i = 0; i < listObject->size(); i++)
		if (listObject->at(i)->GetType() == eID::BRICK)
			listObject_Brick.push_back(listObject->at(i));

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (y + dy > 365.0f)
	{
		DebugOut(L"XSAD");
	}
	CalcPotentialCollisions(&listObject_Brick, coEvents); // Lấy danh sách các va chạm
	 
	

	if (coEvents.size() == 0)
	{
		y += dy; 

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		y += min_ty * dy + ny * 0.4f;

		if (ny != 0)
		{
			vy = 0;
		}
	}


	if (y > 365.0f)
		DebugOut(L"XSAD");

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
	 
}




ItemDagger::~ItemDagger()
{
}