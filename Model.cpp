#include "Model.h"

// --DirextX12-- //
#include "DX12Cmd.h"

// --テクスチャクラス-- //
#include "Texture.h"

// --便利系関数-- //
#include "Util.h"

#include "FumiEngine.h"

// --モデル読み込みのため-- //
#include <fstream>
#include <sstream>
using namespace std;

Model::Model() :
#pragma region 初期化リスト
	// 座標、回転角、スケール
	position_{ 0.0f, 0.0f, 0.0f },
	rotation_{ 0.0f, 0.0f, 0.0f },
	scale_{ 1.0f, 1.0f, 1.0f },

	// 色
	color_{ 1.0f, 1.0f, 1.0f, 1.0f },

	// --透視投影行列の計算-- //
	matProjection_{},

	// --頂点データ-- //
	vertices_{},// -> 頂点データ
	vbView_{},// -> 頂点バッファビュー
	vertBuff_(nullptr),// -> 頂点バッファ

	// --インデックスデータ-- //
	indices_{},// -> インデックスデータ
	ibView_{},// -> インデックスバッファビュー
	indexBuff_(nullptr),// -> インデックスバッファ

	// --定数バッファ-- //
	constBuff_(nullptr)
#pragma endregion
{
#pragma region 定数バッファ初期化処理
	// 関数が成功したかどうかを判別する用変数
	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成
	HRESULT result;

	// 定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ModelBufferData) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = DX12Cmd::GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	ModelBufferData* constMap;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	// マッピング終了
	constBuff_->Unmap(0, nullptr);

	// マテリアル
	resdesc.Width = (sizeof(MaterialBufferData) + 0xff) & ~0xff;

	// マテリアル定数バッファの生成
	result = DX12Cmd::GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&materialBuff_));
	assert(SUCCEEDED(result));

	// マテリアル定数バッファのマッピング
	MaterialBufferData* materialMap;
	result = materialBuff_->Map(0, nullptr, (void**)&materialMap);
	assert(SUCCEEDED(result));
	materialMap->ambient = material_.ambient;
	materialMap->diffuse = material_.diffuse;
	materialMap->specular = material_.specular;
	materialMap->alpha = material_.alpha;
	materialBuff_->Unmap(0, nullptr);
#pragma endregion

	// --透視投影行列の計算-- //
	matProjection_ = XMMatrixPerspectiveFovLH(
		Util::Degree2Radian(45.0f),// -----------> 上下画角45度
		(float)WinAPI::GetWidth() / WinAPI::GetHeight(),// -> アスペクト比（画面横幅/画面縦幅）
		0.1f, 1000.0f// ------------------------> 前端、奥端
	);
}

void Model::CreateModel(std::string name)
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
				Vertices3D vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices_.emplace_back(vertex);

				// 頂点インデックスに追加
				indices_.emplace_back((unsigned short)indices_.size());
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

//#pragma region 法線計算
//	for (size_t i = 0; i < indices_.size() / 3; i++) {
//		// --三角形のインデックスを取り出して、一時的な変数に入れる
//		unsigned short index0 = indices_[i * 3 + 0];
//		unsigned short index1 = indices_[i * 3 + 1];
//		unsigned short index2 = indices_[i * 3 + 2];
//
//		// --三角形を構成する頂点座標をベクトルに代入
//		XMVECTOR p0 = XMLoadFloat3(&vertices_[index0].pos);
//		XMVECTOR p1 = XMLoadFloat3(&vertices_[index1].pos);
//		XMVECTOR p2 = XMLoadFloat3(&vertices_[index2].pos);
//
//		// --p0->p1ベクトル、p0->p2ベクトルを計算（ベクトルの減算）
//		XMVECTOR v1 = XMVectorSubtract(p1, p0);
//		XMVECTOR v2 = XMVectorSubtract(p2, p0);
//
//		// --外積は両方から垂直なベクトル
//		XMVECTOR normal = XMVector3Cross(v1, v2);
//
//		// --正規化
//		normal = XMVector3Normalize(normal);
//
//		// --求めた法線を頂点データに代入
//		XMStoreFloat3(&vertices_[index0].normal, normal);
//		XMStoreFloat3(&vertices_[index1].normal, normal);
//		XMStoreFloat3(&vertices_[index2].normal, normal);
//	}
//#pragma endregion

	// 頂点バッファとインデックスバッファを作成
	CreateBuffer();
}

void Model::Update(Camera* camera)
{
	// 関数が成功したかどうかを判別する用変数
	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成
	HRESULT result;

	// --スケール、回転、平行移動行列の計算-- //
	XMMATRIX matScale, matRot, matTrans;
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(Util::Degree2Radian(rotation_.z));
	matRot *= XMMatrixRotationX(Util::Degree2Radian(rotation_.x));
	matRot *= XMMatrixRotationY(Util::Degree2Radian(rotation_.y));
	matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// --ワールド行列の合成-- //
	XMMATRIX matWorld = XMMatrixIdentity();// -> 変形のリセット
	matWorld *= matScale;// -> ワールド行列にスケーリングを反映
	matWorld *= matRot;// -> ワールド行列に回転を反映
	matWorld *= matTrans;// -> ワールド行列に平行移動を反映

	// --マッピング-- //
	ModelBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	// --定数バッファへデータ転送-- //
	constMap->mat = matWorld * camera->GetMatView() * matProjection_;// -> 行列

	// --繋がりを解除-- //
	constBuff_->Unmap(0, nullptr);
}

void Model::Draw()
{
	// --SRVヒープのハンドルを取得-- //
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// --ハンドルを指定された分まで進める-- //
	srvGpuHandle.ptr += textureHandle_;

	// --定数バッファビュー（CBV）の設定コマンド-- //
	DX12Cmd::GetCmdList()->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
	DX12Cmd::GetCmdList()->SetGraphicsRootConstantBufferView(1, materialBuff_->GetGPUVirtualAddress());

	// --指定されたSRVをルートパラメータ1番に設定-- //
	DX12Cmd::GetCmdList()->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	// --頂点バッファビューの設定コマンド-- //
	DX12Cmd::GetCmdList()->IASetVertexBuffers(0, 1, &vbView_);

	// --インデックスバッファビューの設定コマンド-- //
	DX12Cmd::GetCmdList()->IASetIndexBuffer(&ibView_);

	//// --描画コマンド-- //
	DX12Cmd::GetCmdList()->DrawIndexedInstanced(static_cast<UINT>(indices_.size()), 1, 0, 0, 0);
}

void Model::PreDraw()
{
	// パイプラインステートの設定
	DX12Cmd::GetCmdList()->SetPipelineState(DX12Cmd::GetModelPipeline().pipelineState.Get());
	// ルートシグネチャの設定
	DX12Cmd::GetCmdList()->SetGraphicsRootSignature(DX12Cmd::GetModelPipeline().rootSignature.Get());
	// プリミティブ形状を設定
	DX12Cmd::GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// --デスクリプタヒープの配列をセットするコマンド-- //
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
	DX12Cmd::GetCmdList()->SetDescriptorHeaps(1, ppHeaps);
}

void Model::CreateBuffer()
{
	// 関数が成功したかどうかを判別する用変数
	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成
	HRESULT result;

#pragma region 頂点バッファ作成
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

	// --頂点バッファの設定-- //
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// --リソース設定-- //
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --頂点バッファの生成-- //
	result = DX12Cmd::GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// --頂点バッファビューの作成-- //
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();// -> GPU仮想アドレス
	vbView_.SizeInBytes = sizeVB;// -> 頂点バッファのサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);// -> 頂点1つ分のデータサイズ

	// --Map処理でメインメモリとGPUのメモリを紐づける-- //
	Vertices3D* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// --全頂点に対して-- //
	for (int i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i]; // 座標をコピー
	}

	// --繋がりを解除-- //
	vertBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region インデックスバッファ作成
	// --インデックスデータ全体のサイズ-- //
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

	// --リソース設定-- //
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --インデックスバッファの生成-- //
	result = DX12Cmd::GetDevice()->CreateCommittedResource(
		&heapProp,// -> ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,// -> リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);

	// --インデックスバッファビュー作成-- //
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;

	// --インデックスバッファをマッピング-- //
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));

	// --全インデックスに対して-- //
	for (size_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];
	}

	// --マッピング解除-- //
	indexBuff_->Unmap(0, nullptr);
#pragma endregion
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& fileName)
{
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
