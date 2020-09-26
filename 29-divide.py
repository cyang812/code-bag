# -*- coding: utf-8 -*-
# @Author: cyang
# @Date:   2020-09-25 19:51:28
# @Last Modified by:   cyang
# @Last Modified time: 2020-09-26 10:50:52


import time
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D  #用来给出三维坐标系。

loop_1_list = list()
loop_2_list = list()

def divide(dividend, divisor):

	'''
	假设 dividend 和 divisor 都为0~2^31-1 的整数
	'''

	# if (dividend > 0 and divisor > 0) or (dividend <0 and divisor < 0):
	# 	flag = 1    
	# else:
	# 	flag = 0    
	
	# if abs(dividend) < abs(divisor):
	# 	return 0
	# elif abs(dividend) == abs(divisor):
	# 	return 1 if flag else -1

	# if abs(divisor) == 1:
	# 	if dividend == -2147483648 and divisor == -1: 
	# 		return 2147483648-1
	# 	else:
	# 		return abs(dividend) if flag else -abs(dividend)    

	dividend = abs(dividend)
	divisor = abs(divisor)
	
	result = 0
	loop_1 = 0
	loop_2 = 0

	while dividend >= divisor:
		loop_1 += 1
		temp_divisor, count = divisor, 1

		while dividend >= (temp_divisor<<1):
			loop_2 += 1
			count <<= 1
			temp_divisor <<= 1

		result += count
		dividend -= temp_divisor

	# loop_1_list.append(loop_1)
	# loop_2_list.append(loop_2)

	# if flag:
		# return result, loop_1, loop_2
	# else:
		# return -result, loop_1, loop_2

	return loop_2	

def type_transfer(x,y):
	value = x+y
	return value

if __name__ == '__main__':
	dividend = 2147483647
	divisor = 0
	loop_cnt = 100
	loop_list = [[0 for i in range(100)] for j in range(100)]

	for i in range(1, loop_cnt):
		for j in range(1, i):
			toc = time.time()
			loop_list[i][j] = divide(i, j)
			tic = time.time()
			print(loop_list[i][j], (tic-toc)*1000)

	# 2维图像		
	# x = np.arange(1, loop_cnt, 1)
	# plt.plot(x, loop_1_list)
	# plt.plot(x, loop_2_list)
	# plt.show()	


	# 3维图像
	fig1 = plt.figure()
	ax = Axes3D(fig1)
	X, Y = np.mgrid[1:100:100j, 1:100:100j]
	Z1 = type_transfer(X, Y)
	# numsize=len(Z1)
	# print(numsize, type(Z1))
	Z=Z1
	# print(Z)

	for i in range(loop_cnt):
		for j in range(loop_cnt):
			Z[i][j]=loop_list[i][j]

	# plt.title("This is main title")
	# ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=plt.cm.coolwarm, alpha=0.5)
	ax.plot_surface(X, Y, Z, cmap=plt.cm.rainbow)
	ax.set_xlabel('dividend', color='b')
	ax.set_ylabel('divisor', color='r')
	ax.set_zlabel('loop_cnt', color='g')
	plt.show()