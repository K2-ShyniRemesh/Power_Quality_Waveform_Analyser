# Power Quality Waveform Analyser 

https://github.com/K2-ShyniRemesh/Power_Quality_Waveform_Analyser.git

Power Quality Waveform Analyser is program that reads real-world power quality data from a sensor log file, analyses the voltage waveforms, and produces a structured report.


## Steps to Clone and Run this REPO with gcc

1.  Run the `git clone` command to make a copy of this repository on your local machine:

    ```bash
    git clone https://github.com/K2-ShyniRemesh/Power_Quality_Waveform_Analyser.git
    ```

2.  Navigate to the folder:

    ```bash
    cd C:\
    ```

3.  Compile including all files and link math lib & output as Power_Quality_Waveform_Analyser:

    ```bash
    gcc main.c io.c waveform.c misc.c -o Power_Quality_Waveform_Analyser -lm
    ```

4.  Run the program:

    ```bash
    ./Power_Quality_Waveform_Analyser
    ```

## CMake compiling steps

```cmake
cmake_minimum_required(VERSION 4.1)
project(Power_Quality_Waveform_Analyser C)

set(CMAKE_C_STANDARD 99)

add_executable(Power_Quality_Waveform_Analyser
    main.c
    waveform.h
    waveform.c
    io.c
    io.h
    misc.c
    misc.h
)

target_link_libraries(Power_Quality_Waveform_Analyser m)
```
    1.Build the program
    2.Run the program
