
// Voiceprint RecognitionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Voiceprint Recognition.h"
#include "Voiceprint RecognitionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVoiceprintRecognitionDlg �Ի���




CVoiceprintRecognitionDlg::CVoiceprintRecognitionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVoiceprintRecognitionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVoiceprintRecognitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, flashshow);
	DDX_Control(pDX, IDC_LIST1, listCtrl_1);
	DDX_Control(pDX, IDC_LIST2, listCtrl_2);
	DDX_Control(pDX, IDC_BUTTON1, buttonCtrl_1);
}

BEGIN_MESSAGE_MAP(CVoiceprintRecognitionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CVoiceprintRecognitionDlg ��Ϣ�������

BOOL CVoiceprintRecognitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CRect rectCtrl;
	GetDlgItem(IDC_SHOCKWAVEFLASH1)->GetWindowRect(&rectCtrl);
	rectCtrl.left = 35;
	rectCtrl.right = 217;
	rectCtrl.top = 45;
	rectCtrl.bottom = 200;
	flashshow.MoveWindow(&rectCtrl, true);
	TCHAR strCurDrt[500];
	int nLen = ::GetCurrentDirectory(500, strCurDrt);
	if (strCurDrt[nLen] != '\\') {
		strCurDrt[nLen++] = '\\';
		strCurDrt[nLen] = '\0';
	}
	CString strFileName = strCurDrt;
	strFileName += "flash.swf";
	this->flashshow.LoadMovie(0, strFileName);
	this->flashshow.Play();
	SetWindowPos(&this->flashshow,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);


	CRect rect;
	this->listCtrl_1.GetHeaderCtrl()->EnableWindow(false);                   //�̶����ⲻ���ƶ�
	listCtrl_1.GetClientRect(&rect);                                   //��ȡ��������б���ͼ�ؼ���λ�úʹ�С
	listCtrl_1.SetExtendedStyle(listCtrl_1.GetExtendedStyle()
		| LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);                            //Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����

	listCtrl_1.InsertColumn(0, _T("�ļ���"), LVCFMT_CENTER, rect.Width() / 2, 0);
	listCtrl_1.InsertColumn(1, _T("¼����"), LVCFMT_CENTER, rect.Width() / 2, 1);

	this->listCtrl_2.GetHeaderCtrl()->EnableWindow(false);                   //�̶����ⲻ���ƶ�
	listCtrl_2.GetClientRect(&rect);                                   //��ȡ��������б���ͼ�ؼ���λ�úʹ�С
	listCtrl_2.SetExtendedStyle(listCtrl_2.GetExtendedStyle()
		| LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);                            //Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����

	listCtrl_2.InsertColumn(0, _T("�ļ���"), LVCFMT_CENTER, rect.Width() / 2, 0);
	listCtrl_2.InsertColumn(1, _T("������"), LVCFMT_CENTER, rect.Width() / 2, 1);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVoiceprintRecognitionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVoiceprintRecognitionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVoiceprintRecognitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

