#pragma once
// Direct3D 12 用
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// シーケンスコンテナの一種
#include <vector>

// 文字列クラス
#include <string>

// メッシュ一つのデータ構造体
struct Mesh {

};

class Model {
private:// メンバ変数
	// 各メッシュのデータ
	std::vector<Mesh> meshs_;

	static ID3D12GraphicsCommandList* cmdList_;// -> コマンドリスト

public:// メンバ関数
	/// <summary>
	/// モデル作成
	/// </summary>
	/// <param name="cmdList"></param>
	/// <returns></returns>
	static Model* CreateModel(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
};