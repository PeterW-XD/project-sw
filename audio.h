#ifndef _AUDIO_H
#define _AUDIO_H

#include <linux/ioctl.h>

typedef struct {
	int left, right, ready;
} audio_t;
  

typedef struct {
  audio_t audio;
} audio_arg_t;

#define AUDIO_MAGIC 'q'

/* ioctls and their arguments */
#define AUDIO_READ  _IOR(AUDIO_MAGIC, 1, audio_arg_t *)

#endif
