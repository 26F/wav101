
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "wav101.h"

int main(void)
{

	/*

	wave file must be 16-bit PC (stereo) with 
	sample rate of 44100.

	*/

	FILE* inpf = fopen("testwav.wav", "rb");

	int32_t nsamples;

	int16_t* samples = wave_samples(inpf, &nsamples);

	FILE* fout = fopen("out.wav", "wb");

	wave(nsamples, samples, fout);

	free(samples);

	fclose(inpf);

	return 0;
}