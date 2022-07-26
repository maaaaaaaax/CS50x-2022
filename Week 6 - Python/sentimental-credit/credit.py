# Implement a program that determines whether a provided credit card number is valid according to Luhn’s algorithm.

# Specifications are available at https://cs50.harvard.edu/x/2022/psets/6/credit/ and https://cs50.harvard.edu/x/2022/psets/1/

from cs50 import get_string

number = get_string("Number: ")

if len(number) not in [13, 15, 16]:
    print("INVALID")

if not number.isnumeric():
    print("INVALID")

digits = 0

# Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
for i in number[-2::-2]:
    product = str(int(i) * 2)
    for c in product:
        digits += int(c)

# Add the sum to the sum of the digits that weren’t multiplied by 2.
for j in number[-1::-2]:
    digits += int(j)

# If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
if (digits % 10) != 0:
    print("INVALID")

if number[0] == '4':
    print("VISA")

if number[0:2] in ["34", "37"]:
    print("AMEX")

if number[0:2] in ["51", "52", "53", "54", "55"]:
    print("MASTERCARD")

print("INVALID")