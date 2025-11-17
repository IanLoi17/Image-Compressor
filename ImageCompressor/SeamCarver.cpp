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
		for (int j = 0; j < cols; j++)
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

std::vector<int> SeamCarver::findHorizontalSeam()
{
	int rows = energyMap.rows;
	int cols = energyMap.cols;

	cv::Mat dpTable(rows, cols, CV_64F);
	for (int i = 0; i < rows; i++)
	{
		dpTable.at<double>(i, 0) = energyMap.at<double>(i, 0);
	}

	for (int j = 1; j < cols; ++j)
	{
		for (int i = 0; i < rows; ++i)
		{
			double minEnergy = dpTable.at<double>(i, j - 1);

			if (i > 0)
			{
				minEnergy = cv::min(minEnergy, dpTable.at<double>(i - 1, j - 1));
			}

			if (i < rows - 1)
			{
				minEnergy = cv::min(minEnergy, dpTable.at<double>(i + 1, j - 1));
			}

			dpTable.at<double>(i, j) = energyMap.at<double>(i, j) + minEnergy;
		}
	}

	std::vector<int> seam(cols);

	double minVal;
	cv::Point minLoc;
	cv::minMaxLoc(dpTable.col(cols - 1), &minVal, nullptr, &minLoc, nullptr);
	seam[cols - 1] = minLoc.y;

	for (int j = cols - 2; j >= 0; --j)
	{
		int currRow = seam[j + 1];
		int nextRow = currRow;
		double minEnergy = dpTable.at<double>(currRow, j);

		if (currRow > 0 && dpTable.at<double>(currRow - 1, j) < minEnergy)
		{
			minEnergy = dpTable.at<double>(currRow - 1, j);
			nextRow = currRow - 1;
		}

		if (currRow < rows - 1 && dpTable.at<double>(currRow + 1, j) < minEnergy)
		{
			nextRow = currRow + 1;
		}

		seam[j] = nextRow;
	}

	return seam;
}

std::vector<int> SeamCarver::findVerticalSeamGreedy()
{
	int rows = energyMap.rows;
	int cols = energyMap.cols;

	std::vector<int> seam(rows);

	double minVal;
	cv::Point minLoc;
	cv::minMaxLoc(energyMap.row(0), &minVal, nullptr, &minLoc, nullptr);

	int currCol = minLoc.x;
	seam[0] = currCol;

	for (int i = 1; i < rows; i++)
	{
		double minEnergy = energyMap.at<double>(i, currCol);
		int nextCol = currCol;

		if (currCol > 0 && energyMap.at<double>(i, currCol - 1) < minEnergy)
		{
			minEnergy = energyMap.at<double>(i, currCol - 1);
			nextCol = currCol - 1;
		}

		if (currCol < cols - 1 && energyMap.at<double>(i, currCol + 1) < minEnergy)
		{
			nextCol = currCol + 1;
		}

		currCol = nextCol;
		seam[i] = currCol;
	}

	return seam;
}

void SeamCarver::removeSeam(std::vector<int> const& seam, bool vertical)
{
	if (vertical)
	{
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
	}

	else
	{
		cv::Mat newImg(image.rows - 1, image.cols, image.type());
		for (int j = 0; j < image.cols; ++j)
		{
			int seamRow = seam[j];
			if (seamRow > 0)
			{
				image.col(j).rowRange(0, seamRow).copyTo(newImg.col(j).rowRange(0, seamRow));
			}

			if (seamRow < image.rows - 1)
			{
				image.col(j).rowRange(seamRow + 1, image.rows).copyTo(newImg.col(j).rowRange(seamRow, newImg.rows));
			}
		}

		newImg.copyTo(image);
	}

	calculateEnergyMap();
}

void SeamCarver::removeVerticalSeam(bool useGreedy)
{
	std::vector<int> seam;

	if (useGreedy)
	{
		seam = findVerticalSeamGreedy();
	}

	else
	{
		seam = findVerticalSeam();
	}

	removeSeam(seam, true);
}

void SeamCarver::removeHorizontalSeam()
{
	std::vector<int> seam = findHorizontalSeam();
	removeSeam(seam, false);
}

cv::Mat SeamCarver::getImage() const
{
	return image.clone();
}

cv::Mat SeamCarver::getEnergyMap() const
{
	return energyMap.clone();
}