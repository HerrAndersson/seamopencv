#include "SeamCarver.h"

SeamCarver::SeamCarver(Mat_<Vec3b> img)
{
	image = img;
	duplicate = image;
	energy = Mat(image.rows, image.cols, CV_32S, Scalar(195075));
	ComputeFullEnergy();
}

SeamCarver::~SeamCarver()
{
	duplicate.release();
	energy.release();
}

void SeamCarver::ShowImage()
{
	imshow("Image", image);
}

void SeamCarver::ShowEnergy() 
{
	imshow("Energy", energy);
}

void SeamCarver::SaveImage()
{
	imwrite("Image.png", image);
}

void SeamCarver::SaveEnergy()
{
	imwrite("Energy.png", energy);
}

Mat_<Vec3b> SeamCarver::GetImage()
{
	return image;
}

//Return the value of the energy at a particular pixel
unsigned int SeamCarver::GetEnergy(unsigned int row, unsigned int col)
{
	return energy.at<int>(row, col);
}

void SeamCarver::ShowVerticalSeam(vector<int> seam) 
{
	Mat tmp;
	image.copyTo(tmp);

	for (int i = 0; i < tmp.rows; ++i)
	{
		tmp.at<Vec3b>(i, seam[i]) = Vec3b(0, 0, 255);	//Set the color of the seam to Red
	}

	imshow("Seam", tmp);
	tmp.release();
}

void SeamCarver::ShowHorizontalSeam(vector<int> seam)
{
	Mat tmp;
	image.copyTo(tmp);
	for (int i = 0; i < tmp.cols; ++i)

	{
		tmp.at<Vec3b>(seam[i], i) = Vec3b(0, 0, 255);	//Set the color of the seam to Red
	}

	imshow("Seam", tmp);
	tmp.release();
}

void SeamCarver::ComputeFullEnergy()
{
	//Ensure that the size of the energy matrix matches that of the image
	energy.create(image.rows, image.cols, CV_32S);

	//Scan through the image and update the energy values. Ignore boundary pixels.
	for (int i = 1; i < image.rows - 1; ++i)
	{
		uchar* prev = image.ptr<uchar>(i - 1);	//Pointer to previous row
		uchar* curr = image.ptr<uchar>(i);		//Pointer to current row
		uchar* next = image.ptr<uchar>(i + 1);	//Pointer to next row

		for (int j = 1; j < image.cols - 1; ++j)
		{
			int val = 0;
			//Energy along the x-axis
			val += (prev[3 * j] - next[3 * j]) * (prev[3 * j] - next[3 * j]);
			val += (prev[3 * j + 1] - next[3 * j + 1]) * (prev[3 * j + 1] - next[3 * j + 1]);
			val += (prev[3 * j + 2] - next[3 * j + 2]) * (prev[3 * j + 2] - next[3 * j + 2]);

			//Energy along the y-axis
			val += (curr[3 * j + 3] - curr[3 * j - 3]) * (curr[3 * j + 3] - curr[3 * j - 3]);
			val += (curr[3 * j + 4] - curr[3 * j - 2]) * (curr[3 * j + 4] - curr[3 * j - 2]);
			val += (curr[3 * j + 5] - curr[3 * j - 1]) * (curr[3 * j + 5] - curr[3 * j - 1]);

			energy.at<int>(i, j) = val;
		}
	}
}

void SeamCarver::ComputeEnergyAfterSeamRemoval(vector<int> seam)
{
	Mat tmp = Mat(image.rows, image.cols, CV_32S, Scalar(195075));

	for (int row = 0; row < (int)image.rows; ++row)
	{
		for (int col = 0; col < (int)image.cols; ++col)
		{
			if (col < seam[row] - 1)
				tmp.at<int>(row, col) = energy.at<int>(row, col);

			if (col > seam[row])
				tmp.at<int>(row, col) = energy.at<int>(row, col + 1);

			if (col == seam[row] || col == seam[row] - 1)
			{
				Vec3b l = image.at<Vec3b>(row, col - 1);
				Vec3b r = image.at<Vec3b>(row, col + 1);
				Vec3b u = image.at<Vec3b>(row - 1, col);
				Vec3b d = image.at<Vec3b>(row + 1, col);

				int val = (l[0] - r[0])*(l[0] - r[0]) + (l[1] - r[1])*(l[1] - r[1]) + (l[2] - r[2])*(l[2] - r[2]) +
					(u[0] - d[0])*(u[0] - d[0]) + (u[1] - d[1])*(u[1] - d[1]) + (u[2] - d[2])*(u[2] - d[2]);

				tmp.at<int>(row, col) = val;
			}
		}
	}
	energy = tmp;
}

vector<int> SeamCarver::FindVerticalSeam()
{
	const int r = image.rows;
	const int c = image.cols;

	int* seam = new int[r];

	unsigned int** distTo = new unsigned int*[r];
	for (int i = 0; i < r; i++)
		distTo[i] = new unsigned int[c];

	short** edgeTo = new short*[r];
	for (int i = 0; i < r; i++)
		edgeTo[i] = new short[c];

	//Initialize the distance and edge matrices
	for (int i = 0; i < image.rows; ++i) 
	{
		for (int j = 0; j < image.cols; ++j) 
		{
			if (i == 0)
				distTo[i][j] = 0;
			else			
				distTo[i][j] = numeric_limits<unsigned int>::max();

			edgeTo[i][j] = 0;
		}
	}

	// Relax the edges in topological order
	for (int row = 0; row < image.rows - 1; ++row)
	{
		for (int col = 0; col < image.cols; ++col)
		{
			//Check the pixel to the bottom-left
			if (col != 0)
			{
				if (distTo[row + 1][col - 1] > distTo[row][col] + GetEnergy(row + 1, col - 1))
				{
					distTo[row + 1][col - 1] = distTo[row][col] + GetEnergy(row + 1, col - 1);
					edgeTo[row + 1][col - 1] = 1;
				}
			}
			//Check the pixel right below
			if (distTo[row + 1][col] > distTo[row][col] + GetEnergy(row + 1, col))
			{
				distTo[row + 1][col] = distTo[row][col] + GetEnergy(row + 1, col);
				edgeTo[row + 1][col] = 0;
			}
			//Check the pixel to the bottom-right
			if (col != image.cols - 1)
			{
				if (distTo[row + 1][col + 1] > distTo[row][col] + GetEnergy(row + 1, col + 1))
				{
					distTo[row + 1][col + 1] = distTo[row][col] + GetEnergy(row + 1, col + 1);
					edgeTo[row + 1][col + 1] = -1;
				}
			}
		}
	}

	//Find the bottom of the min-path
	unsigned int min_index = 0, min = distTo[image.rows - 1][0];
	for (int i = 1; i < image.cols; ++i)
	{
		if (distTo[image.rows - 1][i] < min)
		{
			min_index = i;
			min = distTo[image.rows - 1][i];
		}
	}

	//Retrace the min-path and update the 'seam' vector
	seam[image.rows - 1] = min_index;
	for (int i = image.rows - 1; i > 0; --i)
	{
		seam[i - 1] = seam[i] + edgeTo[i][seam[i]];
	}


	for (int i = 0; i < r; i++)
		delete distTo[i];
	delete[] distTo;

	for (int i = 0; i < r; i++)
		delete edgeTo[i];
	delete[] edgeTo;

	vector<int> s;
	for (int i = 0; i < r; i++)
		s.push_back(seam[i]);

	delete[] seam;

	return s;
}

void SeamCarver::RemoveVerticalSeam(vector<int> seam)
{
	//Move all the pixels to the right of the seam, one pixel to the left
	for (int row = 0; row < image.rows; ++row) 
	{
		for (int col = seam[row]; col < image.cols - 1; ++col)
			image.at<Vec3b>(row, col) = image.at<Vec3b>(row, col + 1);
	}

	//Resize the image to remove the last column
	image = image(Rect(0, 0, image.cols - 1, image.rows));

	//Re-compute the energy of the new image
	ComputeFullEnergy();
	//	computeEnergyAfterSeamRemoval(seam);
}

vector<int> SeamCarver::FindHorizontalSeam()
{
	vector<int> seam(image.cols);
	//Transpose the matrices and find the vertical seam
	transpose(image, image);
	transpose(energy, energy);
	seam = FindVerticalSeam();

	//Transpose back
	transpose(image, image);
	transpose(energy, energy);
	return seam;
}

void SeamCarver::RemoveHorizontalSeam(vector<int> seam)
{
	//Transpose the matrices and remove the vertical seam
	transpose(image, image);
	transpose(energy, energy);
	RemoveVerticalSeam(seam);

	//Transpose back
	transpose(image, image);
	transpose(energy, energy);
}
