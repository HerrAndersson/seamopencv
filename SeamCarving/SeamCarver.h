#ifndef SEAMCARVER_H_
#define SEAMCARVER_H_

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

class SeamCarver
{
	Mat_<Vec3b> image;
	Mat_<Vec3b> duplicate;
	Mat energy;

	void ComputeFullEnergy();
	void ComputeEnergyAfterSeamRemoval(vector<int> seam);

public:

	SeamCarver(Mat_<Vec3b> img);
	virtual ~SeamCarver();

	void ShowImage();
	void ShowEnergy();

	void SaveImage();
	void SaveEnergy();

	Mat_<Vec3b> GetImage();
	unsigned int GetEnergy(unsigned int row, unsigned int col);

	vector<int> FindVerticalSeam();
	vector<int> FindHorizontalSeam();

	void RemoveVerticalSeam(vector<int> seam);
	void RemoveHorizontalSeam(vector<int> seam);

	void ShowVerticalSeam(vector<int> seam);
	void ShowHorizontalSeam(vector<int> seam);
};

#endif /* SEAMCARVER_H_ */
