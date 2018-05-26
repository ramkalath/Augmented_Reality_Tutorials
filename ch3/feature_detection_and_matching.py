# *****************************************************************************
# * Filename : feature_detection_and_matching.py
# * Date : 14/May/2018
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : implements feature detection and matching using David Lowe's binaries
# * Detailed Description : To run this code type "python feature_detection_and_matching.py img1.bmp img2.bmp"
# *****************************************************************************
import numpy as np
np.set_printoptions(threshold=np.nan)
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
                print kp1[i], "\t", kp2[matchindx[i][0]]
        return matches
        
        
#****************************************************************************
def match(desc1,desc2):
        """ For each descriptor in the first image, select its match in the second image. input: desc1 (descriptors for the first image), desc2 (same for second image). """
        
        #normalization -> ||v||
        desc1 = np.array([d/np.linalg.norm(d) for d in desc1])  # normalizing the first descriptor vector. This is requried before matching
        desc2 = np.array([d/np.linalg.norm(d) for d in desc2])  # normalizing the second descriptor vector.

        dist_ratio = 0.6        # any match below this values is a weak match and this is the threshold that is used for discarding                             
        desc1_size = desc1.shape
        
        matchscores = np.zeros((desc1_size[0],1),'int') # match scores provide a list of strength of matches
        
        desc2t = desc2.T # precompute matrix transpose
        for i in range(desc1_size[0]):
                dotprods = np.dot(desc1[i,:],desc2t) # vector of dot products
                dotprods = 0.9999*dotprods
                # inverse cosine and sort, return index for features in second image
                indx = np.argsort(np.arccos(dotprods))

                # check if nearest neighbor has angle less than dist_ratio times 2nd
                if np.arccos(dotprods)[indx[0]] < dist_ratio * np.arccos(dotprods)[indx[1]]: # ensuring all the matches are larger than 0.6
                        matchscores[i] = int(indx[0])
        return matchscores

#****************************************************************************
# this function checks if the image is a .pgm file, if not it converts the image to greyscale and then to pgm. Then the new path is returned
def imagecheck(img_path,n): # n is the image number. 1 for the first image, 2 for the second and so on.
        if img_path[-3:] != 'pgm':      # check if the last three characters of the path (i.e. extension of file) is pgm
                pgm_img_name = 'tmp'+str(n)+".pgm"      # lets call the name of the pgm file as tmp#.pgm where # corresponds to image number
                im.open(img_path).convert('L').save(pgm_img_name) # open the image, convert the image to greyscale and save the image
        return pgm_img_name # return the new path of the image

#****************************************************************************
# finds the sift of the image and writes the keypoints and descriptors onto a text file
def sifti_fy(image,n):
        os.system("./sift <"+image+"> "+str(n)+".txt")  # performs the sift operation on an image and creates a text file with the keypoints and descriptor values

#****************************************************************************
# this function parses the text file containing the keypoints and descriptors and then returns the keypoints and descriptors as arrays
def chunker(iterable, chunksize):
        return map(None,*[iter(iterable)]*chunksize)

def read_key(n):
        f = open(str(n)+".txt")
        lines = f.readlines()
        del lines[0]            #first line is not necessary
        #-------------------------------------
        #keypoints
        kp = []
        k = lines[0::8] 
        del lines[0::8]
        for i in k:
                l = "".join(list(i))
                kp.append(map(round,map(float ,l.split(" "))))
        #-------------------------------------
        #descriptors
        des = []
        desc = chunker(lines,7)
        for i in desc:
                l = "".join(list(i))
                des.append(map(int ,l.strip().split(" ")))
        return np.array(kp).T[:-2].T,np.array(des)

#****************************************************************************
# this function appends the imges one beside the other and returns the appended image
def append_images(img1, img2):
        h1 = len(img1)
        w1 = len(img1[0])
        h2 = len(img2)
        w2 = len(img2[0])
        appendedimage = np.zeros((max(h1,h2), w1+w2 ,3), np.uint8) # creates a template image the size of both the images appended together
        appendedimage[:h1, :w1] = img1  # inserting image1 into the template image
        appendedimage[:h2, w1:w1+w2] = img2  # inserting image2 into the template image
        return appendedimage

#****************************************************************************
# this function does a 2 side match and discards asymetric matches 
def match_twosided(desc1,desc2):
    """ Two-sided symmetric version of match(). """
    matches_12 = match(desc1,desc2) # match is the function that computes the matches
    matches_21 = match(desc2,desc1) # checks for reverse match too
    
    ndx_12 = matches_12.nonzero()[0]
    
    # remove matches that are not symmetric
    for n in ndx_12:
        if matches_21[int(matches_12[n])] != n:
            matches_12[n] = 0
    return matches_12
#****************************************************************************

if __name__ == "__main__":
        img_path = sys.argv     # we pass the images to be matches as command line arguments
        img1 = cv2.imread(img_path[1])  # image 1 read from command line
        img2 = cv2.imread(img_path[2])  # image 2 read from command line
        img_path[1] = imagecheck(img_path[1],1) # check if the image is pgm if not convert the image to pgm and return new path for image 1; imagecheck()
        img_path[2] = imagecheck(img_path[2],2) # Check and return the image path as pgm for image 2; goto function imagecheck()
        sifti_fy(img_path[1],1) # performs the sift operation on the first image; goto function sifti_fy()
        sifti_fy(img_path[2],2) # performs the sift operation on the second image goto function sifti_fy()
        kp1,des1 = read_key(1)  # read_key() parses the file and returns keypoints and descriptors as numpy arrays
        kp2,des2 = read_key(2)  # read_key() for the second image
        matchindx = match_twosided(des1,des2) # checks for image1 descriptor correlation with image2 descriptors and vice versa.
        matches = findkps(matchindx,kp1,kp2) # returns a list of keypoints that corresponds to the matching indices
        appendedimage = append_images(img1, img2) # appends the original two images to show the matches
        w = len(img1[0])
        print "number of matches = ", len(matches); # printing the number of matches
        # ------------------------------------------------------------------------------------------------------
        # code below draws red spots to indicate keypoints and green lines between the matching points of appended images. 
        for i in matches:
                (x1, y1) = tuple(np.array([i[0][1],i[0][0]],int)) # sift finds keypoints with subpixel accuracy so we convert this to int before matching
                (x2, y2) = tuple(np.array([i[1][1],i[1][0]],int)) # converting all the poitns to int
                cv2.circle(appendedimage, (x1,y1), 3, (0,0,255), -1) # circling small red dots on the keypoints for easy visiblity
                cv2.circle(appendedimage, (x2+w,y2), 3, (0,0,255), -1) # circling small red dots on image2
                cv2.line(appendedimage, (x1, y1),(x2+w, y2), (0,255,0), 1) # drawing lines between the matching points
                while 1:
                        if cv2.waitKey(5)==27:   # upon "esc" keypress the next match is drawn
                                continue;
                        else:
                                cv2.imshow("",appendedimage) # shows the matched appended image
                                key = cv2.waitKey(0)
                                if key == 115:
                                        cv2.imwrite("appended_matched_image.jpg", appendedimage)   # writes the appended matched image onto disk
                                        break
                                if key == 27:
                                        break
        cv2.imwrite("appended_matched_image.jpg", appendedimage)
#****************************************************************************
