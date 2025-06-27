#!/usr/bin/python3

def closest_number(target):
    numbers = list(range(60, 241, 10))
    target += 2
    closest = min(numbers, key=lambda x: abs(x - target))
    return closest

if __name__ == "__main__":

    flag = ""
    values = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'F', 'S', '{', '}', '_']

    with open('mechanical-display.vcd', 'r') as file:
        data = [line.strip() for line in file][11:]

    last = -1
    first = data[1].split()[0][1:]
    second = data[2].split()[0][1:]
    for i in range(3, len(data) - 1, 2):
        first = data[i].split()[0][1:]
        second = data[i+1].split()[0][1:]
        diff = int(second) - int(first)
        if (diff - 7) > last or (diff + 7) < last:
            last = diff
            val = closest_number(diff)
            char = str(values[int(val/10 - 6)])
            flag += char
    print(flag)
