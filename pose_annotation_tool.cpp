#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#define JOINTS 14

using namespace cv;
using namespace std;


int ESC = 1048603;
int ENTER = 1048586;
int SPACE = 1048608;
int TAB = 1048585;

int joint_points[JOINTS][3] = {-1};
int current_joint = -1;

int r[2], g[2], b[2];



char joints[JOINTS][100] = {"Right ankle",
"Right knee",
"Right hip",
"Left hip",
"Left knee",
"Left ankle",
"Right wrist",
"Right elbow",
"Right shoulder",
"Left shoulder",
"Left elbow",
"Left wrist",
"Neck",
"Head top"};


void CallBackFunc(int event, int x, int y, int flags, void* param)
{
	

	if (event == CV_EVENT_LBUTTONUP)
	{
		joint_points[current_joint][0] = x;
		joint_points[current_joint][1] = y;
		joint_points[current_joint][2] = 0;

		int occlusion = 0;
		Mat &img = *((Mat*)(param)); 
		circle(img, Point(joint_points[current_joint][0], joint_points[current_joint][1]), 10, Scalar(b[occlusion], g[occlusion], r[occlusion]), 3);
		imshow("image", img);
	}
	else if (event == CV_EVENT_RBUTTONUP)
	{
		joint_points[current_joint][0] = x;
		joint_points[current_joint][1] = y;
		joint_points[current_joint][2] = 1;

		int occlusion = 1;
		Mat &img = *((Mat*)(param));
		circle(img, Point(joint_points[current_joint][0], joint_points[current_joint][1]), 10, Scalar(b[occlusion], g[occlusion], r[occlusion]), 3);
		imshow("image", img);
	}

}

void init()
{
	r[0] = 0;
	g[0] = 255;
	b[0] = 255;

	r[1] = 255;
	g[1] = 0;
	b[1] = 255;
}

int main(int argc, char **argv)
{
	init();

	if(argc < 1)
	{
		printf("1:\tpath to list of images\n");
	}

	Mat img;

	namedWindow("image",WINDOW_NORMAL);
	namedWindow("Progress",WINDOW_NORMAL);
	setMouseCallback("image", CallBackFunc, &img);

	

	FILE *fp_list = fopen(argv[1], "r");
	int key_pressed = 0;

	while(!feof(fp_list))
	{
		char path[200];

		fscanf(fp_list, "%s", path);
		img = imread(path);
		Mat progress = Mat::zeros(400, 150, CV_8UC3);

		current_joint = -1;
		for(int j = 0; j < JOINTS; j++)
		{
			joint_points[JOINTS][0] = -1;
			joint_points[JOINTS][1] = -1;
			joint_points[JOINTS][2] = -1;
		}

		imshow("image", img);
		imshow("Progress", progress);
		key_pressed =waitKey(0);
		
		if(key_pressed == SPACE)
		{
			fprintf(stderr, "\nSkipping : %s", path);
			continue;
		}
		else if(key_pressed == ESC)
		{
			fprintf(stderr, "\nEXITING");
			break;
		}
		else if(key_pressed == ENTER)
		{
			fprintf(stderr, "\nProcessing : %s", path);

			for(int j=0; j<JOINTS; )
			{
				current_joint = j;
				putText(progress, joints[j], Point(10, 20+j*20), 1, 1, Scalar(255, 255, 255));

				
				
				imshow("image", img);
				imshow("Progress", progress);
				key_pressed = waitKey(0);

				int occlusion = joint_points[current_joint][2];

				if(key_pressed == TAB)
				{
					joint_points[current_joint][0] = -1;
					joint_points[current_joint][1] = -1;
					joint_points[current_joint][2] = -1;

					putText(progress, "***********", Point(10, 20+j*20), 1, 1, Scalar(255, 255, 255));
					j++;
					imshow("Progress", progress);
				}
				else
				{
					circle(img, Point(joint_points[current_joint][0], joint_points[current_joint][1]), 10, Scalar(b[occlusion], g[occlusion], r[occlusion]), -1);
					j++;
				}
			}



			char annotation_file_name[300];
			sprintf(annotation_file_name, "%s_joints.txt", path);

			fprintf(stderr, "\nSaving : %s", annotation_file_name);

			FILE *fp_annotation = fopen(annotation_file_name, "w");

			fprintf(fp_annotation, "image_path %s", path);
			for(int j=0; j<JOINTS; j++)
				fprintf(fp_annotation, "\n%d %d %d", joint_points[j][0], joint_points[j][1], joint_points[j][2]);

			fclose(fp_annotation);
		}
		
		printf("\nkey pressed : %d", key_pressed);
	}

	fclose(fp_list);
	return 0;
}