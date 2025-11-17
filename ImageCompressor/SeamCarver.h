#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class SeamCarver
{
public:
	SeamCarver(cv::Mat const& img);

	void removeVerticalSeam(bool useGreedy);
	void removeHorizontalSeam(bool useGreedy);
	cv::Mat getImage() const;
	cv::Mat getEnergyMap() const;

private:
	cv::Mat image;
	cv::Mat energyMap;

	void calculateEnergyMap();
	std::vector<int> findVerticalSeam();
	std::vector<int> findHorizontalSeam();
	std::vector<int> findVerticalSeamGreedy();
	std::vector<int> findHorizontalSeamGreedy();
	void removeSeam(std::vector<int> const& seam, bool vertical);
};
