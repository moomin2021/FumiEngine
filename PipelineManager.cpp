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

void PipelineManager::PreDraw(std::string pipelineName)
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
	std::vector<ID3D12DescriptorHeap*> ppHeaps = { srvHeap };
	cmdList->SetDescriptorHeaps(1, ppHeaps.data());
}

PipelineManager::PipelineManager() {
	AddPipeline("Object3D");
	pipelineObj_["Object3D"]->LoadShader("Resources/Shaders/Object3DPS.hlsl", PS);
	pipelineObj_["Object3D"]->LoadShader("Resources/Shaders/Object3DVS.hlsl", VS);
	pipelineObj_["Object3D"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineObj_["Object3D"]->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineObj_["Object3D"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Object3D"]->CreateRootParams(4);
	pipelineObj_["Object3D"]->CreatePipeline();

	AddPipeline("Sprite");
	pipelineObj_["Sprite"]->LoadShader("Resources/Shaders/SpritePS.hlsl", PS);
	pipelineObj_["Sprite"]->LoadShader("Resources/Shaders/SpriteVS.hlsl", VS);
	pipelineObj_["Sprite"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Sprite"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Sprite"]->CreateRootParams(2);
	pipelineObj_["Sprite"]->CreatePipeline();

	AddPipeline("PostEffectTest");
	pipelineObj_["PostEffectTest"]->LoadShader("Resources/Shaders/PostEffectTestPS.hlsl", PS);
	pipelineObj_["PostEffectTest"]->LoadShader("Resources/Shaders/PostEffectTestVS.hlsl", VS);
	pipelineObj_["PostEffectTest"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["PostEffectTest"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["PostEffectTest"]->CreateRootParams(2);
	pipelineObj_["PostEffectTest"]->CreatePipeline();
}

void PipelineManager::AddPipeline(std::string pipelineName)
{
	std::unique_ptr<PipelineObj> obj = std::make_unique<PipelineObj>();
	pipelineObj_.insert(std::make_pair(pipelineName, std::move(obj)));
}