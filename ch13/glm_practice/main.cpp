/*****************************************************************************
 * Author : Ram
 * Date : 2/August/2018
 * Email : ramkalath@gmail.com
 * Breif Description : glm basics
 * Detailed Description : Introduction to glm basics
 *****************************************************************************/
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main()
{
	// let us create a vector of 3 elements --------------------------------------
	glm::vec3 a(1.0f, 2.0f, 3.0f);
	std::cout << a[0] << "\t" << a[1] << "\t" << a[2] << std::endl;
	// ---------------------------------------------------------------------------

	// let us create a 4x4 array of matrices -------------------------------------
	glm::mat4 my_first_matrix(0, 1, 2, 3,
								 4, 5, 6, 7,
								 8, 9, 10, 11,
								 12, 13, 14, 15);
	// ---------------------------------------------------------------------------

	// access individual elements of the matrix ----------------------------------
	std::cout << "printing each element of the matrix ->" << std::endl;
	std::cout << my_first_matrix[0][0] << std::endl;
	// ---------------------------------------------------------------------------

	// now if you want to assign a array of floats to a glm matrix ---------------
	float my_second_matrix_data[16] = {0, 1, 2, 3,
								 4, 5, 6, 7,
								 8, 9, 10, 11,
								 12, 13, 14, 15};
	glm::mat4 my_second_matrix = glm::make_mat4(my_second_matrix_data);
	// ---------------------------------------------------------------------------

	// Multiply matrices ---------------------------------------------------------
	float my_third_matrix_data[16] = {15, 14, 13, 12,
									   11, 10, 9, 8,
									   7, 6, 5, 4,
									   3, 2, 1, 0};
	glm::mat4 my_third_matrix = glm::make_mat4(my_third_matrix_data);
	glm::mat4 multiplied_matrix = my_second_matrix * my_third_matrix;
	// ---------------------------------------------------------------------------

	// print the multiplied matrix -----------------------------------------------
	std::cout << "printing the multiplied matrix ->" << std::endl;
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			std::cout << multiplied_matrix[i][j] << std::endl;
	// ---------------------------------------------------------------------------
	return 0;
}
