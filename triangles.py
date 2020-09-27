# -*- coding: utf-8 -*-

# 期待输出:
# [1]
# [1, 1]
# [1, 2, 1]
# [1, 3, 3, 1]
# [1, 4, 6, 4, 1]
# [1, 5, 10, 10, 5, 1]
# [1, 6, 15, 20, 15, 6, 1]
# [1, 7, 21, 35, 35, 21, 7, 1]
# [1, 8, 28, 56, 70, 56, 28, 8, 1]
# [1, 9, 36, 84, 126, 126, 84, 36, 9, 1]

def triangles_1():
    N=[1]     
    while True:    
        yield N
        #N.append(0) #wrong
        N=N+[0]
        N=[N[i-1]+N[i] for i in range(len(N))]

def triangles_2():
    l = [1]
    while True:
        yield l
        l = [0]+l+[0]
        l = [l[i]+l[i+1] for i in range(len(l)-1)]

def triangles():
    result = []
 
    while True:
        temp = [result[x] for x in range(len(result))] #copy list result
        
        for i in range(1,len(temp)):
            temp[i] = result[i-1]+result[i]
                   
        temp.append(1)
        result = temp #bak up
        yield temp
    
n = 0
results = []
for t in triangles_1():
    results.append(t)
    n = n + 1
    if n == 10:
        break

print('--------')
for t in results:
    print(t)

if results == [
    [1],
    [1, 1],
    [1, 2, 1],
    [1, 3, 3, 1],
    [1, 4, 6, 4, 1],
    [1, 5, 10, 10, 5, 1],
    [1, 6, 15, 20, 15, 6, 1],
    [1, 7, 21, 35, 35, 21, 7, 1],
    [1, 8, 28, 56, 70, 56, 28, 8, 1],
    [1, 9, 36, 84, 126, 126, 84, 36, 9, 1]
]:
    print('测试通过!')
else:
    print('测试失败!')
