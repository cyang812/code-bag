#coding=utf-8
import re
import os,os.path

# 将正则表达式编译成Pattern对象
BOUNDARY = re.compile(r'frame\d+ tim count = ')
#BOUNDARY = "frame2 tim count = " #分隔符

# 处理sentence列表的方法函数
def getAddr(s):  #获取标注位置
    g = s.split(" | ")[0]
    return g

f = open("44_tim_120.txt", "r", encoding='utf-8')
content = f.read()  # 读取全部内容
#content = content.replace(u'frame(.*) tim count = ', u'') #替换书名前的空格
clips = re.split(BOUNDARY,content)
#clips = content.split('\n')
#print(content)
print("列表个数：",clips.__len__()) # 获取列表的个数
sum_num = 0
for i in range(1,(clips.__len__()-1)):
    #print(clips[i])
    sum_num += int(clips[i])

print("sum = ",sum_num)
print("ave = ",sum_num/(clips.__len__() - 1))



