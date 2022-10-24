#include "FMDX12Cmd.h"

// --インスタンスの初期化-- //
DX12Cmd* DX12Cmd::myInstance_ = nullptr;

// --インスタンス読み込み-- //
DX12Cmd* DX12Cmd::GetInstance() {
	// --インスタンスが無かったら生成する-- //
	if (myInstance_ == nullptr) myInstance_ = new DX12Cmd();

	// --インスタンスを返す-- //
	return myInstance_;
}

// --インスタンス解放-- //
void DX12Cmd::Release() {
	// --インスタンスが無かったら何もせずに終了する-- //
	if (myInstance_ == nullptr) return;

	// --インスタンス解放-- //
	delete myInstance_;
	myInstance_ = nullptr;
}

// --コンストラクタ-- //
DX12Cmd::DX12Cmd() :
#pragma region 定数初期化

#pragma endregion
{

}

// --デストラクタ-- //
DX12Cmd::~DX12Cmd() {

}