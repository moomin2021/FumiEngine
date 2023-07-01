#pragma once
#include "float2.h"
#include "Util.h"

class SpotLight {
#pragma region �\����
public:
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		Vector3 lightVec;			// ���C�g�̕���(XYZ)
		float3 lightPos;			// ���C�g�̍��W(XYZ)
		float pad1;// �p�f�B���O
		float3 lightColor;			// ���C�g�̐F(RGB)
		float pad2;// �p�f�B���O
		float3 lightAtten;			// ���C�g�̋��������W��(XYZ)
		float pad3;// �p�f�B���O
		float2 lightFactorAngleCos;	// ���C�g�̌����p�x(�J�n�p�x, �I���p�x)
		bool active;				// �L���t���O
		float pad4;// �p�f�B���O
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// ���C�g����(XYZ)
	Vector3 lightDir_ = { 1.0f, 0.0f, 0.0f };

	// ���C�g���W(XYZ)
	float3 lightPos_ = { 0.0f, 0.0f, 0.0f };

	// ���C�g�F
	float3 lightColor_ = { 1.0f, 1.0f, 1.0f };

	// ���C�g���������W��
	float3 lightAtten_ = { 1.0f, 1.0f, 1.0f };

	// ���C�g�����p�x(�J�n�p�x�A�I���p�x)
	float2 lightFactorAngleCos_ = { 0.5f, 0.2f };

	// �L���t���O
	bool active_ = true;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	/// <summary>
	/// ���C�g�̕���(XYZ)��ݒ�
	/// </summary>
	/// <param name="lightDir"> ���C�g�̕���(XYZ) </param>
	inline void SetLightDir(const Vector3& lightDir) { lightDir_ = Vector3Normalize(lightDir); }

	/// <summary>
	/// ���C�g�̍��W(XYZ)��ݒ�
	/// </summary>
	/// <param name="lightPos"> ���C�g�̍��W(XYZ) </param>
	inline void SetLightPos(const float3& lightPos) { lightPos_ = lightPos; }

	/// <summary>
	/// ���C�g�̐F(RGB)��ݒ�
	/// </summary>
	/// <param name="lightColor"> ���C�g�̐F(RGB) </param>
	inline void SetLightColor(const float3& lightColor) { lightColor_ = lightColor; }

	/// <summary>
	/// ���C�g�̋��������W��(XYZ)��ݒ�
	/// </summary>
	/// <param name="lightAtten"> ���C�g�̋��������W��(XYZ) </param>
	inline void SetLightAtten(const float3& lightAtten) { lightAtten_ = lightAtten; }

	/// <summary>
	/// ���C�g�����p�x(�J�n�p�x�A�I���p�x)��ݒ�
	/// </summary>
	/// <param name="lightFactorAngle"> ���C�g�̌����p�x(�J�n�p�x, �I���p�x) </param>
	inline void SetLightFactorAngle(const float2& lightFactorAngle) {
		lightFactorAngleCos_.x = cosf(Util::Degree2Radian(lightFactorAngle.x));
		lightFactorAngleCos_.y = cosf(Util::Degree2Radian(lightFactorAngle.y));
	}

	/// <summary>
	/// ���C�g�L���t���O��ݒ�
	/// </summary>
	/// <param name="active"> ���C�g�L���t���O </param>
	inline void SetActive(bool active) { active_ = active; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	/// <summary>
	/// ���C�g�̕���(XYZ)���擾
	/// </summary>
	/// <returns> ���C�g�̕���(XYZ) </returns>
	inline const Vector3& GetLightDir() { return lightDir_; }

	/// <summary>
	/// ���C�g�̍��W(XYZ)���擾
	/// </summary>
	/// <returns> ���C�g�̍��W(XYZ) </returns>
	inline const float3& GetLightPos() { return lightPos_; }

	/// <summary>
	/// ���C�g�̐F(RGB)���擾
	/// </summary>
	/// <returns> ���C�g�̐F(RGB) </returns>
	inline const float3& GetLightColor() { return lightColor_; }

	/// <summary>
	/// ���C�g�̋��������W��(XYZ)���擾
	/// </summary>
	/// <returns> ���C�g�̋��������W��(XYZ) </returns>
	inline const float3& GetLightAtten() { return lightAtten_; }

	/// <summary>
	/// ���C�g�����p�x(�J�n�p�x�A�I���p�x)���擾
	/// </summary>
	/// <returns> ���C�g�����p�x(�J�n�p�x�A�I���p�x) </returns>
	inline const float2& GetLightFactorAngle() { return lightFactorAngleCos_; }

	/// <summary>
	/// ���C�g�L���t���O���擾
	/// </summary>
	/// <returns> ���C�g�L���t���O </returns>
	inline bool GetActive() { return active_; }
#pragma endregion
};