import requests
import os,time

def get(list, idx):

	i = '%03d' %list
	j = '%02d' %idx

	url = 'http://2ol8.swmn.net/photo/beautyleg/beautyleg'+i+'/'+j+'.jpg'
	print('wait for url', url)
	# return

	response = requests.get(url)
	pic = response.content

	if response.status_code == 404:
		return '404 not found!'

	return

	if not os.path.exists(iofolder+i):
		os.makedirs(iofolder+i)

	with open(iofolder+'%s.jpg' %i, 'wb') as pp:
		pp.write(pic)

	return 'okay~!'

if __name__ == '__main__':
	# iofolder='test/'
	# start = time.time()

	# for i in range(1, 1636):
	# 	for j in range(1, 2):
	# 		get(i, j)

	# end = time.time()
	# print('complete, use time:', end - start)

	for i in range(746, 175320):

		url = 'http://image.bee-ji.com/' + str(i )
		print('wait for url', url)

		response = requests.get(url)
		if response.status_code == 404:
			print('404 not found!')
		else :
		    print(response.status_code)	