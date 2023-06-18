#pragma once
#include <wrl.h>
#include <d3d12.h>

#pragma comment(lib, "d3d12.lib")

using namespace Microsoft::WRL;

// パイプラインセット
struct PipelineSet {
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelineState;

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
};

// スプライト用パイプライン生成
PipelineSet CreateSpritePipeline();

// オブジェクト3Dのパイプライン生成
PipelineSet CreateObject3DPipeline();