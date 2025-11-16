#include <opencv2/opencv.hpp>
#include <iostream>
#include "SeamCarver.h"

int main()
{
	try
	{
		cv::Mat image = cv::imread("Images/image.jpg");
		if (image.empty())
		{
			std::cout << "ERROR: Could not load image..." << std::endl;
			return -1;
		}

		std::cout << "Origial size: " << image.cols << "x" << image.rows << std::endl;

		SeamCarver carver(image);

		int seamsToRemove = 50;
		for (int i = 0; i < seamsToRemove; i++)
		{
			carver.removeVerticalSeam();
		}

		cv::Mat result = carver.getImage();
		std::cout << "New size: " << result.cols << "x" << result.rows << std::endl;

		cv::imwrite("Images/compressed.jpg", result);

		cv::imshow("Original", image);
		cv::imshow("Compressed (Carved): ", result);
		cv::waitKey(0);

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: " << e.what() << "..." << std::endl;
		return -1;
	}
}