/****************************************************************************
*
*						MMAPI API WaveRecorder¼����
*
*@������WaveRecorder
*
*@��Ա������
*
*--����Start֮ǰ���á�
*	void set_FileName(string des_path);
*	|_______������Ҫ������ļ�Ŀ�꣬�������򲻱���
*
*--��Stop֮ǰ�������á�
*	void set_Callback(CNKDATAUpdateCallback fn);
*	|_______���õõ��µ�ChunkDataʱ�Ĵ����������Բ�����
*
*--��Start������3~4ms��ʱ�䡿
*	void Start();
*	|_______��ʼ������ʼ¼��
*
*--��Stop���ٻ�����4ms��ʱ�䡿
*	void Stop();
*	|_______����¼��������β
*
*--����Stop֮���á�
*	void Reset();
*	|_______����״̬�����set���Ĳ���
*
*@��ע�����캯�����Դ���Ƶ�豸����ʱԼ43ms���ر��豸��������������ʱԼ900ms
*
*����au:�ϳɡ�zzu 15/11/11
*--lastEdit : leafspace
*--contact : 18852923073@163.com
*
****************************************************************************/
#pragma once
#ifndef WAVERECORDER_H
#define WAVERECORDER_H

#include <array>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include "mmsystem.h"                               //������������һϵ�д���

using namespace std;
#pragma comment(lib, "winmm.lib")

// PCM¼������
#define CHANNEL_MUM 1								// ������
#define SAMPLE_RATE 16000							// ÿ�������
#define SAMPLE_BITS 16								// ����λ��
#define CHUNCK_SIZE (SAMPLE_RATE*SAMPLE_BITS/8*1)	// �������ݿ��С = ������*λ��/2*�루�ֽڣ�
#define BUFFER_LAYER 5								// ����������

/* �ص����� */
typedef void(__stdcall *CNKDATAUpdateCallback)(array <char, CHUNCK_SIZE> ChunkData,UINT ChunksCount, bool bIsLast);
/*WaveRecorder��*/
class WaveRecorder 
{
protected:
	/* wav��Ƶͷ����ʽ */
	typedef struct WAVEPCMHDR
	{
		char            riff[4];					// = "RIFF"
		UINT32			size_8;						// = FileSize - 8
		char            wave[4];					// = "WAVE"
		char            fmt[4];						// = "fmt "
		UINT32			fmt_size;					// = PCMWAVEFORMAT�Ĵ�С : 
		
		//PCMWAVEFORMAT
		UINT16	        format_tag;					// = PCM : 1
		UINT16	        channels;					// = ͨ���� : 1
		UINT32			samples_per_sec;			// = ������ : 8000 | 6000 | 11025 | 16000
		UINT32			avg_bytes_per_sec;			// = ÿ��ƽ���ֽ��� : samples_per_sec * bits_per_sample / 8
		UINT16		    block_align;				// = ÿ�������ֽ��� : wBitsPerSample / 8
		UINT16			bits_per_sample;			// = ��������: 8 | 16
		char            data[4];					// = "data";

		//DATA
		UINT32			data_size;					// = �����ݳ��� 
	} WAVEPCMHDR;

private:
	void WaveInitFormat(							// ����PCM��ʽ
		LPWAVEFORMATEX WaveFormat,					//	.PCM
		WORD Ch,									//	.CHANNEL
		DWORD SampleRate,							//	.SAMPLE_RATE
		WORD BitsPerSample);						//	.SAMPLE_BITS
	void WaveFileWrite();
	static DWORD (CALLBACK WaveXAPI_Callback)(		// WaveXAPI�ص�����
		HWAVEIN hwavein,							//	.�����豸
		UINT uMsg,									//	.��Ϣ
		DWORD dwInstance,							//	.����
		DWORD dwParam1,								//	.�����õĻ�����ָ��
		DWORD dwParam2);							//	.����

	static CNKDATAUpdateCallback callback;			// �ص�����ָ��
	static BOOL bCallback;							// �Ƿ��лص�����

	HWAVEIN hwi;									// ��Ƶ�����豸
	static WAVEHDR pwh[BUFFER_LAYER];				// Ӳ��������

	bool bSaveFile;									// �Ƿ�洢�ļ�
	static bool stop;								// �Ƿ񴥷�ֹͣ
	static bool dat_ignore;							// ��ֹ�ظ���¼

	WAVEPCMHDR WavHeader;                           // Ĭ��wav��Ƶͷ������

public:
	string dest_path;								// �洢·��
	FILE* fp;	        							// wave�ļ�ָ��

	WaveRecorder();									// ���죬���豸
	~WaveRecorder();								// �������ر��豸
	void set_Callback(CNKDATAUpdateCallback fn);	// ����ʵʱ�������ݿ�Ļص����������Բ�����
	void set_FileName(string des_path);				// ������Ҫ������ļ�Ŀ�꣬�������򲻱���
	void Start();									// ��ʼ������ʼ¼��
	void Stop();									// ����¼��������β
	void Reset();									// ��������

	static array <char, CHUNCK_SIZE> ChunkData;		// ��ǰ������
	static vector<array<char, CHUNCK_SIZE>> RawData;// ��¼�Ƶ�������
	static UINT ChunksCount;						// �������ڿ����
};
#endif