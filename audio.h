#ifndef _AUDIO_H
#define _AUDIO_H

#include <linux/ioctl.h>

typedef struct {
	unsigned int left1, right1;
	unsigned int left2, right2;
} audio_t;

typedef struct {
	int audio_ready;
} audio_ready_t;

typedef struct {
	audio_t audio;
	audio_ready_t ready;
} audio_arg_t;

#define AUDIO_MAGIC 'q'

/* ioctls and their arguments */
#define AUDIO_READ  _IOR(AUDIO_MAGIC, 1, audio_arg_t *)

#endif
