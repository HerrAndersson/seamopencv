#include "SeamCarver.h"
#include <Windows.h>

int main() 
{
	string name = "towerMedium";
	string c = "_carved";
	string ext = ".png";
	Mat_<Vec3b> image = imread(name + ext);
	vector<int> seam;

	int horizontal = 50;
	int vertical = 50;
	bool done = false;

	if (!image.data) 
	{
		cout << "Invalid input";
		image.release();
		return -1;
	}

	imshow("Original Image", image);
	SeamCarver s(image);
	//s.ShowEnergy();

	while (waitKey(20) != 27 || !done)
	{
		if (horizontal > 0)
		{
			seam = s.FindHorizontalSeam();
			s.ShowHorizontalSeam(seam);
			s.RemoveHorizontalSeam(seam);
			horizontal--;
		}
		else if (vertical > 0)
		{
			seam = s.FindVerticalSeam();
			s.ShowVerticalSeam(seam);
			s.RemoveVerticalSeam(seam);
			vertical--;
		}

		if (horizontal <= 0 || vertical <= 0)
			done = true;
	}

	imshow("Carved Image", s.GetImage());

	imwrite(name + c + ext, s.GetImage());

	image.release();
	return 0;
}



