#include "Object3D.h"

// --DirextX12-- //
#include "DX12Cmd.h"

// --�e�N�X�`���N���X-- //
#include "Texture.h"

// --�֗��n�֐�-- //
#include "Util.h"

D3D12_HEAP_PROPERTIES Object3D::heapProp_ = {};
D3D12_RESOURCE_DESC Object3D::resdesc_ = {};
ID3D12Device* Object3D::device_;
ID3D12GraphicsCommandList* Object3D::cmdList_= nullptr;
Camera* Object3D::camera_ = nullptr;

Object3D::Object3D() :
#pragma region ���������X�g
	position_{ 0.0f, 0.0f, 0.0f },// -> ���W
	rotation_{ 0.0f, 0.0f, 0.0f },// -> ��]�p
	scale_{ 1.0f, 1.0f, 1.0f },// ----> �X�P�[��

	matWorld_{},

	// �萔�o�b�t�@
	constBuff_(nullptr),// -> �I�u�W�F�N�g

	// ���f��
	model_(nullptr),

	dirty(true)
#pragma endregion
{
	// �萔�o�b�t�@����
	GenerateConstBuffer();
}

Object3D* Object3D::CreateObject3D(Model* model)
{
	// �C���X�^���X����
	Object3D* object = new Object3D();

	object->SetModel(model);

	// ���������C���X�^���X��Ԃ�
	return object;
}

void Object3D::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList) {
	device_ = device;// -> �f�o�C�X
	cmdList_ = cmdList;// -> �R�}���h���X�g

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

void Object3D::Draw()
{
	// �I�u�W�F�N�g�̍X�V����
	TransferConstBuffer();

	// �I�u�W�F�N�g�̕`�揈��
	{
		// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h-- //
		cmdList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
		
		model_->Draw();
	}
}

void Object3D::PreDraw() {
	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList_->SetPipelineState(DX12Cmd::GetObject3DPipeline().pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList_->SetGraphicsRootSignature(DX12Cmd::GetObject3DPipeline().rootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// --�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h-- //
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
	cmdList_->SetDescriptorHeaps(1, ppHeaps);
}

void Object3D::SetPos(const Float3& position)
{
	position_ = position;
	dirty = true;
}

void Object3D::SetRot(const Float3& rotation)
{
	rotation_ = rotation;
	dirty = true;
}

void Object3D::SetScale(const Float3& scale)
{
	scale_ = scale;
	dirty = true;
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
	if (dirty) {
		Matrix4 matScale, matRot, matTrans;
		matScale = Matrix4Scale(scale_);
		matRot = Matrix4Identity();
		matRot *= Matrix4RotateZ(Util::Degree2Radian(rotation_.z));
		matRot *= Matrix4RotateX(Util::Degree2Radian(rotation_.x));
		matRot *= Matrix4RotateY(Util::Degree2Radian(rotation_.y));
		matTrans = Matrix4Translate(position_);

		// --���[���h�s��̍���-- //
		matWorld_ = Matrix4Identity();// -> �ό`�̃��Z�b�g
		matWorld_ *= matScale;// -> ���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld_ *= matRot;// -> ���[���h�s��ɉ�]�𔽉f
		matWorld_ *= matTrans;// -> ���[���h�s��ɕ��s�ړ��𔽉f

		dirty = false;
	}

	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	Camera* camera = camera_;

	// �}�b�s���O
	ObjectBuff* objectMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&objectMap);
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�փf�[�^�]��
	objectMap->mat = matWorld_ * camera->GetMatView() * camera->GetMatProjection();

	// �}�b�s���O�I������
	constBuff_->Unmap(0, nullptr);
}