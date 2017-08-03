#include "stdafx.h"
#include "VPR/WavData_CharaParameter.h"
#include "VPR/Model_KMeans.h"
#include "VPR/Model_GMM.h"
#include "Voiceprint RecognitionDlg.h"

WaveRecorder waveRecorder;
char* fileName;
string fileName_t;

void CVoiceprintRecognitionDlg::CompoundFile(vector<FILESTRUCT>& fileLib, int flag)    //���ڽ�txt��Ϣ�뵱ǰ�ļ�������������
{
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
	if (flag == 0) {                                                         //����Ҫ�������ļ���
		strcat_s(path, "wavLib");
	} else if (flag == 1) {
		strcat_s(path, "voiceLib");
	}

	vector<string> files;
	getFiles(path, files);                                                   //��ȡ�ļ����ڵ������ļ�����·������

	strcat_s(path, "\\\\info.list");                                         //ָ���ļ��еĶ�ȡ�ļ�
	ifstream in(path);
	fileLib.clear();
	readList(in, fileLib);                                                   //��ȡlist�ļ�����
	in.close();

	vector<FILESTRUCT> newLib;
	for (int i = 0, j = 0; i < (int) files.size(); ++i) {
		string fileName = files[i];
		if (strcmp(getFileName(fileName).data(), "info.list") == 0) {        //�����ǰ�ļ����ļ���Ϊinfo.list˵����Ϊ�����ļ�
			continue;
		}
		for (j = 0; j < (int) fileLib.size(); ++j) {
			if (strcmp(getFileName(fileName).data(), fileLib[j].fileName.data()) == 0) {         //�����ǰ�ļ������б�����ƥ�䣬��˵������һ��������б���
				newLib.push_back(fileLib[j]);
				break;
			}
		}
		if (j == fileLib.size()) {
			FILESTRUCT item;
			item.fileName = getFileName(fileName).data();
			if (flag == 0) {
				if (::fileName != NULL && (strcmp(::fileName_t.data(), fileName.data()) == 0)) { //���¼���ļ�·���뵱ǰ���δ֪���ļ�·����ͬ
					CString str;
					GetDlgItem(IDC_EDIT1)->GetWindowText(str);               //��ȡ��������
					string tempstr = CStringA(str);
					if (tempstr.size() == 0) {                               //��������ûд
						item.peopleName = "δ֪";
					} else {
						item.peopleName = tempstr;
					}
				} else {
					item.peopleName = "δ֪";
				}
			} else if (flag == 1) {
				item.peopleName = "δ֪";
			}
			newLib.push_back(item);
		}
	}

	ofstream wavOut(path);
	writeList(wavOut, newLib);                                               //����д������
	wavOut.close();

	fileLib.clear();
	fileLib = newLib;
}

int CVoiceprintRecognitionDlg::GetItemSelect(int index)                      //��ȡĳ��listControl��ǰѡ������к�
{
	int count = 0;
	switch (index)
	{
	case 0 : count = this->listCtrl_1.GetItemCount(); break;
	case 1 : count = this->listCtrl_2.GetItemCount(); break;
	default : break;
	}

	for (int i = 0; i < count; ++i) {
		switch (index)
		{
		case 0 : 
			if (this->listCtrl_1.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED) {
				return i;
			}
			break;
		case 1 : 
			if (this->listCtrl_2.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED) {
				return i;
			}
			break;
		default : break;
		}
	}
	return -1;
}

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

bool CVoiceprintRecognitionDlg::OnButton4_refresh()                          //¼���ļ�ˢ��
{
	CString str_f, str_p;
	listCtrl_1.DeleteAllItems();
	for (int i = 0; i < (int) this->wavLib.size(); ++i) {
		FILESTRUCT item = this->wavLib[i];
		str_f = item.fileName.c_str();
		str_p = item.peopleName.c_str();
		listCtrl_1.InsertItem(i, str_f);                                     //�����б��ļ�����Ϣ
		listCtrl_1.SetItemText(i, 1, str_p);                                 //�����б��û���Ϣ
	}
	return true;
}

bool CVoiceprintRecognitionDlg::OnButton5_refresh()                          //ģ���ļ�ˢ��
{
	CString str_f, str_p;
	listCtrl_2.DeleteAllItems();
	for (int i = 0; i < (int) this->voiceLib.size(); ++i) {
		FILESTRUCT item = this->voiceLib[i];
		str_f = item.fileName.c_str();
		str_p = item.peopleName.c_str();
		listCtrl_2.InsertItem(i, str_f);                                     //�����б��ļ�����Ϣ
		listCtrl_2.SetItemText(i, 1, str_p);;                                //�����б��û���Ϣ
	}
	return true;
}

void* record(void* args)                                                     //¼���߳�
{
	waveRecorder.set_FileName((char*)fileName);
	waveRecorder.Start();
	delete fileName;
	return NULL;
}

string getFileName(string path)                                              //��ĳ��·��ת��Ϊĳ���ļ���
{
	return path.substr(path.rfind('\\') + 1, path.size() - path.rfind('\\') - 1);
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

void readList(ifstream& in, vector<FILESTRUCT>& list)                        //��ȡ�ļ������ݵ�list��
{
	char buffer[256];
	string str_f, str_p;
	char fileName[256], peopleName[256];
	if (in.is_open()) {
		while (!in.eof()) {
			in.getline(buffer, 256);
			if (strlen(buffer) == 0) {                                       //��ֹ�������һ��ֻ��һ�����л��ظ���ȡ
				continue;
			}
			sscanf(buffer, "%s %s", &fileName, &peopleName);                 //��ʽ���ַ���
			CChineseCode::UTF_8ToGB2312(str_f, fileName, strlen(fileName));
			CChineseCode::UTF_8ToGB2312(str_p, peopleName, strlen(peopleName));
			FILESTRUCT item(str_f, str_p);
			list.push_back(item);
			memset(buffer, 0, 256);
		}
	}
}

void writeList(ofstream& out, vector<FILESTRUCT>& list)                      //��list�ļ�����д��������
{
	string str_f, str_p;
	if (out.is_open()) {
		for (int i = 0; i < (int) list.size(); ++i) {
			CChineseCode::GB2312ToUTF_8(str_f, (char*) list[i].fileName.data(), list[i].fileName.length());
			CChineseCode::GB2312ToUTF_8(str_p, (char*) list[i].peopleName.data(), list[i].peopleName.length());
			out << str_f.data() << "\t" << str_p.data() << endl;
		}
	}
}

bool trainingWAV(string wavfilePath, string gmmfilePath, string peopleName)  //ѵ��wav�ļ�
{


	return true;
}