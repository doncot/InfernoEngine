#pragma once
#include<Game.h>

namespace Inferno
{
	class TestGame : public Game
	{
	public:
		TestGame();
		~TestGame() final override;

		bool GameLoop() final override;

	};


}