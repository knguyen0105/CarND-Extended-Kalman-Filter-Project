#include <iostream>
#include "tools.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;



Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  /**
  TODO:
    * Calculate the RMSE here.
  */
  VectorXd rmse(4); 
  rmse << 0,0,0,0;

  if (estimations.size() != ground_truth.size() 
        || estimations.size() == 0) 
  {
    std::cout << "invalid estimation or ground truth data" << endl; 
    return rmse;          
  }

  for (int i=0; i < estimations.size(); i++) 
  {
    rmse += (estimations[i] - ground_truth[i]).cwiseProduct(estimations[i] - ground_truth[i]);
  }

  rmse = rmse / estimations.size();

  return rmse.array().sqrt();
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
  /**
  TODO:
    * Calculate a Jacobian here.
  */

  MatrixXd H(3,4);
  double px = x_state(0);
  double py = x_state(1);
  double vx = x_state(2);
  double vy = x_state(3);

  double c1 = px*px + py*py;
  double c2 = std::sqrt(c1);
  double c3 = c1 * c2;

  if (std::fabs(c1) < 0.0001) {
    std::cout << "CalculateJacobian () -Error Division by Zero" << endl;
    return H;
  }

  H << px/c2, py/c2, 0, 0,
       -py/c1, px/c1, 0, 0,
       py*(vx*py - vy*px)/c3, px*(vy*px - vx*py)/c3, px/c2, py/c2;
  
  return H;
}
