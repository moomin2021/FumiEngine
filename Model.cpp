#include "Model.h"

// --モデル読み込みのため-- //
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
	// ファイルストリーム
	std::ifstream file;

	// .objファイルを開く
	const string modelName = name;
	const string fileName = modelName + ".obj";
	const string directoryPath = "Resources/" + modelName + "/";
	file.open(directoryPath + fileName);

	// ファイルオープン失敗をチェック
	assert(!file.fail());

	vector<XMFLOAT3> positions;// -> 頂点座標
	vector<XMFLOAT3> normals;// ---> 法線ベクトル
	vector<XMFLOAT2> texcoords;// -> テクスチャUV

	// 1行ずつ読み込む
	string line;
	while (getline(file, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		// 先頭文字列が[v]なら頂点座標
		if (key == "v") {
			// X, Y, Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			// 座標データに追加
			positions.emplace_back(position);
		}

		// 先頭文字列が[vt]ならテクスチャ
		if (key == "vt") {
			// U, V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			// V方向反転
			texcoord.y = 1.0 - texcoord.y;

			// テクスチャ座標データ追加
			texcoords.emplace_back(texcoord);
		}

		// 先頭文字列が[vn]なら法線ベクトル
		if (key == "vn") {
			// X, Y, Z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		// 先頭文字列が[f]ならポリゴン(三角形)
		if (key == "f") {
			// 半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);// -> スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);// -> スラッシュを飛ばす
				index_stream >> indexNormal;

				// 頂点データの追加
				Vertex3D vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertexes_.emplace_back(vertex);

				// 頂点インデックスに追加
				indexes_.emplace_back((unsigned short)indexes_.size());
			}
		}

		// 先頭文字列が[mtllib]ならマテリアル
		if (key == "mtllib") {
			string fileName;
			line_stream >> fileName;

			// マテリアル読み込み
			LoadMaterial(directoryPath, fileName);
		}
	}

	// ファイルを閉じる
	file.close();
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& fileName) {
	// ファイルストリーム
	ifstream file;

	// マテリアルファイルを開く
	file.open(directoryPath + fileName);

	// ファイルオープン失敗をチェック
	if (file.fail()) assert(0);

	// 1行ずつ読み込む
	string line;
	while (getline(file, line)) {
		// 1行分の文字列をストリームに変換
		istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		// 先頭のタブ文字は無視する
		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		// 先頭文字列が[newmtl]ならマテリアル名
		if (key == "newmtl") {
			// マテリアル名読み込み
			line_stream >> material_.name;
		}

		// 先頭文字列が[Ka]ならアンビエント色
		if (key == "Ka") {
			line_stream >> material_.ambient.x;
			line_stream >> material_.ambient.y;
			line_stream >> material_.ambient.z;
		}

		// 先頭文字列が[Kd]ならディフェーズ職
		if (key == "Kd") {
			line_stream >> material_.diffuse.x;
			line_stream >> material_.diffuse.y;
			line_stream >> material_.diffuse.z;
		}

		// 先頭文字列が[Ks]ならスペキュラー色
		if (key == "Ks") {
			line_stream >> material_.specular.x;
			line_stream >> material_.specular.y;
			line_stream >> material_.specular.z;
		}

		// 先頭文字列が[map_Kd]ならテクスチャファイル名
		if (key == "map_Kd") {
			// テクスチャのファイル名読み込み
			line_stream >> material_.textureFilename;

			// テクスチャ読み込み
			textureHandle_ = LoadTexture(directoryPath + material_.textureFilename);
		}
	}

	// ファイルを閉じる
	file.close();
}