/**
 * @file linalg.c
 * @author FeltonCD20@gcc.edu, AllarassemJJ20@gcc.edu
 * @brief This library implements functions that create and delete Matrices,
 *        and also for performing certain operations on them.
 * @date 2022-02-25
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <errno.h>

#include "linalg.h"

/**
 * @brief Allocate memory and initialize a new matrix of requested size
 * 
 * @param nrows the number of rows in the new matrix
 * @param ncols the number of columns in the new matrix
 * @return Matrix* a pointer to the newly created matrix
 */
Matrix* new_Matrix(size_t nrows, size_t ncols) {
    //Create and return the matrix.
    Matrix* matrix = (Matrix*) malloc(sizeof(Matrix)); /* The matrix to return. */
    init_Matrix(matrix, nrows, ncols);
    return matrix;
}

/**
 * @brief Initialize a matrix of the specified size.
 * 
 * If either nrows or ncols is <= 0 then vals will be set to NULL
 * 
 * @param M the matrix to be initialized
 * @param nrows the number of rows in the new matrix (must be > 0)
 * @param ncols the number of columns in the new matrix (must be > 0)
 */
void init_Matrix(Matrix* M, size_t nrows, size_t ncols) {
    //If M is NULL, nothing else can be done.
    if (M == NULL) {
        return;
    }
    
    M->nrows = nrows;
    M->ncols = ncols;

    //If either of the dimensions are <= 0, vals must be NULL.
    if (nrows <= 0 || ncols <= 0) {
        M->vals = NULL;
        return;
    }

    //Create the 2D array for vals.
    M->vals = (double**) malloc(sizeof(double*) * nrows);
    for (int i = 0; i < nrows; i++) {
        M->vals[i] = (double*) calloc(ncols, sizeof(double));
    }
}

/**
 * @brief Clean up any dynamic memory allocated by init_Matrix
 * 
 * This function does nothing if M or M.values is NULL
 * 
 * @param M the matrix to be cleaned in preparation for deletion
 */
void deinit_Matrix(Matrix* M) {
    //If the matrix or its vals field are null, do nothing.
    if (M == NULL) {
        return;
    }
    if (M->vals == NULL) {
        return;
    }

    //Free the memory for every column, then free the rows.
    for (int i = 0; i < M->nrows; i++) {
        free(M->vals[i]);
    }
    free(M->vals);
    M->vals = NULL;
    M->nrows = 0;
    M->ncols = 0;
}

/**
 * @brief Frees dynamic memory and deletes a Matrix created by new_Matrix
 * 
 * @param M the matrix to be deleted safely
 */
void delete_Matrix(Matrix* M) {
    //Do nothing if M is NULL;
    if (M == NULL) {
        return;
    }

    //Free everything in the matrix, then free the matrix and set its pointer to NULL.
    deinit_Matrix(M);
    free(M);
}

/**
 * @brief Retrieves the value stored at index M[i,j]
 * 
 * @param M the Matrix to check for the indices
 * @param i the row index of the desired value
 * @param j the column index of the desired value
 * @return double the value stored in that index of the matrix or 0.0 if invalid
 *         ** if the index was invalid, errno should be set to EINVAL **
 */
double Matrix_get(Matrix* M, size_t i, size_t j) {
    //If the arguments are invalid, set errno and return 0.0.
    if (M == NULL || M->vals == NULL || i < 0 || j < 0 || i >= M->nrows || j >= M->ncols) {
        errno = EINVAL;
        return 0.0;
    }

    //Return the requested double.
    return M->vals[i][j];
}

/**
 * @brief Store val in the index M[i,j]
 * 
 * @param M the matrix to be modified with the new value
 * @param i the index of the row where val will be added
 * @param j the index of the column where val will be added
 * @param val the value to be added to the matrix
 * @return 0 if the operation was successful, otherwise 1
 */
int Matrix_put(Matrix* M, size_t i, size_t j, double val) {
    //If the arguments are invalid, return 1.
    if (M == NULL || M->vals == NULL || i < 0 || j < 0 || i >= M->nrows || j >= M->ncols) {
        return 1;
    }

    //Set the value and return 0.
    M->vals[i][j] = val;
    return 0;
}

/**
 * @brief Compute the sum of two matrices A and B i.e. A+B
 * 
 * Note that matrix addition is done entry-wise. This function will return NULL 
 * if the matrices do not have the same number of rows and columns or if either 
 * matrix has a vals array of NULL.
 * 
 * @param A The first matrix to include in the sum
 * @param B The second matrix to include in the sum
 * @return Matrix* representing A+B or NULL if the operation is invalid
 */
Matrix* Matrix_add(Matrix* A, Matrix* B) {
    Matrix* ret; /* The sum of A and B that will be returned */

    //If the operation is invalid, return NULL.
    //This is split into two if statements for readability.
    if (A == NULL || B == NULL || A->vals == NULL || B->vals == NULL) {
        return NULL;
    }
    if ( !(A->nrows == B->nrows && B->ncols == A->ncols) ) {
        return NULL;
    }
    //Assign ret to a new Matrix of the proper dimensions.
    ret  = new_Matrix(A->nrows, B->ncols);

    //implementing the addition method
    for(int i = 0; i < ret->nrows; i++){
        for(int j = 0; j < ret->ncols; j++){
            ret->vals[i][j] = A->vals[i][j] + B->vals[i][j];
        }
    }
    return ret;

}

/**
 * @brief Compute the entry-wise L1 norm of a matrix A
 * 
 * @param A the matrix for which the L1 norm should be computed
 * @return double the entry-wise L1 norm of A
 */
double Matrix_l1(Matrix* A) {
    double result = 0; /* The result to return */
    double toAdd = 0; /* Holds a single value to add */
    
    //First, check for errors, like a NULL pointer, vals being NULL, or the bounds being invalid.
    if (A == NULL || A->vals == NULL || A->nrows <= 0 || A->ncols <= 0) {
        errno = EINVAL;
        return 0;
    }
    
    //Calculate the return value.
    for (int i = 0; i < A->nrows; i++) {
        for (int j = 0; j < A->ncols; j++) {
            //Get the absolute value of the next number, then add it to result.
            toAdd = A->vals[i][j];
            if (toAdd < 0) {
                toAdd *= -1;
            }
            result += toAdd;
        }
    }

    return result;
}

/**
 * @brief Compute the entry-wise L2 norm of a matrix A
 * 
 * @param A the matrix for which the L2 norm should be computed
 * @return double the entry-wise L2 norm of A
 */
double Matrix_l2(Matrix* A) {
    double ret = 0; /* Holds the result of this operation to return. */
    if(A == NULL){
        return 0;
    }

    //loop through and add the square of the value
    for(int i = 0; i < A->nrows; i++){
        for(int j = 0; j < A->ncols; j++){
            ret+=pow(A->vals[i][j],2);
        }
    }
    return sqrt(ret);

}

/**
 * @brief Computes the product AB of the two matrices.
 * 
 * This operation is only defined if A.nrows == B.ncols && A.ncols == B.nrows.
 * If either does not hold or A.vals is NULL or B.vals is NULL, then the return 
 * value of this function will also be NULL.
 * 
 * @param A the matrix on the left hand side of the product
 * @param B the matrix on the right hand side of the product
 * @return Matrix* representing AB or NULL if the operation is invalid
 */
Matrix* Matrix_mult(Matrix* A, Matrix* B) {
    Matrix* ret; /* The product of A and B that will be returned */
    
    //If the operation is invalid, return NULL.
    //This is split into two if statements for readability.
    if (A == NULL || B == NULL || A->vals == NULL || B->vals == NULL) {
        return NULL;
    }
    if ( !(A->nrows == B->ncols && A->ncols == B->nrows) ) {
        return NULL;
    }

    //Assign ret to a new Matrix of the proper dimensions.
    ret = new_Matrix(A->nrows, B->ncols);

    //Do the multiplication of the Matrices.
    for (int i = 0; i < ret->nrows; i++) {
        for (int j = 0; j < ret->ncols; j++) {
            //i and j keep track of the current slot of ret, as well as the current
            //row in A and the current column in B. Each value in the current row of A
            //must be multiplied with the corresponding value in the current column of B.
            //The product of each must be added to the current slot in ret.
            for (int k = 0; k < A->ncols; k++) {
                ret->vals[i][j] += A->vals[i][k] * B->vals[k][j];
            }
        }
    }

    //Now return the result.
    return ret;
}