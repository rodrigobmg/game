﻿#include "Object.h"



void Object::RenderBoundingBox(Camera * camera)
{
	RECT rect;

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	D3DXVECTOR2 pos = camera->Transform(l, t);

	Game::GetInstance()->Draw(
		pos.x, 
		pos.y, 
		TextureManager::GetInstance()->GetTexture(
			eID::RENDERBBOX)->Texture ,
			rect.left, 
			rect.top, 
			rect.right, 
			rect.bottom, 
			100);

}



/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT Object::SweptAABBEx(LPOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	Game::SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CollisionEvent * e = new CollisionEvent(t, nx, ny, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void Object::CalcPotentialCollisions(
	vector<LPOBJECT> *coObjects,
	vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);

}

void Object::FilterCollision(
	vector<LPCOLLISIONEVENT> &coEvents,
	vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty,
	float &nx, float &ny) // Hàm này sẽ lấy ra sự kiện va chạm đến sớm nhất trên mỗi trục x , y
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t <= min_tx && c->nx != 0)
		{ // va chạm theo trục x
			min_tx = c->t;
			nx = c->nx;
			min_ix = i;
		}

		if (c->t <= min_ty  && c->ny != 0)
		{ // va chạm theo trục y
			min_ty = c->t;
			ny = c->ny;
			min_iy = i;
		}
	}

	if (min_ix >= 0)
		coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0)
		coEventsResult.push_back(coEvents[min_iy]);
}

int Object::GetHeight()
{
	return _texture->FrameHeight;
}

int Object::GetWidth()
{
	return _texture->FrameWidth;
}

eID Object::GetType()
{
	return type;
}

bool Object::GetIsTake()
{
	return isTake;
}

void Object::SetIsTake(int b)
{
	isTake = b;
}




Object::Object()
{
	vx = vy = 0;
	x = y = 0;
}

Object::Object(eID _type)
{
	this->type = _type;
}

void Object::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Object::SetSpeed(float vx, float vy)
{
	this->vx = vx;
	this->vy = vy;
}

void Object::GetPosition(float & x, float & y)
{
	x = this->x; y = this->y;
}

void Object::GetSpeed(float & vx, float & vy)
{
	vx = this->vx;
	vy = this->vy;
}

float Object::GetX()
{
	return x;
}

float Object::GetY()
{
	return y;
}

float Object::GetVx()
{
	return vx;
}

float Object::GetVy()
{
	return vy;
}

 
bool Object::isCollitionObjectWithObject(Object * obj)	// kiểm tra bằng AABB và Sweept AABB
{  
	if (checkAABB(obj)) // kiểm tra va chạm bằng AABB trước
		return true;

	LPCOLLISIONEVENT e = SweptAABBEx(obj); // kt va chạm giữa 2 object bằng sweptAABB
	bool res = e->t > 0 && e->t <= 1.0f; // ĐK va chạm
	SAFE_DELETE(e); 
	return res;
}

bool Object::checkAABB(Object * obj)
{
	float l, t, r, b;
	float l1, t1, r1, b1;
	this->GetBoundingBox(l, t, r, b);
	obj->GetBoundingBox(l1, t1, r1, b1);

	if (Game::GetInstance()->checkAABB(l, t, r, b, l1, t1, r1, b1))
		return true; 

	return false;
}

Object::~Object()
{
	/*SAFE_DELETE(_texture);*/
	// ko xóa texture vì đây là texture dùng chung được quản lí bởi TextureManager
	SAFE_DELETE(_sprite);
}
