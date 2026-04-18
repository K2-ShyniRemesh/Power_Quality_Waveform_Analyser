cmake_minimum_required(VERSION 4.1)
project(Power_Quality_Waveform_Analyser C)

set(CMAKE_C_STANDARD 99)

add_executable(Power_Quality_Waveform_Analyser main.c
        waveform.h)
