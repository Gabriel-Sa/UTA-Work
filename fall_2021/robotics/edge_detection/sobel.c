#include <X11/Xlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM 512

/******************************************************************/
/* Finds a 3x3 matrix around a given i, j , and returns a         */
/* dynamically allocated array.                                   */
/* a - The original matrix                                        */
/* i, j - the central location of the matrix                      */
/* ret -- the matrix to send the results into                     */
/******************************************************************/
void find3Array(a, i, j, ret) unsigned char a[DIM][DIM];
int i;
int j;
int ret[3][3];
{
  // find the array with the row 3x3 grid around (i, j)
  int row = 0, col = 0;
  for (int k = i - 1; k <= i + 1; k++) {
    col = 0;
    for (int l = j - 1; l <= j + 1; l++) {
      ret[row][col] = a[k][l];
      col++;
    }
    row++;
  }
}

/******************************************************************/
/* Multiplies two square matrices together, and returns a         */
/* dynamically allocated array.                                   */
/* size - The size of the matrix                                  */
/* a, b - matricies to be multiplied.                             */
/******************************************************************/
int getSobelValue(int a[3][3], int b[3][3]) {
  int i = 0, k = 0, sum = 0;
  for (i = 0; i < 3; i++) {
    for (k = 0; k < 3; k++) {
      sum += (a[i][k] * b[i][k]);
    }
  }
  return sum;
}

/******************************************************************/
/* This structure contains the coordinates of a box drawn with    */
/* the left mouse button on the image window.                     */
/* roi.x , roi.y  - left upper corner's coordinates               */
/* roi.width , roi.height - width and height of the box           */
/******************************************************************/
extern XRectangle roi;

/******************************************************************/
/* Main processing routine. This is called upon pressing the      */
/* Process button of the interface.                               */
/* image  - the original greyscale image                          */
/* size   - the actual size of the image                          */
/* proc_image - the image representation resulting from the       */
/*              processing. This will be displayed upon return    */
/*              from this function.                               */
/******************************************************************/
void process_image(image, size, proc_img) unsigned char image[DIM][DIM];
int size[2];
unsigned char proc_img[DIM][DIM];
{
  // Sobel Templates
  int vt[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  int ht[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
  // Run over vertical
  // for (int i = 1; i <= DIM - 2; i++) {
  //   for (int j = 1; j <= DIM - 2; j++) {
  //     int subArray[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
  //     find3Array(image, i, j, subArray);
  //     proc_img[i][j] = abs(getSobelValue(subArray, vt));
  //   }
  // }
  // Run over horizontal
  for (int i = 1; i < DIM - 2; i++) {
    for (int j = 1; j < DIM - 2; j++) {
      int subArray[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
      find3Array(image, i, j, subArray);
      proc_img[i][j] = abs(getSobelValue(subArray, ht));
    }
  }

  // Normalize by finding max value.
  int max = 0;
  for (int i = 0; i <= DIM; i++) {
    for (int k = 0; k <= DIM; k++) {
      if (proc_img[i][k] > max) {
        max = proc_img[i][k];
      }
    }
  }
  float normal = 255 / max;
  for (int i = 0; i <= DIM; i++) {
    for (int k = 0; k <= DIM; k++) {
      proc_img[i][k] = round(proc_img[i][k] * normal);
    }
  }
}
