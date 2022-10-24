#pragma once
// --WindowsAPI-- //
#include <Windows.h>

class WinAPI {
#pragma region メンバ変数
public:

private:
	// --インスタンス-- //
	static WinAPI* myInstance_;

	// --ウィンドウサイズ-- //
	const int winWidth_;// --> 横幅
	const int winHeight_;// -> 縦幅

	// --ウィンドウクラス-- //
	WNDCLASSEX winClass_;

	// --ウィンドウサイズ-- //
	RECT winRect_;

	// --ウィンドウハンドル(オブジェクト)-- //
	HWND winHandle_;

	// --メッセージ-- //
	MSG msg_;

#pragma endregion

#pragma region メンバ関数
public:
	// --インスタンス読み込み-- //
	static WinAPI* GetInstance();

	// --インスタンス解放-- //
	void Release();

	// --初期化処理-- //
	void Initialize();

	// --終了メッセージがきているか参照-- //
	bool IsEndMessage();

	// --ウィンドウクラス登録解除-- //
	void WinClassUnregister();

#pragma region ゲッター
	// --ウィンドゥクラスを参照-- //
	WNDCLASSEX GetWinClass();
#pragma endregion

private:
	// --コンストラクタ-- //
	WinAPI();

	// --デストラクタ-- //
	~WinAPI();
#pragma endregion

	// --禁止-- //
	WinAPI(const WinAPI&) = delete;// --------------> コピーコンストラクタ禁止
	WinAPI& operator = (const WinAPI&) = delete;// -> コピー代入演算子禁止
};