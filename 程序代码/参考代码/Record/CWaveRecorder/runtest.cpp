#include "WaveRecorder.h"
#include "conio.h"

/*����һ���ص�����ʾ��*/
void CALLBACK ChunkArrived(array <char, CHUNCK_SIZE> ChunkData, UINT ChunksCount, bool bIsLast);

int main()
{
	char ch = 0;
	WaveRecorder sound_gbr;
	while (ch!='q')
	{
		ch = _getch();
		switch (ch)
		{
		case 'a':
			cout << "ʵʱ��ȡ��Ƶ���ݲ����������·����test.wav" << endl;
			sound_gbr.set_FileName("test.wav");
			sound_gbr.Start();
			system("pause");
			sound_gbr.Stop();
			sound_gbr.Reset();
			break;
		case 's':
			cout << "��ʵʱ��ȡ��Ƶ���ݣ�ChunkData" << endl;
			sound_gbr.set_Callback(ChunkArrived);
			sound_gbr.Start();
			system("pause");
			sound_gbr.Stop();
			sound_gbr.Reset();
			break;
		case 'd':
			cout << "Start����Ƶ���ݻ�����RawData�������ֱ�Ӵ�����" << endl;
			sound_gbr.Start();
			system("pause");
			sound_gbr.Stop();
			sound_gbr.Reset();
			break;
		default:
			break;
		}
	}
}

/*����һ���ص�����ʾ��*/
void CALLBACK ChunkArrived(array <char, CHUNCK_SIZE> ChunkData, UINT ChunksCount, bool bIsLast)
{
	// ChunkData.data()�����ݵ���ʼָ��
	// ChunkData.size()����CHUNCK_SIZE
	if (bIsLast)
	{
		// �ڴ�������һ�����ݵĴ���
		// AudioWrite(sessionID, ChunkData.data(), ChunkData.size(), AUDIO_LAST);
	}
	// cout << "ChunkData������" << ChunksCount << endl;
	if (ChunksCount > 0)
	{
		// AudioWrite(sessionID, ChunkData.data(), ChunkData.size(), AUDIO_CONTINUE);
	}
	else
	{
		// AudioWrite(sessionID, ChunkData.data(), ChunkData.size(), AUDIO_FIRST );
	}
}