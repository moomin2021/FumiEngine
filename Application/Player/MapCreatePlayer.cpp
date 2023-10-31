#include "MapCreatePlayer.h"

#include "Object3D.h"
#include "Sprite.h"

MapCreatePlayer::~MapCreatePlayer()
{

}

void MapCreatePlayer::Initialize()
{

}

void MapCreatePlayer::Update()
{

}

void MapCreatePlayer::Draw()
{

}

void MapCreatePlayer::OnCollision()
{

}

void MapCreatePlayer::MatUpdate()
{

}

void MapCreatePlayer::Debug()
{

}

void (MapCreatePlayer::* MapCreatePlayer::stateTable[]) () = {
	&MapCreatePlayer::Normal,	// 通常状態
	&MapCreatePlayer::Fly,		// 空中状態
};

void MapCreatePlayer::Normal()
{

}

void MapCreatePlayer::Fly()
{

}

void MapCreatePlayer::Fall()
{

}