#include "SeamCarver.h"
#include <algorithm>
#include <limits>
#include <cmath>

SeamCarver::SeamCarver(cv::Mat const& img) : energyMapValid(false)
{
	if (img.empty())
	{
		throw std::invalid_argument("Cannot create SeamCarver with empty image");
	}
	img.copyTo(image);
	calculateEnergyMap();
	energyMapValid = true;
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

std::vector<int> SeamCarver::findHorizontalSeamGreedy()
{
	int rows = energyMap.rows;
	int cols = energyMap.cols;

	std::vector<int> seam(cols);

	double minVal;
	cv::Point minLoc;
	cv::minMaxLoc(energyMap.col(0), &minVal, nullptr, &minLoc, nullptr);

	int currRow = minLoc.y;
	seam[0] = currRow;

	for (int j = 1; j < cols; j++)
	{
		double minEnergy = energyMap.at<double>(currRow, j);
		int nextRow = currRow;

		if (currRow > 0 && energyMap.at<double>(currRow - 1, j) < minEnergy)
		{
			minEnergy = energyMap.at<double>(currRow - 1, j);
			nextRow = currRow - 1;
		}

		if (currRow < rows - 1 && energyMap.at<double>(currRow + 1, j) < minEnergy)
		{
			nextRow = currRow + 1;
		}

		currRow = nextRow;
		seam[j] = currRow;
	}

	return seam;
}

void SeamCarver::removeSeam(std::vector<int> const& seam, bool vertical)
{
	// Add safety checks
	if (vertical)
	{
		if (image.cols <= 1)
		{
			std::cerr << "Warning: Cannot remove vertical seam, image width is too small" << std::endl;
			return;
		}
		cv::Mat newImg(image.rows, image.cols - 1, image.type());
		for (int i = 0; i < image.rows; i++)
		{
			int seamCol = seam[i];
			// Bounds checking
			if (seamCol < 0 || seamCol >= image.cols)
			{
				std::cerr << "Warning: Invalid seam column " << seamCol << " at row " << i << std::endl;
				continue;
			}
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
		if (image.rows <= 1)
		{
			std::cerr << "Warning: Cannot remove horizontal seam, image height is too small" << std::endl;
			return;
		}
		cv::Mat newImg(image.rows - 1, image.cols, image.type());
		for (int j = 0; j < image.cols; ++j)
		{
			int seamRow = seam[j];
			// Bounds checking
			if (seamRow < 0 || seamRow >= image.rows)
			{
				std::cerr << "Warning: Invalid seam row " << seamRow << " at column " << j << std::endl;
				continue;
			}
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

void SeamCarver::removeHorizontalSeam(bool useGreedy)
{
	std::vector<int> seam;

	if (useGreedy)
	{
		seam = findHorizontalSeamGreedy();
	}

	else
	{
		seam = findHorizontalSeam();
	}

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

cv::Mat SeamCarver::carveSeamOnceAndShow(bool vertical)
{
	cv::Mat newImg = image.clone();
	std::vector<int> seam;
	if (vertical)
	{
		seam = findVerticalSeam();
		for (int i = 0; i < image.rows; i++)
		{
			newImg.at<cv::Vec3b>(i, seam[i]) = cv::Vec3b(0, 0, 255);
		}
	}
	else
	{
		seam = findHorizontalSeam();
		for (int i = 0; i < image.cols; i++)
		{
			newImg.at<cv::Vec3b>(seam[i], i) = cv::Vec3b(0, 0, 255);
		}
	}
	removeSeam(seam, vertical);
	
	return newImg;
}

void SeamCarver::ResetImage(cv::Mat& img)
{
	img.copyTo(image);
	calculateEnergyMap();
}

void SeamCarver::calculateForwardEnergy()
{
	int rows = image.rows;
	int cols = image.cols;
	forwardEnergyMap = cv::Mat(rows, cols, CV_64F);

	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// Calculate forward energy using the approach from the paper
	// "Improved Seam Carving for Video Retargeting"
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			double energy = 0.0;

			// Calculate cost of removing this pixel
			if (j > 0 && j < cols - 1)
			{
				// Horizontal gradient after removal
				double diff = std::abs(gray.at<uchar>(i, j - 1) - gray.at<uchar>(i, j + 1));
				energy += diff;
			}

			if (i > 0 && i < rows - 1)
			{
				// Vertical gradient after removal
				double diff = std::abs(gray.at<uchar>(i - 1, j) - gray.at<uchar>(i + 1, j));
				energy += diff;
			}

			// Add some of the original gradient energy too
			if (j > 0)
				energy += std::abs(gray.at<uchar>(i, j) - gray.at<uchar>(i, j - 1)) * 0.5;
			if (j < cols - 1)
				energy += std::abs(gray.at<uchar>(i, j) - gray.at<uchar>(i, j + 1)) * 0.5;
			if (i > 0)
				energy += std::abs(gray.at<uchar>(i, j) - gray.at<uchar>(i - 1, j)) * 0.5;
			if (i < rows - 1)
				energy += std::abs(gray.at<uchar>(i, j) - gray.at<uchar>(i + 1, j)) * 0.5;

			forwardEnergyMap.at<double>(i, j) = energy;
		}
	}
}

std::vector<int> SeamCarver::findVerticalSeamGraphCut()
{
	// Calculate forward energy for graph cut
	calculateForwardEnergy();

	int rows = forwardEnergyMap.rows;
	int cols = forwardEnergyMap.cols;

	cv::Mat dpTable(rows, cols, CV_64F);
	cv::Mat backtrack(rows, cols, CV_32S);

	// Initialize first row
	forwardEnergyMap.row(0).copyTo(dpTable.row(0));
	for (int j = 0; j < cols; j++)
	{
		backtrack.at<int>(0, j) = -1;
	}

	// Fill DP table using forward energy
	for (int i = 1; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			double minEnergy = std::numeric_limits<double>::max();
			int minPrev = j;

			// Check three possible predecessors
			for (int dj = -1; dj <= 1; dj++)
			{
				int prevJ = j + dj;
				if (prevJ >= 0 && prevJ < cols)
				{
					double energy = dpTable.at<double>(i - 1, prevJ);

					// Add forward energy cost
					energy += forwardEnergyMap.at<double>(i, j);

					// Add diagonal penalty to prefer straight seams
					if (dj != 0)
					{
						energy += std::abs(dj) * 50.0; // Penalty for diagonal moves
					}

					if (energy < minEnergy)
					{
						minEnergy = energy;
						minPrev = prevJ;
					}
				}
			}

			dpTable.at<double>(i, j) = minEnergy;
			backtrack.at<int>(i, j) = minPrev;
		}
	}

	// Find minimum in last row
	std::vector<int> seam(rows);
	double minVal = std::numeric_limits<double>::max();
	int minIdx = 0;

	for (int j = 0; j < cols; j++)
	{
		if (dpTable.at<double>(rows - 1, j) < minVal)
		{
			minVal = dpTable.at<double>(rows - 1, j);
			minIdx = j;
		}
	}

	// Backtrack to find the seam
	seam[rows - 1] = minIdx;
	for (int i = rows - 2; i >= 0; i--)
	{
		seam[i] = backtrack.at<int>(i + 1, seam[i + 1]);
	}

	return seam;
}

std::vector<int> SeamCarver::findHorizontalSeamGraphCut()
{
	// Calculate forward energy for graph cut
	calculateForwardEnergy();

	int rows = forwardEnergyMap.rows;
	int cols = forwardEnergyMap.cols;

	cv::Mat dpTable(rows, cols, CV_64F);
	cv::Mat backtrack(rows, cols, CV_32S);

	// Initialize first column
	for (int i = 0; i < rows; i++)
	{
		dpTable.at<double>(i, 0) = forwardEnergyMap.at<double>(i, 0);
		backtrack.at<int>(i, 0) = -1;
	}

	// Fill DP table using forward energy
	for (int j = 1; j < cols; j++)
	{
		for (int i = 0; i < rows; i++)
		{
			double minEnergy = std::numeric_limits<double>::max();
			int minPrev = i;

			// Check three possible predecessors
			for (int di = -1; di <= 1; di++)
			{
				int prevI = i + di;
				if (prevI >= 0 && prevI < rows)
				{
					double energy = dpTable.at<double>(prevI, j - 1);

					// Add forward energy cost
					energy += forwardEnergyMap.at<double>(i, j);

					// Add diagonal penalty
					if (di != 0)
					{
						energy += std::abs(di) * 50.0;
					}

					if (energy < minEnergy)
					{
						minEnergy = energy;
						minPrev = prevI;
					}
				}
			}

			dpTable.at<double>(i, j) = minEnergy;
			backtrack.at<int>(i, j) = minPrev;
		}
	}

	// Find minimum in last column
	std::vector<int> seam(cols);
	double minVal = std::numeric_limits<double>::max();
	int minIdx = 0;

	for (int i = 0; i < rows; i++)
	{
		if (dpTable.at<double>(i, cols - 1) < minVal)
		{
			minVal = dpTable.at<double>(i, cols - 1);
			minIdx = i;
		}
	}

	// Backtrack to find the seam
	seam[cols - 1] = minIdx;
	for (int j = cols - 2; j >= 0; j--)
	{
		seam[j] = backtrack.at<int>(seam[j + 1], j + 1);
	}

	return seam;
}

void SeamCarver::removeVerticalSeamGraphCut()
{
	std::vector<int> seam = findVerticalSeamGraphCut();
	removeSeam(seam, true);
}

void SeamCarver::removeHorizontalSeamGraphCut()
{
	std::vector<int> seam = findHorizontalSeamGraphCut();
	removeSeam(seam, false);
}

cv::Mat SeamCarver::carveSeamOnceAndShowGraphCut(bool vertical)
{
	cv::Mat newImg = image.clone();
	std::vector<int> seam;

	if (vertical)
	{
		seam = findVerticalSeamGraphCut();
		for (int i = 0; i < image.rows; i++)
		{
			// Show graph cut seams in green
			newImg.at<cv::Vec3b>(i, seam[i]) = cv::Vec3b(0, 255, 0);
		}
	}
	else
	{
		seam = findHorizontalSeamGraphCut();
		for (int i = 0; i < image.cols; i++)
		{
			newImg.at<cv::Vec3b>(seam[i], i) = cv::Vec3b(0, 255, 0);
		}
	}

	removeSeam(seam, vertical);
	return newImg;
}
