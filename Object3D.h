#pragma once
// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --数学関数-- //
#include <DirectXMath.h>
using namespace DirectX;

// --ComPtr用-- //
#include <wrl.h>
using namespace Microsoft::WRL;

// --シーケンスコンテナの一種-- //
#include <vector>

// --頂点データ-- //
struct Vertices3D {
	// --XYZ座標-- //
	XMFLOAT3 pos;

	// --法線データ-- //
	XMFLOAT3 normal;

	// --uv座標-- //
	XMFLOAT2 uv;
};

// --定数バッファ用データ構造体（行列と色）-- //
struct ConstBufferData3D {
	// --色（RBGA）-- //
	XMFLOAT4 color;

	// --行列-- //
	XMMATRIX mat;
};

// --描画タイプ-- //
enum DrawType {
	None = 0,// -> なし
	Cube = 1,// -> 立方体
	Model = 2// -> モデル
};

class Object3D {
	/// メンバ変数
public:
	// 座標、回転角、スケール
	XMFLOAT3 position_;
	XMFLOAT3 rotation_;
	XMFLOAT3 scale_;

	// 色
	XMFLOAT4 color_;

private:
	// --頂点データ-- //
	std::vector<Vertices3D> vertices_;// -> 頂点データ
	D3D12_VERTEX_BUFFER_VIEW vbView_;// -> 頂点バッファビュー
	ComPtr<ID3D12Resource> vertBuff_;// -> 頂点バッファ

	// --インデックスデータ-- //
	std::vector<uint16_t> indices_;// -> インデックスデータ
	D3D12_INDEX_BUFFER_VIEW ibView_;// -> インデックスバッファビュー
	ComPtr<ID3D12Resource> indexBuff_;// -> インデックスバッファ

	// --定数バッファ-- //
	ComPtr<ID3D12Resource> constBuff_;

	// --ビュー変換行列-- //
	XMMATRIX matView_;
	XMFLOAT3 eye_;
	XMFLOAT3 target_;
	XMFLOAT3 up_;

	// --透視投影行列の計算-- //
	XMMATRIX matProjection_;

	// --描画タイプ-- //
	int drawType_;

	/// メンバ関数
public:
	// --コンストラクタ-- //
	Object3D();

	// --立方体の頂点データとインデックスデータを作成-- //
	void CreateCube();

	// --更新処理-- //
	void Update();

	// --描画処理-- //
	void Draw(int textureHandle = 0);

	// --描画前処理-- //
	static void PreDraw();
};