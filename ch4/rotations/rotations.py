# *****************************************************************************
# * Filename : rotations.py
# * Date : 17/May/2018
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : Implements rotation of an image
# * Detailed Description :
# *****************************************************************************

import cv2
import numpy as np
np.set_printoptions(threshold=np.nan)
import math

if __name__ == "__main__":
    frame = cv2.imread("image.jpg") # reading the image
    height = len(frame)  # the height of the image
    width = len(frame[0])  # the width of the image

    # the angle of rotation
    theta = 45

    # this is the rotation matrix
    rot_mat = np.array([[math.cos(np.radians(theta)), -math.sin(np.radians(theta))],
                        [math.sin(np.radians(theta)), math.cos(np.radians(theta))]])

    # print rot_mat

    # now our next task is to create an index list for all the pixels of the image. For instance for a 5x4 image, the index list will be [[[0, 0], [0, 1], [0, 2], [0, 3]],
    #                                [1, 0], [1, 1], [1, 2], [1, 3]],
    #                                [2, 0], [2, 1], [2, 2], [2, 3]],
    #                                [3, 0], [3, 1], [3, 2], [3, 3]],
    #                                [4, 0], [4, 1], [4, 2], [4, 3]]]

    # the idea is to create two strings of numbers
    # [0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3]  -> row1
    # [0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4]  -> row0
    # then stack them vertically to produce [0, 0], [0, 1], [0, 2] ........ [4, 3]. This gives a string of indices of the original image appended one after the other.
    row1 = np.tile(np.arange(width), height)
    row0 = np.repeat(np.arange(width), width)[:len(row1)]
    indices_pre_rotation = np.vstack((row0, row1)).T

    # now multiply the indices with the rotation matrix
    indices_post_rotation = np.dot(rot_mat, indices_pre_rotation.T)
    # some values might become floats during rotation. Lets round them off to obtain int index values
    indices_post_rotation = np.around(indices_post_rotation.T)

    # during rotaion around the z axis on the xy plane, the index values of the image might become negative. The extend of which can be found by finding the range along the x and y axis
    min_value_cols = min(indices_post_rotation.T[0])
    max_value_cols = max(indices_post_rotation.T[0])
    min_value_rows = min(indices_post_rotation.T[1])
    max_value_rows = max(indices_post_rotation.T[1])

    # print indices_post_rotation
    # lets create a new template image using the above range. Note after rotation the image is obviously bigger.
    new_image = np.zeros((int(max_value_rows-min_value_rows+1), int(max_value_cols-min_value_cols+1), 3), np.uint8)

    # print new_image.shape

    # now we just translate the indices to obtain the final index positions. 
    indices_post_rotation.T[0] = indices_post_rotation.T[0] - min_value_cols

    # reshaping the original image into a long row of pixels by appending each row of pixels one behind the other
    frame = frame.reshape(width*height, 1, 3)
    k = 0
    # we assign each row of indices of the old image into each row of the new image
    for i in indices_post_rotation:
        new_image[int(i[0])][int(i[1])] = frame[k][0]
        k+=1

    cv2.imshow("rotated image", new_image)
    cv2.imwrite("rotated_image.jpg", new_image)
    cv2.waitKey()

    # observe the holes in the new image
