
// Voiceprint RecognitionDlg.h : ͷ�ļ�
//

#pragma once
#include "Shockwaveflash.h"

// CVoiceprintRecognitionDlg �Ի���
class CVoiceprintRecognitionDlg : public CDialogEx
{
// ����
public:
	CVoiceprintRecognitionDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VOICEPRINTRECOGNITION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CShockwaveflash flashshow;
	CListCtrl listCtrl_1;
	CListCtrl listCtrl_2;
	CButton buttonCtrl_1;
};
