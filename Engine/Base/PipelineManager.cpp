#include "PipelineManager.h"
#include "DX12Cmd.h"
#include "Texture.h"

std::map<std::string, std::unique_ptr<PipelineObj>> PipelineManager::pipelineObj_ = {};

PipelineManager* PipelineManager::GetInstance()
{
	// インスタンス生成
	static PipelineManager inst;

	// インスタンスを返す
	return &inst;
}

void PipelineManager::PreDraw(std::string pipelineName, bool isDescHeap)
{
	// コマンドリスト取得
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// パイプライン取得
	PipelineSet pipeline = DX12Cmd::GetInstance()->GetPipelineObject3D();

	// SRVヒープ取得
	ID3D12DescriptorHeap* srvHeap = Texture::GetInstance()->GetSRVHeap();

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelineObj_[pipelineName]->GetPipelineState());

	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipelineObj_[pipelineName]->GetRootSignature());

	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// デスクリプタヒープの配列をセットするコマンド
	if (isDescHeap) {
		std::vector<ID3D12DescriptorHeap*> ppHeaps = { srvHeap };
		cmdList->SetDescriptorHeaps(1, ppHeaps.data());
	}
}

PipelineManager::PipelineManager() {
	AddPipeline("Object3D");
	pipelineObj_["Object3D"]->LoadShader("Resources/Shaders/Object3DPS.hlsl", PS);
	pipelineObj_["Object3D"]->LoadShader("Resources/Shaders/Object3DVS.hlsl", VS);
	pipelineObj_["Object3D"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineObj_["Object3D"]->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineObj_["Object3D"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Object3D"]->CreateRootParams(1, 3);
	pipelineObj_["Object3D"]->CreatePipeline(2);

	AddPipeline("Sprite");
	pipelineObj_["Sprite"]->LoadShader("Resources/Shaders/SpritePS.hlsl", PS);
	pipelineObj_["Sprite"]->LoadShader("Resources/Shaders/SpriteVS.hlsl", VS);
	pipelineObj_["Sprite"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Sprite"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Sprite"]->CreateRootParams(1, 1);
	pipelineObj_["Sprite"]->CreatePipeline(1);

	AddPipeline("Gaussian");
	pipelineObj_["Gaussian"]->LoadShader("Resources/Shaders/GaussianPS.hlsl", PS);
	pipelineObj_["Gaussian"]->LoadShader("Resources/Shaders/GaussianVS.hlsl", VS);
	pipelineObj_["Gaussian"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Gaussian"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Gaussian"]->CreateRootParams(2, 1);
	pipelineObj_["Gaussian"]->CreatePipeline(2);

	AddPipeline("HighLumi");
	pipelineObj_["HighLumi"]->LoadShader("Resources/Shaders/HighLumiPS.hlsl", PS);
	pipelineObj_["HighLumi"]->LoadShader("Resources/Shaders/HighLumiVS.hlsl", VS);
	pipelineObj_["HighLumi"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["HighLumi"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["HighLumi"]->CreateRootParams(2, 1);
	pipelineObj_["HighLumi"]->CreatePipeline(2);

	AddPipeline("Bloom");
	pipelineObj_["Bloom"]->LoadShader("Resources/Shaders/BloomPS.hlsl", PS);
	pipelineObj_["Bloom"]->LoadShader("Resources/Shaders/BloomVS.hlsl", VS);
	pipelineObj_["Bloom"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Bloom"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Bloom"]->CreateRootParams(2, 1);
	pipelineObj_["Bloom"]->CreatePipeline(1);

	AddPipeline("TextureBlend");
	pipelineObj_["TextureBlend"]->LoadShader("Resources/Shaders/TextureBlendPS.hlsl", PS);
	pipelineObj_["TextureBlend"]->LoadShader("Resources/Shaders/TextureBlendVS.hlsl", VS);
	pipelineObj_["TextureBlend"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineObj_["TextureBlend"]->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineObj_["TextureBlend"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["TextureBlend"]->CreateRootParams(3, 3);
	pipelineObj_["TextureBlend"]->CreatePipeline(2);
}

void PipelineManager::AddPipeline(std::string pipelineName)
{
	std::unique_ptr<PipelineObj> obj = std::make_unique<PipelineObj>();
	pipelineObj_.insert(std::make_pair(pipelineName, std::move(obj)));
}