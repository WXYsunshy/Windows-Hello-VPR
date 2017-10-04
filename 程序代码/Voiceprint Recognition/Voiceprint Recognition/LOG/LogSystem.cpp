#include "stdafx.h"
#include "LogSystem.h"

void* messageThread(void* arg)
{
	bool isQuit = false;
	while (true) {
        if (p_logSystem->sendFileFlag) {                                     //��ǰ���͵����ļ�
			if ((_access(p_logSystem->filePath.data(), 0)) == -1) {          //����ļ�������
				p_logSystem->sendFileFlag = false;
				break;
			}

			char* filePath = new char[256];
			ReadConfig *readConfig = new ReadConfig;                         //���ļ���ȡ
			bool isSuccess = readConfig->ReadFile();                         //��ȡ�ļ�
			if (isSuccess) {
				strcpy(filePath, readConfig->getPath().data());
			}
			delete readConfig;

			char* fileName = new char[256];
			time_t date = time(0);
			sprintf(fileName, "%d%d%d%d%d%d.wav", localtime(&date)->tm_year,
				localtime(&date)->tm_mon, localtime(&date)->tm_yday, 
				localtime(&date)->tm_hour, localtime(&date)->tm_min, 
				localtime(&date)->tm_sec);
			strcat(filePath, fileName);
			delete fileName;
			cout << filePath << endl;

			char tempStr[256];
			int index = 0;
			for (int i = 0; filePath[i] != 0; ++i) {
				tempStr[index++] = filePath[i];
				if (filePath[i] == '\\') {
					tempStr[index++] = '\\';
				}
			}
			tempStr[index] = 0;

			CopyFileA(p_logSystem->filePath.data(), tempStr, FALSE);
			p_logSystem->socketClient.sendMessage(filePath);                 //����socket�����ļ�·��
			delete filePath;

			/*
			//Socket ���ͷ��� ��δ�ɹ���
            ifstream in;
			in.open(p_logSystem->filePath.data(), fstream::in | ios::binary);//�Զ�������ʽ���ļ�
            p_logSystem->pFilebuf = in.rdbuf();                              //��ȡ�ļ��������
            int fileSize = p_logSystem->pFilebuf->pubseekoff(0, ios::end, ios::in);    //��ȡ�����ļ��Ĵ�С
            p_logSystem->pFilebuf->pubseekpos(0, ios::in);                   //�ƶ�ָ�뵽�ļ���ͷ

            int bufferSize = p_logSystem->socketClient.getBufferSize();      //��ȡsocket�������Ĵ�С
            char *bufferPool = new char[bufferSize];                         //�½��ļ�������

			sprintf(bufferPool, "%d\n", fileSize);
			p_logSystem->socketClient.sendMessage(bufferPool);               //�����ļ���С

            while (fileSize > 0) {                                           //ֻҪ�ļ���С���ڣ���һֱ����
				int sendSize;
                if (fileSize >= bufferSize) {
					sendSize = bufferSize;
                } else {                                                     //�����в����㻺������С�Ĳ���
					sendSize = fileSize;
                }
				p_logSystem->pFilebuf->sgetn(bufferPool, sendSize);          //��ȡ��������
                fileSize -= sendSize;
				p_logSystem->socketClient.sendMessage(bufferPool, sendSize); //���ͻ�����������
            }

            in.close();                                                      //�ر��ļ���
            delete bufferPool;                                               //���������
			*/
            p_logSystem->sendFileFlag = false;
			break;                                                           //�����ļ��Ự�������
        } else {                                                             //��ǰ���͵��ǵ�����Ϣ
            while (p_logSystem->messageQueue.getLength() > 0) {              //ֻҪ��Ϣ�б��л�������Ϣ����һֱ����
                string message = p_logSystem->messageQueue.popMessage();     //��Ϣ�б�����Ϣ
                p_logSystem->socketClient.sendMessage(message);              //����socket������Ϣ
				if (message == "<File>\n") {
					break;
				}

				if (message == "<Finish>\n") {
					isQuit = true;
				}
            }
        }

		if (isQuit) {
			break;
		}
    }
	p_logSystem->stopSocket();
	return 0;
}