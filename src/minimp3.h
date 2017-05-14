#pragma once

#include <cstdint>

#define MP3_MAX_SAMPLES_PER_FRAME (1152*2)
#define MP3_MAX_CHANNELS 2
#define SBLIMIT 32
#define BACKSTEP_SIZE 512
#define EXTRABYTES 24

typedef struct _mp3_info {
    int sample_rate;
    int channels;
    int audio_bytes;  // generated amount of audio per frame
} mp3_info_t;

typedef struct _bitstream {
    const uint8_t *buffer, *buffer_end;
    int index;
    int size_in_bits;
} bitstream_t;


class Decoder {
    public:
    uint8_t last_buf[2*BACKSTEP_SIZE + EXTRABYTES];
    int last_buf_size;
    int frame_size;
    uint32_t free_format_next_header;
    int error_protection;
    int sample_rate;
    int sample_rate_index;
    int bit_rate;
    bitstream_t gb;
    bitstream_t in_gb;
    int nb_channels;
    int mode;
    int mode_ext;
    int lsf;
    int16_t synth_buf[MP3_MAX_CHANNELS][512 * 2];
    int synth_buf_offset[MP3_MAX_CHANNELS];
    int32_t sb_samples[MP3_MAX_CHANNELS][36][SBLIMIT];
    int32_t mdct_buf[MP3_MAX_CHANNELS][SBLIMIT * 18];
    int dither_state;

    Decoder();
};

typedef Decoder mp3_decoder_t;

void mp3_create(mp3_decoder_t &dec);
int mp3_decode(mp3_decoder_t &dec, void *buf, int bytes, signed short *out, mp3_info_t *info);
