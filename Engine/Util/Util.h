#pragma once
#include <vector>
#include <string>
#include <random>

namespace Util {

	const float PI = 3.141592654f;

	// --�������玩��֐�-- //

	/// <summary>
	/// ���ꂽ�l���ő�l�������Ă�����ő�l��Ԃ��A�ŏ��l��������Ă�����ŏ��l��Ԃ��B
	/// �ǂ���ɂ����Ă͂܂�Ȃ��ꍇ�́A�l�����̂܂ܕԂ�
	/// </summary>
	/// <param name="value"> �l </param>
	/// <param name="max"> �ő�l </param>
	/// <param name="min"> �ŏ��l </param>
	/// <returns></returns>
	int Clamp(int value, int max, int min);

	/// <summary>
	/// ���ꂽ�l���ő�l�������Ă�����ő�l��Ԃ��A�ŏ��l��������Ă�����ŏ��l��Ԃ��B
	/// �ǂ���ɂ����Ă͂܂�Ȃ��ꍇ�́A�l�����̂܂ܕԂ�
	/// </summary>
	/// <param name="value"> �l </param>
	/// <param name="max"> �ő�l </param>
	/// <param name="min"> �ŏ��l </param>
	/// <returns></returns>
	float Clamp(float value, float max, float min);

	/// <summary>
	/// �ʓx�@�̒l��x���@�ɂ��ĕԂ�
	/// </summary>
	/// <param name="radian"> �ʓx�@�̒l </param>
	/// <returns></returns>
	float Radian2Degree(float radian);

	/// <summary>
	/// �x���@�̒l���ʓx�@�ɂ��ĕԂ�
	/// </summary>
	/// <param name="degree"> �x���@�̒l </param>
	/// <returns></returns>
	float Degree2Radian(float degree);

	/// <summary>
	/// std::string����WideChar�ɕϊ�
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	std::vector<wchar_t> StringToWideChar(const std::string& str);

	/// <summary>
	/// �Q�[�����N�����Ă���̋N�����Ԃ��擾(s)
	/// </summary>
	/// <returns> ����(s) </returns>
	uint64_t GetTime();

	// �����_��
	/// <summary>
	/// �w�肳�ꂽ�͈͓��Ń����_���Ȑ�����Ԃ�
	/// </summary>
	/// <param name="min"> �ŏ��l </param>
	/// <param name="max"> �ő�l </param>
	/// <returns> �����l </returns>
	uint16_t GetRandomInt(uint16_t min, uint16_t max);

	/// <summary>
	/// �w�肳�ꂽ�͈͓��Ń����_���ȏ����l��Ԃ�
	/// </summary>
	/// <param name="min"> �ŏ��l </param>
	/// <param name="max"> �ő�l </param>
	/// <returns> �����l </returns>
	float GetRandomFloat(float min, float max);

} // namespace MathUtility