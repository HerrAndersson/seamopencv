#include "SeamCarver.h"
#include <Windows.h>

int main() 
{
	string name = "Pictures/tower";
	string ext1 = "_carved";
	string ext2 = "_energy";
	string jpg = ".jpg";
	string png = ".png";
	string type = png;

	Mat_<Vec3b> image = imread(name + type);

	int horizontal = 1;
	int vertical = 350;
	bool done = false;

	if (!image.data) 
	{
		cout << "Invalid input";
		image.release();
		return -1;
	}

	imshow("Original Image", image);
	SeamCarver s(image);
	s.ShowEnergy();
	//s.SaveEnergy(name + ext1 + ext2 + type);

	while (waitKey(20) != 27)
	{
		vector<int> seam = s.FindHorizontalSeam();
		s.ShowHorizontalSeam(seam);
		s.RemoveHorizontalSeam(seam);

		vector<int> seam2 = s.FindVerticalSeam();
		s.ShowVerticalSeam(seam2);
		s.RemoveVerticalSeam(seam2);
	}

	s.ShowImage();

	while (waitKey(20) != 27);

	//s.SaveEnergy(name + ext1 + ext2 + type);
	s.SaveImage(name + ext1 + type);

	image.release();
	return 0;
}



