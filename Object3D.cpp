#include "Object3D.h"
#include "DX12Cmd.h"
#include "Texture.h"
#include "Util.h"

// �ÓI�����o�ϐ��̎���
ID3D12Device* Object3D::device_ = nullptr;// ---------------> �f�o�C�X
ID3D12GraphicsCommandList* Object3D::cmdList_ = nullptr;// -> �R�}���h���X�g
D3D12_HEAP_PROPERTIES Object3D::heapProp_ = {};// ----------> �q�[�v�ݒ�
D3D12_RESOURCE_DESC Object3D::resdesc_ = {};// -------------> ���\�[�X�ݒ�
Camera* Object3D::camera_ = nullptr;// ---------------------> �J����
LightGroup* Object3D::lightGroup_ = nullptr;// -------------> ���C�g

Object3D::Object3D() :
#pragma region ���������X�g
	position_{0.0f, 0.0f, 0.0f},// ----> ���W
	rotation_{0.0f, 0.0f, 0.0f},// ----> ��]�p
	scale_{1.0f, 1.0f, 1.0f},// -------> �X�P�[��
	color_{1.0f, 1.0f, 1.0f, 1.0f},// -> �F(RGBA)
	matWorld_{},// --------------------> ���[���h���W
	constBuff_(nullptr),// ------------> �萔�o�b�t�@
	model_(nullptr),// ----------------> ���f��
	dirty(true)// ---------------------> �_�[�e�B�t���O
#pragma endregion
{
	// �萔�o�b�t�@����
	GenerateConstBuffer();
}

void Object3D::GenerateConstBuffer() {
	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&heapProp_,
		D3D12_HEAP_FLAG_NONE,
		&resdesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void Object3D::TransferConstBuffer() {
	// �_�[�e�B�[�t���O��[ON]�Ȃ�
	if (dirty) {
		// ���[���h���W�v�Z
		Matrix4 matScale, matRot, matTrans;
		matScale = Matrix4Scale(scale_);
		matRot = Matrix4Identity();
		matRot *= Matrix4RotateZ(Util::Degree2Radian(rotation_.z));
		matRot *= Matrix4RotateX(Util::Degree2Radian(rotation_.x));
		matRot *= Matrix4RotateY(Util::Degree2Radian(rotation_.y));
		matTrans = Matrix4Translate(position_);

		// ���[���h�s��̍���
		matWorld_ = Matrix4Identity();// -> �ό`�̃��Z�b�g
		matWorld_ *= matScale;// ---------> ���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld_ *= matRot;// -----------> ���[���h�s��ɉ�]�𔽉f
		matWorld_ *= matTrans;// ---------> ���[���h�s��ɕ��s�ړ��𔽉f

		// �_�[�e�B�[�t���O[OFF]�ɂ���
		dirty = false;
	}

	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// �}�b�s���O
	ObjectBuff* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�փf�[�^�]��
	constMap->viewProj = camera_->GetMatView() * camera_->GetMatProjection();
	constMap->world = matWorld_;
	constMap->cameraPos = camera_->eye_;
	constMap->color = color_;

	// �}�b�s���O�I������
	constBuff_->Unmap(0, nullptr);
}

Object3D* Object3D::CreateObject3D(Model* model)
{
	// �C���X�^���X����
	Object3D* object = new Object3D();

	// ���f���ݒ�
	object->SetModel(model);

	// ���������C���X�^���X��Ԃ�
	return object;
}

void Object3D::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList) {
	device_ = device;// -> �f�o�C�X
	cmdList_ = cmdList;// -> �R�}���h���X�g

	// ���f��������
	Model::Initialize(cmdList);

	// �萔�o�b�t�@�̃q�[�v�ݒ�
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;

	// �萔�o�b�t�@�̃��\�[�X�ݒ�
	resdesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc_.Width = (sizeof(ObjectBuff) + 0xff) & ~0xff;
	resdesc_.Height = 1;
	resdesc_.DepthOrArraySize = 1;
	resdesc_.MipLevels = 1;
	resdesc_.SampleDesc.Count = 1;
	resdesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void Object3D::PreDraw() {
	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList_->SetPipelineState(DX12Cmd::GetObject3DPipeline().pipelineState.Get());
	
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList_->SetGraphicsRootSignature(DX12Cmd::GetObject3DPipeline().rootSignature.Get());
	
	// �v���~�e�B�u�`���ݒ�
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
	cmdList_->SetDescriptorHeaps(1, ppHeaps);
}

void Object3D::Draw()
{
	// �I�u�W�F�N�g�̍X�V����
	TransferConstBuffer();

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h-- //
	cmdList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	// ���C�g�̕`��
	lightGroup_->Draw();

	// ���f���̏������ɕ`��
	model_->Draw();
}

void Object3D::SetPos(const Float3& position)
{
	position_ = position;// -> ���W�ݒ�
	dirty = true;// -> �_�[�e�B�[�t���O��[ON]�ɂ���
}

void Object3D::SetRot(const Float3& rotation)
{
	rotation_ = rotation;// -> ��]�p�ݒ�
	dirty = true;// -> �_�[�e�B�[�t���O��[ON]�ɂ���
}

void Object3D::SetScale(const Float3& scale)
{
	scale_ = scale;// -> �g�k�ݒ�
	dirty = true;// -> �_�[�e�B�[�t���O��[ON]�ɂ���
}

void Object3D::SetColor(const Float4& color) {
	color_ = color;// -> �F(RGBA)�ݒ�
}