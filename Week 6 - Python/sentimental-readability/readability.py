# Implement a program that computes the approximate grade level needed to comprehend some text,
# per https://cs50.harvard.edu/x/2022/psets/6/readability/ and https://cs50.harvard.edu/x/2022/psets/2/readability/

text = input("Text: ")

# Count the letters in the sample text.
# You may assume that a letter is any lowercase character from a to z or any uppercase character from A to Z.

letters = 0
for letter in text:
    if letter.isalpha():
        letters += 1
# print(f"Letters: {letters}")

# Count the number of words in the sample text.
# For the purpose of this problem, we’ll consider any sequence of characters separated by a space to be a word
# (so a hyphenated word like "sister-in-law" should be considered one word, not three).

words = 1
for letter in text:
    if letter == " ":
        words += 1
# print(f"Words: {words}")

# Count the number of sentences in the sample text.
# You should consider any sequence of characters that ends with a . or a ! or a ? to be a sentence.

sentences = 0
for letter in text:
    if letter in [".", "!", "?"]:
        sentences += 1
# print(f"Sentences: {sentences}")

# Calculate the average number of letters per 100 words in the text
l = (letters / words) * 100

# Calculate the average number of sentences per 100 words in the text
s = (sentences / words) * 100

index = 0.0588 * l - 0.296 * s - 15.8

if index < 1:
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")
