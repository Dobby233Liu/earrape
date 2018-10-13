#include <Windows.h>

HCRYPTPROV prov;

int random() {
	if (prov == NULL)
		if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			ExitProcess(1);

	int out;
	CryptGenRandom(prov, sizeof(out), (BYTE *)(&out));
	return out & 0x7fffffff;
}
int main(){
    const int samples = 44100;

	WAVEFORMATEX fmt = { WAVE_FORMAT_PCM, 1, samples, samples, 1, 8, 0 };

	HWAVEOUT hwo;
	waveOutOpen(&hwo, WAVE_MAPPER, &fmt, NULL, NULL, CALLBACK_NULL);

	const int bufsize = samples * 30;
	char *wavedata = (char *)LocalAlloc(0, bufsize);

	WAVEHDR hdr = { wavedata, bufsize, 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hwo, &hdr, sizeof(hdr));

	for (;;) {
		EARRRunAlternatePayloadW();
		int freq1 = 0, freq2 = 0, freq3 = 0;
		int sample1 = 0, sample2 = 0, sample3 = 0;
		for (int i = 0; i < bufsize; i++) {
			if (i % (int)(samples * 0.166) == 0) {
				freq1 = samples / (3580000.0 / (32 * ((random() % 41) * 10 + 200))) / 2;
				freq2 = samples / (3580000.0 / (32 * ((random() % 41) * 10 + 250))) / 2;
				freq3 = samples / (3580000.0 / (32 * ((random() % 41) * 10 + 325))) / 2;
			}

			sample1 = (i % freq1 < freq1 / 2) ? -127 : 127;
			sample2 = (i % freq2 < freq2 / 2) ? -127 : 127;
			sample3 = (i % freq3 < freq3 / 2) ? -127 : 127;

			wavedata[i] = (unsigned char)(((sample1+sample2+sample3)*0.1) + 127);
		}

		waveOutWrite(hwo, &hdr, sizeof(hdr));
        
		while (!(hdr.dwFlags & WHDR_DONE)) {
			Sleep(1);
		}
        
}
return 0;
}