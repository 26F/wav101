
#include <stdint.h>
#include <stdlib.h>

#include "wav101.h"

/*
Single header wave library for reading and writing 16-bit PCM dual channel wave files.
This code can be used and altered at the will of any programmer without limitations. 

Wave files will only work with this library if they are 16-bit PCM stereo wave files with a sample rate of 44100.

*/



// These globals are only visible to this file through the use of static.
static const int16_t num_channels = 2;
static const int16_t bits_per_sample = 16;
static const int32_t sample_rate = 44100;



// this function is only visible to this file through the use of static.
// Creates a wav header. Note only 16-bit PCM stereo
// With a sample rate of 44100
static wavehdr create_wave_hdr(int32_t num_samples)
{

	int32_t subchunk1size = 16;
	int32_t subchunk2size = num_samples * (int32_t)((bits_per_sample / 8));

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
	wheadr.byterate = sample_rate * num_channels * (bits_per_sample / 8);
	wheadr.blockalign = num_channels * bits_per_sample / 8;

	wheadr.bitspersample = bits_per_sample;

	wheadr.subchunk2id[0] = 'd';
	wheadr.subchunk2id[1] = 'a';
	wheadr.subchunk2id[2] = 't';
	wheadr.subchunk2id[3] = 'a';

	wheadr.subchunk2size = subchunk2size;

	return wheadr;
}


// write wave header to file
static void writehdr(wavehdr* wheadr, FILE* file)
{

	fwrite(wheadr, sizeof(wavehdr), 1, file);

}


// write the wave sample data to file
static void write_sample_data(int16_t* samples, int32_t nsamples, FILE* file)
{


	int32_t c = 0;

	_sixteenbitsample sample;

	for (c = 0; c < nsamples; c++) {

		sample.data = samples[c];

		fwrite(&sample, sizeof(_sixteenbitsample), 1, file);

	}

}


// create a wave file from samples
void wave(int32_t nsamples, int16_t* samples, FILE* file)
{


	wavehdr wheadr = create_wave_hdr(nsamples);
	writehdr(&wheadr, file);
	write_sample_data(samples, nsamples, file);

}


// get samples from a wave file
// nsamples is set through pointer dereference inside this function.
int16_t* wave_samples(FILE* inpf, int32_t* nsamples)
{

	wavehdr wheadr;
	fread(&wheadr, sizeof(wavehdr), 1, inpf);

	printf("number samples %d\n", *nsamples);
	printf("bits per sample %d\n", wheadr.bitspersample);
	printf("sample rate %d\n", wheadr.samplerate);
	printf("byte rate %d\n", wheadr.byterate);
	printf("block align %d\n", wheadr.blockalign);
	printf("subchunk1size %d\n", wheadr.subchunk1size);
	printf("subchunk2size %d\n", wheadr.subchunk2size);

	*nsamples = wheadr.subchunk2size / (wheadr.numchannels * (wheadr.bitspersample / bits_per_sample));

	int16_t* samples = (int16_t*) calloc(sizeof(int16_t), *nsamples);

	int32_t c = 0;

	_sixteenbitsample sample;

	for (c = 0; c < *nsamples; c++) {

		fread(&sample, sizeof(sample), 1, inpf);

		samples[c] = sample.data;

	}

	return samples;

}
