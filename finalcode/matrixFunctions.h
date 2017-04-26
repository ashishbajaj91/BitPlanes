#ifndef MATRIXFUNCTIONS_IS_INCLUDED
#define MATRIXFUNCTIONS_IS_INCLUDED

#include <Eigen/Dense>

void ComputeSVD(double *s, Eigen::Matrix3d H)
{
	Eigen::JacobiSVD<Eigen::Matrix3d> svd(H);
	auto sing = svd.singularValues();
	s[0] = sing[0];
	s[1] = sing[1];
	s[2] = sing[2];
}

#endif