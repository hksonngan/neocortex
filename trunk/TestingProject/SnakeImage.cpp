#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvaux.h"
#include "common_defs.h"
#include "shader_snake.h"
#include <conio.h>
#include <iostream>
#include <fstream>



using namespace cv;
using namespace std;

struct Parameters
{
	char* winname;
	Mat image, wind;		//исходное изображение и окно, которое видит пользователь
	int brushSize, numslices;			
	Point	Center;			//центр исходного контура
	int Xmod, Ymod;			//модификаторы для точек
	int resX, resY;			//храним разрешение 
	vector <Point> points;//исходные точки контура
	vector <vector <Point> > ResPoints;	//все результаты сразу
	vector <char *> FNames;				//имена файлов
	int CurrentSlice;					//текущий слой (для вывода результата)

	Parameters(Mat& _image, char* _winname, Mat& _wind, int _Xmod, int _Ymod, int _ns, vector < char *> _FNS)
	{
		winname = _winname;
		image = _image;
		wind= _wind;
		Xmod= _Xmod;
		Ymod= _Ymod;
		if (image.cols<800)
			resX=image.cols;
		else
			resX=800;
		if (image.rows<600)
			resY=image.rows;
		else resY=600;
		brushSize=4;
		numslices=_ns;
		FNames=_FNS;
		CurrentSlice=0;
	}
};

void onMouseEvent(int event, int x, int y, int flags, void* data)
{
	//если кликаем, то рисуем seed
	if ((event == CV_EVENT_LBUTTONDOWN) )
	{
		Parameters* params = (Parameters*)data;
		if (!params->points.empty())
		{
			circle(params->image, Point(params->Center.x, params->Center.y), params->brushSize, Scalar(128, 128, 128));
			circle(params->wind, Point(params->Center.x-params->Xmod, params->Center.y-params->Ymod), params->brushSize, Scalar(128, 128, 128));
			params->points.clear();
		}
		params->Center=Point(x+params->Xmod,y+params->Ymod);
		circle(params->image, Point(x+params->Xmod, y+params->Ymod), params->brushSize, Scalar(0, 0, 255));
		circle(params->wind, Point(x, y), params->brushSize, Scalar(0, 0, 255));
		//запоминаем и рисуем точки
		params->points.push_back(Point(x+params->Xmod-params->brushSize,y+params->Ymod));
		params->points.push_back(Point(x+params->Xmod,y+params->Ymod-params->brushSize));
		params->points.push_back(Point(x+params->Xmod+params->brushSize,y+params->Ymod));
		params->points.push_back(Point(x+params->Xmod,y+params->Ymod+params->brushSize));

		params->image.at<Vec3b>(y+params->Ymod, x+params->Xmod-params->brushSize)[0]=0;
		params->image.at<Vec3b>(y+params->Ymod, x+params->Xmod-params->brushSize)[1]=255;
		params->image.at<Vec3b>(y+params->Ymod, x+params->Xmod-params->brushSize)[2]=0;

		params->image.at<Vec3b>(y+params->Ymod-params->brushSize, x+params->Xmod)[0]=0;
		params->image.at<Vec3b>(y+params->Ymod-params->brushSize, x+params->Xmod)[1]=255;
		params->image.at<Vec3b>(y+params->Ymod-params->brushSize, x+params->Xmod)[2]=0;

		params->image.at<Vec3b>(y+params->Ymod, x+params->Xmod+params->brushSize)[0]=0;
		params->image.at<Vec3b>(y+params->Ymod, x+params->Xmod+params->brushSize)[1]=255;
		params->image.at<Vec3b>(y+params->Ymod, x+params->Xmod+params->brushSize)[2]=0;

		params->image.at<Vec3b>(y+params->Ymod+params->brushSize, x+params->Xmod)[0]=0;
		params->image.at<Vec3b>(y+params->Ymod+params->brushSize, x+params->Xmod)[1]=255;
		params->image.at<Vec3b>(y+params->Ymod+params->brushSize, x+params->Xmod)[2]=0;


		params->wind.at<Vec3b>(y, x-params->brushSize)[0]=0;
		params->wind.at<Vec3b>(y, x-params->brushSize)[1]=255;
		params->wind.at<Vec3b>(y, x-params->brushSize)[2]=0;

		params->wind.at<Vec3b>(y-params->brushSize, x)[0]=0;
		params->wind.at<Vec3b>(y-params->brushSize, x)[1]=255;
		params->wind.at<Vec3b>(y-params->brushSize, x)[2]=0;

		params->wind.at<Vec3b>(y, x+params->brushSize)[0]=0;
		params->wind.at<Vec3b>(y, x+params->brushSize)[1]=255;
		params->wind.at<Vec3b>(y, x+params->brushSize)[2]=0;

		params->wind.at<Vec3b>(y+params->brushSize, x)[0]=0;
		params->wind.at<Vec3b>(y+params->brushSize, x)[1]=255;
		params->wind.at<Vec3b>(y+params->brushSize, x)[2]=0;


		imshow(params->winname, params->wind);
	}
}

//при изменении ползунков X_Offset и  Y_Offset
void XEvent(int position, void* data)
{
	Parameters* params = (Parameters*)data;
	for (int y=params->Ymod; y<params->resY+params->Ymod; y++)
		for(int x=params->Xmod; x<params->resX+params->Xmod; x++)
			params->wind.at<Vec3b>(y-params->Ymod,x-params->Xmod)=params->image.at<Vec3b>(y,x);
	imshow(params->winname, params->wind);
}

void refresh(Parameters &params)
{
	for (int y=params.Ymod; y<params.resY+params.Ymod; y++)
		for(int x=params.Xmod; x<params.resX+params.Xmod; x++)
			params.wind.at<Vec3b>(y-params.Ymod,x-params.Xmod)=params.image.at<Vec3b>(y,x);
}

void attachTB(char* winName, int &a, int &b, int &g, int &iters)
{
	createTrackbar("Alpha", winName, &a, 100);
	createTrackbar("Beta", winName, &b, 100);
	createTrackbar("Gamma", winName, &g, 100);
	createTrackbar("Iterations", winName, &iters, 1000);
}

void attachSliders(char *winName, Parameters &params)
{
	if (params.image.cols>800)
		createTrackbar("X_Offset", winName, &params.Xmod, params.image.cols-params.resX-1, XEvent, &params);
	if (params.image.rows>600)
		createTrackbar("Y_Offset", winName, &params.Ymod, params.image.rows-params.resY-1, XEvent, &params);
}
//gthpfuhe;ftv rfhnbyre
void ReloadSlice(int position, void* data)
{
	Parameters* params = (Parameters*)data;

	params->image=imread(params->FNames[position], 1);
	for (int i=0; i<params->ResPoints[position].size(); i++)
	{
		circle(params->image, params->ResPoints[position][i], 3, Scalar(0, 0, 255), -1);
	}
	refresh(*params);
	imshow(params->winname, params->wind);
}

void attachSliceSlider(char* winname, Parameters &params)
{
	if (params.numslices>1)
		createTrackbar("Slice", winname, &params.CurrentSlice, params.numslices-1, ReloadSlice, &params);
}
//Экспериментально (сглаживаем возможный шум)
void preprocess(Mat& img)
{
	GaussianBlur(img, img, Size(5,5), 0, 0);
	medianBlur(img, img, 5);
}

int main (int argc, char *argv[])
{
	int AIV, BIV, GIV, SITERS;		//параметры метода для слайдеров
	int LXmod=0, LYmod=0;			//помним где мы есть
	Mat window;						//видимая часть изображения
	Mat src_1ch, src_3ch, fake_src_3ch;
	char* winName="Source";
	char Pressed;							//нажатая клавиша
	Texture2D *img;					//
	TextureData2D *data; 
	ShaderSnake *Snake;

	//load files
	int numfiles;
	vector <char *> fnames;

	//load file
	char fname[256]="../Data/list.lst";
	cout<<"Enter path to list-file: "<<endl;
	cin>>fname;
	ifstream ifs(fname);

	if (!ifs.is_open())
	{
		cout<<"Could not open list file"<<endl;
		_getch();
		return 1;
	}

	while (!ifs.eof()) 
	{
		char * str=new char [300];

		ifs>>str;
		fnames.push_back(str);
	}
	numfiles=fnames.size();
		
	//считываем картинку средствами OpenCV
		glfwInit();
		glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, 1);
		if(!glfwOpenWindow(1, 1, 0, 0, 0, 0, 16, 0, GLFW_WINDOW))
		{
			glfwTerminate(); 
			MessageBoxA(0, lpszGlfwError, "Fatal", 0);
			return 1;
		}
		glfwSetWindowTitle("Output window");
		
		//параметры змейки
		float alpha=1.0f, beta=2.0f, gamma=15.0f;
		//целые значения для alpha, beta и gamma
		AIV=(int)alpha;
		BIV=(int)beta;
		GIV=(int)gamma;
		SITERS=30;


	while(1)
	{
		src_3ch=imread(fnames[0],1);

		//создаем окно просмотра
		if ((src_3ch.rows>600)&&(src_3ch.cols>800))
			window=Mat::zeros(600, 800, CV_8UC3);
		if ((src_3ch.rows>600)&&(src_3ch.cols<800))
			window=Mat::zeros(600, src_3ch.cols, CV_8UC3);
		if ((src_3ch.rows<600)&&(src_3ch.cols>800))
			window=Mat::zeros(src_3ch.rows, 800, CV_8UC3);
		if ((src_3ch.rows<600)&&(src_3ch.cols<800))
			window=Mat::zeros(src_3ch.rows, src_3ch.cols, CV_8UC3);

		//	cvtColor(src_3ch, src_1ch, CV_RGB2GRAY);
		src_3ch.copyTo(fake_src_3ch);

		Pressed=-1;


		//предобработка исходного изображения
		//по совету из презентации Nick Govier и David Newman
		//призвана уменьшить зашумленность изображения
		//т.к. алгоритм очень чувствителен к шуму
		//	preprocess(src_1ch);

		//задаем параметры
		Parameters params(fake_src_3ch, "Source", window, LXmod, LYmod, numfiles, fnames);
		refresh(params);

		//показываем исходное изображение
		namedWindow("Source",CV_GUI_EXPANDED|CV_WINDOW_NORMAL|CV_WINDOW_KEEPRATIO);
		setWindowProperty("Source",CV_WND_PROP_FULLSCREEN,CV_WINDOW_FULLSCREEN);
		imshow("Source", params.wind);

		//рисуем на картинке точки
		cvSetMouseCallback(winName, onMouseEvent, &params);

		attachTB("Source", AIV, BIV, GIV, SITERS);
		attachSliders("Source", params);

		//ждем ввода
		Pressed=-1;
		while ((Pressed!=27)&&(Pressed!=13))
			Pressed=waitKey(0);
		if(Pressed==27)
			break;

		//удаляем лишнее
		src_3ch.~Mat();
		fake_src_3ch.~Mat();

		for (int l=0; l< params.numslices; l++)
		{

			src_1ch=imread(params.FNames[l], 0);
			preprocess(src_1ch);
			//основная последовательность
			data = new TextureData2D(src_1ch.cols, src_1ch.rows, 3);
			if (!data)
			{
				MessageBoxA(0, lpszInternalError, "Error", 0);
				return 1;
			}

			//копируем изображение в подходящий формат
			img = new Texture2D(data, 0, GL_TEXTURE_RECTANGLE_ARB);
			if (!img)
			{
				MessageBoxA(0, lpszInternalError, "Error", 0);
				return 1;
			}

			for (int y = 0; y <  src_1ch.rows; y++)
			{

				for (int x = 0; x < src_1ch.cols; x++)
				{
					uchar pixel = src_1ch.at<uchar>(y,x);
					data->Pixel<Vector3D> (x,abs(y-src_1ch.rows+1)) = 
						Vector3D (	(float)pixel / 255.0F,
						(float)pixel / 255.0F,
						(float)pixel / 255.0F);
				}
			}

	//		data->~TextureData2D();
			//производим итерации змейки
			SnakeParams sparam;
			sparam.alpha=(float) AIV;
			sparam.beta=(float) BIV;
			sparam.gamma=(float) GIV;
			Snake=new ShaderSnake();
			Snake->Init();
			Snake->GetReady();
			Snake->FixParams(img, sparam);
			Snake->AddSeed(params.Center.x, abs(params.Center.y-src_1ch.rows+1));

			for (int i=0; i<SITERS; i++)
				Snake->Iterate();

			//достаем точки
			Texture2D* ResPoints=Snake->Output();
			int ResSize=Snake->GetSize();
			vector <Point> OCV_ResPoints;

			for (int i=0; i<ResSize; i++)
			{
				Vector3D temp;
				temp=ResPoints->Data->Pixel<Vector3D>(i,0);
				OCV_ResPoints.push_back(Point(temp.X, abs(temp.Y-src_1ch.rows+1)));
			}
			//закидываем се впараметры
			params.ResPoints.push_back(OCV_ResPoints);
			//основная последовательность
			src_1ch.~Mat();
			Snake->~ShaderSnake();
			data->~TextureData2D();
			img->~Texture2D();
			//			ResPoints=NULL;
		}

		//вывод
		attachSliceSlider("Source", params);

		params.image=imread(params.FNames[0], 1);
		for (int i=0; i<params.ResPoints[0].size(); i++)
		{
			circle(params.image, params.ResPoints[0][i], 3, Scalar(0, 0, 255), -1);
		}
		refresh(params);
		imshow("Source", params.wind);

		//ждем ввода
		Pressed=-1;
		while ((Pressed!=27)&&(Pressed!=13))
			Pressed=waitKey(0);
		if(Pressed==27)
			break;

		params.ResPoints.clear();
		params.image.~Mat();
		LXmod=params.Xmod;
		LYmod=params.Ymod;

	}
	return 0;
}