// sendmidicommand.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

TCHAR usage[] = _T("sendmidicommand <devicename> <CMD[HEX]> <CMD[HEX]> <CMD[HEX]> ...");

int printdevicelist(){
	int i;
	int res = 0;
	int foundflag = 0;
	TCHAR  c_deviceName_tmp[64], c_deviceName[64];
	memset(c_deviceName, 0, 64);
	printf("device list\n");
	for (i = 0; i < 16; i++){
		res = MIDIIn_GetDeviceName(i, c_deviceName_tmp, 64);
		if (res == 0){
			if (foundflag == 0)
			    _tprintf(_T("Device not found\n"));
			return foundflag;
		}
		foundflag++;
		wcscpy_s(c_deviceName, 64, c_deviceName_tmp);
		_tprintf(_T("%s\n"), c_deviceName);
	}
	return 0;

}

int printcommandlost(unsigned char byMIDIMessage[128], int length){
	int i;
	int res = 0;
	printf("Command\n");
	for (i = 0; i < length; i++){
		_tprintf(_T("0x%x "), byMIDIMessage[i]);
	}
	_tprintf(_T("\n"));
	return 0;

}

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR  c_usedeviceName[64];
	unsigned char byMIDIMessage[128];
	int i;
	int msglen = 0;

	/* Option check */
	if (argc < 2){
		_tprintf(usage);
		_tprintf(_T("\n"));
		printdevicelist();
		return 1;
	}
	_tcscpy_s(c_usedeviceName,64, argv[1]);


	for (i = 0; i < 127; i++) {
		if ((i + 2)  >= (argc)) break;
		long value = _tcstol(argv[i + 2],NULL,0);
		byMIDIMessage[i] = (unsigned char)value;
		byMIDIMessage[i+1] = 0;
	}
	msglen = i ;

	printcommandlost(byMIDIMessage, msglen);

	MIDIOut* pMIDIOut = MIDIOut_Open(c_usedeviceName);
	if (pMIDIOut == NULL) {
		printf("MIDI出力デバイスが開けません。");
		return 0;
	}

	MIDIOut_PutMIDIMessage(pMIDIOut, byMIDIMessage, msglen); /* コマンド送信 */
	MIDIOut_Close(pMIDIOut);  /* MIDI出力デバイスを閉じる */
	pMIDIOut = NULL;
	return 1;
//	return 0;
}

