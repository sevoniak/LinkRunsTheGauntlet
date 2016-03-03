///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "soundclass.h"
#include <string>
#include <iostream>
using namespace std;

SoundClass::SoundClass()
{
    m_DirectSound = 0;
    m_primaryBuffer = 0;
    m_bgmBuffers = 0;
	current_bgm = -1;

    // Set the other sounds to be null
    m_playBuffers = 0;

}


SoundClass::SoundClass(const SoundClass& other)
{
}


SoundClass::~SoundClass()
{
}


bool SoundClass::Initialize(HWND hwnd)
{
    bool result;

    // Initialize direct sound and the primary sound buffer.
    result = InitializeDirectSound(hwnd);
    if (!result)
    {
        return false;
    }

	num_bgm = 2;
    m_bgmBuffers = new IDirectSoundBuffer8*[num_bgm];
	LoadWaveFile("Sounds/title.wav", &m_bgmBuffers[0]);
    LoadWaveFile("Sounds/darkWorld.wav", &m_bgmBuffers[1]);

	for (int i = 0; i < num_bgm; i++)
		m_bgmBuffers[i]->SetVolume(-1000);


	num_sounds = 22;
	m_playBuffers = new IDirectSoundBuffer8*[num_sounds];
	LoadWaveFile("Sounds/linkHit.wav", &m_playBuffers[0]);
    LoadWaveFile("Sounds/swordSwing.wav", &m_playBuffers[1]);
    LoadWaveFile("Sounds/arrowShoot.wav", &m_playBuffers[2]);
    LoadWaveFile("Sounds/bombThrow.wav", &m_playBuffers[3]);
    LoadWaveFile("Sounds/bombExplode.wav", &m_playBuffers[4]);
	LoadWaveFile("Sounds/start.wav", &m_playBuffers[5]);
    LoadWaveFile("Sounds/giantHit.wav", &m_playBuffers[6]);
	LoadWaveFile("Sounds/giantDead.wav", &m_playBuffers[7]);
	LoadWaveFile("Sounds/manHit.wav", &m_playBuffers[8]);
	LoadWaveFile("Sounds/manDead.wav", &m_playBuffers[9]);
	LoadWaveFile("Sounds/linkDead.wav", &m_playBuffers[10]);
	LoadWaveFile("Sounds/getHeart.wav", &m_playBuffers[11]);
	LoadWaveFile("Sounds/getHeartContainer.wav", &m_playBuffers[12]);
	LoadWaveFile("Sounds/getKey1.wav", &m_playBuffers[13]);
	LoadWaveFile("Sounds/getKey2.wav", &m_playBuffers[14]);
	LoadWaveFile("Sounds/getKey3.wav", &m_playBuffers[15]);
	LoadWaveFile("Sounds/gameover.wav", &m_playBuffers[16]);
	LoadWaveFile("Sounds/block.wav", &m_playBuffers[17]);
	LoadWaveFile("Sounds/menu.wav", &m_playBuffers[18]);
    LoadWaveFile("Sounds/menu-click.wav", &m_playBuffers[19]);
	LoadWaveFile("Sounds/won.wav", &m_playBuffers[20]);
	LoadWaveFile("Sounds/beep.wav", &m_playBuffers[21]);

	for (int i = 0; i < num_sounds; i++)
		m_playBuffers[i]->SetVolume(-800);

    return true;
}


void SoundClass::Shutdown()
{
    // Release the secondary buffer.
	for (int i = 0; i < num_bgm; i++)
		ShutdownWaveFile(&m_bgmBuffers[i]);
	if (m_bgmBuffers)
	{
		delete m_bgmBuffers;
		m_bgmBuffers = 0;
	}

    // Play buffers
	for (int i = 0; i < num_sounds; i++)
		ShutdownWaveFile(&m_playBuffers[i]);
	if (m_playBuffers)
	{
		delete m_playBuffers;
		m_playBuffers = 0;
	}

    // Shutdown the Direct Sound API.
    ShutdownDirectSound();

    return;
}


bool SoundClass::InitializeDirectSound(HWND hwnd)
{
    HRESULT result;
    DSBUFFERDESC bufferDesc;
    WAVEFORMATEX waveFormat;


    // Initialize the direct sound interface pointer for the default sound device.
    result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Set the cooperative level to priority so the format of the primary sound buffer can be modified.
    result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
    if (FAILED(result))
    {
        return false;
    }

    // Setup the primary buffer description.
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = NULL;
    bufferDesc.guid3DAlgorithm = GUID_NULL;

    // Get control of the primary sound buffer on the default sound device.
    result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Setup the format of the primary sound bufffer.
    // In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nChannels = 2;
    waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    // Set the primary buffer to be the wave format specified.
    result = m_primaryBuffer->SetFormat(&waveFormat);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}


void SoundClass::ShutdownDirectSound()
{
    // Release the primary sound buffer pointer.
    if (m_primaryBuffer)
    {
        m_primaryBuffer->Release();
        m_primaryBuffer = 0;
    }

    // Release the direct sound interface pointer.
    if (m_DirectSound)
    {
//		m_DirectSound->Release();
        m_DirectSound = 0;
    }

    return;
}


bool SoundClass::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
    int error;
    FILE* filePtr;
    unsigned int count;
    WaveHeaderType waveFileHeader;
    WAVEFORMATEX waveFormat;
    DSBUFFERDESC bufferDesc;
    HRESULT result;
    IDirectSoundBuffer* tempBuffer;
    unsigned char* waveData;
    unsigned char* bufferPtr;
    unsigned long bufferSize;


    // Open the wave file in binary.
    error = fopen_s(&filePtr, filename, "rb");
    if (error != 0)
    {
        return false;
    }

    // Read in the wave file header.
    count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
    if (count != 1)
    {
        return false;
    }

    // Check that the chunk ID is the RIFF format.
    if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
        (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
    {
        return false;
    }

    // Check that the file format is the WAVE format.
    if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
        (waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
    {
        return false;
    }

    // Check that the sub chunk ID is the fmt format.
    if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
        (waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
    {
        return false;
    }

    // Check that the audio format is WAVE_FORMAT_PCM.
    if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
    {
        return false;
    }

    // Check that the wave file was recorded in stereo format.
    if (waveFileHeader.numChannels != 2)
    {
        return false;
    }

    // Check that the wave file was recorded at a sample rate of 44.1 KHz.
    if (waveFileHeader.sampleRate != 44100)
    {
        return false;
    }

    // Ensure that the wave file was recorded in 16 bit format.
    if (waveFileHeader.bitsPerSample != 16)
    {
        return false;
    }

    // Check for the data chunk header.
    if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
        (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
    {
        return false;
    }

    // Set the wave format of secondary buffer that this wave file will be loaded onto.
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nChannels = 2;
    waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    // Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = &waveFormat;
    bufferDesc.guid3DAlgorithm = GUID_NULL;

    // Create a temporary sound buffer with the specific buffer settings.
    result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Test the buffer format against the direct sound 8 interface and create the secondary buffer.
    result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Release the temporary buffer.
    tempBuffer->Release();
    tempBuffer = 0;

    // Move to the beginning of the wave data which starts at the end of the data chunk header.
    fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

    // Create a temporary buffer to hold the wave file data.
    waveData = new unsigned char[waveFileHeader.dataSize];
    if (!waveData)
    {
        return false;
    }

    // Read in the wave file data into the newly created buffer.
    count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
    if (count != waveFileHeader.dataSize)
    {
        return false;
    }

    // Close the file once done reading.
    error = fclose(filePtr);
    if (error != 0)
    {
        return false;
    }

    // Lock the secondary buffer to write wave data into it.
    result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
    if (FAILED(result))
    {
        return false;
    }

    // Copy the wave data into the buffer.
    memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

    // Unlock the secondary buffer after the data has been written to it.
    result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
    if (FAILED(result))
    {
        return false;
    }

    // Release the wave data since it was copied into the secondary buffer.
    delete[] waveData;
    waveData = 0;

    return true;
}


void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
    // Release the secondary sound buffer.
    if (*secondaryBuffer)
    {
        (*secondaryBuffer)->Release();
        *secondaryBuffer = 0;
    }

    return;
}


bool SoundClass::PlayBGM(int num)
{
    HRESULT result;
	StopBGM();

    // Play the contents of the secondary sound buffer.
	current_bgm = num;
	if (num >= 0)
	{
		result = m_bgmBuffers[num]->Play(0, 0, DSBPLAY_LOOPING);
		if (FAILED(result))
			return false;
	}
    return true;
}

void SoundClass::StopBGM()
{
	if (current_bgm >= 0)
	{
		m_bgmBuffers[current_bgm]->Stop();
		m_bgmBuffers[current_bgm]->SetCurrentPosition(175000 * 0);
	}
}

// Function will take in an integer value and play the particular sound file assoicated with the number
void SoundClass::PlayDesiredFile(int choice)
{
	if (choice < num_sounds)
		m_playBuffers[choice]->Play(0, 0, 0);
	return;
}