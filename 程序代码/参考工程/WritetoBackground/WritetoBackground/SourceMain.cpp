#include <afx.h>
#include <iostream>
#include <Windows.h>
using namespace std;

int main()
{
	/*
	HWND hWnd = ::FindWindowW(L"LogonUI", NULL);                                                                     //�������±����������ھ�� 
	HWND hWndc = ::GetWindow(hWnd, GW_CHILD);                                                                        //��ü��±��ͻ���������ô����Ǽ��±������ڵ��Ӵ��ڣ����Ǹ���ɫ�Ŀɱ༭���� 

	Sleep(5000);
	char password[256] = "sandaozhishu429";
	if(hWndc) {                                                                                                      //�������˸þ��
		for (int i = 0; i < strlen(password); ++i) {
			::SendMessage(hWnd, WM_CHAR, password[i], NULL);                                                         //���Ͱ�����Ϣ
		}
	}
	*/
	Sleep(5000);
	HWND wnd;                                                                                                        //���ھ��
	wnd = GetForegroundWindow();                                                                                     //��õ�ǰ����Ĵ��ھ��
	DWORD SelfThreadId = GetCurrentThreadId();                                                                       //��ȡ������߳�ID
	DWORD ForeThreadId = GetWindowThreadProcessId(wnd, NULL);                                                        //���ݴ��ھ����ȡ�߳�ID
	AttachThreadInput(ForeThreadId, SelfThreadId, true);                                                             //�����߳�
	wnd = GetFocus();                                                                                                //��ȡ�������뽹��Ĵ��ھ��
	AttachThreadInput(ForeThreadId, SelfThreadId, false);                                                            //ȡ�����ӵ��߳�

	char password[256] = "sandaozhishu429";
	for (int i = 0; i < (int) strlen(password); ++i) {
		SendMessage(wnd, WM_CHAR, WPARAM(password[i]), 0);                                                           //����һ������Ϣ
	}
	SendMessage(wnd, WM_KEYDOWN, VK_RETURN, 0);
	return 0;
}