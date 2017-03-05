/** Pseudocode for Multichannel Inverse Compositional Lucas-Kanade **/
/**
Algorithm
Pre-compute:
Evaluate the gradient of the Template T(x), dT.
Evaluate the Jacobian dW/dp at (x;0)  (differentiate warp with respect to warp parameters at p = 0) // Part of warping function
Compute steepest descent of images sdi = dT*dW/dp 
Compute Hessian Matrix by sdi’ * sdi

Iterate:
Warp I with W(x;p) to compute I(W(x;p))
Compute the error image I(W(x;p)) - T(x)
Compute vector multiplication of steepest descent and error. E = sdi' * error;
Compute dp = inv(H)*E;
Update warp W(x, p) = W(x, p) * W(x, delta)^-1 = W(W(x, delta)^-1, p)
Until dp <= threshold
**/

// Functions
class BPTracker
{
public:
  BitplanesTracker(parameters); // initialize warping function with the parameters
  void setTemplate(cv::Mat& image, const cv::Rect& bbox); // set template to track
  cv::Rect& bbox track(cv::Mat& image, warping_mat); // track template and return bounding box
  void drawBbox(cv::Mat& image, const cv::Rect bbox); // draw bounding box on the image 
  
protected:
  cv::Mat computeGradient(cv::Mat& image, const cv::Rect& bbox); // compute gradient of image at selected area
  cv::Mat cvcomputeSteepestDescentImage(const cv::Mat gradient, warping_mat); // sdi = dT * (dW/dp)
  cv::Mat computeHessian(cv::Mat& image, warping_mat); // Hessian = (sdi)' * (sdi)
  ? computeHammingDistance(cv::Mat& image, cv::Mat& template); // compute Hamming distance
  params = computeParamUpdate(? HammingDistance); // warp parameter arameter update. dp = H \ E. E = sdi' * HammingDistance.
}
