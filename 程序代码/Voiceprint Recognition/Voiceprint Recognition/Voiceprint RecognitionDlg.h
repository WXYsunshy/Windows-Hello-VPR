﻿
// Voiceprint RecognitionDlg.h : 头文件
//

#pragma once
#include "Shockwaveflash.h"

// CVoiceprintRecognitionDlg 对话框
class CVoiceprintRecognitionDlg : public CDialogEx
{
// 构造
public:
	CVoiceprintRecognitionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VOICEPRINTRECOGNITION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
