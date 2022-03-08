
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "wav101.h"

int main(void)
{

	int32_t nchannels = 2;
	int32_t seconds = 60;
	int32_t nsamples = 44100 * seconds * nchannels;

	int16_t* samples = (int16_t*)calloc(sizeof(int16_t), nsamples);

	int32_t c = 0;
	int16_t sample;

	double t = 0.0;

	while (c < nsamples) {

		sample = (int16_t)(sin(t) * 32767);

		t += 0.02;

		// left sample
		samples[c] = sample;
		c++;


		// right sample
		samples[c] = sample;
		c++;

	}

	FILE* outf = fopen("testwav.wav", "wb");

	wave(nsamples, samples, outf);

	free(samples);

	fclose(outf);

	return 0;

}