#include "Model.h"
#include "Texture.h"
#include "DX12Cmd.h"

#include <fstream>
#include <sstream>

using namespace std;

Model::Model(string fileName) :
#pragma region 初期化リスト
	// 頂点データ
	vertex_{},				// 頂点データ
	vertexView_{},			// 頂点バッファービュー
	vertexBuff_(nullptr),	// 頂点バッファ
	
	// インデックスデータ
	index_{},			// インデックスデータ
	indexView_{},		// インデックスバッファビュー
	indexBuff_(nullptr),// インデックスバッファ
	
	// マテリアルデータ
	material_{},			// マテリアルデータ
	materialBuff_(nullptr),	// マテリアルバッファ

	// テクスチャハンドル
	textureHandle_(0)
#pragma endregion
{
	// モデル読み込み
	LoadModel(fileName);

	// 各バッファ作成
	CreateVertexBuff();		// 頂点バッファ
	CreateIndexBuff();		// インデックスバッファ
	CreateMaterialBuff();	// マテリアルバッファ
}

void Model::Draw() {
	// コマンドリスト取得
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// インスタンス取得
	Texture* tex = Texture::GetInstance();

	// SRVヒープのハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = tex->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// ハンドルを指定された分まで進める
	srvGpuHandle.ptr += textureHandle_;

	// 定数バッファビュー（CBV）の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(2, materialBuff_->GetGPUVirtualAddress());

	// 指定されたSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(0, srvGpuHandle);

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vertexView_);

	// インデックスバッファビューの設定コマンド
	cmdList->IASetIndexBuffer(&indexView_);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(static_cast<UINT>(index_.size()), 1, 0, 0, 0);
}

void Model::LoadModel(string name)
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

	vector<float3> positions;	// 頂点座標
	vector<float3> normals;		// 法線ベクトル
	vector<float2> texcoords;	// テクスチャUV

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
			float3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			// 座標データに追加
			positions.emplace_back(position);
		}

		// 先頭文字列が[vt]ならテクスチャ
		if (key == "vt") {
			// U, V成分読み込み
			float2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			// V方向反転
			texcoord.y = 1.0f - texcoord.y;

			// テクスチャ座標データ追加
			texcoords.emplace_back(texcoord);
		}

		// 先頭文字列が[vn]なら法線ベクトル
		if (key == "vn") {
			// X, Y, Z成分読み込み
			float3 normal{};
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
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertex_.emplace_back(vertex);

				// 頂点インデックスに追加
				index_.emplace_back(static_cast<unsigned short>(index_.size()));
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

void Model::LoadMaterial(const string& directoryPath, const string& fileName) {
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

void Model::CreateVertexBuff()
{
	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// 関数実行の成否を判別用の変数
	HRESULT result;

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertex_[0]) * vertex_.size());

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		// ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuff_));
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	vertexView_.BufferLocation = vertexBuff_->GetGPUVirtualAddress();// GPU仮想アドレス
	vertexView_.SizeInBytes = sizeVB;				// 頂点バッファのサイズ
	vertexView_.StrideInBytes = sizeof(vertex_[0]);	// 頂点1つ分のデータサイズ

	// Map処理でメインメモリとGPUのメモリを紐づける
	Vertex* vertMap = nullptr;
	result = vertexBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// 全頂点に対して
	for (size_t i = 0; i < vertex_.size(); i++)
	{
		vertMap[i] = vertex_[i]; // 座標をコピー
	}

	// 繋がりを解除
	vertexBuff_->Unmap(0, nullptr);
}

void Model::CreateIndexBuff()
{
	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// 関数実行の成否を判別用の変数
	HRESULT result;

	// インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * index_.size());

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		// ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// インデックスバッファの生成
	result = device->CreateCommittedResource(
		&heapProp,// ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);

	// インデックスバッファビュー作成
	indexView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	indexView_.Format = DXGI_FORMAT_R16_UINT;
	indexView_.SizeInBytes = sizeIB;

	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));

	// --全インデックスに対して-- //
	for (size_t i = 0; i < index_.size(); i++)
	{
		indexMap[i] = index_[i];
	}

	// --マッピング解除-- //
	indexBuff_->Unmap(0, nullptr);
}

void Model::CreateMaterialBuff()
{
	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// 関数実行の成否を判別用の変数
	HRESULT result;

	// 定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(MaterialBuffer) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&materialBuff_));
	assert(SUCCEEDED(result));

	// マテリアル定数バッファのマッピング
	MaterialBuffer* materialMap;
	result = materialBuff_->Map(0, nullptr, (void**)&materialMap);
	assert(SUCCEEDED(result));
	materialMap->ambient = material_.ambient;
	materialMap->diffuse = material_.diffuse;
	materialMap->specular = material_.specular;
	materialMap->alpha = material_.alpha;
	materialBuff_->Unmap(0, nullptr);
}