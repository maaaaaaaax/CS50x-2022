// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Track the size of our dictionary
int dictionary_size = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Hash new word
    int h = hash(word);
    node *list = table[h];
    while (list != NULL)
    {
        if (strcasecmp(word, list->word) == 0)
        {
            return true;
        }
        else
        {
            list = list->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file != NULL)
    {
        // TODO: Read strings from file one at a time
        char buffer[LENGTH + 1];
        while (fscanf(file, "%s", buffer) > 0)
        {
            // Create a new node for each word
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                return false;
            }
            strcpy(n->word, buffer);
            n->next = NULL;

            // Hash word to obtain a hash value
            int h = hash(buffer);

            // print each line for testing purposes
            // printf("%s %i\n", buffer, h);

            // Insert node into hash table at that location
            node *tmp = table[h];
            table[h] = n;
            n->next = tmp;
            dictionary_size++;
        }
        fclose(file);
        return true;
    }
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictionary_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < 26; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
        free(table[i]);
    }
    return true;
}
