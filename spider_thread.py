from concurrent import futures
import requests
import os,time

def get(url,i,j):
	response = requests.get(url,headers=set_header(url))
	pic=response.content
	if response.status_code==404:
		return '404 not found!'
	if not os.path.exists(iofolder+i):
		os.makedirs(iofolder+i)
	with open(iofolder+'%s/%s.jpg'% (i,j),'wb') as pp:
		pp.write(pic)
	return 'okay~!'
 
def req(i):
	url = 'http://image.bee-ji.com/' + str(i)
	print('wait for url', url)

	response = requests.get(url)
	if response.status_code == 404:
		print('404 not found!')
	else :
		print(response.status_code) 

if __name__ == '__main__':
	limit=65
	iofolder='mm131/'
	start = time.time()
	wokers=100

	with futures.ThreadPoolExecutor(wokers) as e:
		e.map(req,[ str(i) for i in range(1, 175322)])

	end = time.time()

	print('爬取任务已完成,消耗时间:', end - start)

