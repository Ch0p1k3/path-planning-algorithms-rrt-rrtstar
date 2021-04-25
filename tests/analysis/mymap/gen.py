arr = [[0 for j in range(500)] for i in range(400)]

for i in range(30, 34):
    for j in range(40, 200):
        arr[i][j] = 1

for i in range(370, 374):
    for j in range(40, 200):
        arr[i][j] = 1

for i in range(34, 370):
    for j in range(180, 200):
        arr[i][j] = 1

for i in range(44, 48):
    for j in range(20, 160):
        arr[i][j] = 1

for i in range(356, 360):
    for j in range(20, 160):
        arr[i][j] = 1
    
for i in range(48, 356):
    for j in range(150, 160):
        arr[i][j] = 1

tmp = [row[::-1] for row in arr]
for i in range(len(arr)):
    for j in range(len(arr[0])):
        arr[i][j] += tmp[i][j]

with open('tmp.txt', 'w') as f:
    for i in range(len(arr)):
        f.write('<row>')
        for j in range(len(arr[0])):
            f.write(str(arr[i][j]))
            f.write(' ')
        f.write('</row>\n')