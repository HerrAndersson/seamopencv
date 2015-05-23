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


	int horizontal = 10;
	int vertical = 350;
	//for (int i = 0; i < horizontal; i++)
	//{
	//	vector<int> seam = s.FindHorizontalSeam();
	//	s.RemoveHorizontalSeam(seam);
	//}

	//for (int i = 0; i < vertical; i++)
	//{
	//	vector<int> seam2 = s.FindVerticalSeam();
	//	s.RemoveVerticalSeam(seam2);
	//}

	/*while (waitKey(20) != 27)*/
	while (waitKey(20) != 27)
	{
		//if (vertical > 0)
		//{
			vector<int> seam2 = s.FindVerticalSeam();
			s.ShowVerticalSeam(seam2);
			s.RemoveVerticalSeam(seam2);
			waitKey(20);
			vertical--;
		//}

		//if (horizontal)
		//{
			//vector<int> seam = s.FindHorizontalSeam();
			//s.ShowHorizontalSeam(seam);
			//s.RemoveHorizontalSeam(seam);
			//waitKey(20);
			//horizontal--;
		//}
	}

	s.ShowImage();

	while (waitKey(20) != 27);

	//s.SaveEnergy(name + ext1 + ext2 + type);
	s.SaveImage(name + ext1 + type);

	image.release();
	return 0;
}



