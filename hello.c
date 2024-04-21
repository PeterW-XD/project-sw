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


#define SAMPLE_RATE 48000	// 48000kHz
#define word_length 24		// 24 bits per sample
#define duration_sec 1		// 1s
// #define BUF_SIZE (SAMPLE_RATE * duration_sec)
#define BUF_SIZE 4096

// int vga_ball_fd;
int audio_fd;
int left1_buf[BUF_SIZE], right1_buf[BUF_SIZE];
int left2_buf[BUF_SIZE], right2_buf[BUF_SIZE];
// short int out_left[BUF_SIZE];
// short int out_right[BUF_SIZE];
int buf_index = 0;

// Read audio data
void read_audio() {
  audio_arg_t vla;
  if (ioctl(audio_fd, AUDIO_IRQ_READ, &vla)) {
      perror("ioctl(AUDIO_READ) failed");
      return;
  }
  left1_buf[buf_index] = vla.audio.left1;
  right1_buf[buf_index] = vla.audio.right1;
  left2_buf[buf_index] = vla.audio.left2;
  right2_buf[buf_index] = vla.audio.right2;
  buf_index++;
}

// Write address
void write_addr(addr_t *address) {
  addr_arg_t vla;
  vla.addr = *address;
  if (ioctl(audio_fd, ADDR_WRITE, &vla)) {
      perror("ioctl(ADDR_WRITE) failed");
      return;
  }
}

int main()
{
  addr_t address;
  static const char filename[] = "/dev/audio";  // Open the driver
  // static const char file1[] = "./test1.wav";    // Microphone 1 .wav directory
  // static const char file2[] = "./test2.wav";
  FILE *fd1_L = fopen("data1.txt", "w");
  FILE *fd1_R = fopen("data2.txt", "w");
  FILE *fd2_L = fopen("data3.txt", "w");
  FILE *fd2_R = fopen("data4.txt", "w");
  printf("Audio record program started\n");
  if ( (audio_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }
  // usleep(500000);
	while (buf_index < BUF_SIZE) {
  	 //address.addr = buf_index;
     //write_addr(&address);
     //usleep(1);
    read_audio();
	}
	printf("done\n");
	for (int i = 0; i < BUF_SIZE; i++) {    // Truncate to short
    fprintf(fd1_L, "%d\n", (left1_buf[i] << 8) >> 8);
    fprintf(fd1_R, "%d\n", (right1_buf[i] << 8) >> 8);
    fprintf(fd2_L, "%d\n", (left2_buf[i] << 8) >> 8);
    fprintf(fd2_R, "%d\n", (right2_buf[i] << 8) >> 8);
		// out_right[i] = right1_buf[i] / 256;
		// out_left[i] = left1_buf[i] / 256;
	}

	// write_wav(file1, SAMPLE_RATE * duration_sec, out_left, SAMPLE_RATE);
	// write_wav(file2, SAMPLE_RATE * duration_sec, out_right, SAMPLE_RATE);
  
  printf("Audio record program terminating\n");
  return 0;
}
