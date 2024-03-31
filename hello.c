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
#define duration_sec 3		// 3s
#define buf_size (sample_rate * word_length)

// int vga_ball_fd;
int project_top_fd;
int left_buf[buf_size];
int right_buf[buf_size];
int left_ready, right_ready;
int left_index, right_index;

/* Read and print the background color */
//void print_background_color() {
void read_audio() {
  audio_arg_t vla;
  
  if (ioctl(project_top_fd, AUDIO_READ, &vla)) {
      perror("ioctl(READ) failed");
      return;
  }
	//printf("ready = %d\n", vla.audio.ready);
	left_ready = vla.audio.ready % 2;
	right_ready = vla.audio.ready / 2;
	if (left_ready) {
		left_buf[left_index++] = vla.audio.left;
		printf("Left = %d\n", vla.audio.left);
	} else if (right_ready) {
		right_buf[right_index++] = vla.audio.right;
		printf("Right = %d\n", vla.audio.right);
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
  left_index = 0;
	right_index = 0;
  // static const char filename[] = "/dev/vga_ball";
  static const char filename[] = "/dev/audio";
  static const char file1[] = "./test1.wav";
  static const char file2[] = "./test2.wav";
  printf("Audio record program started\n");
  if ( (project_top_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }
	while (left_index < buf_size & right_index < buf_size) {
		read_audio();
    printf("Index = %d\n", left_index);
	}
	printf("done\n");
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
