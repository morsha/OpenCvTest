#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <math.h>

using namespace std;

double within180(double a)
{
	if (a>180)
		a -= 180;
	else if (a<0)
		a += 180;
	return a;
}

double getDistance(double a, double b)
{
	return abs(a - b);
}

bool isBetween(double target, double border1, double border2)
{
	if (border1 < border2)
		border1 += 180;
	if (border1 >= target && target>border2)
		return true;
	else
		return false;
}

double findMin(double a, double b, double c)
{
	double temp = a;
	if (b<temp)
		temp = b;
	if (c<temp)
		temp = c;

	return temp;
}

double findMax(double a, double b, double c)
{
	double temp = a;
	if (b>temp)
		temp = b;
	if (c>temp)
		temp = c;

	return temp;
}

int MINoneIn180(double* k)
{
	int i, j;
	double temp;
	for (i = 0; i<180; i++)
	{
		if (i == 0)
			temp = k[i];
		else if (k[i]<temp)
		{
			temp = k[i];
			j = i;
		}
	}

	return j;
}

int findNearer(double H, double border1, double border2)
{
	if (border1 < border2)
		border1 += 180;

	return getDistance(border1, H)>getDistance(border2, H) ? 1 : 0;
}


int main() {

	char* imageName = "2.bmp";									// �nŪ�����Ϥ����W�r
	double priority[8][182] = { 0 };							// which scheme to use
	double centralhue1 = 0, centralhue2 = 0, w1 = 0, w2 = 0;    // central hue and its w
	double border[4];											// boundaries of template
	double H = 0, S = 0, V = 0;						            // color of H, S, V
	int whichTemplate = 0;
	int nearer = 0;

	//��J�Ϥ�
	IplImage * src = cvLoadImage(imageName);

	//�гy�Msrc�@�˪�IplImage��Ƶ��c
	IplImage* hsv = cvCreateImage(cvGetSize(src), src->depth, 3);		//hsv���A
	IplImage* hsvv = cvCreateImage(cvGetSize(src), src->depth, 3);		//hsv���A�ܤ�
	IplImage* res = cvCreateImage(cvGetSize(src), src->depth, 3);		//�̫��X��

																		//�ഫ��hsv
	cvCvtColor(src, hsv, CV_BGR2HSV);

	//cvGet2D(hsv, x, y).val[0]�Y��pixel�y��(x,y)��Hue��(��*2)
	//cvGet2D(hsv, x, y).val[1]�Y��pixel�y��(x,y)��Sat��(��/255)
	//cvGet2D(hsv, x, y).val[2]�Y��pixel�y��(x,y)��Val��(��/255)

	//�p��C��template���u����*/
	//�@�@8��*180��hue angle=1440�زզX*/
	//�ϥ�priority�}�C�Ӧs���u���� priority[scheme][hue angle center]
	//8��template: i,V,L,I,T,Y,X,N

	/*
	//template i

	w1 = 10;
	for (int i = 0; i < 180; i++)
	{
		border[0] = i + (w1 / 2);
		border[0] = within180(border[0]);

		border[1] = i - (w1 / 2);
		border[1] = within180(border[1]);

		for (int y = 0; y < hsv->height; y++)
		{
			for (int x = 0; x < hsv->width; x++)
			{
				H = cvGet2D(hsv, y, x).val[0];
				V = cvGet2D(hsv, y, x).val[2];


				if (isBetween(H, border[0], border[1]))
					priority[0][i] += 0;
				else
				{
					nearer = findNearer(H, border[0], border[1]);
					priority[0][i] += getDistance(H, border[nearer])*V;
				}
			}
		}
	}
	priority[0][180] = MINoneIn180(priority[0]);
	priority[0][181] = priority[0][(int)priority[0][180]];

	// template V

	{
		w1 = 48;
		for (int i = 0; i < 180; i++)
		{
			border[0] = i + (w1 / 2);
			border[0] = within180(border[0]);

			border[1] = i - (w1 / 2);
			border[1] = within180(border[1]);


			for (int y = 0; y < hsv->height; y++)
			{
				for (int x = 0; x != hsv->width; x++)
				{
					H = cvGet2D(hsv, y, x).val[0];
					V = cvGet2D(hsv, y, x).val[2];

					if (isBetween(H, border[0], border[1]))
						priority[1][i] = 0;
					else
					{
						nearer = findNearer(H, border[0], border[1]);
						priority[1][i] += getDistance(H, border[nearer])*V;
					}
				}
			}
		}
		priority[1][180] = MINoneIn180(priority[1]);
		priority[1][181] = priority[1][(int)priority[1][180]];
	}

	//template L ( second = i-90 )

	{
		w1 = 10;
		w2 = 40;
		for (int i = 0; i<180; i++)
		{
			border[0] = i + (w1 / 2);
			border[0] = within180(border[0]);

			border[1] = i - (w1 / 2);
			border[1] = within180(border[1]);

			border[2] = (i - 90) + (w2 / 2);
			border[2] = within180(border[2]);

			border[3] = (i - 90) - (w2 / 2);
			border[3] = within180(border[3]);

			for (int y = 0; y != hsv->height; y++)
			{
				for (int x = 0; x != hsv->width; x++)
				{
					H = cvGet2D(hsv, y, x).val[0];
					V = cvGet2D(hsv, y, x).val[2];

					if (isBetween(H, border[0], border[1])||isBetween(H, border[2], border[3]))
						priority[2][i] += 0;
					else if (isBetween(H, border[1], border[2]))
					{
						nearer = findNearer(H, border[1], border[2]);
						priority[2][i] += getDistance(H, border[nearer + 1])*V;
					}
					else
					{
						priority[2][i] += MIN(getDistance(H, border[3]), getDistance(H, border[0]))*V;
					}
				}
			}
		}
		priority[2][180] = MINoneIn180(priority[2]);
		priority[2][181] = priority[2][(int)priority[2][180]];

	}
	
	//template I

	{
		w1 = 10;
		w2 = 10;
		for (int i = 0; i<180; i++)
		{
			border[0] = i + (w1 / 2);
			border[0] = within180(border[0]);

			border[1] = i - (w1 / 2);
			border[1] = within180(border[1]);

			border[2] = (i + 180) + (w2 / 2);
			border[2] = within180(border[2]);

			border[3] = (i + 180) - (w2 / 2);
			border[3] = within180(border[3]);

			for (int y = 0; y != hsv->height; y++)
			{
				for (int x = 0; x != hsv->width; x++)
				{
					H = cvGet2D(hsv, y, x).val[0];
					V = cvGet2D(hsv, y, x).val[2];

					if (isBetween(H, border[0], border[1]) || isBetween(H, border[2], border[3]))
						priority[3][i] += 0;
					else
					{
						if (findNearer(H, border[0], border[1]) == 0)
							priority[3][i] += MIN(getDistance(H, border[0]), getDistance(H, border[3]))*V;
						else
							priority[3][i] += MIN(getDistance(H, border[1]), getDistance(H, border[2]))*V;
					}
				}
			}
		}
		priority[3][180] = MINoneIn180(priority[3]);
		priority[3][181] = priority[3][(int)priority[3][180]];

	}
	
	// template T

	w1 = 90;
	for (int i = 0; i < 180; i++)
	{
		border[0] = i + (w1 / 2);
		border[0] = within180(border[0]);

		border[1] = i - (w1 / 2);
		border[1] = within180(border[1]);

		for (int y = 0; y < hsv->height; y++)
		{
			for (int x = 0; x < hsv->width; x++)
			{
				H = cvGet2D(hsv, y, x).val[0];
				V = cvGet2D(hsv, y, x).val[2];


				if (isBetween(H, border[0], border[1]))
					priority[4][i] += 0;
				else
				{
					nearer = findNearer(H, border[0], border[1]);
					priority[4][i] += getDistance(H, border[nearer])*V;
				}
			}
		}
	}
	priority[4][180] = MINoneIn180(priority[4]);
	priority[4][181] = priority[4][(int)priority[4][180]];
	*/
	//template Y

	{
		w1 = 48;
		w2 = 10;
		for (int i = 0; i<180; i++)
		{
			border[0] = i + (w1 / 2);
			border[0] = within180(border[0]);

			border[1] = i - (w1 / 2);
			border[1] = within180(border[1]);

			border[2] = (i + 180) + (w2 / 2);
			border[2] = within180(border[2]);

			border[3] = (i + 180) - (w2 / 2);
			border[3] = within180(border[3]);

			for (int y = 0; y != hsv->height; y++)
			{
				for (int x = 0; x != hsv->width; x++)
				{
					H = cvGet2D(hsv, y, x).val[0];
					V = cvGet2D(hsv, y, x).val[2];

					if (isBetween(H, border[0], border[1]) || isBetween(H, border[2], border[3]))
						priority[5][i] += 0;
					else
					{
						if (findNearer(H, border[0], border[1]) == 0)
							priority[5][i] += MIN(getDistance(H, border[0]), getDistance(H, border[3]))*V;
						else
							priority[5][i] += MIN(getDistance(H, border[1]), getDistance(H, border[2]))*V;
					}
				}
			}
		}
		priority[5][180] = MINoneIn180(priority[5]);
		priority[5][181] = priority[5][(int)priority[5][180]];

	}

	//template X
	{
		w1 = 48;
		w2 = 48;
		for (int i = 0; i<180; i++)
		{
			border[0] = i + (w1 / 2);
			border[0] = within180(border[0]);

			border[1] = i - (w1 / 2);
			border[1] = within180(border[1]);

			border[2] = (i + 180) + (w2 / 2);
			border[2] = within180(border[2]);

			border[3] = (i + 180) - (w2 / 2);
			border[3] = within180(border[3]);

			for (int y = 0; y != hsv->height; y++)
			{
				for (int x = 0; x != hsv->width; x++)
				{
					H = cvGet2D(hsv, y, x).val[0];
					V = cvGet2D(hsv, y, x).val[2];

					if (isBetween(H, border[0], border[1]) || isBetween(H, border[2], border[3]))
						priority[6][i] += 0;
					else
					{
						if (findNearer(H, border[0], border[1]) == 0)
							priority[6][i] += MIN(getDistance(H, border[0]), getDistance(H, border[3]))*V;
						else
							priority[6][i] += MIN(getDistance(H, border[1]), getDistance(H, border[2]))*V;
					}
				}
			}
		}
		priority[6][180] = MINoneIn180(priority[6]);
		priority[6][181] = priority[6][(int)priority[6][180]];

	}


	//��̤ܳp��template
	int tmpt;
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
			tmpt = priority[i][181];
		if (priority[i][361] < tmpt)
		{
			tmpt = priority[i][181];
			whichTemplate = i;
		}
	}

	whichTemplate = 5;

	//�վ�̤ptemplate�һݪ��ƭ�

	switch (whichTemplate)
	{
	case 0:
		w1 = 10;
		cout << "template i " << endl;
		break;

	case 1:
		w1 = 48;
		cout << "template V " << endl;
		break;

	case 2:
		w1 = 10;
		w2 = 40;
		cout << "template L " << endl;
		break;

	case 3:
		w1 = 10;
		w2 = 10;
		cout << "template I " << endl;
		break;

	case 4:
		w1 = 90;
		cout << "template T " << endl;
		break;

	case 5:
		w1 = 48;
		w2 = 10;
		cout << "template Y " << endl;
		break;

	case 6:
		w1 = 48;
		w2 = 48;
		cout << "template X " << endl;
		break;

	case 7:
		cout << "template N " << endl;
		break;
	}




	//�}�l���C��
	switch (whichTemplate)
	{
		case 0:
		case 1:
		case 4:

			centralhue1 = priority[whichTemplate][180];

			border[0] = centralhue1 + (w1 / 2);
			border[0] = within180(border[0]);
			border[1] = centralhue1 - (w1 / 2);
			border[1] = within180(border[1]);

			for (int y = 0; y < hsv->height; y++)
			{
				for (int x = 0; x < hsv->width; x++)
				{
					H = cvGet2D(hsv, y, x).val[0];
					S = cvGet2D(hsv, y, x).val[1];
					V = cvGet2D(hsv, y, x).val[2];


					if (isBetween(H, border[0], border[1])) {}
					else
					{
						nearer = findNearer(H, border[0], border[1]);

						if (nearer == 0)
							H = centralhue1 + (w1 / 2)*(abs(H - centralhue1)) / 90;
						else if (nearer == 1)
							H = centralhue1 - (w1 / 2)*(abs(H - centralhue1)) / 90;
						H = within180(H);
					}
					cvSet2D(hsvv, y, x, CV_RGB(V, S, H));
				}
			}
			break;

		case 2:

			centralhue1 = priority[whichTemplate][180];
			centralhue2 = centralhue1 - 90;
			centralhue2 = within180(centralhue2);
			border[0] = centralhue1 + (w1 / 2);
			border[0] = within180(border[0]);
			border[1] = centralhue1 - (w1 / 2);
			border[1] = within180(border[1]);
			border[2] = centralhue2 + (w2 / 2);
			border[2] = within180(border[2]);
			border[3] = centralhue2 - (w2 / 2);
			border[3] = within180(border[3]);

			for (int y = 0; y < hsv->height; y++)
			{
				for (int x = 0; x < hsv->width; x++)
				{
					H = cvGet2D(hsv, y, x).val[0];
					S = cvGet2D(hsv, y, x).val[1];
					V = cvGet2D(hsv, y, x).val[2];

					if (isBetween(H, centralhue1, centralhue1 - 15))
						H = centralhue1 - (w1 / 2)*getDistance(H, centralhue1) / 15;
					else if (isBetween(H, centralhue1 - 15, centralhue2))
						H = centralhue2 + (w2 / 2)*getDistance(H, centralhue2) / 30;
					else if (isBetween(H, centralhue2, centralhue2 - 75))
						H = centralhue2 - (w2 / 2)*getDistance(H, centralhue2) / 75;
					else
						H = centralhue1 + (w1 / 2)*getDistance(H, centralhue1) / 60;
					H = within180(H);

					cvSet2D(hsvv, y, x, CV_RGB(V, S, H));
				}
			}
			break;

		case 3:
		case 6:

			centralhue1 = priority[whichTemplate][180];
			centralhue2 = centralhue1 - 90;
			centralhue2 = within180(centralhue2);
			border[0] = centralhue1 + (w1 / 2);
			border[0] = within180(border[0]);
			border[1] = centralhue1 - (w1 / 2);
			border[1] = within180(border[1]);
			border[2] = centralhue2 + (w2 / 2);
			border[2] = within180(border[2]);
			border[3] = centralhue2 - (w2 / 2);
			border[3] = within180(border[3]);

			for (int y = 0; y < hsv->height; y++)
			{
				for (int x = 0; x < hsv->width; x++)
				{
					H = cvGet2D(hsv, y, x).val[0];
					S = cvGet2D(hsv, y, x).val[1];
					V = cvGet2D(hsv, y, x).val[2];

					if (isBetween(H, centralhue1, centralhue1-45))
						H = centralhue1 - (w1 / 2)*getDistance(H, centralhue1) / 45;
					else if (isBetween(H, centralhue1 - 45, centralhue2))
						H = centralhue2 + (w2 / 2)*getDistance(H, centralhue2) / 45;
					else if (isBetween(H, centralhue2, centralhue1 + 45))
						H = centralhue2 - (w2 / 2)*getDistance(H, centralhue2) / 45;
					else
						H = centralhue1 + (w1 / 2)*getDistance(H, centralhue1) / 45;
					H = within180(H);
					cvSet2D(hsvv, y, x, CV_RGB(V, S, H));
				}
			}
			break;

		case 5:

			centralhue1 = priority[whichTemplate][180];
			centralhue2 = centralhue1 - 90;
			centralhue2 = within180(centralhue2);
			border[0] = centralhue1 + (w1 / 2);
			border[0] = within180(border[0]);
			border[1] = centralhue1 - (w1 / 2);
			border[1] = within180(border[1]);
			border[2] = centralhue2 + (w2 / 2);
			border[2] = within180(border[2]);
			border[3] = centralhue2 - (w2 / 2);
			border[3] = within180(border[3]);

			for (int y = 0; y < hsv->height; y++)
			{
				for (int x = 0; x < hsv->width; x++)
				{
					H = cvGet2D(hsv, y, x).val[0];
					S = cvGet2D(hsv, y, x).val[1];
					V = cvGet2D(hsv, y, x).val[2];

					if (isBetween(H, centralhue1, centralhue2 + 35.5))
						H = centralhue1 - (w1 / 2)*getDistance(H, centralhue1) / 54.5;
					else if (isBetween(H, centralhue2 + 35.5, centralhue2))
						H = centralhue2 + (w2 / 2)*getDistance(H, centralhue2) / 35.5;
					else if (isBetween(H, centralhue2, centralhue2 - 35.5))
						H = centralhue2 - (w2 / 2)*getDistance(H, centralhue2) / 54.5;
					else
						H = centralhue1 + (w1 / 2)*getDistance(H, centralhue1) / 35.5;
					H = within180(H);

					cvSet2D(hsvv, y, x, CV_RGB(V, S, H));
				}
			}
			break;

	}



	cout << centralhue1;


		

	cvCvtColor(hsvv, res, CV_HSV2BGR);




	cvNamedWindow("Source-RGB", 1);
	cvShowImage("Source-RGB", src);
	cvNamedWindow("QQ", 1);
	cvShowImage("QQ", hsv);
	cvNamedWindow("Q8", 1);
	cvShowImage("Q8", hsvv);
	cvNamedWindow("result", 1);
	cvShowImage("result", res);



	cvWaitKey(0);
	return EXIT_SUCCESS;
}