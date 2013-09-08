#include "ImageProcessor.h"
#include "Maths.h"

#include <vector>
#include <iostream>

void ImageProcessor::bayerRGGBToI420(unsigned char* input, unsigned char* outputY, unsigned char* outputU, unsigned char* outputV, int width, int height) {
	int strideY = width;
	int strideU = (width + 1) / 2;
	int strideV = (width + 1) / 2;
	
	libyuv::BayerRGGBToI420(
		input,
		width,
		outputY,
		strideY,
		outputU,
		strideU,
		outputV,
		strideV,
		width,
		height
	);
}

void ImageProcessor::I420ToYUYV(unsigned char* inputY, unsigned char* inputU, unsigned char* inputV, unsigned char* output, int width, int height) {
	int strideY = width;
	int strideU = (width + 1) / 2;
	int strideV = (width + 1) / 2;
	int outputStride = width * 2;
	
	libyuv::I420ToYUY2(
		inputY, strideY,
		inputU, strideU,
		inputV, strideV,
		output,
		outputStride,
		width, height
	);
}

void ImageProcessor::YUYVToARGB(unsigned char* input, unsigned char* output, int width, int height) {
	int inputStride = width * 2;
	int outputStride = width * 4;

	libyuv::YUY2ToARGB(
		input, inputStride,
		output, outputStride,
		width, height
	);
}

void ImageProcessor::ARGBToRGB24(unsigned char* input, unsigned char* output, int width, int height) {
	int inputStride = width * 4;
	int outputStride = width * 3;

	libyuv::ARGBToRGB24(
		input, inputStride,
		output, outputStride,
		width, height
	);
}

ImageProcessor::YUYV* ImageProcessor::getYuyvPixelAt(unsigned char* image, int width, int height, int x, int y) {
	if (
		x < 0
		|| x > width - 1
		|| y < 0
		|| y > height - 1
	) {
		return NULL;
	}

	YUYV* pixel = new YUYV();

	int strideY = width;
	int strideU = (width + 1) / 2;
	int strideV = (width + 1) / 2;

	int yPos = strideY * y + x;
	int uvPos = strideU * (y / 2) + (x / 2);

	int stride = width * 1;

	pixel->y1 = image[yPos];
	pixel->u = image[uvPos];
	pixel->y2 = image[yPos];
	pixel->v = image[uvPos];

    return pixel;
}

ImageProcessor::YUYVRange ImageProcessor::extractColorRange(unsigned char* image, int width, int height, std::string colorName, int centerX, int centerY, int brushRadius, float stdDev) {
	int Y, U, V;
	std::vector<float> yValues;
	std::vector<float> uValues;
	std::vector<float> vValues;

	std::cout << "extractColorRange " << width <<  "x" << height << ", " << centerX << "x" << centerY << ", " << brushRadius << ", " << stdDev << std::endl;
	
	for (int x = -brushRadius; x < brushRadius; x++) {
		int height = (int)::sqrt(brushRadius * brushRadius - x * x);

		for (int y = -height; y < height; y++) {
			if (
				x + centerX < 0
				|| x + centerX > width - 1
				|| y + centerY < 0
				|| y + centerY > height -1
			) {
				std::cout << "Skip pixel at " << (x + centerX) << "x" << (y + centerY) << std::endl;

				continue;
			}

			YUYV* pixel = getYuyvPixelAt(image, width, height, x + centerX, y + centerY);

			if (pixel != NULL) {
				Y = (pixel->y1 + pixel->y2) / 2;
				U = pixel->u;
				V = pixel->v;

				delete pixel;

				yValues.push_back((float)Y);
				uValues.push_back((float)U);
				vValues.push_back((float)V);

				std::cout << "Y: " << Y << std::endl;
			} else {
				std::cout << "Didn't get pixel at " << (x + centerX) << "x" << (y + centerY) << std::endl;
			}
		}
	}

	float yMean, uMean, vMean;
	float yStdDev = Math::standardDeviation(yValues, yMean);
	float uStdDev = Math::standardDeviation(uValues, uMean);
	float vStdDev = Math::standardDeviation(vValues, vMean);

	YUYVRange range;

	range.minY = (int)(yMean - (float)yStdDev * stdDev);
	range.maxY = (int)(yMean + (float)yStdDev * stdDev);
	range.minU = (int)(uMean - (float)uStdDev * stdDev);
	range.maxU = (int)(uMean + (float)uStdDev * stdDev);
	range.minV = (int)(vMean - (float)vStdDev * stdDev);
	range.maxV = (int)(vMean + (float)vStdDev * stdDev);

	return range;
}