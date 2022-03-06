
// remember to remove stdio after testing.
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/*
single header wave library for reading and writing 16-bit PCM dual channel wave files.

*/



static const int16_t num_channels = 2;
static const int16_t bits_per_sample = 16;
static const int32_t sample_rate = 44100;
//static const int32_t max_positive_vol = 32767;

typedef struct {

	char chunkid[4];  // RIFF
	int32_t chunksize; 
	char format[4];   // WAVE
	char subchunk1id[4];
	int32_t subchunk1size;

	int16_t audioformat;
	int16_t numchannels;

	int32_t samplerate;
	int32_t byterate;

	int16_t blockalign;
	int16_t bitspersample;

	char subchunk2id[4];

	int32_t subchunk2size;

} wavehdr;



typedef struct {

	int16_t data;

} _sixteenbitsample;



wavehdr create_wave_hdr(int32_t num_samples)
{

	int32_t subchunk1size = 16;
	int32_t subchunk2size = num_samples * (int32_t)(num_channels * bits_per_sample / 8);

	wavehdr wheadr;
	wheadr.chunkid[0] = 'R';
	wheadr.chunkid[1] = 'I';
	wheadr.chunkid[2] = 'F';
	wheadr.chunkid[3] = 'F';

	wheadr.chunksize = 4 + (8 + subchunk1size) + (8 + subchunk2size);

	wheadr.format[0] = 'W';
	wheadr.format[1] = 'A';
	wheadr.format[2] = 'V';
	wheadr.format[3] = 'E';

	wheadr.subchunk1id[0] = 'f';
	wheadr.subchunk1id[1] = 'm';
	wheadr.subchunk1id[2] = 't';
	wheadr.subchunk1id[3] = ' ';

	wheadr.subchunk1size = subchunk1size;
	
	wheadr.audioformat = 1; // PCM

	wheadr.numchannels = num_channels;
	wheadr.samplerate  = sample_rate;
	wheadr.byterate = sample_rate * num_channels * bits_per_sample / 8;
	wheadr.blockalign = num_channels * bits_per_sample / 8;

	wheadr.bitspersample = bits_per_sample;

	wheadr.subchunk2id[0] = 'd';
	wheadr.subchunk2id[1] = 'a';
	wheadr.subchunk2id[2] = 't';
	wheadr.subchunk2id[3] = 'a';

	wheadr.subchunk2size = subchunk2size;

	return wheadr;
}



static void writehdr(wavehdr* wheadr, FILE* file)
{

	fwrite(wheadr, sizeof(wavehdr), 1, file);

}



static void write_sample_data(int16_t* samples, int32_t nsamples, FILE* file)
{


	int32_t c = 0;

	_sixteenbitsample sample;

	for (c = 0; c < nsamples; c++) {

		sample.data = samples[c];

		fwrite(&sample, sizeof(_sixteenbitsample), 1, file);

	}

}



void wave(int32_t nsamples, int16_t* samples, FILE* file)
{


	wavehdr wheadr = create_wave_hdr(nsamples);
	writehdr(&wheadr, file);
	write_sample_data(samples, nsamples, file);

}



int16_t* wave_samples(FILE* inpf, int32_t* nsamples)
{

	wavehdr wheadr;
	fread(&wheadr, sizeof(wavehdr), 1, inpf);

	*nsamples = wheadr.subchunk2size / (num_channels * (bits_per_sample / 8));

	int16_t* samples = (int16_t*) calloc(sizeof(int16_t), *nsamples);

	int32_t c = 0;

	_sixteenbitsample sample;

	for (c = 0; c < *nsamples; c++) {

		fread(&sample, sizeof(sample), 1, inpf);

		samples[c] = sample.data;

	}

	return samples;

}



int main(void)
{

	// int32_t nchannels = 2;
	// int32_t seconds = 60;
	// int32_t nsamples = 44100 * seconds * nchannels;

	// int16_t* samples = (int16_t*)calloc(sizeof(int16_t), nsamples);

	// int32_t c = 0;
	// int16_t sample;

	// double t = 0.0;

	// while (c < nsamples) {

	// 	sample = (int16_t)(sin(t) * max_positive_vol);

	// 	t += 0.02;

	// 	samples[c] = sample;
	// 	c++;

	// 	if (c < nchannels) {

	// 		samples[c] = sample;
	// 		c++;

	// 	}

	// }

	// FILE* outf = fopen("testwav.wav", "wb");

	// wave(nsamples, samples, outf);

	// free(samples);

	// fclose(outf);


	FILE* inpf = fopen("testwav.wav", "rb");

	int32_t nsamples;

	int16_t* samples = wave_samples(inpf, &nsamples);

	FILE* fout = fopen("test.wav", "wb");

	wave(nsamples, samples, fout);

	free(samples);

	fclose(inpf);

	return 0;

}