#include "Util.h"

#include <Windows.h>

int Util::Clamp(int value, int max, int min)
{
	// �l���ő�l�������Ă�����ő�l��Ԃ�
	if (value >= max) return max;

	// �l���ŏ��l��������Ă�����ŏ��l��Ԃ�
	if (value <= min) return min;

	// �ǂ���ɂ����Ă͂܂�Ȃ�������l�����̂܂ܕԂ�
	return value;
}

float Util::Clamp(float value, float max, float min)
{
	// �l���ő�l�������Ă�����ő�l��Ԃ�
	if (value >= max) return max;

	// �l���ŏ��l��������Ă�����ŏ��l��Ԃ�
	if (value <= min) return min;

	// �ǂ���ɂ����Ă͂܂�Ȃ�������l�����̂܂ܕԂ�
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
	// �ŏI�I�ɕԂ��p�ϐ�
	std::vector<wchar_t> wchar;
	wchar.resize(128);// -> �T�C�Y��ݒ�

	// �ϊ�
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wchar.data(), static_cast<int>(wchar.size()));

	// �߂�l��Ԃ�
	return wchar;
}