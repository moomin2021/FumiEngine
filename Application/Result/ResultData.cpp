#include "ResultData.h"

ResultData* ResultData::GetInstance()
{
	// インスタンス生成
	static ResultData inst;

	// インスタンスを返す
	return &inst;
}
