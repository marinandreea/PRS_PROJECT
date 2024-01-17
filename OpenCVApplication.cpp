// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"


Mat_<float> dataPreparation(Mat_<Vec3b> img) {

	printf("Sunt in fct de data\n");

	Mat_<Vec3b> hsvImage;
	cv::cvtColor(img, hsvImage, cv::COLOR_BGR2HSV);

	Mat_<float> dst(img.rows * img.cols, 5);
	int k = 0;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			dst(k, 0) = hsvImage(i, j)[0]; // hue
			dst(k, 1) = hsvImage(i, j)[1]; // saturation
			dst(k, 2) = hsvImage(i, j)[2]; // value
			dst(k, 3) = i;
			dst(k, 4) = j;
			k++;
		}
	}

	return dst;

}

double dist(Mat_<float> X, Mat_<double> C) {

	double sum = 0.0;
	for (int i = 0; i < C.cols; i++) {
		//sum += (X(0, i) - C(0, i)) * (X(0, i) - C(0, i)) * (i<3);
		sum += (X(0, i) - C(0, i)) * (X(0, i) - C(0, i));

	}
	double d = sqrt(sum);
	return d;

}

std::pair<std::vector<int>, Mat_<double>> Kmeans(Mat_<float> X, int k) {

	printf("Sunt in fct de Kmeans2\n");
	srand(time(NULL));
	Mat_<double> C(k, X.cols);
	std::vector<int> index(k);

	//initialization
	for (int i = 0; i < k; i++) {
		int ind = rand() % X.rows;

		auto in = std::find(index.begin(), index.end(), ind);
		while (in != index.end()) {
			ind = rand() % X.rows;
			in = std::find(index.begin(), index.end(), ind);
		}
		index.push_back(ind);
		C.row(i) = X.row(ind);
	}

	printf("Am facut initializarea la C\n");


	std::vector<int> L(X.rows);
	int N = 0;
	std::vector<int> L2(X.rows);

	while (N < 10) {
		printf("Am intrat in assignment loop, N: %d\n", N);

		//assignment
		for (int i = 0; i < X.rows; i++) {
			int minDist = 1000;
			int center = -1;
			for (int j = 0; j < C.rows; j++) {
				double d = dist(X.row(i), C.row(j));
				if (d < minDist) {
					minDist = d;
					center = j;
				}
			}
			L[i] = center;
		}

		//update
		for (int i = 0; i < k; i++) {
			std::vector<double> sum(X.cols);
			int nr = 0;
			for (int j = 0; j < L.size();j++) {

				if (L[j] == i) {
					for (int l = 0; l < X.cols;l++) {
						sum[l] += X(j, l);
					}
					nr++;
				}

			}
			for (int t = 0; t < C.cols;t++) {
				C(i, t) = sum[t] / nr;
			}
		}

		//halting
		int ok = 1;
		for (int i = 0; i < L.size();i++) {

			if (L2[i] != L[i]) {
				ok = 0;

			}
		}
		if (ok == 1) {
			N = 11;
		}

		N++;

		for (int i = 0; i < L.size(); i++) {
			L2[i] = L[i];
		}

	}
	printf("Ies din kmeans\n");
	return { L,C };

}

void projectKMeans() {

	//Mat_<Vec3b> imag = imread("ProjectImages\\damiano-baschiera-nijeOSzlaMs-unsplash.jpg", IMREAD_COLOR);
	//Mat_<Vec3b> imag = imread("ProjectImages\\josh-rinard-H9mp1P1VUj4-unsplash.jpg", IMREAD_COLOR);
	//Mat_<Vec3b> imag = imread("ProjectImages\\josh-rinard-TtVchQOLi5g-unsplash.jpg", IMREAD_COLOR);
	//Mat_<Vec3b> imag = imread("ProjectImages\\josh-rinard-XGceNxiv95E-unsplash.jpg", IMREAD_COLOR);
	//Mat_<Vec3b> imag = imread("ProjectImages\\josh-rinard-vj9BCIgFSdY-unsplash.jpg", IMREAD_COLOR);
	//Mat_<Vec3b> imag = imread("ProjectImages\\josh-rinard-kZ19iVCs2yI-unsplash.jpg", IMREAD_COLOR);
	//Mat_<Vec3b> imag = imread("ProjectImages\\josh-rinard-nkvpI-AtM7s-unsplash.jpg", IMREAD_COLOR);
	//Mat_<Vec3b> imag = imread("ProjectImages\\james-wheeler-ZOA-cqKuJAA-unsplash.jpg", IMREAD_COLOR);
	//Mat_<Vec3b> imag = imread("ProjectImages\\miguel-ibanez-cO7zI0lqzqI-unsplash.jpg", IMREAD_COLOR);
	Mat_<Vec3b> imag = imread("ProjectImages\\mickey-o-neil-xL66l--msXU-unsplash.jpg", IMREAD_COLOR);
	//Mat_<Vec3b> imag = imread("ProjectImages\\s-b-vonlanthen-D75_5tWZDQ4-unsplash.jpg", IMREAD_COLOR);


	Mat_<Vec3b> resizedImage;
	cv::resize(imag, resizedImage, cv::Size(500, 500));

	Mat_<float> X = dataPreparation(resizedImage);

	printf("Kmeans:\n");
	std::pair<std::vector<int>, Mat_<double>>  p = Kmeans(X, 30);
	std::cout << p.second;

	Mat_<Vec3b> cImage(resizedImage.rows, resizedImage.cols);
	cImage.setTo(Scalar(255, 255, 255));

	for (int i = 0; i < X.rows; i++) {
		Vec3b hsvColor;
		int cluster = p.first[i];
		hsvColor[0] = p.second(cluster, 0); // Hue
		hsvColor[1] = p.second(cluster, 1); // Saturation
		hsvColor[2] = p.second(cluster, 2); // Value

		cImage(X(i, 3), X(i, 4)) = hsvColor;
	}
	Mat_<Vec3b> finalImage(resizedImage.rows, resizedImage.cols);
	cv::cvtColor(cImage, finalImage, cv::COLOR_HSV2BGR);

	imshow("initial image", resizedImage);
	imshow("output image", finalImage);
	waitKey(0);
}







int main()
{

	projectKMeans();

	return 0;

}