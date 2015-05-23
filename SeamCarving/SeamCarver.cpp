#include "SeamCarver.h"

SeamCarver::SeamCarver(Mat_<Vec3b> img)
{
	image = img;
	energy = Mat(image.rows, image.cols, CV_32S, Scalar(195075));
	ComputeFullEnergy();
}

SeamCarver::~SeamCarver()
{
	energy.release();
}

void SeamCarver::ShowImage()
{
	imshow("Carved Image", image);
}

void SeamCarver::ShowEnergy() 
{
	imshow("Energy", energy);
}

void SeamCarver::SaveImage(string filename)
{
	imwrite(filename, image);
}

void SeamCarver::SaveEnergy(string filename)
{
	imwrite(filename, energy);
}

Mat_<Vec3b> SeamCarver::GetImage()
{
	return image;
}

//Return the energy at a particular pixel
unsigned int SeamCarver::GetEnergy(unsigned int row, unsigned int col)
{
	return energy.at<int>(row, col);
}

void SeamCarver::ShowVerticalSeam(vector<int> seam) 
{
	Mat tmp;
	image.copyTo(tmp);

	//Set the color of the seam to red
	for (int i = 0; i < tmp.rows; ++i)
		tmp.at<Vec3b>(i, seam[i]) = Vec3b(0, 0, 255);

	imshow("Seam", tmp);
	tmp.release();
}

void SeamCarver::ShowHorizontalSeam(vector<int> seam)
{
	Mat tmp;
	image.copyTo(tmp);

	//Set the color of the seam to Red
	for (int i = 0; i < tmp.cols; ++i)
		tmp.at<Vec3b>(seam[i], i) = Vec3b(0, 0, 255);

	imshow("Seam", tmp);
	tmp.release();
}

void SeamCarver::ComputeFullEnergy()
{
	//Sampling pattern.
	//
	//					rgb
	//				 rgb A rgb
	//					rgb
	//

	energy.create(image.rows, image.cols, CV_32S);

	//Scan through the image and update the energy values. Ignore boundary pixels.
	for (int i = 1; i < image.rows - 1; ++i)
	{
		uchar* prev = image.ptr<uchar>(i - 1);	//Previous row
		uchar* curr = image.ptr<uchar>(i);		//Current row
		uchar* next = image.ptr<uchar>(i + 1);	//Next row

		for (int j = 1; j < image.cols - 1; ++j)
		{
			int val = 0;
			int pos = 3 * j;

			//X-axis
			val += (prev[pos] - next[pos]) * (prev[pos] - next[pos]);
			val += (prev[pos + 1] - next[pos + 1]) * (prev[pos + 1] - next[pos + 1]);
			val += (prev[pos + 2] - next[pos + 2]) * (prev[pos + 2] - next[pos + 2]);

			//Y-axis
			val += (curr[pos + 3] - curr[pos - 3]) * (curr[pos + 3] - curr[pos - 3]);
			val += (curr[pos + 4] - curr[pos - 2]) * (curr[pos + 4] - curr[pos - 2]);
			val += (curr[pos + 5] - curr[pos - 1]) * (curr[pos + 5] - curr[pos - 1]);

			energy.at<int>(i, j) = val;
		}
	}
}

vector<int> SeamCarver::FindVerticalSeam()
{
	const int width = image.rows;
	const int height = image.cols;

	int* seam = new int[width];

	unsigned int** distTo = new unsigned int*[width];
	for (int i = 0; i < width; i++)
		distTo[i] = new unsigned int[height];

	short** edgeTo = new short*[width];
	for (int i = 0; i < width; i++)
		edgeTo[i] = new short[height];

	//Initialize the distance and edge matrices
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			if (x == 0)
				distTo[x][y] = 0;
			else			
				distTo[x][y] = numeric_limits<unsigned int>::max();

			edgeTo[x][y] = 0;
		}
	}

	for (int x = 0; x < width - 1; ++x)
	{
		for (int y = 0; y < height; ++y)
		{

			//Left
			if (y != 0)
			{
				if (distTo[x + 1][y - 1] > distTo[x][y] + GetEnergy(x + 1, y - 1))
				{
					distTo[x + 1][y - 1] = distTo[x][y] + GetEnergy(x + 1, y - 1);
					edgeTo[x + 1][y - 1] = 1;
				}
			}

			//Below
			if (distTo[x + 1][y] > distTo[x][y] + GetEnergy(x + 1, y))
			{
				distTo[x + 1][y] = distTo[x][y] + GetEnergy(x + 1, y);
				edgeTo[x + 1][y] = 0;
			}

			//Right
			if (y != height - 1)
			{
				if (distTo[x + 1][y + 1] > distTo[x][y] + GetEnergy(x + 1, y + 1))
				{
					distTo[x + 1][y + 1] = distTo[x][y] + GetEnergy(x + 1, y + 1);
					edgeTo[x + 1][y + 1] = -1;
				}
			}
		}
	}

	//Find the bottom of the min-path
	unsigned int min_index = 0;
	unsigned int min = distTo[width - 1][0];

	for (int y = 1; y < height; ++y)
	{
		if (distTo[width - 1][y] < min)
		{
			min_index = y;
			min = distTo[width - 1][y];

			cout << min_index << " " << min << endl;
		}
	}

	//Retrace the min-path and update the 'seam'
	seam[width - 1] = min_index;
	for (int i = width- 1; i > 0; --i)
	{
		seam[i - 1] = seam[i] + edgeTo[i][seam[i]];
	}

	//Insert the seam in the vector
	vector<int> s;
	for (int i = 0; i < width; i++)
		s.push_back(seam[i]);


	//Cleanup
	delete[] seam;

	for (int i = 0; i < width; i++)
		delete distTo[i];
	delete[] distTo;

	for (int i = 0; i < width; i++)
		delete edgeTo[i];
	delete[] edgeTo;

	return s;
}

void SeamCarver::RemoveVerticalSeam(vector<int> seam)
{
	//Move all pixels to the right of the seam one pixel to the left
	for (int row = 0; row < image.rows; ++row) 
		for (int col = seam[row]; col < image.cols - 1; ++col)
			image.at<Vec3b>(row, col) = image.at<Vec3b>(row, col + 1);

	image = image(Rect(0, 0, image.cols - 1, image.rows));

	ComputeFullEnergy();
}

vector<int> SeamCarver::FindHorizontalSeam()
{
	vector<int> seam(image.cols);

	//Transpose and find vertical seam
	transpose(image, image);
	transpose(energy, energy);
	seam = FindVerticalSeam();

	transpose(image, image);
	transpose(energy, energy);
	return seam;
}

void SeamCarver::RemoveHorizontalSeam(vector<int> seam)
{
	//Transpose and remove vertical seam
	transpose(image, image);
	transpose(energy, energy);
	RemoveVerticalSeam(seam);

	transpose(image, image);
	transpose(energy, energy);
}
