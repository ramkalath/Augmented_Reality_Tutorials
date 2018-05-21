# *****************************************************************************
# * Filename : DLT.py
# * Date : 19/May/2018
# * Author : Ram
# * Email : ramkalath@gmail.com
# * Breif Description : Implements Direct Linear Transform.
# * Detailed Description : Checkout the image file before you go through this code. To run this code type "python DLT.py"
# *****************************************************************************

# see homography_transformation.jpg before you read this code

import numpy as np
from sympy import Matrix

if __name__ == "__main__":
    npoints = 4     # since we have only 4 points
    ip = np.array([[0, 0], [5, 0], [5, 7], [0, 7]])     # initial points before transformation
    fp = np.array([[1, 1], [5, 2], [5, 7], [1, 8]])     # final points after transformation

    A = []
    
    # for each point append two rows of A
    for pointi in range(0, len(ip)):
        A.append([ip[pointi][0], ip[pointi][1], 1, 0, 0, 0, -fp[pointi][0]*ip[pointi][0], -fp[pointi][0]*ip[pointi][1], -fp[pointi][0]])
        A.append([0, 0, 0, ip[pointi][0], ip[pointi][1], 1, -fp[pointi][1]*ip[pointi][0], -fp[pointi][1]*ip[pointi][1], -fp[pointi][1]])

    A = Matrix(A)   # creating a sympy matrix
    h = np.array(A.nullspace())   # calculating the homography matrix
    h = h.reshape(3, 3)     # reshaping h to form a 3x3 matrix
    print h
