# *****************************************************************************
# * Filename : feature_detection_and_matching_OpenCV.py
# * Date : 15/May/2017
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : Implements sift feature detection and matching using OpenCV
# * Detailed Description : To run this code type "python feature_detection_and_matching_OpenCV.py img1.bmp img2.bmp"
# *****************************************************************************

import cv2
import numpy as np
import sys

def append_images(img1, img2):
        h1 = len(img1)
        w1 = len(img1[0])
        h2 = len(img2)
        w2 = len(img2[0])
        appendedimage = np.zeros((max(h1,h2), w1+w2 ,3), np.uint8) # creates a template image the size of both the images appended together
        appendedimage[:h1, :w1] = img1  # inserting image1 into the template image
        appendedimage[:h2, w1:w1+w2] = img2  # inserting image2 into the template image
        return appendedimage

if __name__ == "__main__":
    img_path = sys.argv
    img1 = cv2.imread(img_path[1])          # read the first image
    gray_img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)      # convert the first image to grey
    img2 = cv2.imread(img_path[2])          # read the second image
    gray_img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)      # convert the second image to grey

    sift = cv2.SIFT()                       # create a sift object
    kp1, des1 = sift.detectAndCompute(gray_img1, None)      # compute the keypoints and descriptors for the first image 
    kp2, des2 = sift.detectAndCompute(gray_img2, None)      # compute the keypoints and descriptors for the second image

    # -------------------------------------------------------------------
    # feature matching: we will be using the flann based matcher doing a k nearest neighbour match
    FLANN_INDEX_KDTREE = 0
    index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
    search_params = dict(checks = 50) # above three lines sets the parameters for match

    flann = cv2.FlannBasedMatcher(index_params, search_params)  # creating a flann match object
    matches = flann.knnMatch(des1, des2, k=2)                   # here is where the actual matches happen between the descriptors

    good = []       # collective list of all the good matches
    for m, n in matches:    
        if m.distance < 0.7*n.distance:     # setting a treshold for distance between matches
            good.append(m)                  # if the matches are good the appended to the good list
    
    if len(good) > 10:
        src_pts = np.float32([kp1[m.queryIdx].pt for m in good]).reshape(-1, 1, 2) # retrive the actual source match points
        dst_pts = np.float32([kp2[m.trainIdx].pt for m in good]).reshape(-1, 1, 2) # retriving the destination match points
    # -------------------------------------------------------------------

    img1_width = int(img1.shape[1])    # we are going to append the images so width of the first image will be useful when, in the appended image, we draw points
    # appended_image = np.concatenate((img1, img2), axis=1)   # appending images. This wont work for images with discrepant number of rows, I think. But test images are of the same size so i took the easy way out. Hop over to the other code (feature_detection_and_matching.py) for a proper code snippet to append two diffently shaped images.
    appended_image = append_images(img1, img2)

    for i in range(0, len(src_pts)):
        img1_pos = (int(src_pts[i][0][0]), int(src_pts[i][0][1]))   # converting src_pts points to int, tuple
        img2_pos = (int(dst_pts[i][0][0])+img1_width, int(dst_pts[i][0][1]))    # converting dst_pts to int, tuple
        cv2.circle(appended_image, img1_pos, 3, (0, 0, 255), -1)    # draw a small circle to indicate a red dot
        cv2.circle(appended_image, img2_pos, 3, (0, 0, 255), -1)    # draw a small circle to indicate a red dot
        cv2.line(appended_image, img1_pos, img2_pos, (0, 255, 0), 1)    # draw a line between the dots to obtain matches

    cv2.imshow("", appended_image)
    cv2.imwrite("appended_matched_image.jpg", appended_image)
    cv2.waitKey()
