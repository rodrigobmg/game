﻿#include "Board.h"
 
string Board::FillNumber(string s, UINT MaxNumber)
{
	while (s.size() < MaxNumber)
		s = "0" + s;
	return s;
}

Board::Board(int X, int Y)
{
 	_texture = TextureManager::GetInstance()->GetTexture(eID::BOARD);
	_sprite = new GSprite(_texture, 0);
	x = X;
	y = Y;
}
 
#include "Dagger.h"
void Board::Render(Camera * camera, Simon * simon , int state, Weapon * weaponSub, int RemainingTime)
{ 
	_sprite->Draw(x,y);
	_font.Draw(x, y+15, "SCORE-");
	_font.Draw(x, y + 15, "SCORE-" + FillNumber(std::to_string(simon->GetScore()), (UINT) 6)  );

	_font.Draw(x, y+33, "PLAYER");
	_font.Draw(x, y+51, "ENEMY");


	_font.Draw(x+220 , y + 15, "TIME " + FillNumber(std::to_string(RemainingTime), (UINT)4) );


	_font.Draw(x + 400 -20, y + 15, "STAGE");
	_font.Draw(x + 400+60, y + 15, FillNumber(std::to_string(state),2));


	_font.Draw(x + 400+15, y + 15+18, FillNumber(std::to_string(simon->GetHeartCollect()),2)); // Số lượng tim nhặt dc
	_font.Draw(x + 400 + 15, y + 15 + 18+18, FillNumber(std::to_string(simon->GetLives()),2)); // số mạng sông


	_boardHealth.Draw(x + 100, y + 30, simon->GetHealth(), 5);
	 
	if (weaponSub != NULL)
	{
		switch (weaponSub->GetType())
		{
		case eID::DAGGER:
			weaponSub->RenderIcon(330, 40);
			break;

		case eID::HOLYWATER:
			weaponSub->RenderIcon(332, 33);
			break;


		default:
			break;
		} 

	}

}


Board::~Board()
{
	SAFE_DELETE(_sprite);
}
