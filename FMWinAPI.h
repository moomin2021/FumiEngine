#pragma once
// --WindowsAPI-- //
#include <Windows.h>

class FMWinAPI {
#pragma region メンバ変数
public:

private:
	// --インスタンス-- //
	static FMWinAPI* myInstance_;

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
	static FMWinAPI* GetInstance();

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
	FMWinAPI();

	// --デストラクタ-- //
	~FMWinAPI();
#pragma endregion

	// --禁止-- //
	FMWinAPI(const FMWinAPI&) = delete;// --------------> コピーコンストラクタ禁止
	FMWinAPI& operator = (const FMWinAPI&) = delete;// -> コピー代入演算子禁止
};