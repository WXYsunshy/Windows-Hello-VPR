#include <stdio.h>  
#include <Windows.h>  
#pragma comment(lib, "winmm.lib")  

HWAVEIN hWaveIn;  //�����豸
WAVEFORMATEX waveform; //�ɼ���Ƶ�ĸ�ʽ���ṹ��
BYTE *pBuffer1;//�ɼ���Ƶʱ�����ݻ���
WAVEHDR wHdr1; //�ɼ���Ƶʱ�������ݻ���Ľṹ��
FILE *pf;
int main()
{
	HANDLE          wait;
	waveform.wFormatTag = WAVE_FORMAT_PCM;//������ʽΪPCM
	waveform.nSamplesPerSec = 8000;//�����ʣ�16000��/��
	waveform.wBitsPerSample = 16;//�������أ�16bits/��
	waveform.nChannels = 1;//������������2����
	waveform.nAvgBytesPerSec = 16000;//ÿ��������ʣ�����ÿ���ܲɼ������ֽڵ�����
	waveform.nBlockAlign = 2;//һ����Ĵ�С������bit���ֽ�������������
	waveform.cbSize = 0;//һ��Ϊ0

	wait = CreateEvent(NULL, 0, 0, NULL);
	//ʹ��waveInOpen����������Ƶ�ɼ�
	waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform,(DWORD_PTR)wait, 0L, CALLBACK_EVENT);

	//�����������飨������Խ���������飩����������Ƶ����
	DWORD bufsize = 1024*100;//ÿ�ο���10k�Ļ���洢¼������
	int i = 20;
	fopen_s(&pf, "¼������.wav", "wb");
	while (i--)//¼��20�����������������Ƶ��������紫������޸�Ϊʵʱ¼�����ŵĻ�����ʵ�ֶԽ�����
	{
		pBuffer1 = new BYTE[bufsize];
		wHdr1.lpData = (LPSTR)pBuffer1;
		wHdr1.dwBufferLength = bufsize;
		wHdr1.dwBytesRecorded = 0;
		wHdr1.dwUser = 0;
		wHdr1.dwFlags = 0;
		wHdr1.dwLoops = 1;
		waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//׼��һ���������ݿ�ͷ����¼��
		waveInAddBuffer(hWaveIn, &wHdr1, sizeof (WAVEHDR));//ָ���������ݿ�Ϊ¼�����뻺��
		waveInStart(hWaveIn);//��ʼ¼��
		Sleep(1000);//�ȴ�����¼��1s
		waveInReset(hWaveIn);//ֹͣ¼��
		fwrite(pBuffer1, 1, wHdr1.dwBytesRecorded, pf);
		delete pBuffer1;	
		printf("%ds  ", i);
	}
	fclose(pf);

	waveInClose(hWaveIn);
	return 0;
}