#pragma once
class ImGuiManager
{
#pragma region メンバ変数
private:
#pragma endregion

#pragma region メンバ関数
public:
	// インスタンス取得
	static ImGuiManager* GetInstance();

	// 初期化処理
	void Initialize();

	// ImGui受付開始
	void Begin();

	// ImGui受付終了
	void End();

	// 画面への描画
	void Draw();

private:
	// コンストラクタ
	ImGuiManager();

	// デストラクタ
	~ImGuiManager();
#pragma endregion

#pragma region 特殊関数
	// 禁止
	ImGuiManager(const ImGuiManager&) = delete;				// コピーコンストラクタ禁止
	ImGuiManager& operator = (const ImGuiManager&) = delete;	// コピー代入演算子禁止
#pragma endregion
};