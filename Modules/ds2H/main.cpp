#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <math.h>

typedef float imgtype ;

typedef Eigen::MatrixXf matrixtype ;

std::vector<matrixtype> ds2Hs(imgtype * ds, imgtype *wts )
{
	std::vector<matrixtype> Hs;
	for( int i =0 ; i < 8; i++)
	{
		Hs.push_back(matrixtype::Identity(3,3));
	}
	(Hs[0])(1,2) = wts[0]*ds[0];	// 1 x translation
	(Hs[1])(0,2) = wts[1]*ds[1];	// 2 y translation
	(Hs[2])(0,0) = pow(wts[2],ds[2]);	// 3 scale
	(Hs[2])(1,1) = pow(wts[2],ds[2]); 	// 3 scale
	(Hs[3])(1,0) = wts[3]*ds[3];	// 4 shear
	(Hs[4])(0,0) = pow(wts[4],ds[4]);	// 5 scale non-uniform
	imgtype ct = cos(wts[5]*ds[5]);
	imgtype st = sin(wts[5]*ds[5]);

	(Hs[5])(0,0) = ct;
	(Hs[5])(1,1) = ct;
	(Hs[5])(0,1) = -st;
	(Hs[5])(1,0) = st ;

	ct = cos(wts[6]*ds[6]);
	st = sin(wts[6]*ds[6]);

	(Hs[6])(0,0) = ct;
	(Hs[6])(2,2) = ct;
	(Hs[6])(0,2) = -st;
	(Hs[6])(2,0) = st ;

	ct = cos(wts[7]*ds[7]);
	st = sin(wts[7]*ds[7]);

	(Hs[7])(1,1) = ct;
	(Hs[7])(2,2) = ct;
	(Hs[7])(1,2) = -st;
	(Hs[7])(2,1) = st ;

	return Hs;

}
matrixtype Hs2H(std::vector<matrixtype> Hs)
{
	matrixtype H = matrixtype::Identity(3,3);
	for (int i = 0;i<8;i++)
	{
		H = Hs[i]*H;
	}
	return H; 
}

int main()
{
	imgtype ds[8] = {1,2,3,4,5,6,7,8};
	imgtype wts[8] = {1,2,3,4,5,6,7,8};
	std::vector<matrixtype>	 Hs = ds2Hs(ds,wts);
	matrixtype H = Hs2H(Hs);
	std::cout<< "the H Matrix"<<std::endl;
	std::cout<<H<<std::endl;
	std::cout<<"the the Hs matrices are"<<std::endl;
	for (int i =0 ; i<8;i++)
	{
		std::cout<<"=============   "<<i<<"   ======"<<std::endl;
		std::cout<<Hs[i]<<std::endl;
	}
	//std::cout<<"the Hs matrix"<<std::endl;
	//std::cout<<Hs<<std::endl;
	// Eigen::Matrix<float, 3,3> H_ele = Eigen::MatrixXf::Identity(3,3);
	// std::vector<Eigen::Matrix<float,3,3>> Hs (8,H_ele);

}