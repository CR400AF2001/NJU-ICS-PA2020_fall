#ifndef _DEVICE_AUDIO_H_
#define _DEVICE_AUDIO_H_

#define PCM_BUF_SIZE 16000 * 8 // 0.5s for 16kHz 16bit

// audio ports
#define AUDIO_DATA 0xc0
#define AUDIO_CTL 0xc1

typedef struct
{
	uint32_t len; // in bytes
	uint8_t pcm_buffer[PCM_BUF_SIZE];
} Audio_Data;

void audio_pause(int);
void audio_write(Audio_Data *);
#endif
