#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class SeamCarver
{
public:
	SeamCarver(cv::Mat const& img);

	void removeVerticalSeam();
	void removeHorizontalSeam();
	cv::Mat getImage() const;
	cv::Mat getEnergyMap() const;

private:
	cv::Mat image;
	cv::Mat energyMap;

	void calculateEnergyMap();
	std::vector<int> findVerticalSeam();
	std::vector<int> findHorizontalSeam();
	void removeSeam(std::vector<int> const& seam, bool vertical);
};
