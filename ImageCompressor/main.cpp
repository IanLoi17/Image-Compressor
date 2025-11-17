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

		int seamsToRemove = 50;
		std::cout << "RUNNING Dynamic Programming..." << std::endl;
		SeamCarver carverDP(originalImg.clone());
		auto startDP = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < seamsToRemove; i++)
		{
			carverDP.removeVerticalSeam(false);
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
		}

		auto endGreedy = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> durationGreedy = endGreedy - startGreedy;
		std::cout << "Compressed Greedy Time: " << durationGreedy.count() << " seconds..." << std::endl;

		cv::imwrite("Images/compressedGreedy.jpg", carverGreedy.getImage());

		cv::imshow("Original", originalImg);
		cv::imshow("Compressed (Carved DP): ", carverDP.getImage());
		cv::imshow("Compressed (Carved Greedy): ", carverGreedy.getImage());
		cv::waitKey(0);

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: " << e.what() << "..." << std::endl;
		return -1;
	}
}