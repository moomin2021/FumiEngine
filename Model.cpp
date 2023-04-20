#include "Model.h"
#include "DX12Cmd.h"

ID3D12GraphicsCommandList* Model::cmdList_ = DX12Cmd::GetCmdList();

Model* Model::CreateModel(ID3D12GraphicsCommandList* cmdList)
{
	return nullptr;
}
