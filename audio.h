#ifndef _AUDIO_H
#define _AUDIO_H

#include <linux/ioctl.h>

// Package 1
typedef struct {
	 int left1, right1;
	 int left2, right2;
} audio_t;

typedef struct {
	int audio_ready;
} audio_ready_t;

typedef struct {
	audio_t audio;
	// audio_ready_t ready;
} audio_arg_t;

// Package 2
typedef struct {
	int addr;
} addr_t;

typedef struct
{
	addr_t addr;
} addr_arg_t;


#define AUDIO_MAGIC 'q'

/* ioctls and their arguments */
#define AUDIO_READ  _IOR(AUDIO_MAGIC, 1, audio_arg_t *)
#define AUDIO_IRQ_READ _IOR(AUDIO_MAGIC, 2, audio_arg_t *)
#define ADDR_WRITE	_IOW(AUDIO_MAGIC, 3, addr_arg_t*)

#endif
