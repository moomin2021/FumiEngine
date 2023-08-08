#include "ImGuiManager.h"
#include "WinAPI.h"
#include "DX12Cmd.h"
#include "Texture.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_DX12.h>

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager inst;
	return &inst;
}

void ImGuiManager::Initialize()
{
	// インスタンス取得
	WinAPI* winAPI = WinAPI::GetInstance();
	DX12Cmd* dx12 = DX12Cmd::GetInstance();
	Texture* tex = Texture::GetInstance();

	// ImGuiのコンテキストを生成
	ImGui::CreateContext();

	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winAPI->GetHWND());

	ImGui_ImplDX12_Init(dx12->GetDevice(), static_cast<int>(dx12->GetBackBufferNum()), dx12->GetRTVDesc().Format,
		tex->GetSRVHeap(), tex->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart(), tex->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart());

	ImGuiIO& io = ImGui::GetIO();

	// 標準フォントを追加する
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Begin()
{
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	// 描画前準備
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetInstance()->GetSRVHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}

ImGuiManager::ImGuiManager() {}

ImGuiManager::~ImGuiManager()
{
	// 後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}