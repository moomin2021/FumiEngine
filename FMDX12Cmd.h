#pragma once
class DX12Cmd
{
#pragma region メンバ変数
public:

private:
	// --インスタンス-- //
	static DX12Cmd* myInstance_;
#pragma endregion

#pragma region メンバ関数
public:
	// --インスタンス読み込み-- //
	static DX12Cmd* GetInstance();

	// --インスタンス解放-- //
	void Release();

	// --初期化処理-- //
	void Initialize();

#pragma region ゲッター
	
#pragma endregion

private:
	// --コンストラクタ-- //
	DX12Cmd();

	// --デストラクタ-- //
	~DX12Cmd();
#pragma endregion

	// --禁止-- //
	DX12Cmd(const DX12Cmd&) = delete;// --------------> コピーコンストラクタ禁止
	DX12Cmd& operator = (const DX12Cmd&) = delete;// -> コピー代入演算子禁止
};

