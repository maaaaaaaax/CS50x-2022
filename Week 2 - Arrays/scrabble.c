#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    int score = 0;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        // printf("%c as uppercase is %c.\n", word[i], toupper(word[i]));
        // printf("%c in ASCII is %i.\n", toupper(word[i]), toupper(word[i]));
        int place_in_alphabet = (int) toupper(word[i]) - 65;
        // printf("%c is the %i letter in the alphabet.\n", toupper(word[i]), place_in_alphabet);
        // printf("%c is worth %i points.\n", word[i], POINTS[place_in_alphabet]);
        // printf("\n");
        if (place_in_alphabet >= 0 && place_in_alphabet <= 25)
        {
            score += POINTS[place_in_alphabet];
        }
    }

    return score;
}
