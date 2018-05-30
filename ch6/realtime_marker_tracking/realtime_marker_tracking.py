# *****************************************************************************
# * Filename : realtime_marker_tracking.py
# * Date : 27/May/2018
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : realtime marker detection using sift and lk
# * Detailed Description : to run this code type ./run.sh
# *****************************************************************************

import cv2
import numpy as np
import sys
import time

# params for ShiTomasi corner detection
feature_params = dict(maxCorners = 100,
                      qualityLevel = 0.3,
                      minDistance = 7,
                      blockSize = 7)

# Parameters for lucas kanade optical flow
lk_params = dict(winSize  = (15,15),
                 maxLevel = 2,
                 criteria = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03))

# Displays the frame around the tracked image given the points to be drawn
def display(good_new, img2):
    (x0, y0) = tuple(np.array(good_new[0][0], int))
    (x1, y1) = tuple(np.array(good_new[1][0], int))
    (x2, y2) = tuple(np.array(good_new[2][0], int))
    (x3, y3) = tuple(np.array(good_new[3][0], int))

    cv2.circle(img2, (x0,y0), 3, (0,0,255), -1) # circling small red dots on the keypoints for easy visiblity
    cv2.circle(img2, (x1,y1), 3, (0,0,255), -1) # circling small red dots on the keypoints for easy visiblity
    cv2.circle(img2, (x2,y2), 3, (0,0,255), -1) # circling small red dots on the keypoints for easy visiblity
    cv2.circle(img2, (x3,y3), 3, (0,0,255), -1) # circling small red dots on the keypoints for easy visiblity
    cv2.line(img2, (x0, y0),(x1, y1), (255,0,0), 3) # drawing lines between the matching points
    cv2.line(img2, (x1, y1),(x2, y2), (255,0,0), 3) # drawing lines between the matching points
    cv2.line(img2, (x2, y2),(x3, y3), (255,0,0), 3) # drawing lines between the matching points
    cv2.line(img2, (x3, y3),(x0, y0), (255,0,0), 3) # drawing lines between the matching points
    return img2

if __name__ == "__main__":
    img_path = sys.argv
    marker_img = cv2.imread(img_path[1])
    gray_marker_img = cv2.cvtColor(marker_img, cv2.COLOR_BGR2GRAY)

    print "keep the marker picture still in front of the camera for a short while"
    time.sleep(5)

    # capture from the webcam the first frame to compute the homography using SIFT. This will take some time ---------------
    cap = cv2.VideoCapture(0)
    k=0
    while k<80:
        flag, first = cap.read()
        k+=1

    gray_first = cv2.cvtColor(first, cv2.COLOR_BGR2GRAY)
    print "first frame from the webcam captured"

    # lets compute sift and perform feature matching between both the images ------------------------------------------
    print "computing sift between both the images"
    sift = cv2.SIFT()
    kp_marker, des_marker = sift.detectAndCompute(gray_marker_img, None)
    kp_first, des_first = sift.detectAndCompute(gray_first, None)
    print "sift computed"

    print "now matching"
    FLANN_INDEX_KDTREE = 0
    index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
    search_params = dict(checks = 50)
    flann = cv2.FlannBasedMatcher(index_params, search_params)
    matches = flann.knnMatch(des_marker, des_first, k=2)
    good = []
    for m, n in matches:    
        if m.distance < 0.7*n.distance:
            good.append(m)

    # calculating homography ---------------------------------------------------------------------------------------------
    if len(good) > 10:
        print "calculating homography"
        src_pts = np.float32([kp_marker[m.queryIdx].pt for m in good]).reshape(-1, 1, 2)
        dst_pts = np.float32([kp_first[m.trainIdx].pt for m in good]).reshape(-1, 1, 2)
        h, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)

    else:
        print "could not find a matching marker"
        exit()
    # ---------------------------------------------------------------------------------------------------------------------
    # These are the points to be tracked to draw the frame around the tracked marker. Notice we are defining them for the marker image
    points_on_marker_image = np.array([[0, 0, 1], [marker_img.shape[0], 0, 1], [marker_img.shape[0], marker_img.shape[1], 1], [0, marker_img.shape[1], 1]])

    # p1 is the array of points to be tracked on the first image ----------------------------------------------------------
    p1 = dst_pts

    while True:
        _, subsequent = cap.read() # marker and first image got computed earlier on, now the subsequent images
        gray_subsequent = cv2.cvtColor(subsequent, cv2.COLOR_BGR2GRAY)

        # tracking between first and subsequent frames using LK optical flow ------------------------------------------------
        ps, st, err = cv2.calcOpticalFlowPyrLK(gray_first, gray_subsequent, p1, None, **lk_params)

        # lets compute homography between two images
        h_new, mask = cv2.findHomography(p1, ps, cv2.RANSAC, 5.0)
        h = np.dot(h, h_new) # compounded homography between images

        # lets track the frame over the subsequent points by multiplying the tracked marker with the compunded homography matrix
        points_on_webcam_image = np.dot(h, points_on_marker_image.T).T

        # to draw we need to reshape the points on the webcam image shape
        points_on_webcam_image_reshaped = np.array([[[points_on_webcam_image[0][0]/points_on_webcam_image[0][2], points_on_webcam_image[0][1]/points_on_webcam_image[0][2]]], [[points_on_webcam_image[1][0]/points_on_webcam_image[1][2], points_on_webcam_image[1][1]/points_on_webcam_image[1][2]]], [[points_on_webcam_image[2][0]/points_on_webcam_image[2][2], points_on_webcam_image[2][1]/points_on_webcam_image[2][2]]], [[points_on_webcam_image[3][0]/points_on_webcam_image[3][2], points_on_webcam_image[3][1]/points_on_webcam_image[3][2]]]]) # de-homogenized and reshaped

        # the display function in display.py draws the borders around the images
        subsequent = display(points_on_webcam_image_reshaped, subsequent)

        cv2.imshow("", subsequent)
        key = cv2.waitKey(20)
        if(key == 27):
            break
        if(key == 115):
            cv2.imwrite("tracked_image.jpg", subsequent)
        p1 = ps
        gray_first = gray_subsequent

cv2.destroyAllWindows()
cap.release()
