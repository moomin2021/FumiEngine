#include "Object3D.h"

// --DirextX12-- //
#include "DX12Cmd.h"

// --�e�N�X�`���N���X-- //
#include "Texture.h"

// --�֗��n�֐�-- //
#include "Util.h"

Object3D::Object3D() :
#pragma region ���������X�g
	position_{0.0f, 0.0f, 0.0f},// -> ���W
	rotation_{0.0f, 0.0f, 0.0f},// -> ��]�p
	scale_{1.0f, 1.0f, 1.0f},// ----> �X�P�[��

	// ���_�f�[�^�֘A
	vbView_{},// ------------> ���_�o�b�t�@�r���[
	vertexBuff_(nullptr),// -> ���_�o�b�t�@

	// �C���f�b�N�X�f�[�^
	ibView_{},// -----------> �C���f�b�N�X�o�b�t�@�r���[
	indexBuff_(nullptr),// -> �C���f�b�N�X�o�b�t�@

	// �萔�o�b�t�@
	objectBuff_(nullptr),// -> �I�u�W�F�N�g

	// �������e�s��
	matProjection_{},

	// �J����
	camera_(nullptr),

	// ���f��
	model_(nullptr)
#pragma endregion
{
	// ����������
	Initialize();
}

Object3D* Object3D::CreateObject3D()
{
	// �C���X�^���X����
	Object3D* object = new Object3D();

	// ���������C���X�^���X��Ԃ�
	return object;
}

void Object3D::Draw()
{
	// �I�u�W�F�N�g�̍X�V����
	{
		// --�X�P�[���A��]�A���s�ړ��s��̌v�Z-- //
		XMMATRIX matScale, matRot, matTrans;
		matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(Util::Degree2Radian(rotation_.z));
		matRot *= XMMatrixRotationX(Util::Degree2Radian(rotation_.x));
		matRot *= XMMatrixRotationY(Util::Degree2Radian(rotation_.y));
		matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

		// --���[���h�s��̍���-- //
		XMMATRIX matWorld = XMMatrixIdentity();// -> �ό`�̃��Z�b�g
		matWorld *= matScale;// -> ���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld *= matRot;// -> ���[���h�s��ɉ�]�𔽉f
		matWorld *= matTrans;// -> ���[���h�s��ɕ��s�ړ��𔽉f

		// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
		HRESULT result;

		// �}�b�s���O
		ObjectBuff* objectMap = nullptr;
		result = objectBuff_->Map(0, nullptr, (void**)&objectMap);
		assert(SUCCEEDED(result));

		// �萔�o�b�t�@�փf�[�^�]��
		objectMap->mat = matWorld * camera_->GetMatView() * matProjection_;

		// �}�b�s���O�I������
		objectBuff_->Unmap(0, nullptr);
	}

	// �I�u�W�F�N�g�̕`�揈��
	{
		// --SRV�q�[�v�̃n���h�����擾-- //
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

		// --�n���h�����w�肳�ꂽ���܂Ői�߂�-- //
		srvGpuHandle.ptr += model_->GetTextureHandle();

		// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h-- //
		DX12Cmd::GetCmdList()->SetGraphicsRootConstantBufferView(0, objectBuff_->GetGPUVirtualAddress());
		DX12Cmd::GetCmdList()->SetGraphicsRootConstantBufferView(1, materialBuff_->GetGPUVirtualAddress());

		// --�w�肳�ꂽSRV�����[�g�p�����[�^1�Ԃɐݒ�-- //
		DX12Cmd::GetCmdList()->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

		// --���_�o�b�t�@�r���[�̐ݒ�R�}���h-- //
		DX12Cmd::GetCmdList()->IASetVertexBuffers(0, 1, &vbView_);

		// --�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h-- //
		DX12Cmd::GetCmdList()->IASetIndexBuffer(&ibView_);

		//// --�`��R�}���h-- //
		DX12Cmd::GetCmdList()->DrawIndexedInstanced(static_cast<UINT>(model_->GetIndexes().size()), 1, 0, 0, 0);
	}
}

void Object3D::PreDraw() {
	// �p�C�v���C���X�e�[�g�̐ݒ�
	DX12Cmd::GetCmdList()->SetPipelineState(DX12Cmd::GetObject3DPipeline().pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	DX12Cmd::GetCmdList()->SetGraphicsRootSignature(DX12Cmd::GetObject3DPipeline().rootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	DX12Cmd::GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// --�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h-- //
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
	DX12Cmd::GetCmdList()->SetDescriptorHeaps(1, ppHeaps);
}

void Object3D::SetModel(Model* model)
{
	// ���f���ݒ�
	model_ = model;

	// �o�b�t�@�쐬
	CreateVertexBuff();
	CreateIndexBuff();
	CreateMaterialBuff();
}

void Object3D::Initialize()
{
	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// �I�u�W�F�N�g�̒萔�o�b�t�@���쐬
	{
		// �萔�o�b�t�@�̃q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

		// �萔�o�b�t�@�̃��\�[�X�ݒ�
		D3D12_RESOURCE_DESC resdesc{};
		resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resdesc.Width = (sizeof(ObjectBuff) + 0xff) & ~0xff;
		resdesc.Height = 1;
		resdesc.DepthOrArraySize = 1;
		resdesc.MipLevels = 1;
		resdesc.SampleDesc.Count = 1;
		resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// �萔�o�b�t�@�̐���
		result = DX12Cmd::GetDevice()->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&objectBuff_));
		assert(SUCCEEDED(result));
	}

	// --�������e�s��̌v�Z-- //
	matProjection_ = XMMatrixPerspectiveFovLH(
		Util::Degree2Radian(45.0f),// -----------> �㉺��p45�x
		(float)WinAPI::GetWidth() / WinAPI::GetHeight(),// -> �A�X�y�N�g��i��ʉ���/��ʏc���j
		0.1f, 1000.0f// ------------------------> �O�[�A���[
	);
}

void Object3D::CreateMaterialBuff()
{
	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// �萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// �萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(MaterialBuff) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	result = DX12Cmd::GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&materialBuff_));
	assert(SUCCEEDED(result));

	// �}�e���A���萔�o�b�t�@�̃}�b�s���O
	MaterialBuff* materialMap;
	result = materialBuff_->Map(0, nullptr, (void**)&materialMap);
	assert(SUCCEEDED(result));
	materialMap->ambient = model_->GetMaterial().ambient;
	materialMap->diffuse = model_->GetMaterial().diffuse;
	materialMap->specular = model_->GetMaterial().specular;
	materialMap->alpha = model_->GetMaterial().alpha;
	materialBuff_->Unmap(0, nullptr);
}

void Object3D::CreateVertexBuff()
{
	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(model_->GetVertexes()[0]) * model_->GetVertexes().size());

	// --���_�o�b�t�@�̐ݒ�-- //
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// --���\�[�X�ݒ�-- //
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --���_�o�b�t�@�̐���-- //
	result = DX12Cmd::GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuff_));
	assert(SUCCEEDED(result));

	// --���_�o�b�t�@�r���[�̍쐬-- //
	vbView_.BufferLocation = vertexBuff_->GetGPUVirtualAddress();// -> GPU���z�A�h���X
	vbView_.SizeInBytes = sizeVB;// -> ���_�o�b�t�@�̃T�C�Y
	vbView_.StrideInBytes = sizeof(model_->GetVertexes()[0]);// -> ���_1���̃f�[�^�T�C�Y

	// --Map�����Ń��C����������GPU�̃�������R�Â���-- //
	Vertex3D* vertMap = nullptr;
	result = vertexBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// --�S���_�ɑ΂���-- //
	for (int i = 0; i < model_->GetVertexes().size(); i++)
	{
		vertMap[i] = model_->GetVertexes()[i]; // ���W���R�s�[
	}

	// --�q���������-- //
	vertexBuff_->Unmap(0, nullptr);
}

void Object3D::CreateIndexBuff()
{
	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// --�C���f�b�N�X�f�[�^�S�̂̃T�C�Y-- //
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * model_->GetIndexes().size());

	// --���_�o�b�t�@�̐ݒ�-- //
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// --���\�[�X�ݒ�-- //
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --�C���f�b�N�X�o�b�t�@�̐���-- //
	result = DX12Cmd::GetDevice()->CreateCommittedResource(
		&heapProp,// -> �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,// -> ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);

	// --�C���f�b�N�X�o�b�t�@�r���[�쐬-- //
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;

	// --�C���f�b�N�X�o�b�t�@���}�b�s���O-- //
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));

	// --�S�C���f�b�N�X�ɑ΂���-- //
	for (size_t i = 0; i < model_->GetIndexes().size(); i++)
	{
		indexMap[i] = model_->GetIndexes()[i];
	}

	// --�}�b�s���O����-- //
	indexBuff_->Unmap(0, nullptr);
}
