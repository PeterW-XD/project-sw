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
#define BUF_SIZE 1

// int vga_ball_fd;
int audio_fd;
int data_x, data_y;
int data3, data4;
// short int out_left[BUF_SIZE];
// short int out_right[BUF_SIZE];
int buf_index = 0;

// Read audio data
void read_audio() {
  audio_arg_t vla;
  if (ioctl(audio_fd, AUDIO_IRQ_READ, &vla)) {
      perror("ioctl(AUDIO_IRQ_READ) failed");
      return;
  }
  data_x = vla.audio.left1;
  data_y = vla.audio.right1;
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
  int direction;
  addr_t address;
  static const char filename[] = "/dev/audio";  // Open the driver
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
	while (1) {
    read_audio();
    data_x = data_x << 24 >> 24;
    data_y = data_y << 24 >> 24;
    if (data_x >= -45 && data_x < 45) {
      if (data_y >= 0)
        direction = 90 - data_x;
      else
        direction = 270 + data_x;
    }

    if (data_x >= 0) {
      if (data_y >= 0 && data_y < 45)
        direction = data_y;
      else if (data_y <= -45 && data_y < 0)
        direction = 360 + data_y;
    } else {
      direction = 180 - data_y;
    }
    printf("%d\n", direction);
	}

	printf("done\n");

	// write_wav(file1, SAMPLE_RATE * duration_sec, out_left, SAMPLE_RATE);
	// write_wav(file2, SAMPLE_RATE * duration_sec, out_right, SAMPLE_RATE);
  
  printf("Audio record program terminating\n");
  return 0;
}
