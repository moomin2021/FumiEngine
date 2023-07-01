#pragma once

class PointLight {
#pragma region �\����
public:
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		float3 lightPos;	// ���C�g�̍��W(XYZ)
		float pad1;// �p�f�B���O
		float3 lightColor;	// ���C�g�̐F(XYZ)
		float pad2;// �p�f�B���O
		float3 lightAtten;	// ���C�g���������W��(XYZ)
		bool active;		// ���C�g�L���t���O
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// ���C�g���W(���[���h���W�n)
	float3 lightPos_ = { 0.0f, 0.0f, 0.0f };

	// ���C�g�F
	float3 lightColor_ = { 1.0f, 1.0f, 1.0f };

	// ���C�g���������W��
	float3 lightAtten_ = { 1.0f, 1.0f, 1.0f };

	// �L���t���O
	bool active_ = true;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
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
	/// ���C�g�L���t���O��ݒ�
	/// </summary>
	/// <param name="active"> ���C�g�L���t���O </param>
	inline void SetActive(bool active) { active_ = active; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
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
	/// ���C�g�L���t���O���擾
	/// </summary>
	/// <returns> ���C�g�̗L���t���O </returns>
	inline bool GetActive() { return active_; }
#pragma endregion
};