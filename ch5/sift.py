# *****************************************************************************
# * Filename : sift.py
# * Date : 21/May/2018
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : implements image stitching
# * Detailed Description : to run type "python sift.py img1.bmp img2.bmp"
# *****************************************************************************
import numpy as np
import cv2
from PIL import Image as im
import sys
import os
import scipy.misc as sc
import time


def findkps(matchindx,kp1, kp2):
	matches = []
	locations = np.nonzero(matchindx)[0]
	for i in locations:
		matches.append([kp1[i],kp2[matchindx[i][0]]])
	return matches
	
#****************************************************************************
def match(desc1,desc2):
	""" For each descriptor in the first image, select its match in the second image. input: desc1 (descriptors for the first image), desc2 (same for second image). """
	
	#normalization -> ||v||
	desc1 = np.array([d/np.linalg.norm(d) for d in desc1])
	desc2 = np.array([d/np.linalg.norm(d) for d in desc2])

	dist_ratio = 0.6
	desc1_size = desc1.shape
	
	matchscores = np.zeros((desc1_size[0],1),'int')
	desc2t = desc2.T # precompute matrix transpose
	for i in range(desc1_size[0]):
		dotprods = np.dot(desc1[i,:],desc2t) # vector of dot products
		dotprods = 0.9999*dotprods
		# inverse cosine and sort, return index for features in second image
		indx = np.argsort(np.arccos(dotprods))

		# check if nearest neighbor has angle less than dist_ratio times 2nd
		if np.arccos(dotprods)[indx[0]] < dist_ratio * np.arccos(dotprods)[indx[1]]:
			matchscores[i] = int(indx[0])
	return matchscores
#****************************************************************************
# this function writes the images into pgm format if its not in that format
def imagecheck(img_path,n):
	if img_path[-3:] != 'pgm':
		img = im.open(img_path).convert('L')
		sc.imsave('tmp' + str(n) + '.pgm',img)
		return 'tmp' + str(n) + '.pgm'
	else:
		return img_path

#****************************************************************************
# finds the sift of the image and returns the keypoints onto a text file
def sifti_fy(image,n):
	start_time = time.time()
	os.system("./sift <"+image+"> "+str(n)+".txt")
	print "time for finding the keypoints and descriptors of the image ",time.time() - start_time, "seconds"

#****************************************************************************
# this function reads the keypoints and descriptors from the file that is 
# output by the sift binary
def chunker(iterable, chunksize):
	return map(None,*[iter(iterable)]*chunksize)

def read_key(n):
	f = open(str(n)+".txt")
	lines = f.readlines()
	del lines[0]		#first line is not necessary
	#**************************************************
	#keypoints
	kp = []
	k = lines[0::8]	
	del lines[0::8]
	for i in k:
		l = "".join(list(i))
		kp.append(map(round,map(float ,l.split(" "))))
	#**************************************************
	#descriptors
	des = []
	desc = chunker(lines,7)
	for i in desc:
		l = "".join(list(i))
		des.append(map(int ,l.strip().split(" ")))
	return np.array(kp).T[:-2].T,np.array(des)
#****************************************************************************
def append_images(img1, img2):
	h1 = len(img1)
	w1 = len(img1[0])
	h2 = len(img2)
	w2 = len(img2[0])
	appendedimage = np.zeros((max(h1,h2), w1+w2 ,3), np.uint8)
	appendedimage[:h1, :w1] = img1
	appendedimage[:h2, w1:w1+w2] = img2
	return appendedimage
#****************************************************************************
def match_twosided(desc1,desc2):
    """ Two-sided symmetric version of match(). """
    
    matches_12 = match(desc1,desc2)
    matches_21 = match(desc2,desc1)
    
    ndx_12 = matches_12.nonzero()[0]
    
    # remove matches that are not symmetric
    for n in ndx_12:
        if matches_21[int(matches_12[n])] != n:
            matches_12[n] = 0
    
    return matches_12
#****************************************************************************

def warpTwoImages(img1, img2, H):
	'''warp img2 to img1 with homography H'''
	h1,w1 = img1.shape[:2]
	h2,w2 = img2.shape[:2]
	pts1 = np.float32([[0,0],[0,h1],[w1,h1],[w1,0]]).reshape(-1,1,2)
	pts2 = np.float32([[0,0],[0,h2],[w2,h2],[w2,0]]).reshape(-1,1,2)
	pts2_ = cv2.perspectiveTransform(pts2, H)
        print "time taken to warp the images ",time.time() - start_time, "seconds"
	pts = np.concatenate((pts1, pts2_), axis=0)
	# print "pts.min(axis=0).ravel() = ", pts.min(axis=0).ravel()
	[xmin, ymin] = np.int32(pts.min(axis=0).ravel())
	[xmax, ymax] = np.int32(pts.max(axis=0).ravel())
	# print "xmax = ", xmax
	# print "xmin = ", xmin
	# print "ymax = ", ymax
	# print "ymin = ", ymin
	t = [-xmin,-ymin]
	Ht = np.array([[1,0,t[0]],[0,1,t[1]],[0,0,1]]) # translate

	result = cv2.warpPerspective(img2, Ht.dot(H), (xmax-xmin, ymax-ymin))
	result[t[1]:h1+t[1],t[0]:w1+t[0]] = img1
	return result


if __name__ == "__main__":
	img_path = sys.argv
	img1 = cv2.imread(img_path[1])
	img2 = cv2.imread(img_path[2])
	img_path[1] = imagecheck(img_path[1],1)
	img_path[2] = imagecheck(img_path[2],2)
	sifti_fy(img_path[1],1)
	sifti_fy(img_path[2],2)
	kp1,des1 = read_key(1)
	kp2,des2 = read_key(2)
	matchindx = match_twosided(des1,des2)
	matches = np.array(findkps(matchindx,kp1,kp2))
	matchimg1 = matches[:][:,0]
	matchimg2 = matches[:][:,1]
	
	matchimg1secondcolumn = np.reshape(matchimg1[:,1],(len(matchimg1[:,1]),1))
	matchimg1 = np.float32(np.append(matchimg1secondcolumn.T,np.array([matchimg1[:,0]]),axis=0).T)
	
	matchimg2secondcolumn = np.reshape(matchimg2[:,1],(len(matchimg2[:,1]),1))
	matchimg2 =  np.float32(np.append(matchimg2secondcolumn.T,np.array([matchimg2[:,0]]),axis=0).T)
        start_time = time.time()
	print "matchimg1", len(matchimg1)
	h, mask = cv2.findHomography(matchimg1,matchimg2,cv2.RANSAC,5.0)
        # print "mask =", mask
	print "homography = ", h
        print "time taken for finding homography ",time.time() - start_time, "seconds"
	result = warpTwoImages(img2, img1, h)
	cv2.imshow("",result)
	cv2.waitKey()
	cv2.imwrite("stitched.jpg",result)
 #****************************************************************************
