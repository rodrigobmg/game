﻿#include "Fire.h"

Fire::Fire(int X, int Y, int CountLoop)
{
	this->x = X;
	this->y = Y;
	_texture = TextureManager::GetInstance()->GetTexture(eType::FIRE);
	_sprite = new GSprite(_texture, 150);
	this->CountNeedLoop = CountLoop;
}


Fire::~Fire()
{
}

void Fire::Update(DWORD dt)
{
	Effect::Update(dt);

	//DebugOut(L"GetIndex = %d\n", _sprite->GetIndex());
 

	if (_sprite->GetIndex() == 3) // nếu là frame cuối thì xong, frame cuối trống
	{
		CountNeedLoop--; // giảm số lần cần lặp
		if (CountNeedLoop == 0) // hết lần cần lặp thì kết thúc
			isFinish = true;
		else
		{
			_sprite->_timeLocal = _sprite->_timeAni; 
			//DebugOut(L"CountNeedLoop = %d\n", CountNeedLoop);
		}
	}
		
}
