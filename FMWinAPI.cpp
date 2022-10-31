#include "FMWinAPI.h"

// --ウィンドウプロシージャプロトタイプ宣言-- //
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

// --インスタンスの初期化-- //
WinAPI* WinAPI::myInstance_ = nullptr;

// --インスタンス読み込み-- //
WinAPI* WinAPI::GetInstance() {
	// --インスタンスが無かったら生成する-- //
	if (myInstance_ == nullptr) myInstance_ = new WinAPI();

	// --インスタンスを返す-- //
	return myInstance_;
}

// --インスタンス解放-- //
void WinAPI::Release() {
	// --インスタンスが無かったら何もせずに終了する-- //
	if (myInstance_ == nullptr) return;

	// --インスタンス解放-- //
	delete myInstance_;
	myInstance_ = nullptr;
}

// --コンストラクタ-- //
WinAPI::WinAPI() :
#pragma region 定数初期化
	// --ウィンドウサイズ-- //
	winWidth_(1280),// -> 横幅
	winHeight_(720)// --> 縦幅
#pragma endregion
{

}

// --デストラクタ-- //
WinAPI::~WinAPI() {
	
}

// --初期化処理-- //
void WinAPI::Initialize() {
	// --ウィンドウクラスの設定-- //
	winClass_.cbSize = sizeof(WNDCLASSEX);// -> WNDCLASSEX構造体のサイズ
	winClass_.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定(アドレスを代入)
	winClass_.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
	winClass_.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	winClass_.hCursor = LoadCursor(NULL, IDC_ARROW); // マウスカーソル指定

	// --ウィンドウクラスをOSに登録する-- //
	RegisterClassEx(&winClass_);

	// --ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }-- //
	winRect_ = { 0, 0, winWidth_, winHeight_ };

	// --自動でサイズを補正する-- //
	AdjustWindowRect(&winRect_, WS_OVERLAPPEDWINDOW, false);

	// --ウィンドウオブジェクトの生成-- //
	hwnd_ = CreateWindow(
		winClass_.lpszClassName,// ------> クラス名
		L"DirectXGame",// -------> タイトルバーの文字
		WS_OVERLAPPEDWINDOW,// --> 標準的なウィンドウスタイル
		CW_USEDEFAULT,// --------> 表示X座標(OSに任せる)
		CW_USEDEFAULT,// --------> 表示Y座標(OSに任せる)
		winRect_.right - winRect_.left,// -> ウィンドウ横幅
		winRect_.bottom - winRect_.top,// -> ウィンドウ縦幅
		nullptr,// --------------> 親ウィンドウハンドル
		nullptr,// --------------> メニューハンドル
		winClass_.hInstance,// ----------> 呼び出しアプリケーションハンドル
		nullptr);// -------------> オプション

	// --ウィンドウを表示状態にする-- //
	ShowWindow(hwnd_, SW_SHOW);
}

// --終了メッセージがきているか参照-- //
bool WinAPI::IsEndMessage() {
	// --メッセージがあるか-- //
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_); // キー入力メッセージの処理
		DispatchMessage(&msg_); // プロシージャにメッセージを送る
	}

	// --×ボタンで終了メッセージが来たらゲームループを抜ける-- //
	if (msg_.message == WM_QUIT)
	{
		return true;
	}

	return false;
}

// --ウィンドウクラス登録解除-- //
void WinAPI::WinClassUnregister() { UnregisterClass(winClass_.lpszClassName, winClass_.hInstance); }

// --ウィンドウプロシージャ(システムメッセージを処理するための関数)-- //
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}