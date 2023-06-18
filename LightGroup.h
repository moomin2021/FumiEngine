#pragma once
#include <DirectXMath.h>
#include "DX12Cmd.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

class LightGroup {
private:// �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:// �萔
	static const uint16_t DirLightNum = 3;
	static const uint16_t PointLightNum = 3;
	static const uint16_t SpotLightNum = 3;
	static const uint16_t CircleShadowNum = 1;

public:// �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		// �����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		// ���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		// �_�����p
		PointLight::ConstBufferData pointLights[PointLightNum];
		// �X�|�b�g���C�g�p
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		// �ۉe�p
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};

private:// �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// �����̐F
	XMFLOAT3 ambientColor = { 1.0f, 1.0f, 1.0f };
	// ���s�����̔z��
	DirectionalLight dirLights[DirLightNum];
	// �_�����̔z��
	PointLight pointLights[PointLightNum];
	// �X�|�b�g���C�g�p
	SpotLight spotLights[SpotLightNum];
	// �ۉe�̔z��
	CircleShadow circleShadows[CircleShadowNum];
	// �_�[�e�B�t���O
	bool dirty = false;

public:// �ÓI�����o�֐�
	LightGroup();

private:// �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �萔�o�b�t�@�]��
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// �W���̃��C�g�ݒ�
	/// </summary>
	void DefaultLightSetting();

public:// �����o�֐�

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="color"> ���C�g�F </param>
	void SetAmbientColor(const XMFLOAT3& color);

	/// <summary>
	/// ���s�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index"> ���C�g�ԍ� </param>
	/// <param name="active"> �L���t���O </param>
	void SetDirLightActive(uint16_t index, bool active);

	/// <summary>
	/// ���s�����̃��C�g�������Z�b�g
	/// </summary>
	/// <param name="index"> ���C�g�ԍ� </param>
	/// <param name="lightdir"> ���C�g���� </param>
	void SetDirLightDir(uint16_t index, const XMVECTOR& lightdir);

	/// <summary>
	/// ���s�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index"> ���C�g�ԍ� </param>
	/// <param name="lightcolor"> ���C�g�F </param>
	void SetDirLightColor(uint16_t index, const XMFLOAT3& lightcolor);

	void SetPointLightActive(uint16_t index, bool active);
	void SetPointLightPos(uint16_t index, const XMFLOAT3& lightpos);
	void SetPointLightColor(uint16_t index, const XMFLOAT3& lightcolor);
	void SetPointLightAtten(uint16_t index, const XMFLOAT3& lightAtten);

	void SetSpotLightActive(uint16_t index, bool active);
	void SetSpotLightDir(uint16_t index, const XMVECTOR& lightdir);
	void SetSpotLightPos(uint16_t index, const XMFLOAT3& lightpos);
	void SetSpotLightColor(uint16_t index, const XMFLOAT3& lightcolor);
	void SetSpotLightAtten(uint16_t index, const XMFLOAT3& lightAtten);
	void SetSpotLightFactorAngle(uint16_t index, const XMFLOAT2& lightFactorAngle);

	void SetCircleShadowActive(uint16_t index, bool active);
	void SetCircleShadowCasterPos(uint16_t index, const XMFLOAT3& casterPos);
	void SetCircleShadowDir(uint16_t index, const XMVECTOR& lightdir);
	void SetCircleShadowDistanceCasterLight(uint16_t index, float distanceCasterLight);
	void SetCircleShadowAtten(uint16_t index, const XMFLOAT3& lightAtten);
	void SetCircleShadowFactorAngle(uint16_t index, const XMFLOAT2& lightFactorAngle);
};