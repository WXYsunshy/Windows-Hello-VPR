#pragma once
#include "WavFile_Struct.h"
#include <cmath>

#define MUTEPARAGRAPH 0                                                      //������
#define INTERIMPARAGRAPH 1                                                   //���ɶ�
#define VOICEPARAGRAPH 2                                                     //������
#define OVERPARAGRAPH 3                                                      //��������

class WavFile_Initial :protected WavFile
{
private:
	double energyHigh;                                                       //��ʱ֡����������
	double energyLow;                                                        //��ʱ֡����������
	double zcrHigh;                                                          //��ʱ�����ʸ�����
	double zcrLow;                                                           //��ʱ�����ʵ�����

	static const unsigned long minSilence;                                   //��̾�������
	static const unsigned long minVoiceLength;                               //�����������

	double *dataDouble;                                                      //��ת����Double�����ݣ����Ʒ�Χ��[-1,1]
	vector<double> dataEnergy;                                               //�����ʱ֡����������ΪdataNunber-N
	vector<double> dataZCR;                                                  //�����ʱ������

	double maxEnergy;                                                        //���ڱ������Ķ�ʱ֡����
	double minEnergy;                                                        //���ڱ�����С�Ķ�ʱ֡����
	double maxZCR;                                                           //���ڱ������Ķ�ʱ������
	double minZCR;                                                           //���ڱ�����С�Ķ�ʱ������

	unsigned long voiceNumber;                                               //�����������
	vector<VoiceParagraph> voiceParagraph;                                   //������������ļ�������Ҫ�������������

	double *dataDFT;                                                         //���ڱ���������ɢ����Ҷ�任������
	double *dataIDFT;                                                        //���ڱ���������ɢ����Ҷ�任������

	bool Conversion_Double(void);                                            //���������ֽ�����ת��ΪDouble����
	double Hamming_window(double data);                                      //����������
	short Sign_Function(double data);                                        //���ʱ�����ʵĸ������ź���
	bool Frame_Energy(void);                                                 //�������ʱ֡����
	bool Frame_ZCR(void);                                                    //�������ʱ������
	bool Frame_EnergyZcr(void);                                              //����ͬʱ��ȡ��ʱ֡�������ʱ������
public:
	static const int N;                                                      //��ʾÿ������֡��
	static const int FrameShift;                                             //��������֡��
	static const double PI;                                                  //��ѧԲ����PI
	WavFile_Initial(void) {}
	WavFile_Initial(FILE *f) :WavFile(f) {
		try
		{
			if (WavFile::Read_File() == false) {                             //���ø�������ȡ�ļ�
				throw;
			}
			dataDouble = (double*)malloc(sizeof(double)*WavFile::Get_dataNumber());    //ΪDouble�����������ڴ�ռ�
			if (dataDouble == NULL) {                                        //����ռ�δ�ɹ�
				throw invalid_argument("ERROR : Memory failure !");
			}
			if (Conversion_Double() == false) {                              //�����ֽ�����ת��ΪDouble����
				throw invalid_argument("ERROR : Data change failure !");
			}
			this->Endpoint_Detection();                                      //��ʼ�˵���
		}
		catch (invalid_argument &e) {
			MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
			exit(-1);
		}
	}
	~WavFile_Initial() {
		free(dataDouble);
	}
	double Get_maxEnergy(void);                                              //��ȡ����ʱ֡����
	double Get_minEnergy(void);                                              //��ȡ��С��ʱ֡����
	double Get_maxZCR(void);                                                 //��ȡ����ʱ������
	double Get_minZCR(void);                                                 //��ȡ��С��ʱ������
	double Get_dataNumber(void);                                             //��ȡDouble���ݵĸ���
	double Get_dataEZNumber(void);                                           //��ȡ���������ʵĸ���
	double Get_DataDouble(unsigned long Number);                             //��ȡת�����Double����
	double Get_DataEnergy(unsigned long Number);                             //��������ҵ���Ӧ�Ķ�ʱ֡����
	double Get_DataZCR(unsigned long Number);                                //��������ҵ���Ӧ�Ķ�ʱ������
	int Get_WindowLength(void);                                              //��ȡ֡�������Ĵ�С��
	unsigned long Get_voiceNumber(void);                                     //��ȡ�����������
	VoiceParagraph Get_dataVoicePoint(unsigned long Number);                 //��ȡĳ����������
	void ShowData(void);                                                     //���Ǹ����չʾ���ݺ���
	void SaveNewWav(void);                                                   //����ȥ���հ״��������ļ�
	bool Endpoint_Detection(void);                                           //�˵��⺯��
};
