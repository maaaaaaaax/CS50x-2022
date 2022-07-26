# Implement a program that prints out a double half-pyramid of a specified height,
# per the specifications at https://cs50.harvard.edu/x/2022/psets/6/mario/more/

from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break

for i in range(1, height + 1):
    for l in range(height - i):
        print(" ", end="")
    for j in range(i):
        print("#", end="")
    print("  ", end="")
    for k in range(i):
        print("#", end="")
    print()