# -*- coding: utf-8 -*-

import os

IN_FILE = 'dab.mp2'
OUT_FILE = IN_FILE.split('.')[0]

# 1 head and open
in_file  = open(IN_FILE, 'rb') 
out_file = open(OUT_FILE+'.c', 'w')
in_size  = os.path.getsize(IN_FILE)
out_file.write('uint8_t %s[%d] = {\n    '%(OUT_FILE, in_size))

# 2 content
while True:
	block = in_file.read(1024)
	if block:
		for i in range(0, len(block)):
			out_file.write('0x%02x'%block[i]+', ')
			if not (i+1)%16:
				out_file.write('\n    ')
	else:
		break

# 3 } and close
in_file.close()
out_file.write('\n}')
out_file.close()
