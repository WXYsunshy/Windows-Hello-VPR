
// Voiceprint Recognition.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVoiceprintRecognitionApp:
// �йش����ʵ�֣������ Voiceprint Recognition.cpp
//

class CVoiceprintRecognitionApp : public CWinApp
{
public:
	CVoiceprintRecognitionApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVoiceprintRecognitionApp theApp;