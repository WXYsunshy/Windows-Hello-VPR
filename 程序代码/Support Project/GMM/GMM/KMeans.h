/***************************************************************************
Module Name:
	KMeans

History:
	2003/10/16	Fei Wang
	2013 luxiaoxun
	Edit by leafspace
		TIME
			First time : 2017-7-23
			Last time : 2017-7-24
		CONTACT ME
			E-mail : 18852923073@163.com
***************************************************************************/

#pragma once
#include <fstream>
#include <assert.h>

class KMeans
{
public:
	enum InitMode                                                            //�����ģ�ͳ�ʼ��������ǩ���ͣ��������������ݳ�ʼ���ķ������뱾����m_initMode�����
	{
		InitRandom,
		InitManual,
		InitUniform,
	};

	KMeans(int dimNum = 1, int clusterNum = 1);                              //|Paragram1 : ���ݽ��� |Paragram2 : �ظ���| ��ʼ��Kmeans��
	~KMeans();

	void SetMeans(double **means)      { m_means = means; }                  //��������
	void SetInitMode(int initMode)     { m_initMode = initMode; }            //���ó�ʼ����ʽ
	void SetMaxIterNum(int iterNum)    { m_maxIterNum = iterNum; }           //������ߵ�������
	void SetEndError(double endError)  { m_endError = endError; }            //��������ֹͣ�ж�

	double** GetMean()                 { return m_means; }                   //��ȡ����
	double* GetMean(int i)	           { return m_means[i]; }                //��ȡĳ������
	int GetInitMode()		           { return m_initMode; }                //��ȡ��ʼ���ķ���
	int GetMaxIterNum()		           { return m_maxIterNum; }              //��ȡ��ߵ�������
	double GetEndError()	           { return m_endError; }                //��ȡ����ֹͣ�ж�


	/*	
		SampleFile: <size><dim><data>...
		LabelFile:	<size><label>...
	*/
	void Cluster(const char* sampleFileName, const char* labelFileName);     //���ִأ���GMM�еķ��࣬��ʽ�����ļ��е����ݶ�ȡ
	void Cluster(double *data, int N, int *Label);                           //���ִأ���GMM�еķ��࣬��ʽ��ԭ���û�������
	void Init(std::ifstream& sampleFile);
	void Init(double *data, int N);
	friend std::ostream& operator<<(std::ostream& out, KMeans& kmeans);

private:
	int m_dimNum;                                                            //���ݽ���
	int m_clusterNum;                                                        //�ظ���
	double** m_means;                                                        //����

	int m_initMode;                                                          //���ݳ�ʼ������
	int m_maxIterNum;		                                                 //The stopping criterion regarding the number of iterations
	double m_endError;		                                                 //The stopping criterion regarding the error

	double GetLabel(const double* x, int* label);
	double CalcDistance(const double* x, const double* u, int dimNum);
};
