
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "wav101.h"

int main(int argc, char** argv)
{

	/*

	wave file must be 16-bit PC (stereo) with 
	sample rate of 44100.

	32bit wave files are not supported.

	*/

	if (argc < 2) {

		printf("use: ./a.out mywav.wav\n");
		return 0;

	}

	FILE* inpf = fopen(argv[1], "rb");

	int32_t nsamples;

	int16_t* samples = wave_samples(inpf, &nsamples);

	FILE* fout = fopen("out.wav", "wb");

	wave(nsamples, samples, fout);

	free(samples);

	fclose(inpf);
	fclose(fout);

	return 0;
}