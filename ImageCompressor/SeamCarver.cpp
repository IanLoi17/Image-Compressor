#include "SeamCarver.h"
#include <algorithm>
#include <limits>

SeamCarver::SeamCarver(cv::Mat const& img)
{
	img.copyTo(image);
	calculateEnergyMap();
}

void SeamCarver::calculateEnergyMap()
{
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	cv::Mat gradX, gradY;
	cv::Sobel(gray, gradX, CV_64F, 1, 0, 3);
	cv::Sobel(gray, gradY, CV_64F, 0, 1, 3);

	cv::Mat magnitude;
	magnitude = cv::abs(gradX) + cv::abs(gradY);

	magnitude.convertTo(energyMap, CV_64F);
}

std::vector<int> SeamCarver::findVerticalSeam()
{
	int rows = energyMap.rows;
	int cols = energyMap.cols;

	cv::Mat dpTable(rows, cols, CV_64F);
	energyMap.row(0).copyTo(dpTable.row(0));

	for (int i = 1; i < rows; i++)
	{
		for (int j = 1; j < cols; j++)
		{
			double minEnergy = dpTable.at<double>(i - 1, j);

			if (j > 0)
			{
				minEnergy = cv::min(minEnergy, dpTable.at<double>(i - 1, j - 1));
			}

			if (j < cols - 1)
			{
				minEnergy = cv::min(minEnergy, dpTable.at<double>(i - 1, j + 1));
			}

			dpTable.at<double>(i, j) = energyMap.at<double>(i, j) + minEnergy;
		}
	}

	std::vector<int> seam(rows);

	double minVal;
	cv::Point minLoc;
	cv::minMaxLoc(dpTable.row(rows - 1), &minVal, nullptr, &minLoc, nullptr);
	seam[rows - 1] = minLoc.x;

	for (int i = rows - 2; i >= 0; i--)
	{
		int currCol = seam[i + 1];
		int nextCol = currCol;
		double minEnergy = dpTable.at<double>(i, currCol);

		if (currCol > 0 && dpTable.at<double>(i, currCol - 1) < minEnergy)
		{
			minEnergy = dpTable.at<double>(i, currCol - 1);
			nextCol = currCol - 1;
		}

		if (currCol < cols - 1 && dpTable.at<double>(i, currCol + 1) < minEnergy)
		{
			nextCol = currCol + 1;
		}

		seam[i] = nextCol;
	}

	return seam;
}

void SeamCarver::removeVerticalSeam()
{
	std::vector<int> seam = findVerticalSeam();

	cv::Mat newImg(image.rows, image.cols - 1, image.type());
	for (int i = 0; i < image.rows; i++)
	{
		int seamCol = seam[i];
		if (seamCol > 0)
		{
			image.row(i).colRange(0, seamCol).copyTo(newImg.row(i).colRange(0, seamCol));
		}

		if (seamCol < image.cols - 1)
		{
			image.row(i).colRange(seamCol + 1, image.cols).copyTo(newImg.row(i).colRange(seamCol, newImg.cols));
		}
	}

	newImg.copyTo(image);
	calculateEnergyMap();
}

void SeamCarver::removeHorizontalSeam()
{
	cv::transpose(image, image);
	calculateEnergyMap();
	removeVerticalSeam();
	cv::transpose(image, image);
	calculateEnergyMap();
}

cv::Mat SeamCarver::getImage() const
{
	return image.clone();
}

cv::Mat SeamCarver::getEnergyMap() const
{
	return energyMap.clone();
}