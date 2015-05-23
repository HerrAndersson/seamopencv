#include "SeamCarver.h"
#include <Windows.h>

int main() 
{
	string str = "towerMedium.png";
	Mat_<Vec3b> image = imread(str);

	if (!image.data) 
	{
		cout << "Invalid input";
		image.release();
		return -1;
	}

	imshow("Original Image", image);
	SeamCarver s(image);
	s.ShowEnergy();

	//for (int i = 0; i < 5; ++i)
	//{
	//	vector<int> seam = s.FindHorizontalSeam();
	//	s.ShowHorizontalSeam(seam);
	//	s.RemoveHorizontalSeam(seam);
	//}
	//for (int i = 0; i < 5; ++i) 
	//{
	//	vector<int> seam = s.FindVerticalSeam();
	//	s.ShowVerticalSeam(seam);

	//	Mat tmp;
	//	image.copyTo(tmp);

	//	for (int i = 0; i < tmp.rows; ++i)
	//	{
	//		tmp.at<Vec3b>(i, seam[i]) = Vec3b(0, 0, 255);	//Set the color of the seam to Red
	//	}

	//	imshow("Seam", tmp);
	//	tmp.release();
	//	s.RemoveVerticalSeam(seam);
	//}
	imshow("Carved Image", s.GetImage());

	while (waitKey(20) != 27);

//	imwrite("bench_carved.jpg", s.getImage());

	image.release();
	return 0;
}



