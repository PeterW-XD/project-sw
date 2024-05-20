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
#include <math.h>

#define PI 3.14159265358979

int audio_fd;
int data1, data2;

void calcCoor(double radius, double degrees, double *x, double *y) {
    double radians = degrees * (PI / 180.0);
    *x = radius * cos(radians);
    *y = radius * sin(radians);
}

// Calculate the degree
void calcDeg(int x, int y, int *dir) {
  if (x >= -45 && x <= 45) {
    if (y >= 45 && y <= 90)
      *dir = 90 - x;
    else if (y >= -90 && y <= -45)
      *dir =  270 + x;
  } else {
    if (x > 45 && x < 90)
      *dir = 360 + y;
    else if (x > -90 && x < -45)
      *dir =  180 - y;
  }
}

// Read audio data
void read_audio() {
  audio_arg_t vla;
  if (ioctl(audio_fd, AUDIO_READ, &vla)) {
      perror("ioctl(AUDID_READ) failed");
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
  double radius, degrees, dou_x, dou_y;
  int dir;
  int x_center, y_center;
  addr_t address;
  static const char filename[] = "/dev/audio";  // Open the driver
  // static const char file1[] = "./test1.wav"; // Microphone 1 .wav directory
  // static const char file2[] = "./test2.wav";

  printf("Audio record program started\n");
  if ( (audio_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }
  // Init
  radius = 100;
  x_center = 630;
  y_center = 240;
  // Start the program  

	while (1) {
    address.xcoor = 0;
    address.ycoor = 0;
    address.go = 1;
    write_addr(&address);
    address.go = 0;
    write_addr(&address);
    usleep(500000);
    read_audio();
    calcDeg(data1, data2, &dir);
    // printf("x=%d, y=%d\ndir=%d\n", data1, data2, dir);
    degrees = dir;
    calcCoor(radius, degrees, &dou_x, &dou_y);
    //printf("x_coor=%f, y_coor=%f\n", dou_x, dou_y);
    address.xcoor = x_center + (int)dou_x;
    address.ycoor = y_center + (int)dou_y;
    printf("x_pix=%d, y_pix=%d\n", address.xcoor, address.ycoor);
    write_addr(&address);
	}
  
	printf("done\n");

	// write_wav(file1, SAMPLE_RATE * duration_sec, out_left, SAMPLE_RATE);
	// write_wav(file2, SAMPLE_RATE * duration_sec, out_right, SAMPLE_RATE);
  
  printf("Audio record program terminating\n");
  return 0;
}
