// GMMTest.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//
#include "stdafx.h"
#include "EMGMM.h"
#include "utility.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CEMGMM gmm;
	int d, N;					// �����A�f�[�^��
	double **x;					// �f�[�^
	double maxLik = -DBL_MAX;	// �ΐ��ޓx�̍ő�l

	// �f�[�^�ǂݍ���
	x = LoadMatrix<double>( d , N , "sample.txt" );

	// �����l��ς��Čv�Z
	for(int i=0 ; i<10 ; i++ )
	{
		double lik;
		gmm.SetData( 4 , x , N , d );

		// ��������܂ŌJ��Ԃ�
		for(int i=0 ; i<40 ; i++ )
		{
			printf("%d���..." , i );
			gmm.EStep();
			gmm.MStep();
			lik = gmm.CalcLogliklihood();
			printf("L = %lf\n" , gmm.CalcLogliklihood() );
		}

		if( maxLik < lik )
		{
			// �ޓx���ő�Ȃ�Εۑ�
			maxLik = lik;
			gmm.SaveResult( "result" );
			printf("�X�V L = %lf \n" , lik );
		}
	}

	// ���������
	Free( x );
	return 0;
}

