#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt the user for height
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // For each row
    for (int i = 1; i <= height; i++)
    {
        // Print leading spaces
        int spaces_left = height - i;
        while (spaces_left > 0)
        {
            printf(" ");
            spaces_left = spaces_left - 1;
        }

        // Print left half of pyramid
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        // Print valley
        printf("  ");

        // Print right half of pyramid
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }

        // Start new line
        printf("\n");
    }
}