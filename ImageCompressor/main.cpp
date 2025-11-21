#include <opencv2/opencv.hpp>
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include <iostream>
#include <chrono>
#include "SeamCarver.h"

#define WINDOW_NAME "Seam Carving Project"
#define COMPARISM_WINDOW_NAME "Comparison Dynamic & Greedy"
#define VISUALIZE_WINDOW_NAME "Visualize Seam Carving"

static double GreedyTimeTaken = 0.0;
static double DynamicTimeTaken = 0.0;

int SeamCarveComparism(cv::Mat& finalImg);

int main()
{
	try
	{
		// Disables all warning that are not errors
		cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

		cv::Mat window = cv::Mat(600, 800, CV_8UC3);
		/******************* Comparism Betwn Dynamic & Greedy *******************/
		cv::Mat cmpImg;
		cv::Mat cmpImgResize;
		bool showImageGenerated = false;
		/******************* Showcase Seam Carving *******************/
		cv::Mat originalImg = cv::imread("Images/image.jpg");
		if (originalImg.empty())
		{
			std::cerr << "ERROR: Could not load Images/image.jpg for visualization" << std::endl;
			std::cerr << "Please ensure the image file exists in the Images folder" << std::endl;
			return -1;
		}
		SeamCarver visualizeCarverDP(originalImg.clone());
		cv::Mat visualizeImgSeamCarve = originalImg;
		cv::Mat visualizeImgSeamCarveResize;
		cv::Vec2i currentSeamCarvedDimensions{ originalImg.size().width, originalImg.size().height };

		cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
		cv::namedWindow(VISUALIZE_WINDOW_NAME, cv::WINDOW_NORMAL);
		cvui::init(WINDOW_NAME);
		while (cv::getWindowProperty(WINDOW_NAME, cv::WND_PROP_VISIBLE) >= 0.5)
		{
			cvui::context(WINDOW_NAME);
			// Set window Background color
			window = cv::Scalar(49, 52, 49);

			cvui::beginColumn(window, 50, 50, -1, -1, 10);

			cvui::printf(0.5, 0xff0000, "Comparison: Dynamic vs Greedy");
			cvui::beginRow(-1, -1, 20);
			if (cvui::button("Generate Start"))
			{
				if (!showImageGenerated)
				{
					if (SeamCarveComparism(cmpImg) == 1)
					{
						showImageGenerated = true;
						cv::namedWindow(COMPARISM_WINDOW_NAME, cv::WINDOW_NORMAL);
					}

				}
			}
			if(showImageGenerated)
				cvui::printf(0.5, 0xffffff, "Time Taken Dynamic: %lf, Greedy: %lf", DynamicTimeTaken, GreedyTimeTaken);
			cvui::endRow();

			cvui::printf(0.5, 0xff0000, "Visualize Seam cut (last cut)");
			cvui::printf(0.5, 0xffffff, "Current Dimensions = [%d, %d]", currentSeamCarvedDimensions[0], currentSeamCarvedDimensions[1]);
			cvui::beginRow(-1, -1, 20);
			if (cvui::button("Shrink Width x1"))
			{
				visualizeImgSeamCarve = visualizeCarverDP.carveSeamOnceAndShow(true);
				--currentSeamCarvedDimensions[0];
			}
			if (cvui::button("x10"))
			{
				for (int i = 0; i < 9; ++i)
					visualizeCarverDP.removeVerticalSeam(false);
				visualizeImgSeamCarve = visualizeCarverDP.carveSeamOnceAndShow(true);
				currentSeamCarvedDimensions[0] -= 10;
			}
			if (cvui::button("x100"))
			{
				for (int i = 0; i < 99; ++i)
					visualizeCarverDP.removeVerticalSeam(false);
				visualizeImgSeamCarve = visualizeCarverDP.carveSeamOnceAndShow(true);
				currentSeamCarvedDimensions[0] -= 100;
			}
			cvui::endRow();
			cvui::beginRow(-1, -1, 20);
			if (cvui::button("Shrink Height x1"))
			{
				visualizeImgSeamCarve = visualizeCarverDP.carveSeamOnceAndShow(false);
				--currentSeamCarvedDimensions[1];
			}
			if (cvui::button("x10"))
			{
				for (int i = 0; i < 9; ++i)
					visualizeCarverDP.removeHorizontalSeam(false);
				visualizeImgSeamCarve = visualizeCarverDP.carveSeamOnceAndShow(false);
				currentSeamCarvedDimensions[1] -= 10;
			}
			if (cvui::button("x100"))
			{
				for (int i = 0; i < 99; ++i)
					visualizeCarverDP.removeHorizontalSeam(false);
				visualizeImgSeamCarve = visualizeCarverDP.carveSeamOnceAndShow(false);
				currentSeamCarvedDimensions[1] -= 100;
			}
			cvui::endRow();

			cvui::printf(0.5, 0x00ff00, "Graph Cut (Forward Energy):");
			cvui::beginRow(-1, -1, 20);
			if (cvui::button("GC Width x1"))
			{
				visualizeImgSeamCarve = visualizeCarverDP.carveSeamOnceAndShowGraphCut(true);
				--currentSeamCarvedDimensions[0];
			}
			if (cvui::button("GC x10"))
			{
				for (int i = 0; i < 9; ++i)
					visualizeCarverDP.removeVerticalSeamGraphCut();
				visualizeImgSeamCarve = visualizeCarverDP.carveSeamOnceAndShowGraphCut(true);
				currentSeamCarvedDimensions[0] -= 10;
			}
			if (cvui::button("GC x100"))
			{
				for (int i = 0; i < 99; ++i)
					visualizeCarverDP.removeVerticalSeamGraphCut();
				visualizeImgSeamCarve = visualizeCarverDP.carveSeamOnceAndShowGraphCut(true);
				currentSeamCarvedDimensions[0] -= 100;
			}
			cvui::endRow();
			cvui::beginRow(-1, -1, 20);
			if (cvui::button("GC Height x1"))
			{
				visualizeImgSeamCarve = visualizeCarverDP.carveSeamOnceAndShowGraphCut(false);
				--currentSeamCarvedDimensions[1];
			}
			if (cvui::button("GC x10"))
			{
				for (int i = 0; i < 9; ++i)
					visualizeCarverDP.removeHorizontalSeamGraphCut();
				visualizeImgSeamCarve = visualizeCarverDP.carveSeamOnceAndShowGraphCut(false);
				currentSeamCarvedDimensions[1] -= 10;
			}
			cvui::endRow();

			if (cvui::button("Reset"))
			{
				visualizeCarverDP.ResetImage(originalImg);
				visualizeImgSeamCarve = originalImg;
				currentSeamCarvedDimensions[0] = originalImg.size().width;
				currentSeamCarvedDimensions[1] = originalImg.size().height;
			}

			if (cvui::button("Exit Program"))
			{
				break;
			}

			cvui::endColumn();

			/******************* Showcase Seam Carving Window *******************/
			if (cv::getWindowProperty(VISUALIZE_WINDOW_NAME, cv::WND_PROP_VISIBLE) >= 0.5)
			{
				cvui::context(VISUALIZE_WINDOW_NAME);
				auto dim = cv::getWindowImageRect(VISUALIZE_WINDOW_NAME);

				cvui::update(VISUALIZE_WINDOW_NAME);
				// Only resize if window dimensions are valid (non-zero)
				if (dim.width > 0 && dim.height > 0)
				{
					cv::resize(visualizeImgSeamCarve, visualizeImgSeamCarveResize, cv::Size(dim.width, dim.height));
					cvui::imshow(VISUALIZE_WINDOW_NAME, visualizeImgSeamCarveResize);
				}
				else
				{
					// Display original image if window dimensions are invalid
					cvui::imshow(VISUALIZE_WINDOW_NAME, visualizeImgSeamCarve);
				}
			}
			else
			{
				cv::namedWindow(VISUALIZE_WINDOW_NAME, cv::WINDOW_NORMAL);
			}

			/******************* Comparism Betwn Dynamic & Greedy Window *******************/
			if (showImageGenerated)
			{
				if (cv::getWindowProperty(COMPARISM_WINDOW_NAME, cv::WND_PROP_VISIBLE) >= 0.5)
				{
					cvui::context(COMPARISM_WINDOW_NAME);
					auto dim = cv::getWindowImageRect(COMPARISM_WINDOW_NAME);

					cvui::update(COMPARISM_WINDOW_NAME);
					// Only resize if window dimensions are valid (non-zero)
					if (dim.width > 0 && dim.height > 0)
					{
						cv::resize(cmpImg, cmpImgResize, cv::Size(dim.width, dim.height));
						cvui::imshow(COMPARISM_WINDOW_NAME, cmpImgResize);
					}
					else
					{
						// Display original image if window dimensions are invalid
						cvui::imshow(COMPARISM_WINDOW_NAME, cmpImg);
					}
				}
				else
					showImageGenerated = false;
			}

			cvui::update(WINDOW_NAME);
			cv::imshow(WINDOW_NAME, window);

			if (cv::waitKey(20) == 27)
				break;
		}

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: " << e.what() << "..." << std::endl;
		return -1;
	}
}

int SeamCarveComparism(cv::Mat& finalImg)
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
	DynamicTimeTaken = durationDP.count();

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
	GreedyTimeTaken = durationGreedy.count();

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

	//cv::Mat combined;
	std::vector<cv::Mat> imagesToCombine = { originalImg, separator, dpImg, separator, greedyImg };
	cv::hconcat(imagesToCombine, finalImg);

	//cv::namedWindow("Comparison: Original VS DP VS Greedy", cv::WINDOW_NORMAL);
	//cv::imshow("Comparison: Original VS DP VS Greedy", combined);

	//cv::waitKey(20);
	return 1;
}
