/*
	ゲーム要素の共通ヘッダー
*/
#pragma once

namespace Inferno
{
	//ゲーム要素の状態
	enum class GEAttribute : unsigned int
	{
		UserLock = 0x01, //ユーザーからの操作をロック
		Visible = 0x02, //描画するか否か
		NoHit = 0x04, //衝突判定をするか否か
		//KillMe = 0x08,
	};

	class IGameElement
	{
	public:

	};

}
