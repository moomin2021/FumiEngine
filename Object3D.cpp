#include "Object3D.h"

// --DirextX12-- //
#include "DX12Cmd.h"

// --テクスチャクラス-- //
#include "Texture.h"

// --便利系関数-- //
#include "Util.h"

// --モデル読み込みのため-- //
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

Object3D::Object3D() :
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
	resdesc.Width = (sizeof(ConstBufferData3D) + 0xff) & ~0xff;
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
	ConstBufferData3D* constMap;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	// マッピング終了
	constBuff_->Unmap(0, nullptr);
#pragma endregion

	// --透視投影行列の計算-- //
	matProjection_ = XMMatrixPerspectiveFovLH(
		Util::Degree2Radian(45.0f),// -----------> 上下画角45度
		(float)WinAPI::GetWidth() / WinAPI::GetHeight(),// -> アスペクト比（画面横幅/画面縦幅）
		0.1f, 1000.0f// ------------------------> 前端、奥端
	);
}

void Object3D::Update(Camera* camera)
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
	ConstBufferData3D* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	// --定数バッファへデータ転送-- //
	constMap->mat = matWorld * camera->GetMatView() * matProjection_;// -> 行列
	constMap->color = color_;// -> 色

	// --繋がりを解除-- //
	constBuff_->Unmap(0, nullptr);
}

void Object3D::Draw(int textureHandle)
{
	// --SRVヒープのハンドルを取得-- //
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// --ハンドルを指定された分まで進める-- //
	srvGpuHandle.ptr += textureHandle;

	// --指定されたSRVをルートパラメータ1番に設定-- //
	DX12Cmd::GetCmdList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// --頂点バッファビューの設定コマンド-- //
	DX12Cmd::GetCmdList()->IASetVertexBuffers(0, 1, &vbView_);

	// --インデックスバッファビューの設定コマンド-- //
	DX12Cmd::GetCmdList()->IASetIndexBuffer(&ibView_);

	// --定数バッファビュー（CBV）の設定コマンド-- //
	DX12Cmd::GetCmdList()->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	//// --描画コマンド-- //
	DX12Cmd::GetCmdList()->DrawIndexedInstanced(static_cast<UINT>(indices_.size()), 1, 0, 0, 0);
}

void Object3D::PreDraw()
{
	// パイプラインステートの設定
	DX12Cmd::GetCmdList()->SetPipelineState(DX12Cmd::GetObject3DPipeline().pipelineState.Get());
	// ルートシグネチャの設定
	DX12Cmd::GetCmdList()->SetGraphicsRootSignature(DX12Cmd::GetObject3DPipeline().rootSignature.Get());
	// プリミティブ形状を設定
	DX12Cmd::GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// --デスクリプタヒープの配列をセットするコマンド-- //
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
	DX12Cmd::GetCmdList()->SetDescriptorHeaps(1, ppHeaps);
}

void Object3D::CreateCube()
{
	// 関数が成功したかどうかを判別する用変数
	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成
	HRESULT result;

#pragma region 頂点データ作成
	// 立方体頂点データ
	Vertices3D vertices[] = {
		// 前面
		{{ -0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}},// -> 左下 0
		{{ -0.5f,  0.5f, -0.5f}, {}, {0.0f, 0.0f}},// -> 左上 1
		{{  0.5f, -0.5f, -0.5f}, {}, {1.0f, 1.0f}},// -> 右下 2
		{{  0.5f,  0.5f, -0.5f}, {}, {1.0f, 0.0f}},// -> 右上 3

		// 後面
		{{ -0.5f, -0.5f,  0.5f}, {}, {0.0f, 1.0f}},// -> 左下 4
		{{ -0.5f,  0.5f,  0.5f}, {}, {0.0f, 0.0f}},// -> 左上 5
		{{  0.5f, -0.5f,  0.5f}, {}, {1.0f, 1.0f}},// -> 右下 6
		{{  0.5f,  0.5f,  0.5f}, {}, {1.0f, 0.0f}},// -> 右上 7

		// 左面
		{{ -0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}},// -> 左下 8
		{{ -0.5f, -0.5f,  0.5f}, {}, {0.0f, 0.0f}},// -> 左上 9
		{{ -0.5f,  0.5f, -0.5f}, {}, {1.0f, 1.0f}},// -> 右下 10
		{{ -0.5f,  0.5f,  0.5f}, {}, {1.0f, 0.0f}},// -> 右上 11

		// 右面
		{{  0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}},// -> 左下 12
		{{  0.5f, -0.5f,  0.5f}, {}, {0.0f, 0.0f}},// -> 左上 13
		{{  0.5f,  0.5f, -0.5f}, {}, {1.0f, 1.0f}},// -> 右下 14
		{{  0.5f,  0.5f,  0.5f}, {}, {1.0f, 0.0f}},// -> 右上 15

		// 下面
		{{ -0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}},// -> 左下 16
		{{  0.5f, -0.5f, -0.5f}, {}, {0.0f, 0.0f}},// -> 左上 17
		{{ -0.5f, -0.5f,  0.5f}, {}, {1.0f, 1.0f}},// -> 右下 18
		{{  0.5f, -0.5f,  0.5f}, {}, {1.0f, 0.0f}},// -> 右上 19

		// 上面
		{{ -0.5f,  0.5f, -0.5f}, {}, {0.0f, 1.0f}},// -> 左下 20
		{{  0.5f,  0.5f, -0.5f}, {}, {0.0f, 0.0f}},// -> 左上 21
		{{ -0.5f,  0.5f,  0.5f}, {}, {1.0f, 1.0f}},// -> 右下 22
		{{  0.5f,  0.5f,  0.5f}, {}, {1.0f, 0.0f}},// -> 右上 23
	};

	// 頂点データをコピー
	for (size_t i = 0; i < _countof(vertices); i++) {
		vertices_.push_back(vertices[i]);
	}
#pragma endregion

#pragma region インデックスデータ作成
	// 立方体インデックスデータ
	uint16_t indices[] = {
		// 前面
		0, 1, 2,// -> 三角形1つ目
		2, 1, 3,// -> 三角形2つ目

		// 後面
		6, 5, 4,// -> 三角形3つ目
		5, 6, 7,// -> 三角形4つ目

		// 左面
		8,  9, 10,// -> 三角形5つ目
		11, 10, 9,// -> 三角形6つ目

		// 右面
		14, 13, 12,// -> 三角形8つ目
		13, 14, 15,// -> 三角形7つ目

		// 下面
		16, 17, 18,// -> 三角形9つ目
		19, 18, 17,// -> 三角形10つ目

		// 上面
		22, 21, 20,// -> 三角形11つ目
		21, 22, 23,// -> 三角形12つ目
	};

	// インデックスデータをコピー
	for (size_t i = 0; i < _countof(indices); i++) {
		indices_.push_back(indices[i]);
	}
#pragma endregion

#pragma region 法線計算
	for (size_t i = 0; i < indices_.size() / 3; i++) {
		// --三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short index0 = indices_[i * 3 + 0];
		unsigned short index1 = indices_[i * 3 + 1];
		unsigned short index2 = indices_[i * 3 + 2];

		// --三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices_[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices_[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices_[index2].pos);

		// --p0->p1ベクトル、p0->p2ベクトルを計算（ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		// --外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);

		// --正規化
		normal = XMVector3Normalize(normal);

		// --求めた法線を頂点データに代入
		XMStoreFloat3(&vertices_[index0].normal, normal);
		XMStoreFloat3(&vertices_[index1].normal, normal);
		XMStoreFloat3(&vertices_[index2].normal, normal);
	}
#pragma endregion

	// 頂点バッファとインデックスバッファの作成
	CreateBuffer();
}

void Object3D::CreateModel()
{
#pragma region モデルを読み込む
	// ファイルストリーム
	ifstream file;

	// [obj]ファイルを開く
	file.open("Resources/cube/cube.obj");

	// ファイルオープン失敗をチェック
	assert(!file.fail());

	vector<XMFLOAT3> positions;// -> 頂点座標
	vector<XMFLOAT3> normals;// -> 法線ベクトル
	vector<XMFLOAT2> texcoords;// -> テクスチャUV

	// 1行ずつ読み込む
	string line;

	while (getline(file, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		istringstream line_stream(line);

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

			// 頂点データに追加
			Vertices3D vertex{};
			vertex.pos = position;
			vertices_.emplace_back(vertex);
		}

		// 先頭文字列が[f]ならポリゴン(三角形)
		if (key == "f") {
			// 半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				istringstream index_stream(index_string);
				unsigned short indexPosition;
				index_stream >> indexPosition;

				// 頂点インデックスに追加
				indices_.emplace_back(indexPosition - 1);
			}
		}
	}

	// ファイルを閉じる
	file.close();
#pragma endregion

#pragma region 法線計算
	for (size_t i = 0; i < indices_.size() / 3; i++) {
		// --三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short index0 = indices_[i * 3 + 0];
		unsigned short index1 = indices_[i * 3 + 1];
		unsigned short index2 = indices_[i * 3 + 2];

		// --三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices_[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices_[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices_[index2].pos);

		// --p0->p1ベクトル、p0->p2ベクトルを計算（ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		// --外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);

		// --正規化
		normal = XMVector3Normalize(normal);

		// --求めた法線を頂点データに代入
		XMStoreFloat3(&vertices_[index0].normal, normal);
		XMStoreFloat3(&vertices_[index1].normal, normal);
		XMStoreFloat3(&vertices_[index2].normal, normal);
	}
#pragma endregion

	// 頂点バッファとインデックスバッファの作成
	CreateBuffer();
}

void Object3D::CreateBuffer()
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
