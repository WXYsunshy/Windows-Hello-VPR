#include "WavFile_Initial.h"

int main()
{

	FILE *fp;
	if ((fp = fopen("΢��Ĭ��.wav", "rb")) == NULL) {
		cout << "ERROR : File open failed !" << endl;
		exit(-1);
	}
	WavFile_Initial wavFile(fp);
	return 0;
}