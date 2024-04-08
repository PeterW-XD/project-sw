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
int left1_buf[buf_size], right1_buf[buf_size];
int left2_buf[buf_size], right2_buf[buf_size];
// short int out_left[buf_size];
// short int out_right[buf_size];
int buf_index = 0;

// Read audio data
void read_audio() {
  audio_arg_t vla;
  
  if (ioctl(audio_fd, AUDIO_READ, &vla)) {
      perror("ioctl(AUDIO_READ) failed");
      return;
  }
  left1_buf[buf_index] = vla.audio.left1;
  right1_buf[buf_index] = vla.audio.right1;
  left2_buf[buf_index] = vla.audio.left2;
  right2_buf[buf_index] = vla.audio.right2;
  buf_index++;
}

int main()
{
  static const char filename[] = "/dev/audio";  // Open the driver
  // static const char file1[] = "./test1.wav";    // Microphone 1 .wav directory
  // static const char file2[] = "./test2.wav";
  FILE *fd1_L = fopen("data1_L.txt", "w");
  FILE *fd1_R = fopen("data1_R.txt", "w");
  FILE *fd2_L = fopen("data2_L.txt", "w");
  FILE *fd2_R = fopen("data2_R.txt", "w");
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
    fprintf(fd1_L, "%d\n", (left1_buf[i] << 8) >> 8);
    fprintf(fd1_R, "%d\n", (right1_buf[i] << 8) >> 8);
    fprintf(fd2_L, "%d\n", (left2_buf[i] << 8) >> 8);
    fprintf(fd2_R, "%d\n", (right2_buf[i] << 8) >> 8);
		// out_right[i] = right_buf[i] / 256;
		// out_left[i] = left_buf[i] / 256;
	}

	// write_wav(file1, sample_rate * duration_sec, out_left, sample_rate);
	// write_wav(file2, sample_rate * duration_sec, right_buf, sample_rate);
  
  printf("Audio record program terminating\n");
  return 0;
}
