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
DX12Cmd::DX12Cmd()
#pragma region �萔������

#pragma endregion
{

}

// --�f�X�g���N�^-- //
DX12Cmd::~DX12Cmd() {

}

void DX12Cmd::Initialize(WinAPI* win) {
	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
	HRESULT result;

	/// --�A�_�v�^�̗�-- ///
	/// ��PC�ɂ���O���t�B�b�N�{�[�h���A���z�I�ȃf�o�C�X���܂߂đS�����X�g�A�b�v���� ///
#pragma region

	// --DXGI�t�@�N�g���[�̐���-- //
	// DXGI = �O���t�B�b�N�X�C���t���X�g���N�`��
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// --�A�_�v�^�[�̗񋓗p-- //
	std::vector<IDXGIAdapter4*> adapters;

	// --�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������-- //
	IDXGIAdapter4* tmpAdapter = nullptr;

	// --�p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���-- //
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		// ���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter);
	}

#pragma endregion
	/// --END-- ///

	/// --�A�_�v�^�̑I��-- //
	/// �����o���ꂽ�O���t�B�b�N�X�f�o�C�X�̒��Ő��\�̒Ⴂ���̏��O���āA��p�f�o�C�X���̗p���� ///
#pragma region

	// --�Ó��ȃA�_�v�^��I�ʂ���-- //
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;

		// �A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);

		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i];
			break;
		}
	}

#pragma endregion
	/// --END-- ///

	/// --�f�o�C�X�̐���-- ///
	/// ���̗p�����O���t�B�b�N�X�f�o�C�X�𑀍삷�邽�߂�D3D12Device�I�u�W�F�N�g�𐶐� ///
	/// ������͕��ʂP�Q�[����1�������Ȃ� ///
#pragma region

	// --�Ή����x���̔z��-- //
	D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	// --�O���t�B�b�N�X�f�o�C�X�𑀍삷��ׂ̃I�u�W�F�N�g����-- //
	// ������͕��ʁA�P�Q�[���ɂP�������Ȃ�
	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++)
	{
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion
	/// --END-- ///

	/// --�R�}���h���X�g-- ///
	/// ��GPU�ɁA�܂Ƃ߂Ė��߂𑗂邽�߂̃R�}���h���X�g�𐶐����� //
#pragma region

	// --�R�}���h�A���P�[�^�𐶐�-- //
	// ���R�}���h���X�g�̓R�}���h�A���P�[�^���琶������̂ŁA��ɃR�}���h�A���P�[�^����� //
	// ���R�}���h���X�g�Ɋi�[���閽�߂ׂ̈̃��������Ǘ�����I�u�W�F�N�g //
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,// -> �R�}���h�A���P�[�^�̎��
		IID_PPV_ARGS(&cmdAllocator));// -> �e�C���^�[�t�F�C�X�ŗL��GUID
	assert(SUCCEEDED(result));// -> ID3D12CommandAllocator�C���^�[�t�F�C�X�̃|�C���^���i�[����ϐ��A�h���X

	// --�R�}���h���X�g�𐶐�-- //
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --�R�}���h�L���[-- ///
	/// ���R�}���h���X�g��GPU�ɏ��Ɏ��s�����Ă����ׂ̎d�g�� ///
#pragma region

	// --�R�}���h�L���[�̐ݒ�-- //
	// ��{}�����邱�Ƃō\���̂̒��g��0�ŃN���A���Ă���B
	// ���l0���W���l�ɂȂ�悤��Microsoft�����߂Ă���̂ŁA���̏ꍇ�R�}���h�L���[�̐ݒ��W���ɂ��Ă��� //
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	// --�W���ݒ�ŃR�}���h�L���[�𐶐�-- //
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --�X���b�v�`�F�[��-- ///
	/// ���X���b�v�`�F�[���́A�_�u���o�b�t�@�����O��g���v���o�b�t�@�����O���ȒP�Ɏ������邽�߂̎d�g�� ///
	/// ���\�����̉�ʁi�t�����g�o�b�t�@�j�E�`�撆�̉�ʁi�o�b�N�o�b�t�@�j
#pragma region

	// --�X���b�v�`�F�[���̐ݒ�-- //
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// --��ʂ̕����w�肷��
	swapChainDesc.Width = win->GetWidth();;

	// --��ʂ̍������w�肷��
	swapChainDesc.Height = win->GetHeight();

	// --�F���̏����i�\���`���j
	//��DXGI_FORMAT_R8G8B8A8_UNORM�̓A���t�@���܂ރ`�����l��������8�r�b�g���T�|�[�g
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// --�s�N�Z��������̃}���`�T���v���̐����w�肷��
	swapChainDesc.SampleDesc.Count = 1;

	// --���\�[�X�̎g�p���@���w��
	// ��DXGI_USAGE_BACK_BUFFER�̓��\�[�X���o�b�N�o�b�t�@�Ƃ��Ďg�p����
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;

	// --�X���b�v�`�F�[�����̃o�b�t�@�[�̐����w�肷��
	swapChainDesc.BufferCount = 2;

	// --��ʂ��X���b�v������̃��\�[�X�̏������@���w��
	// ��DXGI_SWAP_EFFECT_FLIP_DISCARD�̓X���b�v������o�b�N�o�b�t�@�[�̓��e��j������ݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// --�X���b�v�`�F�[������̃I�v�V�������w��
	// ��DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH�̓E�B���h�E�ƃt���X�N���[���̐؂�ւ�����>>
	// >>�𑜓x���E�B���h�E�T�C�Y�Ɉ�v����悤�ɕύX����
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// --IDXGISwapChain1��ComPtr��p��-- //
	ComPtr<IDXGISwapChain1> swapChain1;

	// --�X���b�v�`�F�[���̐���-- //
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), win->GetHWND(), &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain1);
	assert(SUCCEEDED(result));

	// --��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����-- //
	swapChain1.As(&swapChain);

#pragma endregion
	/// --END-- ///
}

// --�A�_�v�^����������-- //
void DX12Cmd::InitializeAdapter() {

}