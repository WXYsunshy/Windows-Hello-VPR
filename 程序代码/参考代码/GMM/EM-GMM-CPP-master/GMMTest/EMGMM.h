#pragma once
/*******************************************************************

EMGMM.h		developed by naka_t	2011.03.08


	�����K�E�X���z��EM�A���S���Y���ɂ��p�����[�^����

�@���v���O�����쐬				naka_t	2010.03.08

  Copyright (C) 2011  naka_t <naka_t@apple.ee.uec.ac.jp>
 *******************************************************************/
#pragma warning(disable: 4819)
#include <vector>
#include <cv.h>
#include <cxcore.h> 
#include <highgui.h>

#pragma comment (lib, "cv210.lib") 
#pragma comment( lib, "cxcore210.lib")
#pragma comment( lib, "highgui210.lib")

class CEMGMM
{
public:
	CEMGMM(void);
	~CEMGMM(void);

	// �f�[�^�̃Z�b�g��������
	// m ; �N���X��
	// x : �f�[�^
	// N : �f�[�^��
	// dim : �f�[�^�̎���
	void SetData( int m , double **x , int N ,int dim );

	void EStep();						// E step
	void MStep();						// M step
	int GetCluster(int n);				// �N���X�^�����O���ʂ��擾
	double CalcLogliklihood();			// �ޓx���擾
	void SaveResult( const char *dir );	// ���ʂ�ۑ�

protected:
	int m_m;	// �N���X��
	int m_N;	// �f�[�^��
	int m_d;	// ����

	std::vector<cv::Mat> m_sigma_i;				// ���U
	std::vector<cv::Mat> m_mu_i;				// ����
	std::vector<double> m_alpha_i;				// ������
	std::vector<double> m_N_i;					// i�Ԗڂ̃N���X�ɑ�����f�[�^��
	std::vector<cv::Mat> m_x_n;					// �f�[�^
	std::vector<std::vector<double>> m_P_in;	// P(i|x_n,��)

	double CalcPx_i( cv::Mat &x , int i );		// P( x | i ) �̌v�Z
};
