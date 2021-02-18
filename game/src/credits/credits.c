#include "x86.h"
#include "device/audio.h"
#include "device/video.h"
#include <string.h>

static FILE *fp = NULL;
static Audio_Data audio_data;

void add_irq_handle(int irq, void *handler);

void timer_event(void)
{ // do nothing
}

static int audio_reload = 0;
void audio_event(void)
{
	// need to load new audio data
	audio_reload = 1;
}

static void write_audio_data()
{
	if (fp == NULL)
		return;
	audio_data.len = fread(audio_data.pcm_buffer, 1, PCM_BUF_SIZE, fp);
	// audio_data.len = PCM_BUF_SIZE;
	audio_write(&audio_data);
}

void roll_text();

void main_loop(void)
{
	Log("Occasional audio playback disorder. Please help me debug :-)");
	fp = fopen("credits_bgm.wav", "rb");
	// register audio event
	add_irq_handle(2, audio_event);
	// start audio playback
	audio_pause(0);
	uint32_t tick = 0;
	while (true)
	{
		wait_intr();
		// Log("tick %d", tick);
		if (audio_reload)
		{
			//Log("reload audio");
			// the time for write audio data must be less than the time for playing it
			write_audio_data();
			audio_reload = 0;
		}
		if (tick % 200 == 0)
		{
			roll_text();
		}
		tick++;
	}
}
