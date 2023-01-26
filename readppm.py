import io 
import sys 
import os

def readHeader(f):
	mn = f.readline()

def readppm(filename):
	print(filename)
	filename = filename+".ppm"
	f = open(filename, "r")
	code = f.read(2)
	f.close()
	pixels=[]
	pixels.append(code[1])
	if code == 'P6':
		f = open(filename, "rb")
		content = f.readlines()
		i = 1
		while('#' in content[i].decode('utf-8')):
			i+=1
		wh = content[i].decode('utf-8').split()
		w = wh[0]
		h  = wh[1]
		pixels.append(int(w))
		pixels.append(int(h))
		print(w, h)
		i+=1
		d = content[i].decode('utf-8').strip()
		pixels.append(int(d))
		i+=1 
		while i < len(content):
			for k in range(0, len(content[i])-2, 3):
				r = content[i][k]
				g = content[i][k+1]
				b = content[i][k+2]
				pixels.append(r)
				pixels.append(g)
				pixels.append(b)
			i+=1


	elif code == 'P3':
		f = open(filename, "r")
		content = f.readlines()
		i = 1
		while '#' in content[i]:
			i+=1 
		wh = content[i].split()
		w = wh[0]
		h = wh[1]
		i+=1 
		d = content[i]
		i+=1
		pixels.append(int(w))
		pixels.append(int(h))
		pixels.append(int(d))
		while i < len(content):
			val = int(content[i])
			pixels.append(val)
			i+=1

		#read a p3 file
	else:
		print("err")
		#error

	return pixels

if __name__ == "__main__":
	readppm("P6/coin")