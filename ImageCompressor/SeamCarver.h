#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class SeamCarver
{
public:
	SeamCarver(cv::Mat const& img);

	void removeVerticalSeam(bool useGreedy);
	void removeHorizontalSeam(bool useGreedy);
	void removeVerticalSeamGraphCut();
	void removeHorizontalSeamGraphCut();
	cv::Mat getImage() const;
	cv::Mat getEnergyMap() const;
	cv::Mat carveSeamOnceAndShow(bool vertical);
	cv::Mat carveSeamOnceAndShowGraphCut(bool vertical);
	void ResetImage(cv::Mat& img);

private:
	cv::Mat image;
	cv::Mat energyMap;

	void calculateEnergyMap();
	void calculateForwardEnergy();
	std::vector<int> findVerticalSeam();
	std::vector<int> findHorizontalSeam();
	std::vector<int> findVerticalSeamGreedy();
	std::vector<int> findHorizontalSeamGreedy();
	std::vector<int> findVerticalSeamGraphCut();
	std::vector<int> findHorizontalSeamGraphCut();
	void removeSeam(std::vector<int> const& seam, bool vertical);

	cv::Mat forwardEnergyMap;
};
