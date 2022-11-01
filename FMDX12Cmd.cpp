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
#pragma region ���������X�g
	device(nullptr),// -> �f�o�C�X
	dxgiFactory(nullptr),// -> DXGI�t�@�N�g���[
	cmdAllocator(nullptr),// -> �R�}���h�A���P�[�^
	commandList(nullptr),// -> �R�}���h���X�g
	commandQueue(nullptr),// -> �R�}���h�L���[
	swapChain(nullptr),// -> �X���b�v�`�F�[��
	rtvHeap(nullptr)// -> �����_�[�^�[�Q�b�g�r���[
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

	/// --�f�o�b�N���C���[�̗L���� -- ///
	/// ��Visual Studio�́u�o�́v�E�B���h�E�Œǉ��̃G���[���b�Z�[�W���\���ł���悤�� ///
#pragma region

#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif

#pragma endregion
	/// --END-- ///

	/// --�A�_�v�^�̗�-- ///
	/// ��PC�ɂ���O���t�B�b�N�{�[�h���A���z�I�ȃf�o�C�X���܂߂đS�����X�g�A�b�v���� ///
#pragma region

	// --DXGI�t�@�N�g���[�̐���-- //
	// DXGI = �O���t�B�b�N�X�C���t���X�g���N�`��
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// --�A�_�v�^�[�̗񋓗p-- //
	std::vector<ComPtr<IDXGIAdapter4>> adapters;

	// --�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������-- //
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;

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
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
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

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	//��΂��G���[�̎��~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		//�G���[�̎��~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);	//�x���̎��~�܂�
	}

	//�}������G���[
	D3D12_MESSAGE_ID denyIds[] = {
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};

	//�}�������\�����x��
	D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = _countof(denyIds);
	filter.DenyList.pIDList = denyIds;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;
	//�w�肵���G���[�̕\����}������
	infoQueue->PushStorageFilter(&filter);
#endif

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

	/// --�����_�[�^�[�Q�b�g�r���[-- ///
	/// ���o�b�N�o�b�t�@��`��L�����p�X�Ƃ��Ĉ����ׂ̃I�u�W�F�N�g //
	/// ���_�u���o�b�t�@�����O�ł̓o�b�t�@���Q����̂łQ��� //
#pragma region

	// �������_�[�^�[�Q�b�g�r���[�̓f�X�N���v�^�q�[�v�ɐ�������̂ŁA��Ƀf�X�N���v�^�q�[�v����� //
	// --�f�X�N���v�^�q�[�v�̐ݒ�-- //
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // ���\��2��

	// --�f�X�N���v�^�q�[�v�̐���-- //
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	// ���X���b�v�`�F�[�����ɐ������ꂽ�o�b�N�o�b�t�@�̃A�h���X�����Ă���
	// --�o�b�N�o�b�t�@-- //
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	backBuffers.resize(swapChainDesc.BufferCount);

	// --�X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������-- //
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// --�X���b�v�`�F�[������o�b�t�@���擾
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));

		// --�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

		// --�����\���ŃA�h���X�������
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

		// --�����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

		// --�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		// --�����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}

#pragma endregion
	/// --END-- ///

	/// --�t�F���X�̐���-- ///
	/// ��CPU��GPU�œ������Ƃ邽�߂�DirectX�̎d�g�� ///
#pragma region

	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion
	/// --END-- ///
}

// --�A�_�v�^����������-- //
void DX12Cmd::InitializeAdapter() {

}