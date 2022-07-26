# Implement a program that identifies a person based on their DNA, per https://cs50.harvard.edu/x/2022/psets/6/dna/

import csv
import sys


def main():

    # Check for command-line usage
    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py FILENAME FILENAME")

    # Read database file into a variable
    people = []
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            people.append(row)
    strs = {}
    for column in people[0]:
        if column != "name":
            strs[column] = 0

    # Convert count strings to ints
    for i in range(len(people)):
        for trait in people[i]:
            if trait != "name":
                people[i][trait] = int(people[i][trait])

    # TODO: Read DNA sequence file into a variable
    dna_sequence = ""
    with open(sys.argv[2], "r") as sequence:
        for line in sequence:
            dna_sequence += line.strip()

    # TODO: Find longest match of each STR in DNA sequence
    for sequence in strs:
        # ex. sequence == "AGATC"
        repeats = 0
        test_string = sequence
        while True:
            if test_string in dna_sequence:
                repeats += 1
                test_string += sequence
            else:
                strs[sequence] = repeats
                break

    # print(people[0])
    # print(strs)

    # TODO: Check database for matching profiles
    no_match = True
    for person in people:
        match = True
        for trait in person:
            if trait != "name":
                if person[trait] != strs[trait]:
                    match = False
                    break
        if match:
            no_match = False
            print(person["name"])
            break
    if no_match:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
