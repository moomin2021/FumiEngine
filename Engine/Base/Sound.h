#pragma once
#include <xaudio2.h>
#include <fstream>
#include <memory>
#include <wrl.h>

// �`�����N�w�b�_
struct ChunkHeader {
	char id[4];		// �`�����N����ID
	uint32_t size;	// �`�����N�T�C�Y
};

// RIFF�w�b�_�`�����N
struct RiffHeader {
	ChunkHeader chunk;	// RIFF
	char type[4];		// WAVE
};

// FMT�`�����N
struct FormatChunk {
	ChunkHeader chunk;	// fmt
	WAVEFORMATEX fmt;	// �g�`�t�H�[�}�b�g
};

// �����f�[�^
struct SoundData {
	WAVEFORMATEX wfex;	// �g�`�t�H�[�}�b�g
	BYTE* pBuffer;		// �o�b�t�@�̐擪�A�h���X
	uint32_t bufferSize;// �o�b�t�@�T�C�Y
};

class Sound
{
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region �����o�ϐ�
private:
	static ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;
#pragma endregion

#pragma region �����o�֐�
public:
	// �C���X�^���X�擾
	static Sound* GetInstance();

	// �T�E���h�ǂݍ���
	static SoundData SoundLoadWave(const char* filename);

	// �����f�[�^���
	static void SoundUnload(SoundData* soundData);

	// �����Đ�
	static void SoundPlay(const SoundData& soundData);

	// ������~
	static void SoundStop(const SoundData& soundData);

private:
	// �R���X�g���N�^
	Sound();
#pragma endregion
};