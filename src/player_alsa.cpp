// minimp3 example player application for Linux
#include "minimp3.h"

#include <iostream>
#include <fstream>
#include <alsa/asoundlib.h>

using std::cout;
using std::cerr;
using std::endl;


int main(int argc, char *argv[]) {
    auto mp3 = Decoder();
    mp3_info_t info;
    unsigned char *stream_pos;
    signed short sample_buf[MP3_MAX_SAMPLES_PER_FRAME];
    int bytes_left;
    int frame_size;

    cout << "minimp3 -- a small MPEG-1 Audio Layer III player based on ffmpeg" << endl;
    if (argc < 2) {
        cerr << "Error: No input file specified!" << endl;
        return 1;
    }

    auto fs = std::ifstream(argv[1], std::ios::binary);
    if (!fs.good()) {
        cerr << "Error: " << argv[1] << ": Cannot open mp3 file!" << endl;
        return 1;
    }

    fs.seekg(0, fs.end);
    auto len = fs.tellg();
    fs.seekg(0, fs.beg);
    auto buf = new std::ifstream::char_type[len];
    fs.read(buf, len);

    bytes_left = static_cast<int>(len) - 100;
    stream_pos = reinterpret_cast<unsigned char*>(buf);

    cout << "Now Playing: " << argv[1] << endl;

    frame_size = mp3_decode(mp3, stream_pos, bytes_left, sample_buf, &info);
    if (!frame_size) {
        cerr << "Error: " << argv[1] << ": Not a valid MP3 audio file!" << endl;
        return 1;
    }

    int err = 0;
    int channels = info.channels;
    unsigned int rate = info.sample_rate;
    snd_pcm_t *handle = NULL;
    snd_pcm_hw_params_t *hwparams = NULL;

    snd_pcm_hw_params_alloca(&hwparams);

    err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        cerr << "Playback open error: " << snd_strerror(err) << endl;
        return err;
    }

    err = snd_pcm_hw_params_any(handle, hwparams);
    if (err < 0) {
        cerr << "Error: Broken configuration for playback: no configurations available: "
             << snd_strerror(err) << endl;
        return err;
    }

    err = snd_pcm_hw_params_set_access(handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
        cerr << "Error: Access type not available for playback: "
             << snd_strerror(err) << endl;
        return err;
    }

    err = snd_pcm_hw_params_set_format(handle, hwparams, SND_PCM_FORMAT_S16_LE);
    if (err < 0) {
        cerr << "Error: Sample format not available for playback: "
             << snd_strerror(err) << endl;
        return err;
    }

    err = snd_pcm_hw_params_set_rate_near(handle, hwparams, &rate, NULL);
    if (err < 0) {
        cerr << "Error: Rate " << rate << "Hz not available for playback: "
             << snd_strerror(err) << endl;
        return err;
    }

    err = snd_pcm_hw_params_set_channels (handle, hwparams, channels);
    if (err < 0) {
        cerr << "Error: Channels count " << channels << "not available for playbacks: "
             << snd_strerror(err) << endl;
        return err;
    }

    err = snd_pcm_hw_params(handle, hwparams);
    if (err < 0) {
        cerr << "Error: Unable to set hw params for playback: "
             << snd_strerror(err) << endl;
        return err;
    }

    while ((bytes_left >= 0) && (frame_size > 0)) {
        stream_pos += frame_size;
        bytes_left -= frame_size;
        if ((err = snd_pcm_writei(handle, sample_buf, info.audio_bytes/4)) != info.audio_bytes/4) {
            cerr << "Error: Write to audio interface failed: " << snd_strerror (err) << endl;
                exit (1);
        }
        frame_size = mp3_decode(mp3, stream_pos, bytes_left, sample_buf, NULL);
    }

    snd_pcm_hw_params_free(hwparams);
    snd_pcm_close(handle);
    return 0;
}
