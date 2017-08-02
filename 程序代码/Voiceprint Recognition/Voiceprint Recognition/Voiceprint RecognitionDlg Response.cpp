#include "stdafx.h"
#include "Voiceprint RecognitionDlg.h"

WaveRecorder waveRecorder;
char* fileName;

bool CVoiceprintRecognitionDlg::OnButton1_record(char* fileName)             //����¼���߳�
{
	::fileName = fileName;
	pthread_attr_t attr;                                                     //�߳����Խṹ�壬�����߳�ʱ����Ĳ���  
	pthread_attr_init(&attr);                                                //��ʼ��
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);             //����������Ҫָ���߳����Բ���
    int ret = pthread_create(&thread_recordID, &attr, record, (void*)&fileName);  
    if(ret != 0) {
		MessageBoxA(NULL, "ERROR : Can't create thread !", "ERROR", MB_ICONHAND);
		return false;
	}
	return true;
}

bool CVoiceprintRecognitionDlg::OnButton1_cancel()                           //����¼��
{
	waveRecorder.Stop();
	waveRecorder.Reset();
	pthread_cancel(this->thread_recordID);
	return true;
}

void* record(void* args)                                                     //¼���߳�
{
	waveRecorder.set_FileName((char*)fileName);
	waveRecorder.Start();
	delete fileName;
	return NULL;
}  