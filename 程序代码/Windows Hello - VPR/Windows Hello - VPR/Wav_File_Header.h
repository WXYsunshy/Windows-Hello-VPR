#pragma once
#include <vector>
#include <iostream>
#include <windows.h>
using namespace std;

struct VoiceParagraph
{
	unsigned long begin;                                                     //�������俪ʼ��
	unsigned long end;                                                       //�������������
	unsigned long voiceLength;                                               //�������䳤��
	VoiceParagraph(unsigned long be, unsigned long en, unsigned long vo) {   //�����ʼ��
		begin = be;
		end = en;
		voiceLength = vo;
	}
};

class Wav_File
{
private:
	FILE *fp;                                                                //�ļ�ָ��
	char *fileName;                                                          //�ļ������ַ

	char id_RIFF[5];                                                         //RIFF���־
	unsigned long fileSize;                                                  //�ļ������ֽ���
	char id_WAVE[5];                                                         //WAV��־

	char id_FMT[5];                                                          //��ʽ���־
	unsigned long formatLength;                                              //��ʽ�鳤�ȣ�16Ϊ������18˵���и�����Ϣ
	short formatTag;                                                         //��ʽ���ֵ=1 ��ʾ���뷽ʽΪPCMu�ɱ���
	short channelsNumber;                                                    //������
	unsigned long sampleRate;                                                //ÿ���������
	unsigned long secondBytes;                                               //ÿ�����ݵ��ֽ���
	short chunkAlign;                                                        //�����ֽ���
	short sampleBits;                                                        //����λ��
	short appendInformation;                                                 //������Ϣ��ͨ��formatLength���ж�

	char id_FACT[5];                                                         //���ӿ��־
	unsigned long appendLength;                                              //���ӿ鳤��
	unsigned long appendNone;                                                //δ֪

	char id_DATA[5];                                                         //���ӿ��־
	unsigned long dataSize;                                                  //���ݲ����ֽ���
	char *data;                                                              //���ݲ���

	long *dataTuple;                                                         //ÿ������������
	unsigned long dataNumber;                                                //���������ݸ���
	long dataMax;                                                            //�������ݵ����ֵ
	long dataMin;                                                            //�������ݵ���Сֵ

	bool Conversion_Tuple(void);                                             //��ֱ�Ӷ�ȡ������ת��Ϊ��������
	long MakeWord(long NumberA, long NumberB);                               //�ϲ��ֽ�
public:
	Wav_File(void) {
		dataMax = 0;
		dataMin = 1000000;
	}
	Wav_File(FILE *f) {
		fp = f;
		::Wav_File();
	}
	void Give_FP(FILE *f) {
		fp = f;
	}
	bool Read_File(void);                                                    //��ȡ�ļ�
	unsigned long Get_SampleRate(void);                                      //��ȡ����Ƶ��
	short Get_ChunkAlign(void);                                              //��ȡ�����ֽ���
	long Get_Data(unsigned long Number);                                     //��ȡĳ��λ���ϵ�����
	unsigned long Get_dataNumber(void);                                      //��ȡ�������ݸ���
	void ShowData(void);                                                     //�������
	void SaveNewWav(unsigned long voiceNumber, vector<VoiceParagraph> voiceParagraph);           //����ȥ���հ״��������ļ�������1Ϊ����ĸ���������2Ϊ������������Ŀ�ʼ�����������Ϣ
};