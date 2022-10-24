#include "FMDX12Cmd.h"

// --�C���X�^���X�̏�����-- //
DX12Cmd* DX12Cmd::myInstance_ = nullptr;

// --�C���X�^���X�ǂݍ���-- //
DX12Cmd* DX12Cmd::GetInstance() {
	// --�C���X�^���X�����������琶������-- //
	if (myInstance_ == nullptr) myInstance_ = new DX12Cmd();

	// --�C���X�^���X��Ԃ�-- //
	return myInstance_;
}

// --�C���X�^���X���-- //
void DX12Cmd::Release() {
	// --�C���X�^���X�����������牽�������ɏI������-- //
	if (myInstance_ == nullptr) return;

	// --�C���X�^���X���-- //
	delete myInstance_;
	myInstance_ = nullptr;
}

// --�R���X�g���N�^-- //
DX12Cmd::DX12Cmd() :
#pragma region �萔������

#pragma endregion
{

}

// --�f�X�g���N�^-- //
DX12Cmd::~DX12Cmd() {

}