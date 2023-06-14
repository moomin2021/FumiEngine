#include "Util.h"

#include <Windows.h>

int Util::Clamp(int value, int max, int min)
{
	// 値が最大値を上回っていたら最大値を返す
	if (value >= max) return max;

	// 値が最小値を下回っていたら最小値を返す
	if (value <= min) return min;

	// どちらにも当てはまらなかったら値をそのまま返す
	return value;
}

float Util::Clamp(float value, float max, float min)
{
	// 値が最大値を上回っていたら最大値を返す
	if (value >= max) return max;

	// 値が最小値を下回っていたら最小値を返す
	if (value <= min) return min;

	// どちらにも当てはまらなかったら値をそのまま返す
	return value;
}

float Util::Radian2Degree(float radian)
{
	return radian * 180.0f / PI;
}

float Util::Degree2Radian(float degree)
{
	return degree * PI / 180.0f;
}

std::vector<wchar_t> Util::StringToWideChar(const std::string& str)
{
	// 最終的に返す用変数
	std::vector<wchar_t> wchar;
	wchar.resize(128);// -> サイズを設定

	// 変換
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wchar.data(), static_cast<int>(wchar.size()));

	// 戻り値を返す
	return wchar;
}