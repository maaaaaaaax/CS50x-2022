#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool only_digits(string s);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    if (!only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    int key = atoi(argv[1]);
    // printf("Key is %i\n", key);
    string p = get_string("plaintext: ");
    printf("ciphertext: ");
    int length = strlen(p);
    for (int i = 0; i < length; i++)
    {
        if (isalpha(p[i]))
        {
            if (islower(p[i]))
            {
                char c = (((p[i]) + key - 97) % 26) + 97;
                printf("%c", c);
            }
            else
            {
                char c = (((p[i]) + key - 65) % 26) + 65;
                printf("%c", c);
            }
        }
        else
        {
            printf("%c", p[i]);
        }
    }
    printf("\n");
    return 0;
}

bool only_digits(string s)
{
    int length = strlen(s);
    for (int i = 0; i < length; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}