#include "WaveRecorder.h"
#include "conio.h"

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