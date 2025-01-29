#include "filetools.h"

#include <stdio.h>
#include <stdlib.h>

const char* ReadTextFile(const char* fPath)
{
    FILE* file = fopen(fPath, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    if (fseek(file, 0, SEEK_END)) {
        perror("Error seeking file end");
        fclose(file);
        return NULL;
    }
    
    long file_size = ftell(file);
    if (file_size == -1) {
        perror("Error getting file size");
        fclose(file);
        return NULL;
    }

    rewind(file);

    char* buffer = malloc((size_t)file_size + 1); // +1 for null terminator
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read != (size_t)file_size) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[bytes_read] = (char)0; // Null-terminate the string

    if (fclose(file)) {
        perror("Error closing file");
        // Proceed since buffer is already loaded
    }

	return buffer;
}
