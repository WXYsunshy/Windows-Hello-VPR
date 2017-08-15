#include <io.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "Model_GMM.h"
#include "Model_KMeans.h"
#include "CChineseCode.h"
#include "WaveRecorder.h"
#include "WavData_CharaParameter.h"

string getFileName(string path);                                                                                     //��ĳ��·��ת��Ϊĳ���ļ���
void writePassword(const char* password);
void getFiles(string path, vector<string>& files);                                                                   //��ȡpath�ļ����µ������ļ���

int main()
{
	//Todo ¼��
	WaveRecorder waveRecorder;
	waveRecorder.set_FileName("tempRecord.wav");
	waveRecorder.Start();
	Sleep(5500);
	waveRecorder.Stop();
	waveRecorder.Reset();
	

	FILE *fp;
	if ((fp = fopen("tempRecord.wav", "rb")) == NULL) {                                                                 //�������ļ�
		cout << "ERROR : File open failed !" << endl;
		exit(-1);
	}

	//Todo ��ʼ�������ļ��� ��ȡ�����ļ�����
	WavFile_Initial *wavFile = new WavFile_Initial(fp);                                                              //��ȡ�����ļ�����
	fclose(fp);
	for (unsigned long i = 0; i < wavFile->Get_voiceNumber(); ++i) {
		wavFile->Pre_emphasis(wavFile->Get_dataVoicePoint(i), wavFile->Get_WavFileData());                           //�Կ��÷�Χ�ڵ����ݽ���Ԥ����
	}

	//Todo ��ʼ������������ ��������������������
	double *dataSpace = NULL;
	
	CharaParameter *charaParameter = new CharaParameter(wavFile->Get_frameNumber());                                 //��ʼ�����������
	for (unsigned long i = 1; i <= wavFile->Get_frameNumber(); ++i) {                                                //��֡����
		dataSpace = new double[WavFile_Initial::N];                                                                  //�½�֡���ݿռ�
		memset(dataSpace, 0, sizeof(double) * WavFile_Initial::N);
		wavFile->Frame_Data(wavFile->Get_WavFileData(), i, dataSpace, WavFile_Initial::N);                           //��֡���Ӵ�
		charaParameter->Push_data(i, dataSpace);                                                                     //����֡��ɵ����ݱ����������������
	}
	
	unsigned long sampleRate = wavFile->Get_SampleRate();
	delete wavFile;

	//Todo ����MFCC����
	charaParameter->MFCC_CharaParameter(sampleRate);                                                                 //����MFCC��������

	/******************************ʶ��ʾ��******************************/
	char szModuleFilePath[MAX_PATH];
	int n = GetModuleFileNameA(0, szModuleFilePath, MAX_PATH);               //��õ�ǰִ���ļ���·��
	szModuleFilePath[strrchr(szModuleFilePath, '\\') - szModuleFilePath + 1] = 0;      //�����һ��"\\"����ַ���Ϊ0
	
	int index = 0;
	char filePath[MAX_PATH];
	for (int i = 0; i < (int) strlen(szModuleFilePath); ++i) {               //��ȫ//
		filePath[index++] = szModuleFilePath[i];
		if (szModuleFilePath[i] == '\\') {
			filePath[index++] = '\\';
		}
	}
	filePath[index++] = 0;                                                   //ĩβ����

	char path[MAX_PATH];
	strcpy_s(path, filePath);
	strcat_s(path, "voiceLib");

	vector<string> files;
	getFiles(path, files);                                                   //��ȡ�ļ����ڵ������ļ�����·������


	//Todo ��ʼ�����п��GMMģ��
	int gmmNumber = files.size();
	GMM **gmmLib = new GMM*[gmmNumber];
	ifstream *gmmFiles = new ifstream[gmmNumber];
	for (int i = 0; i < gmmNumber; ++i) {
		gmmLib[i] = new GMM(CharaParameter::MelDegreeNumber, GMM::SGMNumber);
		gmmFiles[i].open(files[i]);
		assert(gmmFiles[i]);
		gmmFiles[i] >> *gmmLib[i];
		gmmFiles[i].close();
	}

	//Todo ʶ�����
	cout << "TIP : Begin reservation ..." << endl;
	double *libProbability = new double[gmmNumber];
	cout << "Frame number is " << charaParameter->Get_frameNumber() << endl;
	for (int i = 0; i < gmmNumber; ++i) {
		libProbability[i] = 0;
		for (unsigned long j = 0; j < charaParameter->Get_frameNumber(); ++j) {                                      //���㵱ǰGMM�£�Ŀ��������������GMMģ���µĸ����ܶ�
			double tempData = gmmLib[i]->GetProbability(charaParameter->Get_frameMelParameter(j));                   //��ȡGMM����ֵ
			if (tempData > 0) {                                                                                      //ȡ��������
				tempData = log10(tempData);
			}
			libProbability[i] += tempData;
		}
	}

	int countMax = 0;
	cout << "TIP : Probability data is ";
	for (int i = 0; i < gmmNumber; ++i) {
		cout << libProbability[i] << "\t";
		if (libProbability[i] > libProbability[countMax]) {
			countMax = i;
		}
	}
	cout << endl;

	if (strcmp(getFileName(files[countMax]).data(), "me.txt") == 0) {
		cout << "ʶ��ɹ���- ��½..." << endl;

		char password[512];
		ifstream passwordFile("my.key");
		if (passwordFile.is_open()) {
			passwordFile.getline(password, 512);
		}
		passwordFile.close();
		string str;
		//CChineseCode::GB2312ToUTF_8(str, password, strlen(password));
		writePassword("sandaozhishu429");
	} else {
		cout << "ʶ����: δ֪��" << endl;
		MessageBoxA(NULL, "�Բ�����û��Ȩ�޵�½ !", "����", MB_ICONHAND);
	}

	return 0;
}

string getFileName(string path)                                              //��ĳ��·��ת��Ϊĳ���ļ���
{
	return path.substr(path.rfind('\\') + 1, path.size() - path.rfind('\\') - 1);
}

void writePassword(const char* password) 
{
	//Sleep(5000);
	HWND hWnd = GetForegroundWindow();                                                                               //��õ�ǰ����Ĵ��ھ��
	DWORD SelfThreadId = GetCurrentThreadId();                                                                       //��ȡ������߳�ID
	DWORD ForeThreadId = GetWindowThreadProcessId(hWnd, NULL);                                                       //���ݴ��ھ����ȡ�߳�ID
	AttachThreadInput(ForeThreadId, SelfThreadId, true);                                                             //�����߳�
	hWnd = GetFocus();                                                                                               //��ȡ�������뽹��Ĵ��ھ��
	AttachThreadInput(ForeThreadId, SelfThreadId, false);                                                            //ȡ�����ӵ��߳�
	for (int i = 0; i < (int) strlen(password); ++i) {
		SendMessage(hWnd, WM_CHAR, WPARAM(password[i]), 0);                                                          //����һ������Ϣ
	}
	SendMessage(hWnd, WM_KEYDOWN, VK_RETURN, 0);
}

void getFiles(string path, vector<string>& files)                            //��ȡpath�ļ����µ������ļ���
{
    long hFile = 0;
    struct _finddata_t fileinfo;
    string p;
    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
            if((fileinfo.attrib & _A_SUBDIR)) {                              //�ж��Ƿ�Ϊ�ļ���
				if(strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);  
				}
            } else {
				files.push_back(p.assign(path).append("\\\\").append(fileinfo.name));
			}
        } while(_findnext(hFile, &fileinfo) == 0);  
		_findclose(hFile);
	}
}
