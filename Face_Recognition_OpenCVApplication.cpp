// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include<queue>
#include<random>
using namespace std;
float medie;

typedef struct {
	Point p;
	int dir;
}chainCode;


void testOpenImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image",src);
		waitKey();
	}
}

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName)==0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName,"bmp");
	while(fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(),src);
		if (waitKey()==27) //ESC pressed
			break;
	}
}

void testImageOpenAndSave()
{
	Mat src, dst;

	src = imread("Images/Lena_24bits.bmp", CV_LOAD_IMAGE_COLOR);	// Read the image

	if (!src.data)	// Check for invalid input
	{
		printf("Could not open or find the image\n");
		return;
	}

	// Get the image resolution
	Size src_size = Size(src.cols, src.rows);

	// Display window
	const char* WIN_SRC = "Src"; //window for the source image
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Dst"; //window for the destination (processed) image
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, src_size.width + 10, 0);

	cvtColor(src, dst, CV_BGR2GRAY); //converts the source image to a grayscale one

	imwrite("Images/Lena_24bits_gray.bmp", dst); //writes the destination to file

	imshow(WIN_SRC, src);
	imshow(WIN_DST, dst);

	printf("Press any key to continue ...\n");
	waitKey(0);
}

void testNegativeImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]
		
		Mat src = imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height,width,CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
			{
				uchar val = src.at<uchar>(i,j);
				uchar neg = 255 - val;
				dst.at<uchar>(i,j) = neg;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image",src);
		imshow("negative image",dst);
		waitKey();
	}
}

void testParcurgereSimplaDiblookStyle()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = src.clone();

		double t = (double)getTickCount(); // Get the current time [s]

		// the fastest approach using the ?diblook style?
		uchar *lpSrc = src.data;
		uchar *lpDst = dst.data;
		int w = (int) src.step; // no dword alignment is done !!!
		for (int i = 0; i<height; i++)
			for (int j = 0; j < width; j++) {
				uchar val = lpSrc[i*w + j];
				lpDst[i*w + j] = 255 - val;
			}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image",src);
		imshow("negative image",dst);
		waitKey();
	}
}

void testColor2Gray()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src = imread(fname);

		int height = src.rows;
		int width = src.cols;

		Mat dst = Mat(height,width,CV_8UC1);

		// Asa se acceseaaza pixelii individuali pt. o imagine RGB 24 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
			{
				Vec3b v3 = src.at<Vec3b>(i,j);
				uchar b = v3[0];
				uchar g = v3[1];
				uchar r = v3[2];
				dst.at<uchar>(i,j) = (r+g+b)/3;
			}
		}
		
		imshow("input image",src);
		imshow("gray image",dst);
		waitKey();
	}
}

void testBGR2HSV()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;

		// Componentele d eculoare ale modelului HSV
		Mat H = Mat(height, width, CV_8UC1);
		Mat S = Mat(height, width, CV_8UC1);
		Mat V = Mat(height, width, CV_8UC1);

		// definire pointeri la matricele (8 biti/pixeli) folosite la afisarea componentelor individuale H,S,V
		uchar* lpH = H.data;
		uchar* lpS = S.data;
		uchar* lpV = V.data;

		Mat hsvImg;
		cvtColor(src, hsvImg, CV_BGR2HSV);

		// definire pointer la matricea (24 biti/pixeli) a imaginii HSV
		uchar* hsvDataPtr = hsvImg.data;

		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				int hi = i*width * 3 + j * 3;
				int gi = i*width + j;

				lpH[gi] = hsvDataPtr[hi] * 510 / 360;		// lpH = 0 .. 255
				lpS[gi] = hsvDataPtr[hi + 1];			// lpS = 0 .. 255
				lpV[gi] = hsvDataPtr[hi + 2];			// lpV = 0 .. 255
			}
		}

		imshow("input image", src);
		imshow("H", H);
		imshow("S", S);
		imshow("V", V);

		waitKey();
	}
}

void testResize()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		Mat dst1,dst2;
		//without interpolation
		resizeImg(src,dst1,320,false);
		//with interpolation
		resizeImg(src,dst2,320,true);
		imshow("input image",src);
		imshow("resized image (without interpolation)",dst1);
		imshow("resized image (with interpolation)",dst2);
		waitKey();
	}
}

void testCanny()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src,dst,gauss;
		src = imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		double k = 0.4;
		int pH = 50;
		int pL = (int) k*pH;
		GaussianBlur(src, gauss, Size(5, 5), 0.8, 0.8);
		Canny(gauss,dst,pL,pH,3);
		imshow("input image",src);
		imshow("canny",dst);
		waitKey();
	}
}

void testVideoSequence()
{
	VideoCapture cap("Videos/rubic.avi"); // off-line video from file
	//VideoCapture cap(0);	// live video from web cam
	if (!cap.isOpened()) {
		printf("Cannot open video capture device.\n");
		waitKey(0);
		return;
	}
		
	Mat edges;
	Mat frame;
	char c;

	while (cap.read(frame))
	{
		Mat grayFrame;
		cvtColor(frame, grayFrame, CV_BGR2GRAY);
		Canny(grayFrame,edges,40,100,3);
		imshow("source", frame);
		imshow("gray", grayFrame);
		imshow("edges", edges);
		c = cvWaitKey(0);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished\n"); 
			break;  //ESC pressed
		};
	}
}


void testSnap()
{
	VideoCapture cap(0); // open the deafult camera (i.e. the built in web cam)
	if (!cap.isOpened()) // openenig the video device failed
	{
		printf("Cannot open video capture device.\n");
		return;
	}

	Mat frame;
	char numberStr[256];
	char fileName[256];
	
	// video resolution
	Size capS = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	// Display window
	const char* WIN_SRC = "Src"; //window for the source frame
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Snapped"; //window for showing the snapped frame
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, capS.width + 10, 0);

	char c;
	int frameNum = -1;
	int frameCount = 0;

	for (;;)
	{
		cap >> frame; // get a new frame from camera
		if (frame.empty())
		{
			printf("End of the video file\n");
			break;
		}

		++frameNum;
		
		imshow(WIN_SRC, frame);

		c = cvWaitKey(10);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished");
			break;  //ESC pressed
		}
		if (c == 115){ //'s' pressed - snapp the image to a file
			frameCount++;
			fileName[0] = NULL;
			sprintf(numberStr, "%d", frameCount);
			strcat(fileName, "Images/A");
			strcat(fileName, numberStr);
			strcat(fileName, ".bmp");
			bool bSuccess = imwrite(fileName, frame);
			if (!bSuccess) 
			{
				printf("Error writing the snapped image\n");
			}
			else
				imshow(WIN_DST, frame);
		}
	}

}

void MyCallBackFunc(int event, int x, int y, int flags, void* param)
{
	//More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
	Mat* src = (Mat*)param;
	if (event == CV_EVENT_LBUTTONDOWN)
		{
			printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
				x, y,
				(int)(*src).at<Vec3b>(y, x)[2],
				(int)(*src).at<Vec3b>(y, x)[1],
				(int)(*src).at<Vec3b>(y, x)[0]);
		}
}

void testMouseClick()
{
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", MyCallBackFunc, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}
}

/* Histogram display function - display a histogram using bars (simlilar to L3 / PI)
Input:
name - destination (output) window name
hist - pointer to the vector containing the histogram values
hist_cols - no. of bins (elements) in the histogram = histogram image width
hist_height - height of the histogram image
Call example:
showHistogram ("MyHist", hist_dir, 255, 200);
*/
void showHistogram(const std::string& name, int* hist, const int  hist_cols, const int hist_height)
{
	Mat imgHist(hist_height, hist_cols, CV_8UC3, CV_RGB(255, 255, 255)); // constructs a white image

	//computes histogram maximum
	int max_hist = 0;
	for (int i = 0; i<hist_cols; i++)
	if (hist[i] > max_hist)
		max_hist = hist[i];
	double scale = 1.0;
	scale = (double)hist_height / max_hist;
	int baseline = hist_height - 1;

	for (int x = 0; x < hist_cols; x++) {
		Point p1 = Point(x, baseline);
		Point p2 = Point(x, baseline - cvRound(hist[x] * scale));
		line(imgHist, p1, p2, CV_RGB(255, 0, 255)); // histogram bins colored in magenta
	}

	imshow(name, imgHist);
}
bool Inauntru(Mat img, int i, int j)
{
	if (img.rows <= i || i < 0 || img.cols <= j || j < 0)
		return 0;
	return 1;
}
Mat dilatare1(Mat src)
{
	Mat dst = Mat(src.rows, src.cols, CV_8UC1);

	int di[] = { 0, -1, -1, -1, 0, 1, 1, 1 };
	int dj[] = { 1, 1, 0, -1, -1, -1, 0, 1 };

	for (int i = 0; i < dst.rows; i++)
		for (int j = 0; j < dst.cols; j++)
		{
			dst.at<uchar>(i, j) = 255;
		}

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
			if (src.at<uchar>(i, j) == 0)
			{
				dst.at<uchar>(i, j) = 0;




				for (int k = 0; k < 9; k++)
					if (Inauntru(dst, i + di[k], j + dj[k]))
					{
						dst.at<uchar>(i + di[k], j + dj[k]) = 0;
					}

			}
	return dst;
}

Mat eroziune1(Mat src)
{
	Mat dst = Mat(src.rows, src.cols, CV_8UC1);

	int di[] = { 0, -1, -1, -1, 0, 1, 1, 1 };
	int dj[] = { 1, 1, 0, -1, -1, -1, 0, 1 };

	for (int i = 0; i < dst.rows; i++)
		for (int j = 0; j < dst.cols; j++)
		{
			dst.at<uchar>(i, j) = 255;
		}

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			if (src.at<uchar>(i, j) == 0)
			{
				int ok = 1;



				for (int k = 0; k < 9; k++)
				{
					if (Inauntru(dst, i + di[k], j + dj[k]))
					{
						if (src.at<uchar>(i + di[k], j + dj[k]) != 0) {
							ok = 0;
						}
					}
				}

				if (ok == 1) {
					dst.at<uchar>(i, j) = 0;
				}
			}
		}
	return dst;

}
Mat RGB2HSVProiect(Mat src)
{

	float r, g, b, m, M, C, V, S, H;

	Mat h = Mat(src.rows, src.cols, CV_8UC1);
	Mat s = Mat(src.rows, src.cols, CV_8UC1);
	Mat v = Mat(src.rows, src.cols, CV_8UC1);
	Mat dst = Mat(src.rows, src.cols, CV_8UC3);
	Vec3b pixel;
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			r = (float)src.at<Vec3b>(i, j)[0] / 255;
			g = (float)src.at<Vec3b>(i, j)[1] / 255;
			b = (float)src.at<Vec3b>(i, j)[2] / 255;

			M = max(r, max(g, b));
			m = min(r, min(g, b));

			C = M - m;

			V = M;
			if (V != 0)
			{
				S = C / V;
			}
			else
			{
				S = 0;

			}
			if (C != 0)
			{
				if (M == r)
					H = 60 * (g - b) / C;
				if (M == g)
					H = 120 + 60 * (b - r) / C;
				if (M == b)
					H = 240 + 60 * (r - g) / C;
			}
			else

				H = 0;
			if (H < 0)
				H = H + 360;

			pixel[0] = H * 255 / 360;
			pixel[1] = S * 255;
			pixel[2] = V * 255;

			dst.at<Vec3b>(i, j) = pixel;


			//h.at<uchar>(i, j) = H * 255 / 360;
			//s.at<uchar>(i, j) = S * 255;
			//v.at<uchar>(i, j) = V * 255;
		}
	return dst;



}

Mat negative(Mat src)
{
	Mat dst2 = Mat(src.rows, src.cols, CV_8UC1);

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			uchar val = src.at<uchar>(i, j);
			uchar neg = 255 - val;
			dst2.at<uchar>(i, j) = neg;
		}
	return dst2;
}
/*
Mat testNegativeImage1(Mat src)
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				uchar val = src.at<uchar>(i, j);
				uchar neg = 255 - val;
				dst.at<uchar>(i, j) = neg;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms]
		printf("Time = %.3f [ms]\n", t * 1000);

		return dst;
	}
}
*/
Mat etichetare1(Mat src)
{
	Mat MatriceEticheta = Mat::zeros(src.rows, src.cols, CV_32SC1);
	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
	int di[] = { -1,-1,-1,0,0,1,1,1 };
	int dj[] = { -1,0,1,-1,1,-1,0,1 };
	//int labels[][] = { 0 };
	int vi = 0;
	int vj = 0;

	//int matricePuncteStangaSus[2][100];
	//int matricePuncteDreaptaJos[2][100];
//	for (int i = 0; i < 2; i++)
		//for (int j = 0; j < 100; j++) {
		//	matricePuncteStangaSus[i][j] = 0;
		//	matricePuncteDreaptaJos[i][j] = 0;
	//	}

//	int cnt = 0;


	int nrEtichete = 0;
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++) {
			if (src.at<uchar>(i, j) == 0 && MatriceEticheta.at<int>(i, j) == 0) {
				nrEtichete++;
				MatriceEticheta.at<int>(i, j) = nrEtichete;
				queue<Point> coada;
				coada.push({ j, i });
				//		matricePuncteStangaSus[0][cnt] = i;
				//		matricePuncteStangaSus[1][cnt] = j;
				while (!coada.empty()) {
					Point pct = coada.front(); coada.pop();
					//			matricePuncteDreaptaJos[0][cnt] = pct.x;
					//			matricePuncteDreaptaJos[1][cnt] = pct.y;
					for (int k = 0; k < 8; k++) {
						vi = di[k];
						vj = dj[k];
						if (Inauntru(src, pct.y + vi, pct.x + vj))
							if (src.at<uchar>(pct.y + vi, pct.x + vj) == 0 && MatriceEticheta.at<int>(pct.y + vi, pct.x + vj) == 0)
							{
								coada.push(Point(pct.x + vj, pct.y + vi));
								MatriceEticheta.at<int>(pct.y + vi, pct.x + vj) = nrEtichete;
							}
					}

				}
				//cnt++;
			}

		}

	default_random_engine gen;
	uniform_int_distribution<int> d(0, 255);
	Vec3b *colors = (Vec3b*)malloc(nrEtichete * sizeof(Vec3b));

	for (int i = 0; i < nrEtichete; i++)
	{
		colors[i].val[0] = d(gen);
		colors[i].val[1] = d(gen);
		colors[i].val[2] = d(gen);
	}

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			if (src.at<uchar>(i, j) != 255)
				dst.at<Vec3b>(i, j) = colors[MatriceEticheta.at<int>(i, j)];
			else
				dst.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
		}
	//for (int j = matricePuncteStangaSus[1][cnt]; j < matricePuncteDreaptaJos[1][cnt]; j++)
		//dst.at<Vec3b>(matricePuncteStangaSus[0][cnt], j) = Vec3b(0, 0, 0);

	return dst;
}
Mat etichetare_fata(Mat src)
{
	Mat MatriceEticheta = Mat::zeros(src.rows, src.cols, CV_32SC1);
	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
	int di[] = { -1,-1,-1,0,0,1,1,1 };
	int dj[] = { -1,0,1,-1,1,-1,0,1 };
	//int labels[][] = { 0 };
	int vi = 0;
	int vj = 0;
	int vecArie[256] = { 0 };

	//int matricePuncteStangaSus[2][100];
	//int matricePuncteDreaptaJos[2][100];
//	for (int i = 0; i < 2; i++)
		//for (int j = 0; j < 100; j++) {
		//	matricePuncteStangaSus[i][j] = 0;
		//	matricePuncteDreaptaJos[i][j] = 0;
	//	}

//	int cnt = 0;


	int nrEtichete = 0;
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++) {
			if (src.at<uchar>(i, j) == 0 && MatriceEticheta.at<int>(i, j) == 0) {
				nrEtichete++;
				MatriceEticheta.at<int>(i, j) = nrEtichete;
				queue<Point> coada;
				coada.push({ j, i });
				//		matricePuncteStangaSus[0][cnt] = i;
				//		matricePuncteStangaSus[1][cnt] = j;
				while (!coada.empty()) {
					Point pct = coada.front(); coada.pop();
					//			matricePuncteDreaptaJos[0][cnt] = pct.x;
					//			matricePuncteDreaptaJos[1][cnt] = pct.y;
					for (int k = 0; k < 8; k++) {
						vi = di[k];
						vj = dj[k];
						if (Inauntru(src, pct.y + vi, pct.x + vj))
							if (src.at<uchar>(pct.y + vi, pct.x + vj) == 0 && MatriceEticheta.at<int>(pct.y + vi, pct.x + vj) == 0)
							{
								coada.push(Point(pct.x + vj, pct.y + vi));
								MatriceEticheta.at<int>(pct.y + vi, pct.x + vj) = nrEtichete;
								vecArie[MatriceEticheta.at<int>(pct.y + vi, pct.x + vj)]++;
							}
					}

				}
				//cnt++;
			}

		}

	default_random_engine gen;
	uniform_int_distribution<int> d(0, 255);
	Vec3b *colors = (Vec3b*)malloc(nrEtichete * sizeof(Vec3b));

	for (int i = 0; i < nrEtichete; i++)
	{
		colors[i].val[0] = d(gen);
		colors[i].val[1] = d(gen);
		colors[i].val[2] = d(gen);
	}

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			if (src.at<uchar>(i, j) != 255 && vecArie[MatriceEticheta.at<int>(i, j)] > 20000)
				dst.at<Vec3b>(i, j) = colors[MatriceEticheta.at<int>(i, j)];
			else
				dst.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
		}
	//for (int j = matricePuncteStangaSus[1][cnt]; j < matricePuncteDreaptaJos[1][cnt]; j++)
		//dst.at<Vec3b>(matricePuncteStangaSus[0][cnt], j) = Vec3b(0, 0, 0);

	for (int i = 0; i < 256; i++)
		printf("%d ", vecArie[i]);
	return dst;
}

void detectarePiele()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat src = imread(fname, CV_8SC3);
		Mat MatriceEticheta = Mat::zeros(src.rows, src.cols, CV_32SC1);
		//Mat hsv;
		Mat dst = RGB2HSVProiect(src);
		Mat dst2 = Mat(dst.rows, dst.cols, CV_8UC1);
		Mat dst3 = Mat(dst.rows, dst.cols, CV_8UC1);
		Mat dst4 = Mat(dst.rows, dst.cols, CV_8UC1);
		Mat dstNegativ = Mat(src.rows, src.cols, CV_8UC1);
		Mat dstNegativ1 = Mat(src.rows, src.cols, CV_8UC1);
		Mat dstNegativ2 = Mat(src.rows, src.cols, CV_8UC1);
		Mat dstDilatat1 = Mat(src.rows, src.cols, CV_8UC1);
		Mat dstDilatat2 = Mat(src.rows, src.cols, CV_8UC1);
		Mat dstDilatat3 = Mat(src.rows, src.cols, CV_8UC1);
		Mat dstEtichetare = Mat(src.rows, src.cols, CV_8UC1);
		Mat dstEtichetareFata = Mat(src.rows, src.cols, CV_8UC1);


		int di[] = { 0, -1, -1, -1, 0, 1, 1, 1 };
		int dj[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
		Vec3b pixel;



		for (int i = 0; i < src.rows; i++)
			for (int j = 0; j < src.cols; j++) {

				if (dst.at<Vec3b>(i, j)[0] > 150 && dst.at<Vec3b>(i, j)[0] < 200)
					dst2.at<uchar>(i, j) = 255;
				else
					dst2.at<uchar>(i, j) = 0;

			}
		dst3 = eroziune1(dst2);
		//dst4 = etichetare1(dst2);
		dstNegativ = negative(dst3);
		dstNegativ1 = eroziune1(dstNegativ);
		dstNegativ2 = eroziune1(dstNegativ1);
		dstDilatat1 = dilatare1(dstNegativ2);
		dstDilatat2 = dilatare1(dstDilatat1);
		dstDilatat3 = dilatare1(dstDilatat2);
		dstEtichetare = etichetare1(dstDilatat3);
		dstEtichetareFata = etichetare_fata(dstDilatat3);


		imshow("src", src);
		imshow("HSV", dst);
		imshow("HSV Binarizata", dst2);
		imshow("Eroziune", dst3);
		imshow("Negativ", dstNegativ);
		imshow("Negativ eroziune", dstNegativ1);
		imshow("Negativ eroziuine 2", dstNegativ2);
		imshow("Dilatare 1", dstDilatat1);
		imshow("Dilatare 2", dstDilatat2);
		imshow("Dilatare 3", dstDilatat3);
		imshow("Etichetare", dstEtichetare);
		imshow("Etichetare fata", dstEtichetareFata);

		//imshow("Etichetare", dst4);

		//imshow("Deschidere", dst4);
		waitKey(0);
		//varianta noastra operatii morfologice,etichetare
		//negativ apoi etichetare
		//posibil factor subtiere pt cerc
		//varianta2:key min,clusterizare
	}

}

int main()
{
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Open image\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - Image negative - diblook style\n");
		printf(" 4 - BGR->HSV\n");
		printf(" 5 - Resize image\n");
		printf(" 6 - Canny edge detection\n");
		printf(" 7 - Edges in a video sequence\n");
		printf(" 8 - Snap frame from live video\n");
		printf(" 9 - Mouse callback demo\n");
		printf(" 10 -Detectie fata\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				testOpenImage();
				break;
			case 2:
				testOpenImagesFld();
				break;
			case 3:
				testParcurgereSimplaDiblookStyle(); //diblook style
				break;
			case 4:
				//testColor2Gray();
				testBGR2HSV();
				break;
			case 5:
				testResize();
				break;
			case 6:
				testCanny();
				break;
			case 7:
				testVideoSequence();
				break;
			case 8:
				testSnap();
				break;
			case 9:
				testMouseClick();
				break;
			case 10:
				detectarePiele();
		}
	}
	while (op!=0);
	return 0;
}