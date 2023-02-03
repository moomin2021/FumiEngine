#pragma once
#include <DirectXMath.h>

class DirectionalLight {
private:// �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:// �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\��
	struct ConstBufferData {
		XMVECTOR lightv;// -> ���C�g�֕�����\��
		XMFLOAT3 lightcolor;// -> ���C�g�̐F
		bool active;// -> �L���t���O
	};

private:// �����o�ϐ�
	// ���C�g��������
	XMVECTOR lightdir = { 1.0f, 0.0f, 0.0f, 0.0f };

	// ���C�g�F
	XMFLOAT3 lightcolor = { 1.0f, 1.0f, 1.0f };

	// �L���t���O
	bool active = false;

public:// �����o�֐�
	inline void SetLightDir(const XMVECTOR& lightdir) { this->lightdir = DirectX::XMVector3Normalize(lightdir); }
	inline const XMVECTOR& GetLightDir() { return lightdir; }
	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; };
	inline const XMFLOAT3& GetLightColor() { return lightcolor; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }
};