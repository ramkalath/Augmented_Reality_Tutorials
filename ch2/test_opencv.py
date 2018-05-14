# *****************************************************************************
# * Filename : test_opencv.py
# * Date : 14/May/2018
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : Tests if opencv works properly using python
# * Detailed Description : to run the code type "python test_opencv.py"
# *****************************************************************************

import cv2
import numpy as np

image = cv2.imread("test.jpg")
cv2.imshow("my first opencv window", image)
cv2.waitKey()
