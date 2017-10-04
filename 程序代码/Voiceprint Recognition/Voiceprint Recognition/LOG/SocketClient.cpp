#include "stdafx.h"
#include "SocketClient.h"

void SocketClient::setServerIP(string serverIP) 
{
    this->serverIP = serverIP;
}

void SocketClient::setPORT(int PORT) 
{
    this->PORT = PORT;
}

int SocketClient::getBufferSize() 
{
    return this->bufferSize;
}

bool SocketClient::pingTest()                                                //pingĿ������������Ŀ�������Ƿ�����
{
	if(_access("temp", 0) == -1) {                                       //�ж�logs�ļ����Ƿ����
        if (_mkdir("temp") == -1) {                                      //����logs�ļ���
            return false;
        }
    }

    string targetIP = this->serverIP;
    char fileName[256] = "temp//";
    char cmdstr[256] = "";
    strcat_s(fileName, targetIP.data());
    strcat_s(fileName, ".txt");
    
    strcat_s(cmdstr, "cmd /c ping ");
    strcat_s(cmdstr, targetIP.data());
    strcat_s(cmdstr, " -n 1 -w 500 >");
    strcat_s(cmdstr, fileName);
    
    WinExec(cmdstr, SW_HIDE);
    Sleep(500);
    
    ifstream inStream(fileName, ios::binary);
    if (!inStream.is_open()) {
        return false;
    }
    
    char buffer[512];
    inStream.getline(buffer, 512);
    inStream.getline(buffer, 512);
    inStream.getline(buffer, 512);
    
    if (strcmp(buffer, "����ʱ��") == 0) {
        return false;
    } else {
        return true;
    }
}

bool SocketClient::linkTest() 
{
    if (connect(this->client, (struct sockaddr*)&this->server, sizeof(this->server)) == INVALID_SOCKET) {
        cout << "ERROR : Socket connect failed !" << endl;
        return false;
    } else {
        return true;
    }
}

bool SocketClient::initSocket() 
{
    memset(this->bufferPool, 0, this->bufferSize);                           //���ջ�������ʼ��
    if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0) {
        cout << "ERROR : Winsock load failed !" << endl;
        return false;
    }

    this->server.sin_family = AF_INET;                                       //��Ҫ���ӵķ������ĵ�ַ��Ϣ
    this->server.sin_addr.s_addr = inet_addr(this->serverIP.data());         //�������е�IP��ַת��Ϊ�����Ʊ�ʾ�������ֽ�˳��IP��ַ
    this->server.sin_port = htons(this->PORT);
        
    this->client = socket(AF_INET, SOCK_STREAM, 0);                          //�����ͻ������׽�
    if (this->client == INVALID_SOCKET) {
        cout << "ERROR : Socket create failed !" << endl;
        return false;
    }
        
    return true;
}

bool SocketClient::sendMessage(string message)                               //����socket������Ϣ
{
    int msgLen;
	if ((msgLen = send(this->client, message.data(), strlen(message.data()), 0)) == SOCKET_ERROR) {
        cout << "ERROR : Socket send message error !" << endl;
        return false;
	}
	return true;
}

bool SocketClient::sendMessage(string message, int messageLen)               //����socket������Ϣ
{
    int msgLen;
	if ((msgLen = send(this->client, message.data(), messageLen, 0)) == SOCKET_ERROR) {
        cout << "ERROR : Socket send message error !" << endl;
        return false;
	} else if (msgLen != messageLen) {
		cout << "ERROR : Socket send message has failure byte (" << msgLen << ") !" << endl;
		return false;
	}
	return true;
}

void SocketClient::freeResourse() 
{
    closesocket(this->client);
    WSACleanup();
    delete this->bufferPool;
}