#include "Wav_File_Handle.h"

const int Wav_File_Work::N = 256;                                            //��ʼ��ÿ�����Ĵ���
const int Wav_File_Work::FrameShift = 10;                                    //��ʼ����������֡��
const double Wav_File_Work::PI = 3.14159;                                    //��ʼ��Բ����
const unsigned long Wav_File_Work::minSilence = 6;                           //��̾�������
const unsigned long Wav_File_Work::minVoiceLength = 15;                      //�����������


bool Wav_File_Work::Conversion_Double(void)                                  //���������ֽ�����ת��ΪDouble����
{
	try
	{
		cout << "TIP : Change to double data ..." << endl;
		const double Flag = pow((double)2, Wav_File::Get_ChunkAlign() * 8);  //��ʾ����ԭ�������ֵ��һ��
		for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {     //����ÿ������
			dataDouble[i] = (double)Wav_File::Get_Data(i) / Flag;            //����ÿ��������[-1,1]֮��
		}
	}
	catch (invalid_argument &e){
		cerr << e.what() << endl;
		MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
		return false;
	}
	return true;
}

double Wav_File_Work::Hamming_window(double data)                            //����������
{
	if (data >= 0 && data <= (N - 1)) {                                      //0<= n <= N-1�����
		return 0.54 - 0.46*cos(2 * PI*data / (N - 1));                       //������ֵ
	}
	else {
		return 0;
	}
}

short Wav_File_Work::Sign_Function(double data)                              //���ʱ�����ʵĸ������ź���
{
	if (data >= 0) {
		return 1;
	}
	else {
		return 0;
	}
}

bool Wav_File_Work::Frame_Energy(void)                                       //�������ʱ֡����
{
	cout << "TIP : Calculate Energy ..." << endl;
	maxEnergy = 0;                                                           //����ʱ֡������0
	minEnergy = 1000000;                                                     //��С��ʱ֡������1000000
	double sum = 0;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift){        //�������ж�ʱ֡�������ݵĸ���
		for (unsigned long j = i; j < i + N; ++j) {                          //�������е�ÿһ������
			sum += pow(dataDouble[j] * Hamming_window(i + N - 1 - j), 2);    //��ÿһ�����ݵ�����
		}
		if (sum > maxEnergy) {                                               //��ȡ����ʱ֡����
			maxEnergy = sum;
		}
		if (sum < minEnergy) {                                               //��ȡ����ʱ֡����
			minEnergy = sum;
		}
		dataEnergy.push_back(sum);                                           //����֡�Ķ�ʱ֡��������
		sum = 0;
	}

	return true;
}

bool Wav_File_Work::Frame_ZCR(void)                                          //�������ʱ������
{
	cout << "TIP : Calculate ZCR ..." << endl;
	maxZCR = 0;                                                              //����ʱ��������0
	minZCR = 1000000;                                                        //��С��ʱ��������1000000
	double sum = 0;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift) {        //�������ж�ʱ֡���������ݵĸ���
		for (unsigned long j = i; j < i + N; ++j) {                          //�������е�ÿһ������
			sum += abs(Sign_Function(dataDouble[j]) - Sign_Function(dataDouble[j - 1]))//�������еľ���ֵ����
				*Hamming_window(i + N - 1 - j);
		}
		sum /= 2;
		if (sum > maxZCR) {                                                  //��ȡ����ʱ������
			maxZCR = sum;
		}
		if (sum < minZCR) {                                                  //��ȡ����ʱ������
			minZCR = sum;
		}
		dataZCR.push_back(sum);
		sum = 0;
	}
	return true;
}

bool Wav_File_Work::Frame_EnergyZcr(void)                                    //����ͬʱ��ȡ��ʱ֡�������ʱ������
{
	cout << "TIP : Calculate Engry and ZCR ..." << endl;
	maxEnergy = 0;                                                           //����ʱ֡������0
	minEnergy = 1000000;                                                     //��С��ʱ֡������1000000
	maxZCR = 0;                                                              //����ʱ��������0
	minZCR = 1000000;                                                        //��С��ʱ��������1000000
	double sumEnergy = 0;
	double sumZcr = 0;
	double hanming = 0;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift) {        //�������ж�ʱ֡�������ݵĸ���
		for (unsigned long j = i; j < i + N; ++j) {                          //�������е�ÿһ������
			hanming = Hamming_window(i + N - 1 - j);
			sumEnergy += pow(dataDouble[j] * hanming, 2);                    //��ÿһ�����ݵ�����
			sumZcr += abs(Sign_Function(dataDouble[j]) - Sign_Function(dataDouble[j - 1]))       //�������еľ���ֵ����
				*hanming;
		}
		sumZcr /= 2;

		if (sumEnergy > maxEnergy) {                                         //��ȡ����ʱ֡����
			maxEnergy = sumEnergy;
		}
		if (sumEnergy < minEnergy) {                                         //��ȡ����ʱ֡����
			minEnergy = sumEnergy;
		}
		if (sumZcr > maxZCR) {                                               //��ȡ����ʱ������
			maxZCR = sumZcr;
		}
		if (sumZcr < minZCR) {                                               //��ȡ����ʱ������
			minZCR = sumZcr;
		}
		dataEnergy.push_back(sumEnergy);                                     //����֡�Ķ�ʱ֡��������
		dataZCR.push_back(sumZcr);
		sumEnergy = 0;
		sumZcr = 0;

	}

	return true;
}

double Wav_File_Work::Get_maxEnergy(void)                                    //��ȡ����ʱ֡����
{
	return maxEnergy;
}
double Wav_File_Work::Get_minEnergy(void)                                    //��ȡ��С��ʱ֡����
{
	return minEnergy;
}
double Wav_File_Work::Get_maxZCR(void)                                       //��ȡ����ʱ������
{
	return maxZCR;
}
double Wav_File_Work::Get_minZCR(void)                                       //��ȡ��С��ʱ������
{
	return minZCR;
}

double Wav_File_Work::Get_dataNumber(void)                                   //��ȡDouble���ݵĸ���
{
	return Wav_File::Get_dataNumber();
}

double Wav_File_Work::Get_dataEZNumber(void)                                 //��ȡ���������ʵĸ���
{
	return min(dataEnergy.size(), dataZCR.size());
}

double Wav_File_Work::Get_DataDouble(unsigned long Number)                   //��ȡת�����Double����
{
	if (Number >= (Wav_File::Get_dataNumber()) || Number < 0) {              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataDouble[Number];
	}
}

double Wav_File_Work::Get_DataEnergy(unsigned long Number)                   //��������ҵ���Ӧ�Ķ�ʱ֡����
{
	if (Number >= dataEnergy.size() || Number < 0) {                         //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataEnergy[Number];
	}
}
double Wav_File_Work::Get_DataZCR(unsigned long Number)                      //��������ҵ���Ӧ�Ķ�ʱ������
{
	if (Number >= dataZCR.size() || Number < 0) {                            //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataZCR[Number];
	}
}
int Wav_File_Work::Get_WindowLength(void)                                    //��ȡ֡�������Ĵ�С��
{
	return N;
}

unsigned long Wav_File_Work::Get_voiceNumber(void)                           //��ȡ�����������
{
	return voiceNumber;
}
VoiceParagraph Wav_File_Work::Get_dataVoicePoint(unsigned long Number)       //��ȡĳ����������
{
	if (Number >= voiceNumber || Number < 0) {                               //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
	}
	else {
		return voiceParagraph[Number];
	}
}

void Wav_File_Work::ShowData(void)                                           //���Ǹ����չʾ���ݺ���
{
	int max = 0, min = 0;
	cout << "TIP : Double data " << endl;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {
		//cout << dataDouble[i] << "\t";
		if (dataDouble[i] > dataDouble[max]) {
			max = i;
		}
		if (dataDouble[i] < dataDouble[min]) {
			min = i;
		}
	}
	cout << endl;
	cout << "Max " << dataDouble[max] << endl;
	cout << "Min " << dataDouble[min] << endl;
	
	cout << "TIP : Parameter " << endl;
	cout << "Max energy " << Get_maxEnergy() << endl;
	cout << "Min energy " << Get_minEnergy() << endl;
	cout << "Max ZCR " << Get_maxZCR() << endl;
	cout << "Min ZCR " << Get_minZCR() << endl;

	/*
	cout << "TIP : Energy " << endl;
	for (auto i : dataEnergy) {
	cout << i << "\t";
	}
	cout << endl;

	cout << "TIP : ZCR " << endl;
	for (auto i : dataZCR) {
	cout << i << "\t";
	}
	cout << endl;
	*/
	/*
	for (int i = 0; i < (Wav_File::Get_dataNumber() - N); ++i) {
	cout << Get_DataEnergy(i) << endl;
	}

	for (int i = 0; i < (Wav_File::Get_dataNumber() - N); ++i) {
	cout << Get_DataZCR(i) << endl;
	}
	*/
}

void Wav_File_Work::SaveNewWav(void)                                         //����ȥ���հ״��������ļ�
{
	Wav_File::SaveNewWav(voiceNumber, voiceParagraph);                       //���ø�������ɺ���
}

bool Wav_File_Work::Endpoint_Detection(void)                                 //�˵��⺯��
{
	//this->Frame_Energy();                                                    //�����ʱ֡����
	//this->Frame_ZCR();                                                       //�����ʱ������
	this->Frame_EnergyZcr();                                                 //�����ʱ֡�������ʱ������

	energyHigh = 10;                                                         //��ʼ����ʱ֡����������
	energyLow = 2;                                                           //��ʼ����ʱ֡����������
	zcrHigh = 10;                                                            //��ʼ����ʱ�����ʸ�����
	zcrLow = 5;                                                              //��ʼ����ʱ�����ʵ�����

	energyHigh = min(energyHigh, Get_maxEnergy() / 4);                       //������ʱ֡����������
	energyLow = min(energyLow, Get_maxEnergy() / 8);                         //������ʱ֡����������
	zcrHigh = min(zcrHigh, Get_maxZCR() / 4);                                //������ʱ֡����������
	zcrLow = min(zcrLow, Get_maxZCR() / 8);                                  //������ʱ֡����������

	int statusFlag = 0;                                                      //��������״̬��־
	unsigned long begin = 0;                                                 //������������
	unsigned long end = 0;                                                   //����������յ�
	unsigned long voiceLength = 0;                                           //��������ĳ���
	unsigned long silence = 0;                                               //��������ĳ���
	voiceNumber = 0;

	for (unsigned long i = 0, frame = 0; i < Wav_File::Get_dataNumber() - N; ++i) { //����ÿһ֡
		frame = (i - N) / Wav_File_Work::FrameShift + 1;
		if (i <= 256){
			frame = 0;
		}
		switch (statusFlag)
		{
		case MUTEPARAGRAPH:
		case INTERIMPARAGRAPH:
			if (Get_DataEnergy(frame) > energyHigh) {                            //֡������������������,����������
				begin = max(i - voiceLength - 1, 0);
				statusFlag = VOICEPARAGRAPH;
				voiceLength++;
				silence = 0;
			}
			else if (Get_DataEnergy(frame) > energyLow || Get_DataZCR(frame) > zcrLow) {       //���ɶ�
				statusFlag = INTERIMPARAGRAPH;
				voiceLength++;
			}
			else {                                                           //������
				statusFlag = MUTEPARAGRAPH;
				voiceLength = 0;
			}
			break;
		case VOICEPARAGRAPH:
			if (Get_DataEnergy(frame) > Get_minEnergy() || Get_DataZCR(frame) > Get_minZCR()) {//������������
				voiceLength++;
			}
			else {                                                           //����������
				silence++;
				if (silence < minSilence) {                                  //����������������δ����
					voiceLength++;
				}
				else {
					if (voiceLength < minVoiceLength) {                      //�����γ���̫�̣���Ϊ������
						statusFlag = MUTEPARAGRAPH;
						silence = 0;
						voiceLength = 0;
					}
					else {                                                   //��������
						statusFlag = OVERPARAGRAPH;
						end = max(begin + voiceLength, 0);
					}
				}
			}
			break;
		case OVERPARAGRAPH:
			voiceParagraph.push_back(VoiceParagraph(begin, end, voiceLength));         //��������������Ϣ
			voiceNumber++;                                                   //��������+1
			statusFlag = MUTEPARAGRAPH;
			break;
		default:
			MessageBoxA(NULL, "ERROR : Status failure !", "ERROR", MB_ICONHAND);
			return false;
			break;
		}
	}
	cout << "TIP : Voice number is " << voiceNumber << endl;

	return true;
}