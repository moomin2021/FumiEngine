#include "Object3D.h"

// --DirextX12-- //
#include "DX12Cmd.h"

// --�e�N�X�`���N���X-- //
#include "Texture.h"

// --�֗��n�֐�-- //
#include "Util.h"

// --���f���ǂݍ��݂̂���-- //
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

Object3D::Object3D() :
#pragma region ���������X�g
	// ���W�A��]�p�A�X�P�[��
	position_{ 0.0f, 0.0f, 0.0f },
	rotation_{ 0.0f, 0.0f, 0.0f },
	scale_{ 1.0f, 1.0f, 1.0f },

	// �F
	color_{ 1.0f, 1.0f, 1.0f, 1.0f },

	// --�������e�s��̌v�Z-- //
	matProjection_{},

	// --���_�f�[�^-- //
	vertices_{},// -> ���_�f�[�^
	vbView_{},// -> ���_�o�b�t�@�r���[
	vertBuff_(nullptr),// -> ���_�o�b�t�@
	
	// --�C���f�b�N�X�f�[�^-- //
	indices_{},// -> �C���f�b�N�X�f�[�^
	ibView_{},// -> �C���f�b�N�X�o�b�t�@�r���[
	indexBuff_(nullptr),// -> �C���f�b�N�X�o�b�t�@
	
	// --�萔�o�b�t�@-- //
	constBuff_(nullptr)
#pragma endregion
{
#pragma region �萔�o�b�t�@����������
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬
	HRESULT result;

	// �萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// �萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferData3D) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	ConstBufferData3D* constMap;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	// �}�b�s���O�I��
	constBuff_->Unmap(0, nullptr);
#pragma endregion

	// --�������e�s��̌v�Z-- //
	matProjection_ = XMMatrixPerspectiveFovLH(
		Util::Degree2Radian(45.0f),// -----------> �㉺��p45�x
		(float)WinAPI::GetWidth() / WinAPI::GetHeight(),// -> �A�X�y�N�g��i��ʉ���/��ʏc���j
		0.1f, 1000.0f// ------------------------> �O�[�A���[
	);
}

void Object3D::Update(Camera* camera)
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬
	HRESULT result;

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

	// --�}�b�s���O-- //
	ConstBufferData3D* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	// --�萔�o�b�t�@�փf�[�^�]��-- //
	constMap->mat = matWorld * camera->GetMatView() * matProjection_;// -> �s��
	constMap->color = color_;// -> �F

	// --�q���������-- //
	constBuff_->Unmap(0, nullptr);
}

void Object3D::Draw(int textureHandle)
{
	// --SRV�q�[�v�̃n���h�����擾-- //
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// --�n���h�����w�肳�ꂽ���܂Ői�߂�-- //
	srvGpuHandle.ptr += textureHandle;

	// --�w�肳�ꂽSRV�����[�g�p�����[�^1�Ԃɐݒ�-- //
	DX12Cmd::GetCmdList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// --���_�o�b�t�@�r���[�̐ݒ�R�}���h-- //
	DX12Cmd::GetCmdList()->IASetVertexBuffers(0, 1, &vbView_);

	// --�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h-- //
	DX12Cmd::GetCmdList()->IASetIndexBuffer(&ibView_);

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h-- //
	DX12Cmd::GetCmdList()->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	//// --�`��R�}���h-- //
	DX12Cmd::GetCmdList()->DrawIndexedInstanced(static_cast<UINT>(indices_.size()), 1, 0, 0, 0);
}

void Object3D::PreDraw()
{
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

void Object3D::CreateCube()
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬
	HRESULT result;

#pragma region ���_�f�[�^�쐬
	// �����̒��_�f�[�^
	Vertices3D vertices[] = {
		// �O��
		{{ -0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}},// -> ���� 0
		{{ -0.5f,  0.5f, -0.5f}, {}, {0.0f, 0.0f}},// -> ���� 1
		{{  0.5f, -0.5f, -0.5f}, {}, {1.0f, 1.0f}},// -> �E�� 2
		{{  0.5f,  0.5f, -0.5f}, {}, {1.0f, 0.0f}},// -> �E�� 3

		// ���
		{{ -0.5f, -0.5f,  0.5f}, {}, {0.0f, 1.0f}},// -> ���� 4
		{{ -0.5f,  0.5f,  0.5f}, {}, {0.0f, 0.0f}},// -> ���� 5
		{{  0.5f, -0.5f,  0.5f}, {}, {1.0f, 1.0f}},// -> �E�� 6
		{{  0.5f,  0.5f,  0.5f}, {}, {1.0f, 0.0f}},// -> �E�� 7

		// ����
		{{ -0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}},// -> ���� 8
		{{ -0.5f, -0.5f,  0.5f}, {}, {0.0f, 0.0f}},// -> ���� 9
		{{ -0.5f,  0.5f, -0.5f}, {}, {1.0f, 1.0f}},// -> �E�� 10
		{{ -0.5f,  0.5f,  0.5f}, {}, {1.0f, 0.0f}},// -> �E�� 11

		// �E��
		{{  0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}},// -> ���� 12
		{{  0.5f, -0.5f,  0.5f}, {}, {0.0f, 0.0f}},// -> ���� 13
		{{  0.5f,  0.5f, -0.5f}, {}, {1.0f, 1.0f}},// -> �E�� 14
		{{  0.5f,  0.5f,  0.5f}, {}, {1.0f, 0.0f}},// -> �E�� 15

		// ����
		{{ -0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}},// -> ���� 16
		{{  0.5f, -0.5f, -0.5f}, {}, {0.0f, 0.0f}},// -> ���� 17
		{{ -0.5f, -0.5f,  0.5f}, {}, {1.0f, 1.0f}},// -> �E�� 18
		{{  0.5f, -0.5f,  0.5f}, {}, {1.0f, 0.0f}},// -> �E�� 19

		// ���
		{{ -0.5f,  0.5f, -0.5f}, {}, {0.0f, 1.0f}},// -> ���� 20
		{{  0.5f,  0.5f, -0.5f}, {}, {0.0f, 0.0f}},// -> ���� 21
		{{ -0.5f,  0.5f,  0.5f}, {}, {1.0f, 1.0f}},// -> �E�� 22
		{{  0.5f,  0.5f,  0.5f}, {}, {1.0f, 0.0f}},// -> �E�� 23
	};

	// ���_�f�[�^���R�s�[
	for (size_t i = 0; i < _countof(vertices); i++) {
		vertices_.push_back(vertices[i]);
	}
#pragma endregion

#pragma region �C���f�b�N�X�f�[�^�쐬
	// �����̃C���f�b�N�X�f�[�^
	uint16_t indices[] = {
		// �O��
		0, 1, 2,// -> �O�p�`1��
		2, 1, 3,// -> �O�p�`2��

		// ���
		6, 5, 4,// -> �O�p�`3��
		5, 6, 7,// -> �O�p�`4��

		// ����
		8,  9, 10,// -> �O�p�`5��
		11, 10, 9,// -> �O�p�`6��

		// �E��
		14, 13, 12,// -> �O�p�`8��
		13, 14, 15,// -> �O�p�`7��

		// ����
		16, 17, 18,// -> �O�p�`9��
		19, 18, 17,// -> �O�p�`10��

		// ���
		22, 21, 20,// -> �O�p�`11��
		21, 22, 23,// -> �O�p�`12��
	};

	// �C���f�b�N�X�f�[�^���R�s�[
	for (size_t i = 0; i < _countof(indices); i++) {
		indices_.push_back(indices[i]);
	}
#pragma endregion

#pragma region �@���v�Z
	for (size_t i = 0; i < indices_.size() / 3; i++) {
		// --�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices_[i * 3 + 0];
		unsigned short index1 = indices_[i * 3 + 1];
		unsigned short index2 = indices_[i * 3 + 2];

		// --�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices_[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices_[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices_[index2].pos);

		// --p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		// --�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		// --���K��
		normal = XMVector3Normalize(normal);

		// --���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices_[index0].normal, normal);
		XMStoreFloat3(&vertices_[index1].normal, normal);
		XMStoreFloat3(&vertices_[index2].normal, normal);
	}
#pragma endregion

	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̍쐬
	CreateBuffer();
}

void Object3D::CreateModel()
{
#pragma region ���f����ǂݍ���
	// �t�@�C���X�g���[��
	ifstream file;

	// [obj]�t�@�C�����J��
	file.open("Resources/cube/cube.obj");

	// �t�@�C���I�[�v�����s���`�F�b�N
	assert(!file.fail());

	vector<XMFLOAT3> positions;// -> ���_���W
	vector<XMFLOAT3> normals;// -> �@���x�N�g��
	vector<XMFLOAT2> texcoords;// -> �e�N�X�`��UV

	// 1�s���ǂݍ���
	string line;

	while (getline(file, line)) {
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		// �擪������[v]�Ȃ璸�_���W
		if (key == "v") {
			// X, Y, Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			// ���W�f�[�^�ɒǉ�
			positions.emplace_back(position);

			// ���_�f�[�^�ɒǉ�
			Vertices3D vertex{};
			vertex.pos = position;
			vertices_.emplace_back(vertex);
		}

		// �擪������[f]�Ȃ�|���S��(�O�p�`)
		if (key == "f") {
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// ���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				istringstream index_stream(index_string);
				unsigned short indexPosition;
				index_stream >> indexPosition;

				// ���_�C���f�b�N�X�ɒǉ�
				indices_.emplace_back(indexPosition - 1);
			}
		}
	}

	// �t�@�C�������
	file.close();
#pragma endregion

#pragma region �@���v�Z
	for (size_t i = 0; i < indices_.size() / 3; i++) {
		// --�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices_[i * 3 + 0];
		unsigned short index1 = indices_[i * 3 + 1];
		unsigned short index2 = indices_[i * 3 + 2];

		// --�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices_[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices_[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices_[index2].pos);

		// --p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		// --�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		// --���K��
		normal = XMVector3Normalize(normal);

		// --���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices_[index0].normal, normal);
		XMStoreFloat3(&vertices_[index1].normal, normal);
		XMStoreFloat3(&vertices_[index2].normal, normal);
	}
#pragma endregion

	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̍쐬
	CreateBuffer();
}

void Object3D::CreateBuffer()
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬
	HRESULT result;

#pragma region ���_�o�b�t�@�쐬
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

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
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// --���_�o�b�t�@�r���[�̍쐬-- //
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();// -> GPU���z�A�h���X
	vbView_.SizeInBytes = sizeVB;// -> ���_�o�b�t�@�̃T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);// -> ���_1���̃f�[�^�T�C�Y

	// --Map�����Ń��C����������GPU�̃�������R�Â���-- //
	Vertices3D* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// --�S���_�ɑ΂���-- //
	for (int i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}

	// --�q���������-- //
	vertBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�쐬
	// --�C���f�b�N�X�f�[�^�S�̂̃T�C�Y-- //
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

	// --���\�[�X�ݒ�-- //
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
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
	for (size_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];
	}

	// --�}�b�s���O����-- //
	indexBuff_->Unmap(0, nullptr);
#pragma endregion
}
