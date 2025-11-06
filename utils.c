#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils.h"

char *getID(int i)
{
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

int validateIntRange(int value, int min, int max, const char *errMsg) {
    if (value < min || value > max) {
        if (errMsg) fprintf(stderr, "%s\n", errMsg);
        return 0;
    }
    return 1;
}

int createFile(const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        fprintf(stderr, "createFile: Could not create file %s: %s\n", path, strerror(errno));
        return 0;
    }
    fclose(file);
    return 1;
}
