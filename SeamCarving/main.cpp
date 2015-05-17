#include "SeamCarver.h"

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

	vector<int> sm = s.FindVerticalSeam();
	s.ShowVerticalSeam(sm);

	//for (int i = 0; i < 5; ++i)
	//{
	//	vector<int> seam = s.FindHorizontalSeam();
	//	s.ShowHorizontalSeam(seam);
	//	s.RemoveHorizontalSeam(seam);
	//}
	//for (int i = 0; i < 5; ++i) {
	//	vector<int> seam = s.FindVerticalSeam();
	//	s.ShowVerticalSeam(seam);
	//	s.RemoveVerticalSeam(seam);
	//}
	imshow("Carved Image", s.GetImage());

	imshow("Carved Image", s.GetImage());
//	cout << "Seam Length: " << seam.size() << endl;
//	s.showImage();
//	s.showEnergy();
	while (waitKey(20) != 27);

//	imwrite("bench_carved.jpg", s.getImage());

//	for (int i = 0; i < 5; ++i) 
//	{
//		for (int j = 0; j < 5; ++j) 
//		{
//			cout << s.energy.at<int32_t>(i,j) << " ";
//		}
//		cout << endl;
//	}

	image.release();
	return 0;
}



