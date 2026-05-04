#include"waveform.h"
#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*    FINDS NUMBER OF ROWS IN CSV
 *  reads through to find \n or EOF
 *  for each \n -> increases count
 *  increases count for EOF if needed
 */
int countRows(FILE *file) {
    int ch;
    int count = 0;
    int previousCh = '\n';

    //iterates, reading each char until EOF
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;//increment for each newline found
        }
        previousCh = ch;
    }

    //makes sure that count is increased to include the last line
    if (previousCh != '\n') {
        count++;
    }

    //ternary operation to exclude the line with heading
    //while returning zero for empty files
    return (count > 0) ? (count - 1) : 0;
}

/*     REMOVE EXTENSION FROM FILENAME
 * Removes the file extension from fileName
 * goes to end of string
 * finds last occurrence of "."
 * ends string at that point
 */
void removeExtension(char *filePath) {
    char *end = filePath + strlen(filePath);

    while (end > filePath && *end != '.') --end;

    if (end > filePath) {
        *end = '\0';
    }
}


void Sort(WaveformSample *Log,int rows) {
    {
        // Skipping the header by adding 1
        WaveformSample *ptr = Log +1;
        int dataCount = rows-1;

        // Sort the structs using pointers
        for (int i = 0; i < dataCount; i++) {

            for (int j = i + 1; j < dataCount; j++) {

                // Compare magnitude of phase_voltage[0]
                if (fabs((ptr + j)->phase_voltage[0]) < fabs((ptr + i)->phase_voltage[0])) {

                    // Swap the entire struct contents
                    WaveformSample temp = *(ptr + i);
                    *(ptr + i) = *(ptr + j);
                    *(ptr + j) = temp;
                }
            }
        }
    }
}