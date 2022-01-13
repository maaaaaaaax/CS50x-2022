#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Prompt the user for their name
    string name = get_string("What's your name? ");

    // Greet the user
    printf("hello, %s\n", name);
}