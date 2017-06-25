#include "Wav_File_Header.h"

bool Wav_File::Conversion_Tuple(void)
{
	try
	{
		cout << "TIP : Change to real format data ..." << endl;
		dataNumber = dataSize / chunkAlign;                                  //������ݸ���
		dataTuple = (long*)malloc(sizeof(long)*dataNumber);                  //Ϊ���ֽ����ݿռ丳ֵ
		long tempNum = 0;                                                    //��������ÿ�����ֽ�����
		for (unsigned long i = 0; i < dataNumber; ++i) {                     //�ܹ���dataNumber������
			int Flag = 0;
			for (short j = 0; j < chunkAlign; ++j) {                         //ÿ�������������ֽ�
				tempNum = MakeWord((long)data[i*chunkAlign + j], tempNum);   //ÿ�ν�tempNum��Ϊ���ֽ����ݣ����ֽ�Ϊ���ֽ���������������				
			}
			dataTuple[i] = tempNum;                                          //���������ֽ�����
			tempNum = 0;                                                     //��ʼ������������

			if (dataMax < dataTuple[i]) {                                    //������ֵ
				dataMax = dataTuple[i];
			}
			if (dataMin > dataTuple[i]) {                                    //�����Сֵ
				dataMin = dataTuple[i];
			}
		}
	}
	catch (invalid_argument &e) {
		cerr << e.what() << endl;
		MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
		return false;
	}
	return true;
}

long Wav_File::MakeWord(long NumberA, long NumberB)                          //�ϲ��ֽڣ�NumberA��ʾ���ֽ�λ��NumberB��ʾ���ֽ�λ
{
	int Flag = 0;                                                            //��־��������ʾ��������������1Ϊ����0Ϊ��
	if (NumberA >= 0) {
		if (NumberB >= 0) {
			return NumberA * 256 + NumberB;                                  //�ߵ��ֽڶ�����������ֱ����λ�ϲ�
		}
		else {
			return NumberA * 256 + abs(NumberB) + 128;                       //���ֽ�Ϊ���������ֽ�Ϊ����
		}
	}
	else {
		Flag = 1;
		if (NumberB > 0) {                                                   //���ֽ�Ϊ���������ֽ�Ϊ����
			return -1 * (abs(NumberA) * 256 + NumberB);
		}
		else {                                                               //�ߵ��ֽڶ��Ǹ���
			return -1 * (abs(NumberA) * 256 + abs(NumberB) + 128);
		}
	}
}

bool Wav_File::Read_File(void)
{
	try
	{
		cout << "TIP : Reading file ..." << endl;
		fread(id_RIFF, sizeof(char), 4, fp);                                   //��ȡ'RIFF'
		id_RIFF[4] = 0;                                                        //ĩβ����
		if (strcmp(id_RIFF, "RIFF")) {
			throw invalid_argument("ERROR : File not RIFF file !");
		}
		fread(&fileSize, sizeof(unsigned long), 1, fp);                        //��ȡ�ļ��Ĵ�С
		fread(id_WAVE, sizeof(char), 4, fp);                                   //��ȡ'WAVE'
		id_WAVE[4] = 0;
		if (strcmp(id_WAVE, "WAVE")) {
			throw invalid_argument("ERROR : File not WAVE file !");
		}
		fread(id_FMT, sizeof(char), 4, fp);                                    //��ȡ'FMT'
		id_FMT[3] = 32;
		id_FMT[4] = 0;
		fread(&formatLength, sizeof(unsigned long), 1, fp);                    //
		fread(&formatTag, sizeof(short), 1, fp);                               //��ȡ�ļ���ǩ
		fread(&channelsNumber, sizeof(short), 1, fp);                          //��ȡͨ����Ŀ
		fread(&sampleRate, sizeof(unsigned long), 1, fp);                      //��ȡ����Ƶ��
		fread(&secondBytes, sizeof(unsigned long), 1, fp);                     //��ȡÿ��������
		fread(&chunkAlign, sizeof(short), 1, fp);                              //��ȡ�����
		fread(&sampleBits, sizeof(short), 1, fp);                              //��ȡ������С
		if (formatLength > 16) {
			fread(&appendInformation, sizeof(short), 1, fp);                   //��ȡ������Ϣ
		}
		fread(id_DATA, sizeof(char), 4, fp);                                   //��ȡ'DATA'
		id_DATA[4] = 0;
		fread(&dataSize, sizeof(unsigned long), 1, fp);                        //��ȡ���ݴ�С
		data = (char*)malloc(sizeof(char)*dataSize);                           //�������ݵĴ洢�ռ�
		fread(data, sizeof(char), dataSize, fp);                               //��ȡ����
		fclose(fp);
	}
	catch (invalid_argument &e)
	{
		MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
		return false;
	}
	if (Conversion_Tuple() == true) {                                          //ת����ʽ�ɹ�
		MessageBoxA(NULL, "TIP : File read ok !", "TIP", MB_ICONASTERISK);
	}
	else {
		MessageBoxA(NULL, "ERROR : Data change failure !", "ERROR", 0);
		return false;
	}

	return true;
}

unsigned long Wav_File::Get_SampleRate(void)                                 //��ȡ����Ƶ��
{
	return sampleRate;
}

short Wav_File::Get_ChunkAlign(void)                                         //��ȡ�����ֽ���
{
	return chunkAlign;
}

long Wav_File::Get_Data(unsigned long Number)                                //��ȡĳ��λ���ϵ�����
{
	if (Number >= dataNumber) {                                              //�������Ҫ�������������ݸ���
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		return -1;
	}
	else {
		return dataTuple[Number];
	}
}

unsigned long Wav_File::Get_dataNumber(void)                                 //��ȡ�������ݸ���
{
	return dataNumber;
}

void Wav_File::ShowData(void)
{
	cout << "TIP : Show data ..." << endl;
	cout << id_RIFF << endl;
	cout << fileSize << endl;
	cout << id_WAVE << endl << endl;

	cout << id_FMT << endl;
	cout << formatLength << endl;
	cout << formatTag << endl;
	cout << channelsNumber << endl;
	cout << sampleRate << endl;
	cout << secondBytes << endl;

	cout << chunkAlign << endl;
	cout << sampleBits << endl;
	cout << appendInformation << endl << endl;

	cout << id_FACT << endl;
	cout << appendLength << endl;
	cout << appendNone << endl << endl;

	cout << id_DATA << endl;
	cout << dataSize << endl;

	for (unsigned int i = 0; i < dataNumber; ++i) {
		cout << dataTuple[i] << "\t";
	}
	cout << endl;

	cout << "MAX :" << dataMax << endl;
	cout << "MIN :" << dataMin << endl;
}

void Wav_File::SaveNewWav(unsigned long voiceNumber, vector<VoiceParagraph> voiceParagraph)      //����ȥ���հ״��������ļ�������1Ϊ����ĸ���������2Ϊ������������Ŀ�ʼ�����������Ϣ
{
	unsigned long endPointLength = 0;
	for (unsigned long i = 0; i < voiceNumber; ++i){
		VoiceParagraph temp = voiceParagraph[i];
		endPointLength += temp.voiceLength;
	}
	unsigned long removerLength = dataNumber - endPointLength;
	unsigned long removerSize = removerLength*chunkAlign;

	//�������ݲ����϶࣬��ʱ�޷����
	//Wav_File* newWav = (Wav_File*)malloc(sizeof(Wav_File));
	//newWav = this;

	FILE *fp;
	if ((fp = fopen("new file.wav", "wb")) == NULL) {
		cout << "ERROR : File open failed !" << endl;
		exit(-1);
	}

	try
	{
		unsigned long tempLength;
		cout << "TIP : Writing file ..." << endl;
		fwrite(id_RIFF, sizeof(char), 4, fp);                                   //д��'RIFF'
		tempLength = fileSize - removerSize;
		fwrite(&tempLength, sizeof(unsigned long), 1, fp);                        //д���ļ��Ĵ�С
		fwrite(id_WAVE, sizeof(char), 4, fp);                                   //д��'WAVE'
		fwrite(id_FMT, sizeof(char), 4, fp);                                    //д��'FMT'
		fwrite(&formatLength, sizeof(unsigned long), 1, fp);                    //д���ʽ�鳤��
		fwrite(&formatTag, sizeof(short), 1, fp);                               //д���ļ���ǩ
		fwrite(&channelsNumber, sizeof(short), 1, fp);                          //д��ͨ����Ŀ
		fwrite(&sampleRate, sizeof(unsigned long), 1, fp);                      //д�����Ƶ��
		fwrite(&secondBytes, sizeof(unsigned long), 1, fp);                     //д��ÿ��������
		fwrite(&chunkAlign, sizeof(short), 1, fp);                              //д������
		fwrite(&sampleBits, sizeof(short), 1, fp);                              //д��������С
		if (formatLength > 16) {
			fwrite(&appendInformation, sizeof(short), 1, fp);                   //д�븽����Ϣ
		}
		fwrite(id_DATA, sizeof(char), 4, fp);                                   //д��'DATA'
		tempLength = dataSize - removerSize;
		fwrite(&tempLength, sizeof(unsigned long), 1, fp);                        //д�����ݴ�С

		unsigned long n = 0;
		for (unsigned long i = 0; i < voiceNumber; ++i){
			VoiceParagraph tempParagraph = voiceParagraph[i];
			for (unsigned long j = tempParagraph.begin; j < tempParagraph.end; ++j, n += 2){
				fwrite(&data[j * 2], sizeof(char), 1, fp);
				fwrite(&data[j * 2 + 1], sizeof(char), 1, fp);
			}
		}
		fclose(fp);
	}
	catch (invalid_argument &e)
	{
		MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
	}
}