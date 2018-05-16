# *****************************************************************************
# * Filename : feature_detection_and_matching_OpenCV.py
# * Date : 15/May/2017
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : Implements sift using OpenCV
# * Detailed Description :
# *****************************************************************************

import cv2
import numpy as np
import sys

if __name__ == "__main__":
    img_path = sys.argv
    img1 = cv2.imread(img_path[1])
    gray_img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
    img2 = cv2.imread(img_path[2])
    gray_img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)

    sift = cv2.SIFT()
    kp1, des1 = sift.detectAndCompute(gray_img1, None)
    kp2, des2 = sift.detectAndCompute(gray_img2, None)

    FLANN_INDEX_KDTREE = 0
    index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
    search_params = dict(checks = 50)

    flann = cv2.FlannBasedMatcher(index_params, search_params)
    matches = flann.knnMatch(des1, des2, k=2)

    good = []
    for m, n in matches:
        if m.distance < 0.7*n.distance:
            good.append(m)
    
    if len(good) > 10:
        src_pts = np.float32([kp1[m.queryIdx].pt for m in good]).reshape(-1, 1, 2)
        dst_pts = np.float32([kp2[m.trainIdx].pt for m in good]).reshape(-1, 1, 2)

    img1_width = int(img1.shape[1])
    print img1_width
    appended_image = np.concatenate((img1, img2), axis=1)

    for i in range(0, len(src_pts)):
        img1_pos = (int(src_pts[i][0][0]), int(src_pts[i][0][1]))
        img2_pos = (int(dst_pts[i][0][0])+img1_width, int(dst_pts[i][0][1]))
        cv2.circle(appended_image, img1_pos, 3, (0, 0, 255), -1)
        cv2.circle(appended_image, img2_pos, 3, (0, 0, 255), -1)
        cv2.line(appended_image, img1_pos, img2_pos, (0, 255, 0), 1)

    cv2.imshow("", appended_image)
    cv2.imwrite("appended_matched_image.jpg", appended_image)
    cv2.waitKey()
