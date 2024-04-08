/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "audio.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "write_wav.h"


#define sample_rate 48000	// 48000kHz
#define word_length 24		// 24 bits per sample
#define duration_sec 2		// 1s
#define buf_size (sample_rate * duration_sec)

// int vga_ball_fd;
int audio_fd;
int left_buf[buf_size];
int right_buf[buf_size];
short int out_left[buf_size];
short int	out_right[buf_size];
int buf_index = 0;

// Read audio data
void read_audio() {
  audio_arg_t vla;
  
  if (ioctl(audio_fd, AUDIO_READ, &vla)) {
      perror("ioctl(READ) failed");
      return;
  }
  left_buf[buf_index++] = vla.audio.left;
  right_buf[buf_index++] = vla.audio.right;
}

int main()
{
  static const char filename[] = "/dev/audio";  // Open the driver
  static const char file1[] = "./test1.wav";    // Microphone 1 .wav directory
  static const char file2[] = "./test2.wav";
  printf("Audio record program started\n");
  if ( (audio_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }
	while (buf_index < buf_size) {
		read_audio();
	}
	printf("done\n");
	for (int i = 0; i < buf_size; i++) {    // Truncate to short
		out_right[i] = right_buf[i] / 256;
		out_left[i] = left_buf[i] / 256;
	}
	write_wav(file2, sample_rate * duration_sec, out_right, sample_rate);
	write_wav(file1, sample_rate * duration_sec, out_left, sample_rate);
	//write_wav(file2, sample_rate * duration_sec, right_buf, sample_rate);
  
  printf("Audio record program terminating\n");
  return 0;
}
