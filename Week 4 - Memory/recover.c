#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Remember filenames
    char *infile = argv[1];

    // Open input file
    FILE *file = fopen(infile, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }

    int image_count = 0;
    char image_name[8];
    sprintf(image_name, "%.3i.jpg", image_count);
    // sprintf(image_name, "%.3i.jpg", image_count);

    BYTE buffer[512];
    int BLOCK_SIZE = 512;
    bool writing = false;
    FILE *output = fopen(image_name, "w");
    if (output == NULL)
    {
        fclose(output);
        return 1;
    }

    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // whenever we find a new jpeg
        bool four_a = buffer[3] == 0xe0 || buffer[3] == 0xe1 || buffer[3] == 0xe2 || buffer[3] == 0xe3 || buffer[3] == 0xe4;
        bool four_b = buffer[3] == 0xe5 || buffer[3] == 0xe6 || buffer[3] == 0xe7 || buffer[3] == 0xe8 || buffer[3] == 0xe9;
        bool four_c = buffer[3] == 0xea || buffer[3] == 0xeb || buffer[3] == 0xec || buffer[3] == 0xed || buffer[3] == 0xef;
        bool four_d = buffer[3] == 0xed;
        bool four = four_a || four_b || four_c || four_d;
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && four)
        {
            // if we're already writing to an existing JPEG, close that file and start writing to a new file
            if (writing)
            {
                // open a new jpeg
                image_count++;
                sprintf(image_name, "%.3i.jpg", image_count);
                output = fopen(image_name, "w");
                if (output == NULL)
                {
                    fclose(output);
                    return 1;
                }
                // write to the new jpeg
            }
            // if this is our first jpeg, open a new jpeg and start writing to it
            else
            {
                writing = true;
            }
        }
        // if writing, write these bytes to the open image
        if (writing)
        {
            fwrite(buffer, 1, BLOCK_SIZE, output);
        }
    }
    fclose(file);
    fclose(output);
    // For testing the naming of the next image
    // printf("%s\n", image_name);
}