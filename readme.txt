
Simple wave module for C which only deals with 16-bit PCM (stereo) wave files at a sample rate of 44100 Hz.

This code is provided for free without any license. 

usage:
gcc -Wall -Werror -Wfatal-errors reading_writing_example.c wav101.c

The code does not handle some common wav chunks such as the LIST chunk and as such is more tailored towards writing waves as opposed to reading/writing.
