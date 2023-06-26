#pragma once
#include <d3dx12.h>
#include <wrl.h>

#include <vector>

using namespace Microsoft::WRL;

// シェーダーの種類
enum ShaderType {
	VS,
	PS
};

class PipelineObj
{
#pragma region メンバ変数
private:
	// シェーダーオブジェクト
	ComPtr<ID3DBlob> vsBlob_;	// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob_;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob_;	// エラーオブジェクト

	// 頂点レイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_;

	// ルートパラメータ
	std::vector<CD3DX12_ROOT_PARAMETER> rootParams_;

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;

	// パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_;
#pragma endregion

#pragma region クラス間共通メンバ変数
private:
	// デスクリプタレンジの設定
	static CD3DX12_DESCRIPTOR_RANGE descRangeSRV_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PipelineObj();

	/// <summary>
	/// シェーダー読み込み
	/// </summary>
	/// <param name="fileName"> シェーダーのファイル名 </param>
	/// <param name="shaderType"> シェーダーの種類 </param>
	void LoadShader(std::string fileName, ShaderType shaderType);

	/// <summary>
	/// インプットレイアウトに新規設定を追加
	/// </summary>
	/// <param name="semanticName"> セマンティック名 </param>
	/// <param name="format"> リソースデータ形式(要素数とビット数を表す) </param>
	/// <param name="index"> 同じセマンティック名が複数あるとき使うインデックス </param>
	void AddInputLayout(const char* semanticName, DXGI_FORMAT format, uint16_t index = 0);

	/// <summary>
	/// ルートパラメータ作成
	/// </summary>
	/// <param name="addValue"> 作成するルートパラメータの数 </param>
	void CreateRootParams(uint16_t addValue);

	void CreatePipeline();

private:
	/// <summary>
	/// シェーダーの読み込みとコンパイル
	/// </summary>
	/// <param name="fileName"> シェーダーのファイル名 </param>
	/// <param name="target"> シェーダーモデル指定 </param>
	/// <param name="shaderBlob"> シェーダーオブジェクト </param>
	void ShaderCompileFromFile(std::string fileName, LPCSTR target, ID3D10Blob** shaderBlob);
#pragma endregion

#pragma region ゲッター
public:
	/// <summary>
	/// パイプラインステート取得
	/// </summary>
	/// <returns> パイプラインステート </returns>
	ID3D12PipelineState* GetPipelineState() { return pipelineState_.Get(); }

	/// <summary>
	/// ルートシグネチャを取得
	/// </summary>
	/// <returns> ルートシグネチャ </returns>
	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }
#pragma endregion
};