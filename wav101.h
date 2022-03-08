
#ifndef WAV101
#define WAV101

#include <stdint.h>
#include <stdio.h>

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


void wave(int32_t nsamples, int16_t* samples, FILE* file);
int16_t* wave_samples(FILE* inpf, int32_t* nsamples);


#endif // WAV101