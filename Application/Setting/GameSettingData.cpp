#include "GameSettingData.h"

GameSettingData* GameSettingData::GetInstance()
{
	// インスタンス生成
	static GameSettingData inst;

	// インスタンスを返す
	return &inst;
}
