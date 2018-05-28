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

def append_images(img1, img2):
	h1 = len(img1)
	w1 = len(img1[0])
	h2 = len(img2)
	w2 = len(img2[0])
	appendedimage = np.zeros((max(h1,h2), w1+w2 ,3), np.uint8)
	appendedimage[:h1, :w1] = img1
	appendedimage[:h2, w1:w1+w2] = img2
	return appendedimage

def match_and_display(good_old, good_new, img1, img2):
    appendedimage = append_images(img1, img2)
    w = img1.shape[1]
    for i in range(0, len(good_old)):
        (x1, y1) = tuple(np.array(good_old[i][0], int))
        (x2, y2) = tuple(np.array(good_new[i][0], int))
        cv2.circle(appendedimage, (x1,y1), 3, (0,0,255), -1) # circling small red dots on the keypoints for easy visiblity
        cv2.circle(appendedimage, (x2+w,y2), 3, (0,0,255), -1) # circling small red dots on image2
        cv2.line(appendedimage, (x1, y1),(x2+w, y2), (0,255,0), 1) # drawing lines between the matching points
        cv2.imshow("", appendedimage)
        cv2.waitKey(10)
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
        # print tuple(good_old[i][0])
        


