#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include "SeamCarver.h"

int main()
{
	try
	{
		cv::Mat originalImg = cv::imread("Images/image.jpg");
		if (originalImg.empty())
		{
			std::cout << "ERROR: Could not load image..." << std::endl;
			return -1;
		}

		std::cout << "Origial size: " << originalImg.cols << "x" << originalImg.rows << std::endl;

		int seamsToRemove = 200;
		std::cout << "RUNNING Dynamic Programming..." << std::endl;
		SeamCarver carverDP(originalImg.clone());
		auto startDP = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < seamsToRemove; i++)
		{
			carverDP.removeVerticalSeam(false);
			//carverDP.removeHorizontalSeam(false);
		}

		auto endDP = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> durationDP = endDP - startDP;
		std::cout << "Compressed DP Time: " << durationDP.count() << " seconds..." << std::endl;

		cv::imwrite("Images/compressed.jpg", carverDP.getImage());

		std::cout << "RUNNING Greedy Algorithm..." << std::endl;
		SeamCarver carverGreedy(originalImg.clone());
		auto startGreedy = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < seamsToRemove; i++)
		{
			carverGreedy.removeVerticalSeam(true);
			//carverGreedy.removeHorizontalSeam(true);
		}

		auto endGreedy = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> durationGreedy = endGreedy - startGreedy;
		std::cout << "Compressed Greedy Time: " << durationGreedy.count() << " seconds..." << std::endl;

		cv::imwrite("Images/compressedGreedy.jpg", carverGreedy.getImage());

		cv::Mat dpImg = carverDP.getImage();
		cv::Mat greedyImg = carverGreedy.getImage();

		if (dpImg.rows != originalImg.rows)
		{
			int bottomPadding = originalImg.rows - dpImg.rows;
			cv::copyMakeBorder(dpImg, dpImg, 0, bottomPadding, 0, 0, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
			cv::copyMakeBorder(greedyImg, greedyImg, 0, bottomPadding, 0, 0, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
		}

		cv::putText(originalImg, "Original", cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255), 3);
		cv::putText(dpImg, "DP Carved", cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255), 3);
		cv::putText(greedyImg, "Greedy Carved", cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255), 3);

		int thickness = 20;
		cv::Scalar blackColor(0, 0, 0);

		cv::Mat separator(originalImg.rows, thickness, originalImg.type(), blackColor);

		cv::Mat combined;
		std::vector<cv::Mat> imagesToCombine = { originalImg, separator, dpImg, separator, greedyImg };
		cv::hconcat(imagesToCombine, combined);
		
		cv::namedWindow("Comparison: Original VS DP VS Greedy", cv::WINDOW_NORMAL);
		cv::imshow("Comparison: Original VS DP VS Greedy", combined);

		cv::waitKey(0);

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: " << e.what() << "..." << std::endl;
		return -1;
	}
}