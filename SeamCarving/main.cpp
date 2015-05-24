#include "SeamCarver.h"
#include <Windows.h>

int main() 
{
	string name = "Pictures/beach";
	string ext1 = "_carved";
	string ext2 = "_energy";
	string jpg = ".jpg";
	string png = ".png";
	string type = jpg;

	Mat_<Vec3b> image = imread(name + type);

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


	int horizontal = 250;
	int vertical = 350;
	for (int i = 0; i < horizontal; i++)
	{
		vector<int> seam = s.FindHorizontalSeam();
		s.ShowHorizontalSeam(seam);
		s.RemoveHorizontalSeam(seam);
		waitKey(1);
	}

	for (int i = 0; i < vertical; i++)
	{
		vector<int> seam2 = s.FindVerticalSeam();
		s.ShowVerticalSeam(seam2);
		s.RemoveVerticalSeam(seam2);
		waitKey(1);
	}

	//int i = 300;
	//while (waitKey(1) != 27 && i > 0)
	//{
	//	vector<int> seam2 = s.FindVerticalSeam();
	//	s.ShowVerticalSeam(seam2);
	//	s.RemoveVerticalSeam(seam2);
	//	waitKey(1);

	//	vector<int> seam = s.FindHorizontalSeam();
	//	s.ShowHorizontalSeam(seam);
	//	s.RemoveHorizontalSeam(seam);
	//	waitKey(1);

	//	i--;
	//}

	s.ShowImage();

	while (waitKey(20) != 27);

	s.SaveImage(name + ext1 + type);

	image.release();
	return 0;
}



