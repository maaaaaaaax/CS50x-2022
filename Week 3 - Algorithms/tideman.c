#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // If name is a match for the name of a valid candidate
    for (int k = 0; k < candidate_count; k++)
    {
        if (strcmp(name, candidates[k]) == 0)
        {
            //  Update the ranks array to indicate that the voter has the candidate as their rank preference
            // (where 0 is the first preference, 1 is the second preference, etc.)
            ranks[rank] = k;
            // printf("Voted successfully. Rank %i is %s.\n", rank, candidates[ranks[rank]]);
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // For each rank
    for (int j = 0; j < candidate_count; j++)
    {
        // Increment the count of each candidate below that rank, vs. this rank
        for (int k = j + 1; k < candidate_count; k++)
        {
            preferences[ranks[j]][ranks[k]]++;
            // printf("preferences[%i][%i] is %i.\n", ranks[j], ranks[k], preferences[ranks[j]][ranks[k]]);
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Add all pairs of candidates where one candidate is preferred to pairs[]
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("%s beats %s by a margin of %i.\n", candidates[pairs[i].winner], candidates[pairs[i].loser], (preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]));
    // }
    // printf("\n");
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Sort the pairs array in decreasing order of strength of victory,
    // where strength of victory is defined to be the number of voters who prefer the preferred candidate
    // If multiple pairs have the same strength of victory, you may assume that the order does not matter.
    for (int i = 0; i < pair_count; i++)
    {
        // int biggest_victory = (number who prefered winner over loser) - (number who prefered loser over winner)
        int biggest_victory = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        for (int j = i + 1; j < pair_count; j++)
        {
            int current_victory = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            if (current_victory > biggest_victory)
            {
                pair temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
    // printf("Sorting pairs...\n");
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("%s beats %s by a margin of %i.\n", candidates[pairs[i].winner], candidates[pairs[i].loser], (preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]));
    // }
    // printf("\n");
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // For each pair
    int locked_pairs_count = 0;
    for (int i = 0; i < pair_count; i++)
    {
        // If the locking in the pair doesn't create a cycle
        bool seen_winners[candidate_count];
        for (int j = 0; j < candidate_count; j++)
        {
            seen_winners[j] = false;
        }
        seen_winners[pairs[i].winner] = true;
        int level = 0;
        while (level <= locked_pairs_count)
        {
            // For each candidate...
            // add that candidate's index to seen_winners if it isn't already present
            for (int k = 0; k < candidate_count; k++)
            {
                // ... who is locked in over anyone in the seen_winners array ...
                for (int l = 0; l < candidate_count; l++)
                {
                    if (seen_winners[l])
                    {
                        if (locked[k][l])
                        {
                            seen_winners[k] = true;
                        }
                    }
                }
            }
            level++;
        }

        if (!seen_winners[pairs[i].loser])
        {
            // Lock in the winner over the loser
            locked[pairs[i].winner][pairs[i].loser] = true;
            locked_pairs_count++;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // For each pair
    // for (int i = 0; i < pair_count; i++)
    // {
    //     // if no candidate is locked in over the winner of that pair
    //     bool unbeaten = true;
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         if (locked[j][pairs[i].winner])
    //         {
    //             unbeaten = false;
    //             break;
    //         }
    //     }

    //     if (unbeaten)
    //     {
    //         printf("%s\n", candidates[pairs[i].winner]);
    //         break;
    //     }
    // }

    // for each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // if no one is locked in over them, print their name
        bool winner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                winner = false;
                break;
            }
        }
        if (winner)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}