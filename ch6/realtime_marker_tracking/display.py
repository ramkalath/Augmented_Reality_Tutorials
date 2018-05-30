# *****************************************************************************
# * Filename : <file_name>
# * Date : <date>
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description :
# * Detailed Description :
# *****************************************************************************

import numpy as np
import cv2

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

