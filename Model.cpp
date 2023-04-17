#include "Model.h"

#include "Texture.h"

// --���f���ǂݍ��݂̂���-- //
#include <fstream>
#include <sstream>
using namespace std;

// --DirextX12-- //
#include "DX12Cmd.h"

ID3D12GraphicsCommandList* Model::cmdList_ = nullptr;// -> �R�}���h���X�g

Model* Model::CreateModel(std::string fileName) {
	Model* model = new Model();
	model->LoadModel(fileName);

	// �萔�o�b�t�@�쐬
	model->CreateVertexBuff();// ---> ���_�o�b�t�@
	model->CreateIndexBuff();// ----> �C���f�b�N�X�o�b�t�@
	model->CreateMaterialBuff();// -> �}�e���A���o�b�t�@

	return model;
}

void Model::Initialize(ID3D12GraphicsCommandList* cmdList) {
	cmdList_ = cmdList;
}

void Model::Draw() {
	// --SRV�q�[�v�̃n���h�����擾-- //
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// --�n���h�����w�肳�ꂽ���܂Ői�߂�-- //
	srvGpuHandle.ptr += textureHandle_;

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h-- //
	cmdList_->SetGraphicsRootConstantBufferView(1, materialBuff_->GetGPUVirtualAddress());

	// --�w�肳�ꂽSRV�����[�g�p�����[�^1�Ԃɐݒ�-- //
	cmdList_->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	// --���_�o�b�t�@�r���[�̐ݒ�R�}���h-- //
	cmdList_->IASetVertexBuffers(0, 1, &vbView_);

	// --�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h-- //
	cmdList_->IASetIndexBuffer(&ibView_);

	//// --�`��R�}���h-- //
	cmdList_->DrawIndexedInstanced(static_cast<UINT>(indexes_.size()), 1, 0, 0, 0);
}

void Model::LoadModel(std::string name)
{
	// �t�@�C���X�g���[��
	std::ifstream file;

	// .obj�t�@�C�����J��
	const string modelName = name;
	const string fileName = modelName + ".obj";
	const string directoryPath = "Resources/" + modelName + "/";
	file.open(directoryPath + fileName);

	// �t�@�C���I�[�v�����s���`�F�b�N
	assert(!file.fail());

	vector<XMFLOAT3> positions;// -> ���_���W
	vector<XMFLOAT3> normals;// ---> �@���x�N�g��
	vector<XMFLOAT2> texcoords;// -> �e�N�X�`��UV

	size_t num = 0;

	// 1�s���ǂݍ���
	string line;
	while (getline(file, line)) {
		num++;

		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

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
		}

		// �擪������[vt]�Ȃ�e�N�X�`��
		if (key == "vt") {
			// U, V�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			// V�������]
			texcoord.y = 1.0 - texcoord.y;

			// �e�N�X�`�����W�f�[�^�ǉ�
			texcoords.emplace_back(texcoord);
		}

		// �擪������[vn]�Ȃ�@���x�N�g��
		if (key == "vn") {
			// X, Y, Z�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			// �@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		// �擪������[f]�Ȃ�|���S��(�O�p�`)
		if (key == "f") {
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// ���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);// -> �X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);// -> �X���b�V�����΂�
				index_stream >> indexNormal;

				// ���_�f�[�^�̒ǉ�
				Vertex3D vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertexes_.emplace_back(vertex);

				// ���_�C���f�b�N�X�ɒǉ�
				indexes_.emplace_back((unsigned short)indexes_.size());
			}
		}

		// �擪������[mtllib]�Ȃ�}�e���A��
		if (key == "mtllib") {
			string fileName;
			line_stream >> fileName;

			// �}�e���A���ǂݍ���
			LoadMaterial(directoryPath, fileName);
		}
	}

	num = num;

	// �t�@�C�������
	file.close();
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& fileName) {
	// �t�@�C���X�g���[��
	ifstream file;

	// �}�e���A���t�@�C�����J��
	file.open(directoryPath + fileName);

	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) assert(0);

	// 1�s���ǂݍ���
	string line;
	while (getline(file, line)) {
		// 1�s���̕�������X�g���[���ɕϊ�
		istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		// �擪�̃^�u�����͖�������
		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		// �擪������[newmtl]�Ȃ�}�e���A����
		if (key == "newmtl") {
			// �}�e���A�����ǂݍ���
			line_stream >> material_.name;
		}

		// �擪������[Ka]�Ȃ�A���r�G���g�F
		if (key == "Ka") {
			line_stream >> material_.ambient.x;
			line_stream >> material_.ambient.y;
			line_stream >> material_.ambient.z;
		}

		// �擪������[Kd]�Ȃ�f�B�t�F�[�Y�E
		if (key == "Kd") {
			line_stream >> material_.diffuse.x;
			line_stream >> material_.diffuse.y;
			line_stream >> material_.diffuse.z;
		}

		// �擪������[Ks]�Ȃ�X�y�L�����[�F
		if (key == "Ks") {
			line_stream >> material_.specular.x;
			line_stream >> material_.specular.y;
			line_stream >> material_.specular.z;
		}

		// �擪������[map_Kd]�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd") {
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material_.textureFilename;

			// �e�N�X�`���ǂݍ���
			textureHandle_ = LoadTexture(directoryPath + material_.textureFilename);
		}
	}

	// �t�@�C�������
	file.close();
}

void Model::CreateVertexBuff()
{
	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertexes_[0]) * vertexes_.size());

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
	vbView_.StrideInBytes = sizeof(vertexes_[0]);// -> ���_1���̃f�[�^�T�C�Y

	// --Map�����Ń��C����������GPU�̃�������R�Â���-- //
	Vertex3D* vertMap = nullptr;
	result = vertexBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// --�S���_�ɑ΂���-- //
	for (int i = 0; i < vertexes_.size(); i++)
	{
		vertMap[i] = vertexes_[i]; // ���W���R�s�[
	}

	// --�q���������-- //
	vertexBuff_->Unmap(0, nullptr);
}

void Model::CreateIndexBuff()
{
	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// --�C���f�b�N�X�f�[�^�S�̂̃T�C�Y-- //
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexes_.size());

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
	for (size_t i = 0; i < indexes_.size(); i++)
	{
		indexMap[i] = indexes_[i];
	}

	// --�}�b�s���O����-- //
	indexBuff_->Unmap(0, nullptr);
}

void Model::CreateMaterialBuff()
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
	materialMap->ambient = material_.ambient;
	materialMap->diffuse = material_.diffuse;
	materialMap->specular = material_.specular;
	materialMap->alpha = material_.alpha;
	materialBuff_->Unmap(0, nullptr);
}