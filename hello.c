/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Origin: Stephen A. Edwards (Columbia University)
 * Author: Sound localizer
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

// #define SAMPLE_RATE 48000	// 48000kHz
// #define word_length 24		// 24 bits per sample
// #define duration_sec 1		// 1s
// #define BUF_SIZE (SAMPLE_RATE * duration_sec)
#define BUF_SIZE 1024

int audio_fd;
int data1, data2;

// Read audio data
void read_audio() {
  audio_arg_t vla;
  if (ioctl(audio_fd, AUDIO_IRQ_READ, &vla)) {
      perror("ioctl(AUDIO_IRQ_READ) failed");
      return;
  }
  data1 = vla.audio.left1;
  data2 = vla.audio.right1;
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
  // static const char file1[] = "./test1.wav"; // Microphone 1 .wav directory
  // static const char file2[] = "./test2.wav";

  printf("Audio record program started\n");
  if ( (audio_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }
  
  address.addr = 0;
  address.go = 1;
  write_addr(&address);
  address.go = 0;
  write_addr(&address);
	usleep(1000000);
  address.go = 2;
  write_addr(&address);
	while (buf_index < BUF_SIZE) {
   	address.addr = buf_index;
    write_addr(&address);
    read_audio();
	}
	printf("done\n");
	for (int i = 0; i < BUF_SIZE; i++) {    // Received data is 16 bits wide 
    fprintf(fd1, "%d\n", ((data1[i] / 16384) << 18) >> 18);	// Extend the sign	
		fprintf(fd2, "%d\n", ((data1[i] % 16384) << 18) >> 18);	// Extend the sign
		fprintf(fd3, "%d\n", ((data2[i] / 16384) << 18) >> 18);	// Extend the sign
		fprintf(fd4, "%d\n", ((data2[i] % 16384) << 18) >> 18);	// Extend the sign
		fprintf(fd5, "%d\n", ((data3[i] / 16384) << 18) >> 18);	// Extend the sign
		fprintf(fd6, "%d\n", ((data3[i] % 16384) << 18) >> 18);	// Extend the sign
		fprintf(fd7, "%d\n", ((data4[i] / 16384) << 18) >> 18);	// Extend the sign
		fprintf(fd8, "%d\n", ((data4[i] % 16384) << 18) >> 18);	// Extend the sign
		// fprintf(fd1_L, "%d\n", (left1_buf[i] << 16) >> 16);	// Extend the sign
    // fprintf(fd1_R, "%d\n", (right1_buf[i] << 16) >> 16);
    // fprintf(fd2_L, "%d\n", (left2_buf[i] << 16) >> 16);
    // fprintf(fd2_R, "%d\n", (right2_buf[i] << 16) >> 16);
	}

	// write_wav(file1, SAMPLE_RATE * duration_sec, out_left, SAMPLE_RATE);
	// write_wav(file2, SAMPLE_RATE * duration_sec, out_right, SAMPLE_RATE);
  
  printf("Audio record program terminating\n");
  return 0;
}
