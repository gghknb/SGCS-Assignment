#include "stdafx.h"

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "sift.h"
#include "imgfeatures.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define debug 0
#define PI 3.141592
#define square(a) (a)*(a)

LARGE_INTEGER Start, End, Frequency;	// variables for time measure
double TimeTaken;

/* the maximum number of keypoint NN candidates to check during BBF search */
#define KDTREE_BBF_MAX_NN_CHKS 200

/* threshold on squared ratio of distances between NN and 2nd NN */
#define NN_SQ_DIST_RATIO_THR 0.49
//#define NN_SQ_DIST_RATIO_THR 0.79

char* out_img_name = NULL;
int display = 1;
int intvls = SIFT_INTVLS;
double sigma = SIFT_SIGMA;
double contr_thr = SIFT_CONTR_THR;
int curv_thr = SIFT_CURV_THR;
int img_dbl = SIFT_IMG_DBL;
int descr_width = SIFT_DESCR_WIDTH;
int descr_hist_bins = SIFT_DESCR_HIST_BINS;

using namespace cv;
using namespace std;

void tic( );
double toc( );
int Problem1();
int Problem2();
int MatchAndDraw(struct feature*, struct feature*, int, int, IplImage*, int);

// main ----------------------------------------------------------------------

// 
// Complete the MatchAndDraw() function below
//

int main()
{
	// Match box.bmp and box2.bmp and show the matching score and matched points
	// Problem2() will not start until the window is closed because of cvWaitKey( 0 );
	Problem1();

	// Match box.bmp and lena_24.bmp and show the matching score and matched points
	Problem2();

	return 0;
}



int Problem1()
{
	int n1, n2, sco, i, j;
	CString inFile1, inFile2;
	IplImage *Image1, *Image2;
	struct feature *feat1, *feat2;

	inFile1 = "data1/box.bmp";
	inFile2 = "data1/box2.bmp";

	Image1 = cvLoadImage(inFile1, CV_LOAD_IMAGE_COLOR );
	if( ! Image1 ) {
		fatal_error( "Error in loading image...\n");
		cvWaitKey( 0 );
	}
	n1 = _sift_features( Image1, &feat1, intvls, sigma, contr_thr, curv_thr,
						img_dbl, descr_width, descr_hist_bins );
	printf("---- #feat in %s = %d ----- \n", inFile1, n1);

	Image2 = cvLoadImage(inFile2, CV_LOAD_IMAGE_COLOR );
	if( ! Image2 ) {
		fatal_error( "Error in loading image...\n");
		cvWaitKey( 0 );
	}
	n2 = _sift_features( Image2, &feat2, intvls, sigma, contr_thr, curv_thr,
						img_dbl, descr_width, descr_hist_bins );
	printf("---- #feat in %s = %d ----- \n", inFile2, n2);

	IplImage *stacked;
	stacked = stack_imgs( Image1, Image2 );

	tic();

	int m = MatchAndDraw( feat1, feat2, n1, n2, stacked, Image1->width);

	double time = toc();
	printf( "Time taken: %f sec\n", time );

	fprintf( stderr, "Found %d total matches\n\n\n", m );


	cvNamedWindow( "Matche1" , 1 );
	cvShowImage( "Matche1" , stacked );

	cvWaitKey( 0 );
	return 0;
}


int Problem2()
{
	int n1, n2, sco, i, j;
	CString inFile1, inFile2;
	IplImage *Image1, *Image2;
	struct feature *feat1, *feat2;

	inFile1 = "data1/box.bmp";
	inFile2 = "data1/lena_24.bmp";

	Image1 = cvLoadImage(inFile1, CV_LOAD_IMAGE_COLOR );
	if( ! Image1 ) {
		fatal_error( "Error in loading image...\n");
		cvWaitKey( 0 );
	}
	n1 = _sift_features( Image1, &feat1, intvls, sigma, contr_thr, curv_thr,
						img_dbl, descr_width, descr_hist_bins );
	printf("---- #feat in %s = %d ----- \n", inFile1, n1);

	Image2 = cvLoadImage(inFile2, CV_LOAD_IMAGE_COLOR );
	if( ! Image2 ) {
		fatal_error( "Error in loading image...\n");
		cvWaitKey( 0 );
	}
	n2 = _sift_features( Image2, &feat2, intvls, sigma, contr_thr, curv_thr,
						img_dbl, descr_width, descr_hist_bins );
	printf("---- #feat in %s = %d ----- \n", inFile2, n2);

	IplImage *stacked;
	stacked = stack_imgs( Image1, Image2 );

	tic();

	int m = MatchAndDraw( feat1, feat2, n1, n2, stacked, Image1->width);

	double time = toc();
	printf( "Time taken: %f sec\n", time );

	fprintf( stderr, "Found %d total matches\n", m );


	cvNamedWindow( "Matche2" , 1 );
	cvShowImage( "Matche2" , stacked );

	cvWaitKey( 0 );
	return 0;
}

int MatchAndDraw(struct feature *feat1, struct feature *feat2, int n1, int n2, IplImage* stacked, int width)
{
	int i, j, k, d, m, min_idx; 
	double d0, d1, d2;
	CvPoint pt1, pt2;
	struct feature *f1, *f2;
	double *descr1, *descr2, dist;

	//
	// Complete the codes below to calculate the distance between each keypoint.
	// For each keypoint with high similarity (small distance), a line will be drawn 
	// on the stitched image.
	//
	// From a keypoint f1, the descriptor is accessed by f1->descr
	// Each descriptor is a one dimensional array of size 128.
	// 

	m = 0;
	min_idx = 0;
	d = feat1->d; // dimension of the descriptor (fixed to 128)
	for( i = 0; i < n1; i++ ) {
		f1 = feat1 + i;
		descr1 = f1->descr; 
		d1 = d2 = DBL_MAX;
		
		//
		// Complete this part to find a matching keypoint from f1 in image1 toward image2
		// 
		for(int j = 0 ; j <n2 ; j++)
		{
			d0 = 0;
			f2 = feat2 + j;
			descr2 = f2->descr;
			for(int x = 0 ; x < d ; x++)
			{
				d0 += (descr1[x] -descr2[x]) * (descr1[x] -descr2[x]);
			}
			d0 = sqrt(d0);

			if( d0 < d1)
			{
				d2 = d1;
				d1 = d0;
				min_idx = j;
			}
			else if(d0 > d1 && d0 < d2)
				d2 = d0;

		}

		if( NN_SQ_DIST_RATIO_THR >= d1/d2   /* Complete this condition */ ) {
			f2 = feat2 + min_idx;
			pt1 = cvPoint( cvRound( f1->x ), cvRound( f1->y ) );
			pt2 = cvPoint( cvRound( f2->x ), cvRound( f2->y ) );
			pt2.x += width;
			cvLine( stacked, pt1, pt2, CV_RGB(255,0,255), 1, 8, 0 ); // draw a line
			m++;
		}
    }

	return m;
}

/********************************************************************************/
//	tic
//
//	Measure the starting time
/********************************************************************************/
void tic ( )		// Starting point of measuring time
{
	QueryPerformanceCounter(&Start);
}

/********************************************************************************/
//	toc
//
//	Measure the finishing time
/********************************************************************************/
double toc ( )	// Finishing point of measuring time
{
	QueryPerformanceCounter(&End);
	QueryPerformanceFrequency(&Frequency);

	TimeTaken = (double) (End.QuadPart-Start.QuadPart)/Frequency.QuadPart;	// seconds

	return TimeTaken;
}
