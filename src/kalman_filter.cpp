#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

const float DoublePI = 2 * M_PI;

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */
  x_ = F_ * x_; 
  MatrixXd Ft = F_.transpose(); 
  P_ = F_ * P_ * Ft + Q_;
}

void KalmanFilter::KF(const VectorXd &y){
  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd K =  P_ * Ht * Si;
  // New state
  x_ = x_ + (K * y);
  int x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations (Laser)
  */
	VectorXd z_pred = H_ * x_;
  VectorXd y = z - z_pred;
  
  KF(y);
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations (Radar)
  */
  double rho = sqrt(x_(0)*x_(0) + x_(1)*x_(1));
  if(rho < 0.000001)  rho = 0.000001;  
  double theta = atan2(x_(1) , x_(0));
  double rho_dot = (x_(0)*x_(2) + x_(1)*x_(3)) / rho;
  
  VectorXd h = VectorXd(3); // h(x_)
  h << rho, theta, rho_dot;
  
  VectorXd y = z - h;
  //normalize 
  while(y(1) > M_PI){
    y(1) -= DoublePI;
  }
  while(y(1) < -M_PI){
    y(1) += DoublePI;
  }

	KF(y);
}
