/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "write_wav.h"
#include <stdint.h>
// int vga_ball_fd;
int project_top_fd;
const int sample_rate = 48000;	// 48000kHz
const int word_length = 24;			// 24 bits per sample
const int duration_sec = 10;		// 10s
const int buf_size = sample_rate * word_length;
int left_buf[buf_size];
int right_buf[buf_size];
int buf_index = 0;

/* Read and print the background color */
//void print_background_color() {
void read_audio() {
  vga_ball_arg_t vla;
  
  if (ioctl(project_top_fd, AUDIO_READ, &vla)) {
      perror("ioctl(READ) failed");
      return;
  }
	if (vla.audio.ready == 1) {
		left_buf[buf_index++] = vla.audio.left;
		right_buf[buf_index++] = vla.audio.right;
	}
  //printf("%02x %02x %02x\n",
	// vla.background.red, vla.background.green, vla.background.blue);
}

/* Set the background color */
//void set_background_color(const vga_ball_color_t *c)
//{
//  vga_ball_arg_t vla;
//  vla.background = *c;
//  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_BACKGROUND, &vla)) {
//      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
//      return;
//  }
//}

int main()
{
  // int i;
  // static const char filename[] = "/dev/vga_ball";
  static const char filename[] = "/dev/project_top";
  static const char file1[] = "test1.wav";
  static const char file2[] = "test2.wav";
  printf("Audio record program started\n");
  if ( (project_top_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }
	while (buf_index < buf_size) {
		read_audio();
	}
	write_wav(file1, sample_rate, word_length, duration_sec, left_buf);
	write_wav(file2, sample_rate, word_length, duration_sec, right_buf);

//  static const vga_ball_color_t colors[] = {
//    { 0xff, 0x00, 0x00 }, /* Red */
//    { 0x00, 0xff, 0x00 }, /* Green */
//    { 0x00, 0x00, 0xff }, /* Blue */
//    { 0xff, 0xff, 0x00 }, /* Yellow */
//    { 0x00, 0xff, 0xff }, /* Cyan */
//    { 0xff, 0x00, 0xff }, /* Magenta */
//    { 0x80, 0x80, 0x80 }, /* Gray */
//    { 0x00, 0x00, 0x00 }, /* Black */
//    { 0xff, 0xff, 0xff }  /* White */
//  };
//
//# define COLORS 9
//
//  printf("Audio record program started\n");
//
//  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
//    fprintf(stderr, "could not open %s\n", filename);
//    return -1;
//  }
//
//  printf("initial state: ");
//  print_background_color();
//
//  for (i = 0 ; i < 24 ; i++) {
//    set_background_color(&colors[i % COLORS ]);
//    print_background_color();
//    usleep(400000);
//  }
  
  printf("Audio record program terminating\n");
  return 0;
}
