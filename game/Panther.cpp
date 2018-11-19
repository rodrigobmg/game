﻿#include "Panther.h"



Panther::Panther(float X, float Y, int Trend, float autoGoX_Dx)
{
	type = eID::PANTHER;
	Health = 1;
	vx = vy = 0;
	trend = Trend;
	x = X;
	y = Y;
	AutoGoX_Backup_X = x;
	AutoGoX_Dx = autoGoX_Dx;

	_texture = TextureManager::GetInstance()->GetTexture(eID::PANTHER);
	_sprite = new GSprite(_texture, 200);

	isSitting = 1;
	isRunning = 0;
	isJumping = 0;
	isStart = 0;
	isAutoGoX = 0;
}



void Panther::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + _texture->FrameWidth;
	bottom = y + _texture->FrameHeight;
}

void Panther::Update(DWORD dt, Simon * simon, vector<LPOBJECT>* coObjects)
{
	GameObject::Update(dt);
	vy += SIMON_GRAVITY * dt; // Simple fall down

	float DistanceLimit = 177.0f;
	if (trend == 1)
		DistanceLimit -= 85;
	if (abs(simon->GetX() - (x+_texture->FrameWidth)) <= DistanceLimit && isStart == 0)
	{
		isSitting = false;
		isRunning = true;
		// chuyển qua trạng thái chạy
		Run();

		isStart = 1;
		isAutoGoX = 1;
	}


	





	
	if (isSitting)
	{
		_sprite->SelectIndex(PANTHER_ANI_SITTING);
	}
	else
	{
		if (isRunning)
		{
			if (PANTHER_ANI_RUNNING_BEGIN <= _sprite->GetIndex() && _sprite->GetIndex() < PANTHER_ANI_RUNNING_END)
			{
				_sprite->Update(dt);
			}
			else
				_sprite->SelectIndex(PANTHER_ANI_RUNNING_BEGIN);
		}
	}



#pragma region Xét va chạm đất

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	vector<LPOBJECT> list_Brick;
	list_Brick.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == eID::BRICK)
			list_Brick.push_back(coObjects->at(i));
	}

	CalcPotentialCollisions(&list_Brick, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		
		//x += min_tx * dx + nx * 0.4f;
		x += dx;
		if (ny == -1) // chỉ xét va chạm hướng xuống
			y += min_ty * dy + ny * 0.4f;
		else 		
			y += dy;
		 
		if (ny == -1)
		{
			vy = 0;
			if (isJumping)
			{
				isJumping = false; // kết thúc nhảy
				if (x < simon->GetX()) // simon ở bên phải
				{
					trend = 1; // đổi hướng panther qua phải 
				}
				else
				{
					trend = -1; // đổi hướng panther qua trái
				}
				Run();
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
#pragma endregion
	 

	if (isAutoGoX == true)
	{
		if (abs(x - AutoGoX_Backup_X) >= AutoGoX_Dx)
		{
			x = x - (abs(x - AutoGoX_Backup_X) - AutoGoX_Dx);
			isAutoGoX = false;
			vx = 0;

			Jump(); // Sau khi chạy xong thì nhảy

			DebugOut(L"[PANTHER] end auto go X\n");
		}
	}

}

void Panther::Render(Camera * camera)
{
	if (Health <= 0)
		return;

//	_sprite->Update(dt);

	D3DXVECTOR2 pos = camera->Transform(x, y);
	if (trend == -1)
		_sprite->Draw((int)pos.x, (int)pos.y);
	else
		_sprite->DrawFlipX((int)pos.x, (int)pos.y);

	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);

}

bool Panther::GetIsStart()
{
	return isStart;
}

void Panther::Jump()
{
	if (isJumping == true)
		return;
	vy -= PANTHER_VYJUMP;
	vx = PANTHER_VXJUMP * trend;
	isJumping = true;

}

void Panther::Run()
{
	vx = PANTHER_SPEED_RUNNING * trend;
	isRunning = true;
}



Panther::~Panther()
{
}
