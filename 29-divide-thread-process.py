# -*- coding: utf-8 -*-
# @Author: cyang
# @Date:   2020-09-25 19:51:28
# @Last Modified by:   cyang
# @Last Modified time: 2020-09-26 13:16:21


import time
import threading
from multiprocessing import Process

import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D  #用来给出三维坐标系。

loop_1_list = list()
loop_2_list = list()

def divide(dividend, divisor, result_list):

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

	# return loop_2	
	# print(divisor, loop_2)
	# result_list.append(loop_2)

def divide_thread(dividend, i, j, result_list):
	for idx in range(i, j):
		divide(dividend, idx, result_list)


if __name__ == '__main__':
	dividend = 2147483647
	divisor = 0
	loop_cnt = 10000
	loop_list = [[0 for i in range(100)] for j in range(100)]

	result_normal = list()
	result_thread = list()
	result_process = list()

	#normal 
	print("---Normal case---")
	toc = time.time()
	for i in range(1, loop_cnt):
		divide(dividend, i, result_normal)
	tic = time.time()
	print("Normal way: %s" % ((tic-toc)*1000))

	#thread
	print("---Thread case---")
	toc = time.time()
	i = 1
	j = loop_cnt
	thread_lists = []
	thread_cnt = 4
	for idx in range(thread_cnt):
		i = int(1 + idx*loop_cnt/thread_cnt)
		j = int(i + loop_cnt/thread_cnt)
		t = threading.Thread(target=divide_thread, args=(dividend, i, j, result_thread))
		thread_lists.append(t)
	
	for thread_list in thread_lists:
		thread_list.start()	
		# thread_list.join()

	tic = time.time()
	print("Mutil Thread: %s" % ((tic-toc)*1000))

	#process
	print("---Process case---")
	toc = time.time()
	i = 1
	j = loop_cnt
	process_lists = []
	process_cnt = thread_cnt
	for idx in range(process_cnt):
		i = int(1 + idx*loop_cnt/process_cnt)
		j = int(i + loop_cnt/process_cnt)
		p = Process(target=divide_thread, args=(dividend, i, j, result_process))
		process_lists.append(p)
	
	for process_list in process_lists:
		process_list.start()	

	tic = time.time()
	print("Multiprocessing: %s" % ((tic-toc)*1000))	
		
	is_alive = [1 for i in range(process_cnt)]
	while sum(is_alive):
		for idx in range(len(process_lists)):
			if not process_lists[idx].is_alive():
				is_alive[idx] = 0

	tic = time.time()
	print("Multiprocessing: %s" % ((tic-toc)*1000))