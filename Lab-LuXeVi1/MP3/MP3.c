#include <stdio.h>
#include <mpg123.h>
#include <portaudio.h>

// Function to play an MP3 file
void playMP3(const char *filename) {
    mpg123_handle *mh;
    mpg123_init();
    mh = mpg123_new(NULL, NULL);
    
    if (mh == NULL) {
        fprintf(stderr, "Unable to initialize mpg123\n");
        return;
    }

    if (mpg123_open(mh, filename) != MPG123_OK ||
        mpg123_format_none(mh) != MPG123_OK ||
        mpg123_format(mh, 44100, MPG123_STEREO, MPG123_ENC_SIGNED_16) != MPG123_OK) {
        fprintf(stderr, "Cannot initialize MPG123\n");
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        return;
    }

    int channels, encoding;
    long rate;
    mpg123_getformat(mh, &rate, &channels, &encoding);

    PaError err;
    PaStream *stream;
    err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return;
    }

    err = Pa_OpenDefaultStream(&stream, 0, channels, paInt16, rate, 256, NULL, NULL);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return;
    }

    mpg123_scan(mh);

    size_t buffer_size = mpg123_outblock(mh);
    unsigned char *buffer = (unsigned char*)malloc(buffer_size);
    long total_frames = mpg123_length(mh);

    Pa_StartStream(stream);
    long current_frame = 0;

    while (current_frame < total_frames) {
        size_t done;
        mpg123_read(mh, buffer, buffer_size, &done);
        current_frame += done / (channels * sizeof(short));
        Pa_WriteStream(stream, buffer, done);
        printf("Current second: %.2f\r", (double)current_frame / rate);
    }

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    free(buffer);
}

int main() {
    const char *mp3File = "your_mp3_file.mp3";
    playMP3(mp3File);
    return 0;
}
