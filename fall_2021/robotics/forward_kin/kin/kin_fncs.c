#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415927
#endif

/*
Gabriel de Sa
1001676832
*/

/*
  \brief printMatrix

  \param 4x4 matrix a

  Prints the matrix by row and column usinng printf.

  \return none
*/
void printMatrix(double a[4][4]){
  int i = 0, k = 0;
  for(i = 0; i < 4; i++)
  {
    for(k = 0; k < 4; k++)
    {
      printf("%f ", a[i][k]);
    }
    printf("\n");
  }
}

/*
  \brief matrixMultiply

  \param 4x4 matrix a
  \param 4x4 matrix b.

  Multiples a x b into a 4x4 matrix and sets b to equal the resulting matrix.

  \return none
*/
matrixMultiply(a, b)
double a[4][4];
double b[4][4];
{
  int i = 0, k = 0, j = 0;
  double c[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
  double sum=0;
  for(i = 0; i < 4; i++)
  {
    for(k = 0; k < 4; k++)
    {
      sum = 0;
      for(j = 0; j < 4; j++)
      {
        sum += a[i][j] * b[j][k];
      }
      c[i][k] = sum;
    }
  }
  //copy matrix
  for(i = 0; i < 4; i++)
  {
    for(k = 0; k < 4; k++)
    {
      b[i][k] = c[i][k];
    }
  }
}

/*
  \brief generateRMatrix

  \param 4x4 matrix to be altered
  \param x, y, or z inputted as 0, 1, or 2
  \param the angle, theta.

  Alters input matrix to be a 4x4 rotation matrix which can be used for robotic kinematics

  \return none
*/
generateRMatrix(input, option, theta)
double input[4][4];
int option;
double theta;
{
  int i = 0, j = 0;
  double x[4][4] =
    {{1, 0, 0, 0},
    {0, cos(theta), -sin(theta), 0},
    {0, sin(theta), cos(theta), 0},
    {0, 0, 0, 1}};
  double y[4][4] = {
    {cos(theta), 0, sin(theta), 0},
    {0, 1, 0, 0},
    {-sin(theta), 0, cos(theta), 0},
    {0, 0, 0, 1}
  };
  double z[4][4] = {
    {cos(theta), -sin(theta), 0, 0},
    {sin(theta), cos(theta), 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  };
  switch (option) {
    case 0:
      for(i = 0; i < 4; i ++)
      {
        for(j = 0; j < 4; j++)
        {
          input[i][j] = x[i][j];
        }
      }
      break;

    case 1:
      for(i = 0; i < 4; i ++)
      {
        for(j = 0; j < 4; j++)
        {
          input[i][j] = y[i][j];
        }
      }
      break;

    case 2:
      for(i = 0; i < 4; i ++)
      {
        for(j = 0; j < 4; j++)
        {
          input[i][j] = z[i][j];
        }
      }
      break;
  }
}

fwd_kin(theta, x)
double *theta;
double x[3];
{
  double l[4] = {0.25, 0.2, 0.2, 0.15}; // Init an array containing all the lengths
  double d[3] = {-.04, 0.04, -0.04}; // Init an array containing displacements
  double dy[4][4] = { // init an array for displacement of y's, init set to d_y(d_3)
      {1, 0, 0, 0},
      {0, 1, 0, d[2]},
      {0, 0, 1, d[2]},
      {0, 0, 0, 1}
    };
    double d_l[4][4] = { // Init array for d(l), init set to d_x(l_3)
        {1, 0, 0, l[3]},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
      };
  // Init an array for rotations, and the final array where all multiplication will be done.
  double r[4][4], temp[4][4];
  double final[4][4] = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
  generateRMatrix(r, 1, theta[3]); // Ry(theta_3)
  int i = 0, k = 0, j = 0;
  for(i = 0; i < 4; i++) // Num of T transforms D_E_T x C_D_T x C_A_T x B_A_T
  {
    switch(i)
    {
      case 0:
        break;
      case 1:
        dy[1][3] = d[1]; // set new displacement
        dy[2][3] = 0; // set z axis displacement to zero
        d_l[0][3] = l[2]; // D_x(l2)
        generateRMatrix(r, 1, theta[2]); // R_y(theta_2)
        break;
      case 2:
        d_l[0][3] = l[1]; // D_x(l1)
        dy[1][3] = d[0];
        generateRMatrix(r, 1, theta[1]); // R_y(theta_1)
        break;
      case 3:
        dy[1][3] = 0;
        d_l[0][3] = 0; // remove previous x
        d_l[2][3] = l[0]; // d_z[l_0]
        generateRMatrix(r, 2, theta[0]); // R_z(theta_0)
        break;
    }
    matrixMultiply(dy, final); // inward multiplication of matricies into final matrix
    matrixMultiply(d_l, final);
    matrixMultiply(r, final);
  }
  x[0] = final[0][3];
  x[1] = final[1][3];
  x[2] = final[2][3];
}


inv_kin(x, theta)
double *x;
double theta[6];
{
  //Find theta 0
  theta[0] = atan2(x[1], x[0]) - asin(-.04/(sqrt(pow(x[0], 2) + pow(x[1], 2))));
  // Find prime axis positions assuming l3 is pointing at 90 degrees down
  double x_p = x[0];     // x'
  double y_c = sqrt(pow(x[0], 2) + pow(x[1], 2));
  double y_x = sqrt(pow(y_c, 2) - pow(-.04, 2));
  double z = (x[2] + .15)-.25; // y'
  x_p = y_x + .04; // x"

  //Find theta 2 with new frame and it can only be positive
  theta[2] = acos(((pow(x_p, 2)+pow(z, 2))-pow(0.2, 2)-pow(0.2, 2))/(2*0.2*0.2));

  //Find Theta 1 which can only be negative given restriction of robot.
  double y = atan2(z, x_p);
  double a = acos((pow(.2, 2) + pow(x_p, 2) + pow(z, 2) - pow(.2, 2))/(2*.2*(sqrt(pow(x_p, 2)+pow(z, 2)))));
  theta[1] = (y+a)*-1;

  //Finding theta 3 orientation is given as π/2
  theta[3] = (M_PI/2) - theta[1] - theta[2]; //Theta 3
  theta[4] = 0; //Theta 4 Does not have a net effect on position and can be left as 0
  theta[5] = 0; //Theta 5 Does not have a net effect on position and can be left as 0

}
