#pragma once
#include <xaudio2.h>
#include <fstream>
#include <memory>
#include <wrl.h>

// チャンクヘッダ
struct ChunkHeader {
	char id[4];		// チャンク毎のID
	uint32_t size;	// チャンクサイズ
};

// RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk;	// RIFF
	char type[4];		// WAVE
};

// FMTチャンク
struct FormatChunk {
	ChunkHeader chunk;	// fmt
	WAVEFORMATEX fmt;	// 波形フォーマット
};

// 音声データ
struct SoundData {
	WAVEFORMATEX wfex;	// 波形フォーマット
	BYTE* pBuffer;		// バッファの先頭アドレス
	uint32_t bufferSize;// バッファサイズ
};

class Sound
{
private:
	// エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region メンバ変数
private:
	static ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	// インスタンス取得
	static Sound* GetInstance();

	// サウンド読み込み
	static SoundData SoundLoadWave(const char* filename);

	// 音声データ解放
	static void SoundUnload(SoundData* soundData);

	// 音声再生
	static void SoundPlay(const SoundData& soundData);

	// 音声停止
	static void SoundStop(const SoundData& soundData);

private:
	// コンストラクタ
	Sound();
#pragma endregion
};