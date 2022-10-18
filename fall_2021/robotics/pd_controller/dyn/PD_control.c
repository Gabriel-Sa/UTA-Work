#include <stdio.h>
#include <math.h>


double PD_control(theta, theta_dot, theta_ref, theta_dot_ref)
double theta, theta_dot, theta_ref, theta_dot_ref;
{
  // t = I(theta_dot_ref) + B(theta_dot) + G(theta)
  // I is inertia of the arm, b is the viscous friction in the join, and G is gravity.
  double g_constant = 1.175997;
  double b = .1;
  static double calls = 0;
  calls++;
  double f = 5000;
  double force = f + (g_constant * sin(theta)) + (b*theta_dot);
  static double prevVelocity = 0;
  static double prevDelta = 0;
  double delta = theta_dot - prevVelocity;
  static double deltaAdd = 0;
  deltaAdd+=delta;
  if(calls == 500){
    printf("Delta = %f\n", delta);
    printf("I = %f\n", f/deltaAdd);
    deltaAdd = 0;
    calls = 0;
  }
  prevVelocity = theta_dot;
  prevDelta = delta;
  return(force);
}
