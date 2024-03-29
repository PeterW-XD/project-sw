#include "write_wav.h"

struct wav_header {
	char riff[4];
	int32_t flength;
	char wave[4];
	char fmt[4];
	int32_t format_size;
	int16_t format_type;
	int16_t num_chans;
	int32_t sample_rate;
	int32_t byte_per_second;
	int16_t byte_per_sample;
	int16_t bits_per_sample;
	char data[4];
	int32_t dlength;
};

void write_wav(const char *name, const int sample_rate, const int length, 
	const int duration_sec, int *buffer) 
{
	// const int sample_rate = 48000;
	// const int length = 24;
	struct wav_header wavh;

	strncpy(wavh.riff, "RIFF", 4);
	strncpy(wavh.wave, "WAVE", 4);
	strncpy(wavh.fmt, "fmt", 4);
	strncpy(wavh.data, "data", 4);

	wavh.format_size = length;	// 24
	wavh.format_type = 1;
	wavh.num_chans = 1;
	wavh.sample_rate = sample_rate; // 48000
	wavh.bits_per_sample = length; //24
	wavh.byte_per_sample = wavh.bits_per_sample * wavh.num_chans / 8;
	wavh.byte_per_second = wavh.sample_rate * wavh.byte_per_sample;
	
	// const int duration_sec = 10;
	const int buffer_size = sample_rate * duration_sec;

	wavh.dlength = buffer_size * wavh.byte_per_sample;
	wavh.flength = wavh.dlength + 44;

	// int buffer[buffer_size] = {};

	FILE *fd = fopen(name, "w");
	fwrite(&wavh, 1, sizeof(wavh), fd);
	fwrite(buffer, 4, buffer_size, fd);
	
}
