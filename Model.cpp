#include "Model.h"

// --���f���ǂݍ��݂̂���-- //
#include <fstream>
#include <sstream>
using namespace std;

#include "FumiEngine.h"

Model* Model::CreateModel(std::string fileName) {
	Model* model = new Model();
	model->LoadModel(fileName);

	return model;
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

	// 1�s���ǂݍ���
	string line;
	while (getline(file, line)) {
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