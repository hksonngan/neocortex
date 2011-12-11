#pragma once

#include "GLee/GLee.h"
#include "Graphics.h"
#include "ScanData.h"
#include "shader_snake.h"
#include <fstream>

#include "K-means/K-means.h"

#include "include/c_ballPivot.h"

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <vcclr.h>

#include "opencv/cv.h"
#include "opencv/highgui.h"

using namespace std;
using namespace cv;

#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"User32.lib")
#pragma comment (lib,"gdi32.lib")

#define MEMORY_LIMIT 1300000000
#define K_MAX 50

struct TColor 
{
 float R, G, B, A;						// ��������� �����
 TColor():R(0), G(0), B(0), A(0) {};
 TColor(float value_R, float value_G, float value_B, float value_A): 
        R(value_R), G(value_G), B(value_B), A(value_A) {};
 TColor(float value_R, float value_G, float value_B): R(value_R), G(value_G), B(value_B) {};
};

struct TVoxel							// ��������� ������
{
 int SegmentIndex_2D, SegmentIndex_3D;  // ������� ���������, ������� ����������� �������

 // ����������� �� ���������
 TVoxel(): SegmentIndex_2D(-1), SegmentIndex_3D(-1) {};
};

struct TSegment					// ������� �� ��������
{
 float Volume;					// ����� ��������
 short MinDensity;				// ����������� �������� ��������� 
 short MaxDensity;				// ������������ �������� ���������
 
 TColor Color;					// ���� ��������
 uchar Transparensy;			// ������� ������������
 bool Visible;					// ������� ��������� ��������, �������� � �������

 TColor tmpColor;				// ��������������� ���� ��������
 uchar tmpTransparensy;			// ��������������� ��������������
 bool tmpVisible;				// ��������������� ������� ���������

 // ����������� �������� �� ���������
 TSegment(): Volume(0.0f), MinDensity(0), MaxDensity(0), 
	         Color(TColor(0.0f, 0.0f, 0.0f, 0.0f)), Transparensy(255), Visible(false), 
			 tmpColor(TColor(0.0f, 0.0f, 0.0f, 0.0f)), tmpTransparensy(255), tmpVisible(false) {};
 
};

/* struct TSnakeParameters
{
 float alpha, beta, gamma;		// ��������� ������ ��������� �������
 int iterations_number;			// ���������� �������� ������

 // ����������� �� ���������
 TSnakeParameters(): alpha(1.0f), beta(2.0f), gamma(15.0f), iterations_number(30) {};
}; */

namespace Neocortex 
{
 using namespace System;
 using namespace System::ComponentModel;
 using namespace System::Collections;
 using namespace System::Windows::Forms;
 using namespace System::Data;
 using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>

	//public delegate void MyVisibleDelegate_2D(size_t iLayer, size_t index, bool value);
	//public delegate void MyColorDelegate_2D(size_t iLayer, size_t index, TColor Value_Color);

	//public delegate void MyVisibleDelegate_3D(size_t index, bool value);
	//public delegate void MyColorDelegate_3D(size_t index, TColor Value_Color);

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		//void ChangeVisible_2D(size_t index, bool value)	{VoxelsData[index].IsVisible_2D = value;}
		//void ChangeVisible_3D(size_t index, bool value) {VoxelsData[index].IsVisible_3D = value;}

		//void ChangeVoxelColor_2D(size_t index, TColor Value_Color) {VoxelsData[index].Color_2D = Value_Color;}
		//void ChangeVoxelColor_3D(size_t index, TColor Value_Color) {VoxelsData[index].Color_3D = Value_Color;}

		
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			ReleaseTextures();

            if (hRC) {
                wglMakeCurrent(NULL,NULL);
                wglDeleteContext(hRC);
                hRC = NULL;
            }

            ReleaseDC(hWnd,hDC);
            hDC = NULL;
            if (components) {
                delete components;
            }
		}

	private: void ReleaseTextures()  // ������������ ��������, ������� ���������� 
			 {
              if (layerTextures) 
			  { 
               glDeleteTextures(InputData->sizeZ, layerTextures);
               delete [] layerTextures;
               layerTextures = NULL;
              }
             }

	protected: 

	private: System::Windows::Forms::MenuStrip^  MenuStrip;

	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_DownloadFile;
	private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_DownloadFolder;
	private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_Exit;


	private: System::Windows::Forms::Panel^  RenderingPanel;
	private: System::Windows::Forms::Label^  Label_ClustersCount;
	private: System::Windows::Forms::TrackBar^  TrackBar_ClustersCount;
	private: System::Windows::Forms::GroupBox^  GroupBoxSegmentation;

	private: System::Windows::Forms::OpenFileDialog^  OpenDataDialog;

	public:
		//MyVisibleDelegate^ StaticDelInst_Visible_2D;
		//MyVisibleDelegate^ StaticDelInst_Visible_3D;
		//MyColorDelegate^ StaticDelInst_Color_2D;
		//MyColorDelegate^ StaticDelInst_Color_3D;

		int PartPointer;							// ��������� �� ������� ������ ����
		System::String^ FolderPath;					// ���� � �������� �� ������
		vector <WCHAR*> *StrFiles;					// ������ ������ ����
		ScanData* InputData;						// �������� ������
		KmeansMethod* ClusterizationMethod;			// �����, ������������ ��� �������������
		TVoxel* VoxelsData;							// ������ ��������� �������� � ����������
		vector <TSegment>* Segments_2D;				// ������ ��������� ��� ������� ���� ������
		vector <TSegment>* Segments_3D;				// ������ ��������� ��� ���������� ���� ��� ������� ������
		TSnakeParameters* SnakeParameters;			// ��������� ������ ��������� �������

		ballPivot *qw;
		
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		/// <summary>
        /// Required designer variable.
        /// </summary>
        HDC     hDC;
        HGLRC   hRC;
        HWND    hWnd;
        
        GLint theBox;
		GLuint *layerTextures, shaderTexture;

        float angleXRotation, angleYRotation;		// ���� �������� ����������� ������ ������������ ����
		System::Drawing::Point mousePosition; 
		float distance_x, distance_y, distance_z;
		float depth;
	private:
		short MinDensity, MaxDensity;
		short low_density, high_density;

		float BrightnessMult;				  // �������� ��������� �������
		float AlphaTestValue;				  // �������� ������ ������������ alpha 

		size_t StartLayerIndex_3D;			  // ������ ���������� ���� � 3D-������ ���������� � �����������
		bool CellValueChanging;				  // ������� ��������� �������� ������ ������� ����� ��������������

		private: System::Windows::Forms::Timer^  RenderTimer;
		private: System::Windows::Forms::Label^  Label_Status;
		private: System::Windows::Forms::ProgressBar^  ProgressBar_Iterations;
		private: System::Windows::Forms::ProgressBar^  ProgressBar_Layers;

		private: System::Windows::Forms::DataGridView^  DataGridView_Clusters;

private: System::Windows::Forms::TrackBar^  TrackBar_IterationsCount;
private: System::Windows::Forms::Label^  Label_IterationsCount;

private: System::Windows::Forms::Button^  Button_Clusterization;
private: System::ComponentModel::BackgroundWorker^  BackgroundWorker;
private: System::Windows::Forms::Button^  Button_VisualizeSelectedClusters;

private: System::Windows::Forms::ColorDialog^  ColorDialog;
private: System::Windows::Forms::GroupBox^  GroupBoxSegmentsInfo;




private: System::Windows::Forms::CheckBox^  CheckBoxClusters;

private: System::Windows::Forms::Label^  Label_BrightnessMult;
private: System::Windows::Forms::TextBox^  TextBox_BrightnessMult;
private: System::Windows::Forms::CheckBox^  CheckBox_Transparency;
private: System::Windows::Forms::CheckBox^  CheckBox_DepthTest;

private: System::Windows::Forms::RadioButton^  RadioButton_2D;

private: System::Windows::Forms::GroupBox^  GroupBoxAlgorithm;

private: System::Windows::Forms::RadioButton^  RadioButton_Consequtive;
private: System::Windows::Forms::RadioButton^  RadioButton_GPU;

private: System::Windows::Forms::RadioButton^  RadioButton_3D;
private: System::Windows::Forms::GroupBox^  GroupBoxDiapason;

private: System::Windows::Forms::Label^  Label_HighBorder;

private: System::Windows::Forms::Label^  Label_LowBorder;
private: System::Windows::Forms::CheckBox^  CheckBox_ColorsExport;

private: System::Windows::Forms::TextBox^  TextBox_LowBorder;
private: System::Windows::Forms::TextBox^  TextBox_HighBorder;

private: System::Windows::Forms::FolderBrowserDialog^  FolderBrowserDialog;
private: System::Windows::Forms::ToolStripMenuItem^  ��Bin�����ToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ����������ToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  �Bin������ToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ��������ToolStripMenuItem;
private: System::Windows::Forms::TabControl^  TabControlOptions;

private: System::Windows::Forms::TabPage^  Visualization;
private: System::Windows::Forms::TabPage^  Segmentation;
private: System::Windows::Forms::TabPage^  Reconstruction;
private: System::Windows::Forms::TabPage^  Localization;
private: System::Windows::Forms::GroupBox^  GroupBox_Methods;

private: System::Windows::Forms::RadioButton^  RadioButtonTextures_2D;
private: System::Windows::Forms::RadioButton^  RadioButtonRaycasting;

private: System::Windows::Forms::RadioButton^  RadioButtonTextures_3D;
private: System::Windows::Forms::CheckBox^  CheckBox_AlphaTest;

private: System::Windows::Forms::TrackBar^  TrackBar_BrightnessMult;
private: System::Windows::Forms::Label^  Label_ThresholdAlphaValue;

private: System::Windows::Forms::TextBox^  TextBox_AlphaValue;

private: System::Windows::Forms::TrackBar^  TrackBar_Alpha;
private: System::Windows::Forms::Button^  Button_NextPart;

private: System::Windows::Forms::Button^  Button_PreviousPart;
private: System::Windows::Forms::Label^  Label_FinishLayer;

private: System::Windows::Forms::Label^  Label_StartLayer;

private: System::Windows::Forms::NumericUpDown^  NumericUpDown_Start;
private: System::Windows::Forms::Label^  Label_LayerInfo;

private: System::Windows::Forms::TrackBar^  TrackBar_Layers;
private: System::Windows::Forms::GroupBox^  GroupBox_Modes;


private: System::Windows::Forms::NumericUpDown^  NumericUpDown_Finish;
private: System::Windows::Forms::Label^  Label_LayersDistance;
private: System::Windows::Forms::GroupBox^  GroupBox_Scales;






private: System::Windows::Forms::TabPage^  Information;
private: System::Windows::Forms::Label^  Label_CurrentScaleZ;

private: System::Windows::Forms::Label^  Label_CurrentScaleY;


private: System::Windows::Forms::Label^  Label_CurrentScaleX;


private: System::Windows::Forms::TrackBar^  TrackBar_ScaleY;

private: System::Windows::Forms::TrackBar^  TrackBar_ScaleX;

private: System::Windows::Forms::TextBox^  TextBox_ScaleZ;

private: System::Windows::Forms::TextBox^  TextBox_ScaleY;

private: System::Windows::Forms::TextBox^  TextBox_ScaleX;
private: System::Windows::Forms::TrackBar^  TrackBar_ScaleZ;


private: System::Windows::Forms::DataGridViewTextBoxColumn^  SegmentNumber;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  SegmentVolume;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Density_Minimum;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Density_Maximum;
private: System::Windows::Forms::DataGridViewButtonColumn^  SegmentColor;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  SegmentTransparensy;
private: System::Windows::Forms::DataGridViewCheckBoxColumn^  IsSegmentVisible;
private: System::Windows::Forms::Label^  Label_LayersNumber;

private: System::Windows::Forms::Label^  Label_LayerHeight;

private: System::Windows::Forms::Label^  Label_LayerWidth;

private: System::Windows::Forms::Label^  Label_FileSize;
private: System::Windows::Forms::Label^  Label_InputScaleZ;

private: System::Windows::Forms::Label^  Label_InputScaleY;



private: System::Windows::Forms::Label^  Label_InputScaleX;
private: System::Windows::Forms::CheckBox^  CheckBox_SnakeSegmentation;





private: System::Windows::Forms::Button^  ButtonReconstructionSegments;
private: System::Windows::Forms::Button^  ButtonReconstructionData;
private: System::Windows::Forms::TrackBar^  TrackBar_LayersDistance;
private: System::Windows::Forms::GroupBox^  GroupBox_SnakeParameters;

private: System::Windows::Forms::Label^  Label_BetaValue;
private: System::Windows::Forms::Label^  Label_AlphaValue;
private: System::Windows::Forms::TrackBar^  TrackBar_IterationsNumber;



private: System::Windows::Forms::TrackBar^  TrackBar_GammaValue;

private: System::Windows::Forms::TrackBar^  TrackBar_BetaValue;

private: System::Windows::Forms::TrackBar^  TrackBar_AlphaValue;
private: System::Windows::Forms::Label^  Label_IterationsNumber;


private: System::Windows::Forms::Label^  Label_GammaValue;

private: System::Windows::Forms::Button^  ButtonLocalize;
private: System::Windows::Forms::CheckBox^  CheckBox_Gradients;
private: System::Windows::Forms::TextBox^  YPoint_TextBox;
private: System::Windows::Forms::TextBox^  XPoint_TextBox;
private: System::Windows::Forms::Label^  YPoint_Label;
private: System::Windows::Forms::Label^  XPoint_Label;
private: System::Windows::Forms::CheckBox^  CheckBox_Reconstruction;
private: System::Windows::Forms::CheckBox^  CheckBox_Localization;



















private: System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->MenuStrip = (gcnew System::Windows::Forms::MenuStrip());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_DownloadFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��Bin�����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����������ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_DownloadFolder = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�Bin������ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��������ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_Exit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->RenderingPanel = (gcnew System::Windows::Forms::Panel());
			this->Label_ClustersCount = (gcnew System::Windows::Forms::Label());
			this->TrackBar_ClustersCount = (gcnew System::Windows::Forms::TrackBar());
			this->GroupBoxSegmentation = (gcnew System::Windows::Forms::GroupBox());
			this->CheckBox_SnakeSegmentation = (gcnew System::Windows::Forms::CheckBox());
			this->GroupBoxAlgorithm = (gcnew System::Windows::Forms::GroupBox());
			this->RadioButton_Consequtive = (gcnew System::Windows::Forms::RadioButton());
			this->RadioButton_GPU = (gcnew System::Windows::Forms::RadioButton());
			this->TrackBar_IterationsCount = (gcnew System::Windows::Forms::TrackBar());
			this->GroupBoxDiapason = (gcnew System::Windows::Forms::GroupBox());
			this->TextBox_HighBorder = (gcnew System::Windows::Forms::TextBox());
			this->TextBox_LowBorder = (gcnew System::Windows::Forms::TextBox());
			this->Label_HighBorder = (gcnew System::Windows::Forms::Label());
			this->Label_LowBorder = (gcnew System::Windows::Forms::Label());
			this->CheckBox_ColorsExport = (gcnew System::Windows::Forms::CheckBox());
			this->Label_IterationsCount = (gcnew System::Windows::Forms::Label());
			this->CheckBoxClusters = (gcnew System::Windows::Forms::CheckBox());
			this->OpenDataDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->RenderTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->Label_Status = (gcnew System::Windows::Forms::Label());
			this->ProgressBar_Iterations = (gcnew System::Windows::Forms::ProgressBar());
			this->ProgressBar_Layers = (gcnew System::Windows::Forms::ProgressBar());
			this->DataGridView_Clusters = (gcnew System::Windows::Forms::DataGridView());
			this->SegmentNumber = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->SegmentVolume = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Density_Minimum = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Density_Maximum = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->SegmentColor = (gcnew System::Windows::Forms::DataGridViewButtonColumn());
			this->SegmentTransparensy = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->IsSegmentVisible = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
			this->Button_Clusterization = (gcnew System::Windows::Forms::Button());
			this->BackgroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->Button_VisualizeSelectedClusters = (gcnew System::Windows::Forms::Button());
			this->ColorDialog = (gcnew System::Windows::Forms::ColorDialog());
			this->GroupBoxSegmentsInfo = (gcnew System::Windows::Forms::GroupBox());
			this->Label_BrightnessMult = (gcnew System::Windows::Forms::Label());
			this->TextBox_BrightnessMult = (gcnew System::Windows::Forms::TextBox());
			this->CheckBox_Transparency = (gcnew System::Windows::Forms::CheckBox());
			this->CheckBox_DepthTest = (gcnew System::Windows::Forms::CheckBox());
			this->RadioButton_2D = (gcnew System::Windows::Forms::RadioButton());
			this->RadioButton_3D = (gcnew System::Windows::Forms::RadioButton());
			this->FolderBrowserDialog = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->TabControlOptions = (gcnew System::Windows::Forms::TabControl());
			this->Information = (gcnew System::Windows::Forms::TabPage());
			this->Label_InputScaleZ = (gcnew System::Windows::Forms::Label());
			this->Label_InputScaleY = (gcnew System::Windows::Forms::Label());
			this->Label_InputScaleX = (gcnew System::Windows::Forms::Label());
			this->Label_LayersNumber = (gcnew System::Windows::Forms::Label());
			this->Label_LayerHeight = (gcnew System::Windows::Forms::Label());
			this->Label_LayerWidth = (gcnew System::Windows::Forms::Label());
			this->Label_FileSize = (gcnew System::Windows::Forms::Label());
			this->Visualization = (gcnew System::Windows::Forms::TabPage());
			this->GroupBox_Scales = (gcnew System::Windows::Forms::GroupBox());
			this->TrackBar_ScaleZ = (gcnew System::Windows::Forms::TrackBar());
			this->TrackBar_ScaleY = (gcnew System::Windows::Forms::TrackBar());
			this->TrackBar_ScaleX = (gcnew System::Windows::Forms::TrackBar());
			this->TextBox_ScaleZ = (gcnew System::Windows::Forms::TextBox());
			this->TextBox_ScaleY = (gcnew System::Windows::Forms::TextBox());
			this->TextBox_ScaleX = (gcnew System::Windows::Forms::TextBox());
			this->Label_CurrentScaleZ = (gcnew System::Windows::Forms::Label());
			this->Label_CurrentScaleY = (gcnew System::Windows::Forms::Label());
			this->Label_CurrentScaleX = (gcnew System::Windows::Forms::Label());
			this->TrackBar_LayersDistance = (gcnew System::Windows::Forms::TrackBar());
			this->Label_LayersDistance = (gcnew System::Windows::Forms::Label());
			this->GroupBox_Modes = (gcnew System::Windows::Forms::GroupBox());
			this->NumericUpDown_Finish = (gcnew System::Windows::Forms::NumericUpDown());
			this->NumericUpDown_Start = (gcnew System::Windows::Forms::NumericUpDown());
			this->Label_LayerInfo = (gcnew System::Windows::Forms::Label());
			this->TrackBar_Layers = (gcnew System::Windows::Forms::TrackBar());
			this->Label_FinishLayer = (gcnew System::Windows::Forms::Label());
			this->Label_StartLayer = (gcnew System::Windows::Forms::Label());
			this->Button_NextPart = (gcnew System::Windows::Forms::Button());
			this->Button_PreviousPart = (gcnew System::Windows::Forms::Button());
			this->TextBox_AlphaValue = (gcnew System::Windows::Forms::TextBox());
			this->TrackBar_Alpha = (gcnew System::Windows::Forms::TrackBar());
			this->TrackBar_BrightnessMult = (gcnew System::Windows::Forms::TrackBar());
			this->Label_ThresholdAlphaValue = (gcnew System::Windows::Forms::Label());
			this->CheckBox_AlphaTest = (gcnew System::Windows::Forms::CheckBox());
			this->GroupBox_Methods = (gcnew System::Windows::Forms::GroupBox());
			this->RadioButtonRaycasting = (gcnew System::Windows::Forms::RadioButton());
			this->RadioButtonTextures_3D = (gcnew System::Windows::Forms::RadioButton());
			this->RadioButtonTextures_2D = (gcnew System::Windows::Forms::RadioButton());
			this->Localization = (gcnew System::Windows::Forms::TabPage());
			this->CheckBox_Localization = (gcnew System::Windows::Forms::CheckBox());
			this->YPoint_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->XPoint_TextBox = (gcnew System::Windows::Forms::TextBox());
			this->YPoint_Label = (gcnew System::Windows::Forms::Label());
			this->XPoint_Label = (gcnew System::Windows::Forms::Label());
			this->CheckBox_Gradients = (gcnew System::Windows::Forms::CheckBox());
			this->ButtonLocalize = (gcnew System::Windows::Forms::Button());
			this->GroupBox_SnakeParameters = (gcnew System::Windows::Forms::GroupBox());
			this->Label_IterationsNumber = (gcnew System::Windows::Forms::Label());
			this->Label_GammaValue = (gcnew System::Windows::Forms::Label());
			this->Label_BetaValue = (gcnew System::Windows::Forms::Label());
			this->Label_AlphaValue = (gcnew System::Windows::Forms::Label());
			this->TrackBar_IterationsNumber = (gcnew System::Windows::Forms::TrackBar());
			this->TrackBar_GammaValue = (gcnew System::Windows::Forms::TrackBar());
			this->TrackBar_BetaValue = (gcnew System::Windows::Forms::TrackBar());
			this->TrackBar_AlphaValue = (gcnew System::Windows::Forms::TrackBar());
			this->Segmentation = (gcnew System::Windows::Forms::TabPage());
			this->Reconstruction = (gcnew System::Windows::Forms::TabPage());
			this->CheckBox_Reconstruction = (gcnew System::Windows::Forms::CheckBox());
			this->ButtonReconstructionData = (gcnew System::Windows::Forms::Button());
			this->ButtonReconstructionSegments = (gcnew System::Windows::Forms::Button());
			this->MenuStrip->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_ClustersCount))->BeginInit();
			this->GroupBoxSegmentation->SuspendLayout();
			this->GroupBoxAlgorithm->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_IterationsCount))->BeginInit();
			this->GroupBoxDiapason->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->DataGridView_Clusters))->BeginInit();
			this->GroupBoxSegmentsInfo->SuspendLayout();
			this->TabControlOptions->SuspendLayout();
			this->Information->SuspendLayout();
			this->Visualization->SuspendLayout();
			this->GroupBox_Scales->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_ScaleZ))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_ScaleY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_ScaleX))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_LayersDistance))->BeginInit();
			this->GroupBox_Modes->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NumericUpDown_Finish))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NumericUpDown_Start))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_Layers))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_Alpha))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_BrightnessMult))->BeginInit();
			this->GroupBox_Methods->SuspendLayout();
			this->Localization->SuspendLayout();
			this->GroupBox_SnakeParameters->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_IterationsNumber))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_GammaValue))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_BetaValue))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_AlphaValue))->BeginInit();
			this->Segmentation->SuspendLayout();
			this->Reconstruction->SuspendLayout();
			this->SuspendLayout();
			// 
			// MenuStrip
			// 
			this->MenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->����ToolStripMenuItem});
			this->MenuStrip->Location = System::Drawing::Point(0, 0);
			this->MenuStrip->Name = L"MenuStrip";
			this->MenuStrip->Size = System::Drawing::Size(1273, 26);
			this->MenuStrip->TabIndex = 1;
			this->MenuStrip->Text = L"menuStrip1";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->ToolStripMenuItem_DownloadFile, 
				this->ToolStripMenuItem_DownloadFolder, this->ToolStripMenuItem_Exit});
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(54, 22);
			this->����ToolStripMenuItem->Text = L"����";
			// 
			// ToolStripMenuItem_DownloadFile
			// 
			this->ToolStripMenuItem_DownloadFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->��Bin�����ToolStripMenuItem, 
				this->����������ToolStripMenuItem});
			this->ToolStripMenuItem_DownloadFile->Name = L"ToolStripMenuItem_DownloadFile";
			this->ToolStripMenuItem_DownloadFile->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->ToolStripMenuItem_DownloadFile->Size = System::Drawing::Size(266, 22);
			this->ToolStripMenuItem_DownloadFile->Text = L"��������� ������";
			// 
			// ��Bin�����ToolStripMenuItem
			// 
			this->��Bin�����ToolStripMenuItem->Name = L"��Bin�����ToolStripMenuItem";
			this->��Bin�����ToolStripMenuItem->Size = System::Drawing::Size(195, 22);
			this->��Bin�����ToolStripMenuItem->Text = L"�� bin-�����...";
			this->��Bin�����ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ToolStripMenuItem_DownloadFile_Click);
			// 
			// ����������ToolStripMenuItem
			// 
			this->����������ToolStripMenuItem->Name = L"����������ToolStripMenuItem";
			this->����������ToolStripMenuItem->Size = System::Drawing::Size(195, 22);
			this->����������ToolStripMenuItem->Text = L"�� ��������...";
			this->����������ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ToolStripMenuItem_DownloadFolder_Click);
			// 
			// ToolStripMenuItem_DownloadFolder
			// 
			this->ToolStripMenuItem_DownloadFolder->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->�Bin������ToolStripMenuItem, 
				this->��������ToolStripMenuItem});
			this->ToolStripMenuItem_DownloadFolder->Name = L"ToolStripMenuItem_DownloadFolder";
			this->ToolStripMenuItem_DownloadFolder->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::E));
			this->ToolStripMenuItem_DownloadFolder->Size = System::Drawing::Size(266, 22);
			this->ToolStripMenuItem_DownloadFolder->Text = L"������� ������";
			// 
			// �Bin������ToolStripMenuItem
			// 
			this->�Bin������ToolStripMenuItem->Name = L"�Bin������ToolStripMenuItem";
			this->�Bin������ToolStripMenuItem->Size = System::Drawing::Size(197, 22);
			this->�Bin������ToolStripMenuItem->Text = L"� bin-������...";
			// 
			// ��������ToolStripMenuItem
			// 
			this->��������ToolStripMenuItem->Name = L"��������ToolStripMenuItem";
			this->��������ToolStripMenuItem->Size = System::Drawing::Size(197, 22);
			this->��������ToolStripMenuItem->Text = L"� �������...";
			// 
			// ToolStripMenuItem_Exit
			// 
			this->ToolStripMenuItem_Exit->Name = L"ToolStripMenuItem_Exit";
			this->ToolStripMenuItem_Exit->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::F4));
			this->ToolStripMenuItem_Exit->Size = System::Drawing::Size(266, 22);
			this->ToolStripMenuItem_Exit->Text = L"�����";
			this->ToolStripMenuItem_Exit->Click += gcnew System::EventHandler(this, &MainForm::ToolStripMenuItem_Exit_Click);
			// 
			// RenderingPanel
			// 
			this->RenderingPanel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->RenderingPanel->BackColor = System::Drawing::SystemColors::ControlText;
			this->RenderingPanel->Location = System::Drawing::Point(3, 29);
			this->RenderingPanel->Name = L"RenderingPanel";
			this->RenderingPanel->Size = System::Drawing::Size(714, 891);
			this->RenderingPanel->TabIndex = 3;
			this->RenderingPanel->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::RenderingPanel_MouseWheel);
			this->RenderingPanel->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &MainForm::RenderingPanel_PreviewKeyDown);
			this->RenderingPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::RenderingPanel_MouseMove);
			this->RenderingPanel->Click += gcnew System::EventHandler(this, &MainForm::RenderingPanel_Click);
			this->RenderingPanel->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::RenderingPanel_MouseClick);
			this->RenderingPanel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::RenderingPanel_MouseDown);
			// 
			// Label_ClustersCount
			// 
			this->Label_ClustersCount->AutoSize = true;
			this->Label_ClustersCount->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->Label_ClustersCount->Location = System::Drawing::Point(12, 28);
			this->Label_ClustersCount->Name = L"Label_ClustersCount";
			this->Label_ClustersCount->Size = System::Drawing::Size(223, 17);
			this->Label_ClustersCount->TabIndex = 4;
			this->Label_ClustersCount->Text = L"������������ ����� ���������:";
			// 
			// TrackBar_ClustersCount
			// 
			this->TrackBar_ClustersCount->Location = System::Drawing::Point(288, 24);
			this->TrackBar_ClustersCount->Maximum = 50;
			this->TrackBar_ClustersCount->Minimum = 2;
			this->TrackBar_ClustersCount->Name = L"TrackBar_ClustersCount";
			this->TrackBar_ClustersCount->Size = System::Drawing::Size(234, 56);
			this->TrackBar_ClustersCount->TabIndex = 5;
			this->TrackBar_ClustersCount->Value = 10;
			this->TrackBar_ClustersCount->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_ClustersCount_ValueChanged);
			// 
			// GroupBoxSegmentation
			// 
			this->GroupBoxSegmentation->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->GroupBoxSegmentation->BackColor = System::Drawing::SystemColors::Control;
			this->GroupBoxSegmentation->Controls->Add(this->CheckBox_SnakeSegmentation);
			this->GroupBoxSegmentation->Controls->Add(this->GroupBoxAlgorithm);
			this->GroupBoxSegmentation->Controls->Add(this->TrackBar_IterationsCount);
			this->GroupBoxSegmentation->Controls->Add(this->GroupBoxDiapason);
			this->GroupBoxSegmentation->Controls->Add(this->CheckBox_ColorsExport);
			this->GroupBoxSegmentation->Controls->Add(this->Label_IterationsCount);
			this->GroupBoxSegmentation->Controls->Add(this->CheckBoxClusters);
			this->GroupBoxSegmentation->Controls->Add(this->Label_ClustersCount);
			this->GroupBoxSegmentation->Controls->Add(this->TrackBar_ClustersCount);
			this->GroupBoxSegmentation->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->GroupBoxSegmentation->Location = System::Drawing::Point(3, 10);
			this->GroupBoxSegmentation->Name = L"GroupBoxSegmentation";
			this->GroupBoxSegmentation->Size = System::Drawing::Size(526, 438);
			this->GroupBoxSegmentation->TabIndex = 11;
			this->GroupBoxSegmentation->TabStop = false;
			this->GroupBoxSegmentation->Text = L"��������� �����������";
			// 
			// CheckBox_SnakeSegmentation
			// 
			this->CheckBox_SnakeSegmentation->AutoSize = true;
			this->CheckBox_SnakeSegmentation->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->CheckBox_SnakeSegmentation->Location = System::Drawing::Point(47, 332);
			this->CheckBox_SnakeSegmentation->Name = L"CheckBox_SnakeSegmentation";
			this->CheckBox_SnakeSegmentation->Size = System::Drawing::Size(465, 21);
			this->CheckBox_SnakeSegmentation->TabIndex = 20;
			this->CheckBox_SnakeSegmentation->Text = L"�������������� �������, ��������� ������� ��������� �������";
			this->CheckBox_SnakeSegmentation->UseVisualStyleBackColor = true;
			// 
			// GroupBoxAlgorithm
			// 
			this->GroupBoxAlgorithm->Controls->Add(this->RadioButton_Consequtive);
			this->GroupBoxAlgorithm->Controls->Add(this->RadioButton_GPU);
			this->GroupBoxAlgorithm->Location = System::Drawing::Point(46, 243);
			this->GroupBoxAlgorithm->Name = L"GroupBoxAlgorithm";
			this->GroupBoxAlgorithm->Size = System::Drawing::Size(430, 61);
			this->GroupBoxAlgorithm->TabIndex = 10;
			this->GroupBoxAlgorithm->TabStop = false;
			this->GroupBoxAlgorithm->Text = L"�������� �����������";
			// 
			// RadioButton_Consequtive
			// 
			this->RadioButton_Consequtive->AutoSize = true;
			this->RadioButton_Consequtive->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->RadioButton_Consequtive->Location = System::Drawing::Point(19, 27);
			this->RadioButton_Consequtive->Name = L"RadioButton_Consequtive";
			this->RadioButton_Consequtive->Size = System::Drawing::Size(155, 21);
			this->RadioButton_Consequtive->TabIndex = 8;
			this->RadioButton_Consequtive->Text = L"����������������";
			this->RadioButton_Consequtive->UseVisualStyleBackColor = true;
			// 
			// RadioButton_GPU
			// 
			this->RadioButton_GPU->AutoSize = true;
			this->RadioButton_GPU->Checked = true;
			this->RadioButton_GPU->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->RadioButton_GPU->Location = System::Drawing::Point(210, 27);
			this->RadioButton_GPU->Name = L"RadioButton_GPU";
			this->RadioButton_GPU->Size = System::Drawing::Size(188, 21);
			this->RadioButton_GPU->TabIndex = 9;
			this->RadioButton_GPU->TabStop = true;
			this->RadioButton_GPU->Text = L"������������ ��� GPU";
			this->RadioButton_GPU->UseVisualStyleBackColor = true;
			// 
			// TrackBar_IterationsCount
			// 
			this->TrackBar_IterationsCount->Location = System::Drawing::Point(290, 74);
			this->TrackBar_IterationsCount->Maximum = 50;
			this->TrackBar_IterationsCount->Minimum = 1;
			this->TrackBar_IterationsCount->Name = L"TrackBar_IterationsCount";
			this->TrackBar_IterationsCount->Size = System::Drawing::Size(233, 56);
			this->TrackBar_IterationsCount->TabIndex = 7;
			this->TrackBar_IterationsCount->Value = 10;
			this->TrackBar_IterationsCount->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_IterationsCount_ValueChanged);
			// 
			// GroupBoxDiapason
			// 
			this->GroupBoxDiapason->Controls->Add(this->TextBox_HighBorder);
			this->GroupBoxDiapason->Controls->Add(this->TextBox_LowBorder);
			this->GroupBoxDiapason->Controls->Add(this->Label_HighBorder);
			this->GroupBoxDiapason->Controls->Add(this->Label_LowBorder);
			this->GroupBoxDiapason->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->GroupBoxDiapason->Location = System::Drawing::Point(46, 136);
			this->GroupBoxDiapason->Name = L"GroupBoxDiapason";
			this->GroupBoxDiapason->Size = System::Drawing::Size(430, 79);
			this->GroupBoxDiapason->TabIndex = 11;
			this->GroupBoxDiapason->TabStop = false;
			this->GroupBoxDiapason->Text = L"�������� �������� ���������";
			// 
			// TextBox_HighBorder
			// 
			this->TextBox_HighBorder->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->TextBox_HighBorder->Location = System::Drawing::Point(359, 36);
			this->TextBox_HighBorder->Name = L"TextBox_HighBorder";
			this->TextBox_HighBorder->Size = System::Drawing::Size(52, 22);
			this->TextBox_HighBorder->TabIndex = 4;
			this->TextBox_HighBorder->TextChanged += gcnew System::EventHandler(this, &MainForm::TextBox_HighBorder_TextChanged);
			// 
			// TextBox_LowBorder
			// 
			this->TextBox_LowBorder->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->TextBox_LowBorder->Location = System::Drawing::Point(150, 36);
			this->TextBox_LowBorder->Name = L"TextBox_LowBorder";
			this->TextBox_LowBorder->Size = System::Drawing::Size(52, 22);
			this->TextBox_LowBorder->TabIndex = 3;
			this->TextBox_LowBorder->TextChanged += gcnew System::EventHandler(this, &MainForm::TextBox_LowBorder_TextChanged);
			// 
			// Label_HighBorder
			// 
			this->Label_HighBorder->AutoSize = true;
			this->Label_HighBorder->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->Label_HighBorder->Location = System::Drawing::Point(217, 38);
			this->Label_HighBorder->Name = L"Label_HighBorder";
			this->Label_HighBorder->Size = System::Drawing::Size(124, 17);
			this->Label_HighBorder->TabIndex = 1;
			this->Label_HighBorder->Text = L"������� �������:";
			// 
			// Label_LowBorder
			// 
			this->Label_LowBorder->AutoSize = true;
			this->Label_LowBorder->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->Label_LowBorder->Location = System::Drawing::Point(13, 38);
			this->Label_LowBorder->Name = L"Label_LowBorder";
			this->Label_LowBorder->Size = System::Drawing::Size(120, 17);
			this->Label_LowBorder->TabIndex = 0;
			this->Label_LowBorder->Text = L"������ �������:";
			// 
			// CheckBox_ColorsExport
			// 
			this->CheckBox_ColorsExport->AutoSize = true;
			this->CheckBox_ColorsExport->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->CheckBox_ColorsExport->Location = System::Drawing::Point(47, 411);
			this->CheckBox_ColorsExport->Name = L"CheckBox_ColorsExport";
			this->CheckBox_ColorsExport->Size = System::Drawing::Size(375, 21);
			this->CheckBox_ColorsExport->TabIndex = 19;
			this->CheckBox_ColorsExport->Text = L"�������������� �������� ��������� �������� ����";
			this->CheckBox_ColorsExport->UseVisualStyleBackColor = true;
			this->CheckBox_ColorsExport->Click += gcnew System::EventHandler(this, &MainForm::CheckBox_ColorsExport_Click);
			// 
			// Label_IterationsCount
			// 
			this->Label_IterationsCount->AutoSize = true;
			this->Label_IterationsCount->Location = System::Drawing::Point(10, 79);
			this->Label_IterationsCount->Name = L"Label_IterationsCount";
			this->Label_IterationsCount->Size = System::Drawing::Size(230, 17);
			this->Label_IterationsCount->TabIndex = 6;
			this->Label_IterationsCount->Text = L"���������� �������� ���������:";
			// 
			// CheckBoxClusters
			// 
			this->CheckBoxClusters->AutoSize = true;
			this->CheckBoxClusters->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->CheckBoxClusters->Location = System::Drawing::Point(47, 371);
			this->CheckBoxClusters->Name = L"CheckBoxClusters";
			this->CheckBoxClusters->Size = System::Drawing::Size(177, 21);
			this->CheckBoxClusters->TabIndex = 0;
			this->CheckBoxClusters->Text = L"���������� ��������";
			this->CheckBoxClusters->UseVisualStyleBackColor = true;
			this->CheckBoxClusters->Click += gcnew System::EventHandler(this, &MainForm::CheckBoxClusters_Click);
			this->CheckBoxClusters->CheckedChanged += gcnew System::EventHandler(this, &MainForm::CheckBoxClusters_CheckedChanged);
			// 
			// OpenDataDialog
			// 
			this->OpenDataDialog->FileName = L"*.bin";
			this->OpenDataDialog->Filter = L"bin-�����(*.bin)|*.bin|All Files(*.*)|*.*";
			this->OpenDataDialog->RestoreDirectory = true;
			this->OpenDataDialog->Title = L"��������� bin-����";
			// 
			// RenderTimer
			// 
			this->RenderTimer->Enabled = true;
			this->RenderTimer->Interval = 30;
			this->RenderTimer->Tick += gcnew System::EventHandler(this, &MainForm::RenderTimer_Tick);
			// 
			// Label_Status
			// 
			this->Label_Status->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->Label_Status->AutoSize = true;
			this->Label_Status->Location = System::Drawing::Point(7, 924);
			this->Label_Status->Name = L"Label_Status";
			this->Label_Status->Size = System::Drawing::Size(91, 17);
			this->Label_Status->TabIndex = 12;
			this->Label_Status->Text = L"Label_Status";
			// 
			// ProgressBar_Iterations
			// 
			this->ProgressBar_Iterations->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->ProgressBar_Iterations->Location = System::Drawing::Point(3, 945);
			this->ProgressBar_Iterations->Name = L"ProgressBar_Iterations";
			this->ProgressBar_Iterations->Size = System::Drawing::Size(714, 23);
			this->ProgressBar_Iterations->Step = 1;
			this->ProgressBar_Iterations->TabIndex = 13;
			// 
			// ProgressBar_Layers
			// 
			this->ProgressBar_Layers->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->ProgressBar_Layers->Location = System::Drawing::Point(3, 974);
			this->ProgressBar_Layers->Name = L"ProgressBar_Layers";
			this->ProgressBar_Layers->Size = System::Drawing::Size(714, 23);
			this->ProgressBar_Layers->Step = 1;
			this->ProgressBar_Layers->TabIndex = 14;
			// 
			// DataGridView_Clusters
			// 
			this->DataGridView_Clusters->AllowUserToAddRows = false;
			this->DataGridView_Clusters->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->DataGridView_Clusters->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->DataGridView_Clusters->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(7) {this->SegmentNumber, 
				this->SegmentVolume, this->Density_Minimum, this->Density_Maximum, this->SegmentColor, this->SegmentTransparensy, this->IsSegmentVisible});
			this->DataGridView_Clusters->Location = System::Drawing::Point(6, 23);
			this->DataGridView_Clusters->Name = L"DataGridView_Clusters";
			this->DataGridView_Clusters->RowTemplate->Height = 24;
			this->DataGridView_Clusters->Size = System::Drawing::Size(517, 188);
			this->DataGridView_Clusters->TabIndex = 15;
			this->DataGridView_Clusters->CellValueChanged += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::DataGridView_Clusters_CellValueChanged);
			this->DataGridView_Clusters->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::DataGridView_Clusters_CellEndEdit);
			this->DataGridView_Clusters->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::DataGridView_Clusters_CellClick);
			// 
			// SegmentNumber
			// 
			this->SegmentNumber->HeaderText = L"������ ��������";
			this->SegmentNumber->Name = L"SegmentNumber";
			this->SegmentNumber->ReadOnly = true;
			// 
			// SegmentVolume
			// 
			this->SegmentVolume->HeaderText = L"����� ��������";
			this->SegmentVolume->Name = L"SegmentVolume";
			this->SegmentVolume->ReadOnly = true;
			// 
			// Density_Minimum
			// 
			this->Density_Minimum->HeaderText = L"����������� ���������";
			this->Density_Minimum->Name = L"Density_Minimum";
			this->Density_Minimum->ReadOnly = true;
			// 
			// Density_Maximum
			// 
			this->Density_Maximum->HeaderText = L"������������ ���������";
			this->Density_Maximum->Name = L"Density_Maximum";
			this->Density_Maximum->ReadOnly = true;
			// 
			// SegmentColor
			// 
			dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
			dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->SegmentColor->DefaultCellStyle = dataGridViewCellStyle1;
			this->SegmentColor->HeaderText = L"����";
			this->SegmentColor->Name = L"SegmentColor";
			this->SegmentColor->Resizable = System::Windows::Forms::DataGridViewTriState::True;
			// 
			// SegmentTransparensy
			// 
			this->SegmentTransparensy->HeaderText = L"��������������";
			this->SegmentTransparensy->Name = L"SegmentTransparensy";
			// 
			// IsSegmentVisible
			// 
			this->IsSegmentVisible->HeaderText = L"����������";
			this->IsSegmentVisible->Name = L"IsSegmentVisible";
			// 
			// Button_Clusterization
			// 
			this->Button_Clusterization->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->Button_Clusterization->Location = System::Drawing::Point(41, 469);
			this->Button_Clusterization->Name = L"Button_Clusterization";
			this->Button_Clusterization->Size = System::Drawing::Size(183, 32);
			this->Button_Clusterization->TabIndex = 16;
			this->Button_Clusterization->Text = L"�������������� ������";
			this->Button_Clusterization->UseVisualStyleBackColor = true;
			this->Button_Clusterization->Click += gcnew System::EventHandler(this, &MainForm::Button_Clusterization_Click);
			// 
			// BackgroundWorker
			// 
			this->BackgroundWorker->WorkerReportsProgress = true;
			this->BackgroundWorker->WorkerSupportsCancellation = true;
			this->BackgroundWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainForm::BackgroundWorker_DoWork);
			this->BackgroundWorker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &MainForm::BackgroundWorker_RunWorkerCompleted);
			this->BackgroundWorker->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &MainForm::BackgroundWorker_ProgressChanged);
			// 
			// Button_VisualizeSelectedClusters
			// 
			this->Button_VisualizeSelectedClusters->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->Button_VisualizeSelectedClusters->Location = System::Drawing::Point(244, 469);
			this->Button_VisualizeSelectedClusters->Name = L"Button_VisualizeSelectedClusters";
			this->Button_VisualizeSelectedClusters->Size = System::Drawing::Size(241, 32);
			this->Button_VisualizeSelectedClusters->TabIndex = 17;
			this->Button_VisualizeSelectedClusters->Text = L"���������� ��������� ��������";
			this->Button_VisualizeSelectedClusters->UseVisualStyleBackColor = true;
			this->Button_VisualizeSelectedClusters->Click += gcnew System::EventHandler(this, &MainForm::Button_VisualizeSelectedClusters_Click);
			// 
			// GroupBoxSegmentsInfo
			// 
			this->GroupBoxSegmentsInfo->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->GroupBoxSegmentsInfo->Controls->Add(this->DataGridView_Clusters);
			this->GroupBoxSegmentsInfo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->GroupBoxSegmentsInfo->Location = System::Drawing::Point(1, 524);
			this->GroupBoxSegmentsInfo->Name = L"GroupBoxSegmentsInfo";
			this->GroupBoxSegmentsInfo->Size = System::Drawing::Size(533, 217);
			this->GroupBoxSegmentsInfo->TabIndex = 18;
			this->GroupBoxSegmentsInfo->TabStop = false;
			this->GroupBoxSegmentsInfo->Text = L"���������� � ���������";
			// 
			// Label_BrightnessMult
			// 
			this->Label_BrightnessMult->AutoSize = true;
			this->Label_BrightnessMult->Location = System::Drawing::Point(15, 181);
			this->Label_BrightnessMult->Name = L"Label_BrightnessMult";
			this->Label_BrightnessMult->Size = System::Drawing::Size(143, 17);
			this->Label_BrightnessMult->TabIndex = 9;
			this->Label_BrightnessMult->Text = L"��������� �������:";
			// 
			// TextBox_BrightnessMult
			// 
			this->TextBox_BrightnessMult->Location = System::Drawing::Point(164, 181);
			this->TextBox_BrightnessMult->Name = L"TextBox_BrightnessMult";
			this->TextBox_BrightnessMult->Size = System::Drawing::Size(56, 22);
			this->TextBox_BrightnessMult->TabIndex = 10;
			this->TextBox_BrightnessMult->Text = L"30";
			this->TextBox_BrightnessMult->TextChanged += gcnew System::EventHandler(this, &MainForm::TextBox_BrightnessMult_TextChanged);
			// 
			// CheckBox_Transparency
			// 
			this->CheckBox_Transparency->AutoSize = true;
			this->CheckBox_Transparency->Location = System::Drawing::Point(244, 134);
			this->CheckBox_Transparency->Name = L"CheckBox_Transparency";
			this->CheckBox_Transparency->Size = System::Drawing::Size(124, 21);
			this->CheckBox_Transparency->TabIndex = 11;
			this->CheckBox_Transparency->Text = L"������������";
			this->CheckBox_Transparency->UseVisualStyleBackColor = true;
			this->CheckBox_Transparency->CheckedChanged += gcnew System::EventHandler(this, &MainForm::CheckBox_Transparency_CheckedChanged);
			// 
			// CheckBox_DepthTest
			// 
			this->CheckBox_DepthTest->AutoSize = true;
			this->CheckBox_DepthTest->Enabled = false;
			this->CheckBox_DepthTest->Location = System::Drawing::Point(393, 134);
			this->CheckBox_DepthTest->Name = L"CheckBox_DepthTest";
			this->CheckBox_DepthTest->Size = System::Drawing::Size(119, 21);
			this->CheckBox_DepthTest->TabIndex = 12;
			this->CheckBox_DepthTest->Text = L"���� �������";
			this->CheckBox_DepthTest->UseVisualStyleBackColor = true;
			// 
			// RadioButton_2D
			// 
			this->RadioButton_2D->AutoSize = true;
			this->RadioButton_2D->Checked = true;
			this->RadioButton_2D->Location = System::Drawing::Point(16, 34);
			this->RadioButton_2D->Name = L"RadioButton_2D";
			this->RadioButton_2D->Size = System::Drawing::Size(168, 21);
			this->RadioButton_2D->TabIndex = 1;
			this->RadioButton_2D->TabStop = true;
			this->RadioButton_2D->Text = L"�� ������ ���� (2D)";
			this->RadioButton_2D->UseVisualStyleBackColor = true;
			this->RadioButton_2D->CheckedChanged += gcnew System::EventHandler(this, &MainForm::RadioButton_2D_CheckedChanged);
			// 
			// RadioButton_3D
			// 
			this->RadioButton_3D->AutoSize = true;
			this->RadioButton_3D->Location = System::Drawing::Point(16, 61);
			this->RadioButton_3D->Name = L"RadioButton_3D";
			this->RadioButton_3D->Size = System::Drawing::Size(242, 21);
			this->RadioButton_3D->TabIndex = 0;
			this->RadioButton_3D->Text = L"������������������ ���� (3D)";
			this->RadioButton_3D->UseVisualStyleBackColor = true;
			this->RadioButton_3D->CheckedChanged += gcnew System::EventHandler(this, &MainForm::RadioButton_3D_CheckedChanged);
			// 
			// FolderBrowserDialog
			// 
			this->FolderBrowserDialog->Description = L"����� �������� � ������� ����:";
			this->FolderBrowserDialog->RootFolder = System::Environment::SpecialFolder::MyComputer;
			this->FolderBrowserDialog->ShowNewFolderButton = false;
			// 
			// TabControlOptions
			// 
			this->TabControlOptions->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->TabControlOptions->Controls->Add(this->Information);
			this->TabControlOptions->Controls->Add(this->Visualization);
			this->TabControlOptions->Controls->Add(this->Localization);
			this->TabControlOptions->Controls->Add(this->Segmentation);
			this->TabControlOptions->Controls->Add(this->Reconstruction);
			this->TabControlOptions->Location = System::Drawing::Point(730, 28);
			this->TabControlOptions->Name = L"TabControlOptions";
			this->TabControlOptions->SelectedIndex = 0;
			this->TabControlOptions->Size = System::Drawing::Size(543, 969);
			this->TabControlOptions->TabIndex = 19;
			this->TabControlOptions->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::TabControlOptions_SelectedIndexChanged);
			// 
			// Information
			// 
			this->Information->BackColor = System::Drawing::SystemColors::Control;
			this->Information->Controls->Add(this->Label_InputScaleZ);
			this->Information->Controls->Add(this->Label_InputScaleY);
			this->Information->Controls->Add(this->Label_InputScaleX);
			this->Information->Controls->Add(this->Label_LayersNumber);
			this->Information->Controls->Add(this->Label_LayerHeight);
			this->Information->Controls->Add(this->Label_LayerWidth);
			this->Information->Controls->Add(this->Label_FileSize);
			this->Information->Location = System::Drawing::Point(4, 25);
			this->Information->Name = L"Information";
			this->Information->Size = System::Drawing::Size(535, 940);
			this->Information->TabIndex = 4;
			this->Information->Text = L"����������";
			// 
			// Label_InputScaleZ
			// 
			this->Label_InputScaleZ->AutoSize = true;
			this->Label_InputScaleZ->Location = System::Drawing::Point(14, 252);
			this->Label_InputScaleZ->Name = L"Label_InputScaleZ";
			this->Label_InputScaleZ->Size = System::Drawing::Size(243, 17);
			this->Label_InputScaleZ->TabIndex = 6;
			this->Label_InputScaleZ->Text = L"������������ ������ ����� ��� Z:";
			// 
			// Label_InputScaleY
			// 
			this->Label_InputScaleY->AutoSize = true;
			this->Label_InputScaleY->Location = System::Drawing::Point(14, 213);
			this->Label_InputScaleY->Name = L"Label_InputScaleY";
			this->Label_InputScaleY->Size = System::Drawing::Size(243, 17);
			this->Label_InputScaleY->TabIndex = 5;
			this->Label_InputScaleY->Text = L"������������ ������ ����� ��� Y:";
			// 
			// Label_InputScaleX
			// 
			this->Label_InputScaleX->AutoSize = true;
			this->Label_InputScaleX->Location = System::Drawing::Point(14, 174);
			this->Label_InputScaleX->Name = L"Label_InputScaleX";
			this->Label_InputScaleX->Size = System::Drawing::Size(243, 17);
			this->Label_InputScaleX->TabIndex = 4;
			this->Label_InputScaleX->Text = L"������������ ������ ����� ��� X:";
			// 
			// Label_LayersNumber
			// 
			this->Label_LayersNumber->AutoSize = true;
			this->Label_LayersNumber->Location = System::Drawing::Point(14, 132);
			this->Label_LayersNumber->Name = L"Label_LayersNumber";
			this->Label_LayersNumber->Size = System::Drawing::Size(220, 17);
			this->Label_LayersNumber->TabIndex = 3;
			this->Label_LayersNumber->Text = L"���������� ����������� ����:";
			// 
			// Label_LayerHeight
			// 
			this->Label_LayerHeight->AutoSize = true;
			this->Label_LayerHeight->Location = System::Drawing::Point(14, 92);
			this->Label_LayerHeight->Name = L"Label_LayerHeight";
			this->Label_LayerHeight->Size = System::Drawing::Size(153, 17);
			this->Label_LayerHeight->TabIndex = 2;
			this->Label_LayerHeight->Text = L"������ ������� ����:";
			// 
			// Label_LayerWidth
			// 
			this->Label_LayerWidth->AutoSize = true;
			this->Label_LayerWidth->Location = System::Drawing::Point(14, 54);
			this->Label_LayerWidth->Name = L"Label_LayerWidth";
			this->Label_LayerWidth->Size = System::Drawing::Size(155, 17);
			this->Label_LayerWidth->TabIndex = 1;
			this->Label_LayerWidth->Text = L"������ ������� ����:";
			// 
			// Label_FileSize
			// 
			this->Label_FileSize->AutoSize = true;
			this->Label_FileSize->Location = System::Drawing::Point(14, 19);
			this->Label_FileSize->Name = L"Label_FileSize";
			this->Label_FileSize->Size = System::Drawing::Size(225, 17);
			this->Label_FileSize->TabIndex = 0;
			this->Label_FileSize->Text = L"������ ����� �������� ������:";
			// 
			// Visualization
			// 
			this->Visualization->BackColor = System::Drawing::SystemColors::Control;
			this->Visualization->Controls->Add(this->GroupBox_Scales);
			this->Visualization->Controls->Add(this->TrackBar_LayersDistance);
			this->Visualization->Controls->Add(this->Label_LayersDistance);
			this->Visualization->Controls->Add(this->GroupBox_Modes);
			this->Visualization->Controls->Add(this->NumericUpDown_Finish);
			this->Visualization->Controls->Add(this->NumericUpDown_Start);
			this->Visualization->Controls->Add(this->Label_LayerInfo);
			this->Visualization->Controls->Add(this->TrackBar_Layers);
			this->Visualization->Controls->Add(this->Label_FinishLayer);
			this->Visualization->Controls->Add(this->Label_StartLayer);
			this->Visualization->Controls->Add(this->Button_NextPart);
			this->Visualization->Controls->Add(this->Button_PreviousPart);
			this->Visualization->Controls->Add(this->TextBox_AlphaValue);
			this->Visualization->Controls->Add(this->TrackBar_Alpha);
			this->Visualization->Controls->Add(this->TrackBar_BrightnessMult);
			this->Visualization->Controls->Add(this->Label_ThresholdAlphaValue);
			this->Visualization->Controls->Add(this->CheckBox_AlphaTest);
			this->Visualization->Controls->Add(this->GroupBox_Methods);
			this->Visualization->Controls->Add(this->Label_BrightnessMult);
			this->Visualization->Controls->Add(this->TextBox_BrightnessMult);
			this->Visualization->Controls->Add(this->CheckBox_Transparency);
			this->Visualization->Controls->Add(this->CheckBox_DepthTest);
			this->Visualization->Location = System::Drawing::Point(4, 25);
			this->Visualization->Name = L"Visualization";
			this->Visualization->Padding = System::Windows::Forms::Padding(3);
			this->Visualization->Size = System::Drawing::Size(535, 940);
			this->Visualization->TabIndex = 0;
			this->Visualization->Text = L"������������";
			// 
			// GroupBox_Scales
			// 
			this->GroupBox_Scales->Controls->Add(this->TrackBar_ScaleZ);
			this->GroupBox_Scales->Controls->Add(this->TrackBar_ScaleY);
			this->GroupBox_Scales->Controls->Add(this->TrackBar_ScaleX);
			this->GroupBox_Scales->Controls->Add(this->TextBox_ScaleZ);
			this->GroupBox_Scales->Controls->Add(this->TextBox_ScaleY);
			this->GroupBox_Scales->Controls->Add(this->TextBox_ScaleX);
			this->GroupBox_Scales->Controls->Add(this->Label_CurrentScaleZ);
			this->GroupBox_Scales->Controls->Add(this->Label_CurrentScaleY);
			this->GroupBox_Scales->Controls->Add(this->Label_CurrentScaleX);
			this->GroupBox_Scales->Location = System::Drawing::Point(24, 548);
			this->GroupBox_Scales->Name = L"GroupBox_Scales";
			this->GroupBox_Scales->Size = System::Drawing::Size(488, 186);
			this->GroupBox_Scales->TabIndex = 33;
			this->GroupBox_Scales->TabStop = false;
			this->GroupBox_Scales->Text = L"�������";
			// 
			// TrackBar_ScaleZ
			// 
			this->TrackBar_ScaleZ->Enabled = false;
			this->TrackBar_ScaleZ->Location = System::Drawing::Point(241, 126);
			this->TrackBar_ScaleZ->Maximum = 1000;
			this->TrackBar_ScaleZ->Name = L"TrackBar_ScaleZ";
			this->TrackBar_ScaleZ->Size = System::Drawing::Size(242, 56);
			this->TrackBar_ScaleZ->TabIndex = 8;
			this->TrackBar_ScaleZ->Value = 1000;
			this->TrackBar_ScaleZ->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_ScaleZ_ValueChanged);
			// 
			// TrackBar_ScaleY
			// 
			this->TrackBar_ScaleY->Enabled = false;
			this->TrackBar_ScaleY->Location = System::Drawing::Point(240, 78);
			this->TrackBar_ScaleY->Maximum = 1000;
			this->TrackBar_ScaleY->Name = L"TrackBar_ScaleY";
			this->TrackBar_ScaleY->Size = System::Drawing::Size(243, 56);
			this->TrackBar_ScaleY->TabIndex = 7;
			this->TrackBar_ScaleY->Value = 1000;
			this->TrackBar_ScaleY->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_ScaleY_ValueChanged);
			// 
			// TrackBar_ScaleX
			// 
			this->TrackBar_ScaleX->Enabled = false;
			this->TrackBar_ScaleX->Location = System::Drawing::Point(241, 31);
			this->TrackBar_ScaleX->Maximum = 1000;
			this->TrackBar_ScaleX->Name = L"TrackBar_ScaleX";
			this->TrackBar_ScaleX->Size = System::Drawing::Size(241, 56);
			this->TrackBar_ScaleX->TabIndex = 6;
			this->TrackBar_ScaleX->Value = 1000;
			this->TrackBar_ScaleX->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_ScaleX_ValueChanged);
			// 
			// TextBox_ScaleZ
			// 
			this->TextBox_ScaleZ->Enabled = false;
			this->TextBox_ScaleZ->Location = System::Drawing::Point(117, 129);
			this->TextBox_ScaleZ->Name = L"TextBox_ScaleZ";
			this->TextBox_ScaleZ->Size = System::Drawing::Size(118, 22);
			this->TextBox_ScaleZ->TabIndex = 5;
			this->TextBox_ScaleZ->Text = L"1,000";
			this->TextBox_ScaleZ->TextChanged += gcnew System::EventHandler(this, &MainForm::TextBox_ScaleZ_TextChanged);
			// 
			// TextBox_ScaleY
			// 
			this->TextBox_ScaleY->Enabled = false;
			this->TextBox_ScaleY->Location = System::Drawing::Point(117, 81);
			this->TextBox_ScaleY->Name = L"TextBox_ScaleY";
			this->TextBox_ScaleY->Size = System::Drawing::Size(118, 22);
			this->TextBox_ScaleY->TabIndex = 4;
			this->TextBox_ScaleY->Text = L"1,000";
			this->TextBox_ScaleY->TextChanged += gcnew System::EventHandler(this, &MainForm::TextBox_ScaleY_TextChanged);
			// 
			// TextBox_ScaleX
			// 
			this->TextBox_ScaleX->Enabled = false;
			this->TextBox_ScaleX->Location = System::Drawing::Point(117, 35);
			this->TextBox_ScaleX->Name = L"TextBox_ScaleX";
			this->TextBox_ScaleX->Size = System::Drawing::Size(118, 22);
			this->TextBox_ScaleX->TabIndex = 3;
			this->TextBox_ScaleX->Text = L"1,000";
			this->TextBox_ScaleX->TextChanged += gcnew System::EventHandler(this, &MainForm::TextBox_ScaleX_TextChanged);
			// 
			// Label_CurrentScaleZ
			// 
			this->Label_CurrentScaleZ->AutoSize = true;
			this->Label_CurrentScaleZ->Location = System::Drawing::Point(19, 132);
			this->Label_CurrentScaleZ->Name = L"Label_CurrentScaleZ";
			this->Label_CurrentScaleZ->Size = System::Drawing::Size(92, 17);
			this->Label_CurrentScaleZ->TabIndex = 2;
			this->Label_CurrentScaleZ->Text = L"����� ��� Z:";
			// 
			// Label_CurrentScaleY
			// 
			this->Label_CurrentScaleY->AutoSize = true;
			this->Label_CurrentScaleY->Location = System::Drawing::Point(19, 84);
			this->Label_CurrentScaleY->Name = L"Label_CurrentScaleY";
			this->Label_CurrentScaleY->Size = System::Drawing::Size(92, 17);
			this->Label_CurrentScaleY->TabIndex = 1;
			this->Label_CurrentScaleY->Text = L"����� ��� Y:";
			// 
			// Label_CurrentScaleX
			// 
			this->Label_CurrentScaleX->AutoSize = true;
			this->Label_CurrentScaleX->Location = System::Drawing::Point(19, 38);
			this->Label_CurrentScaleX->Name = L"Label_CurrentScaleX";
			this->Label_CurrentScaleX->Size = System::Drawing::Size(92, 17);
			this->Label_CurrentScaleX->TabIndex = 0;
			this->Label_CurrentScaleX->Text = L"����� ��� X:";
			// 
			// TrackBar_LayersDistance
			// 
			this->TrackBar_LayersDistance->Location = System::Drawing::Point(231, 302);
			this->TrackBar_LayersDistance->Maximum = 1000;
			this->TrackBar_LayersDistance->Minimum = 1;
			this->TrackBar_LayersDistance->Name = L"TrackBar_LayersDistance";
			this->TrackBar_LayersDistance->Size = System::Drawing::Size(284, 56);
			this->TrackBar_LayersDistance->TabIndex = 32;
			this->TrackBar_LayersDistance->Value = 1;
			this->TrackBar_LayersDistance->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_LayersDistance_ValueChanged);
			// 
			// Label_LayersDistance
			// 
			this->Label_LayersDistance->AutoSize = true;
			this->Label_LayersDistance->Location = System::Drawing::Point(15, 306);
			this->Label_LayersDistance->Name = L"Label_LayersDistance";
			this->Label_LayersDistance->Size = System::Drawing::Size(187, 17);
			this->Label_LayersDistance->TabIndex = 31;
			this->Label_LayersDistance->Text = L"���������� ����� ������:";
			// 
			// GroupBox_Modes
			// 
			this->GroupBox_Modes->Controls->Add(this->RadioButton_2D);
			this->GroupBox_Modes->Controls->Add(this->RadioButton_3D);
			this->GroupBox_Modes->Location = System::Drawing::Point(243, 26);
			this->GroupBox_Modes->Name = L"GroupBox_Modes";
			this->GroupBox_Modes->Size = System::Drawing::Size(264, 100);
			this->GroupBox_Modes->TabIndex = 30;
			this->GroupBox_Modes->TabStop = false;
			this->GroupBox_Modes->Text = L"����� ������ � �������";
			// 
			// NumericUpDown_Finish
			// 
			this->NumericUpDown_Finish->Enabled = false;
			this->NumericUpDown_Finish->Location = System::Drawing::Point(395, 431);
			this->NumericUpDown_Finish->Name = L"NumericUpDown_Finish";
			this->NumericUpDown_Finish->Size = System::Drawing::Size(44, 22);
			this->NumericUpDown_Finish->TabIndex = 29;
			this->NumericUpDown_Finish->ValueChanged += gcnew System::EventHandler(this, &MainForm::NumericUpDown_Finish_ValueChanged);
			// 
			// NumericUpDown_Start
			// 
			this->NumericUpDown_Start->Enabled = false;
			this->NumericUpDown_Start->Location = System::Drawing::Point(196, 429);
			this->NumericUpDown_Start->Name = L"NumericUpDown_Start";
			this->NumericUpDown_Start->Size = System::Drawing::Size(44, 22);
			this->NumericUpDown_Start->TabIndex = 28;
			this->NumericUpDown_Start->ValueChanged += gcnew System::EventHandler(this, &MainForm::NumericUpDown_Start_ValueChanged);
			// 
			// Label_LayerInfo
			// 
			this->Label_LayerInfo->AutoSize = true;
			this->Label_LayerInfo->Location = System::Drawing::Point(15, 369);
			this->Label_LayerInfo->Name = L"Label_LayerInfo";
			this->Label_LayerInfo->Size = System::Drawing::Size(105, 17);
			this->Label_LayerInfo->TabIndex = 27;
			this->Label_LayerInfo->Text = L"������� ����:";
			// 
			// TrackBar_Layers
			// 
			this->TrackBar_Layers->Enabled = false;
			this->TrackBar_Layers->Location = System::Drawing::Point(159, 364);
			this->TrackBar_Layers->Maximum = 0;
			this->TrackBar_Layers->Name = L"TrackBar_Layers";
			this->TrackBar_Layers->Size = System::Drawing::Size(356, 56);
			this->TrackBar_Layers->TabIndex = 26;
			this->TrackBar_Layers->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_Layers_ValueChanged);
			// 
			// Label_FinishLayer
			// 
			this->Label_FinishLayer->AutoSize = true;
			this->Label_FinishLayer->Location = System::Drawing::Point(279, 433);
			this->Label_FinishLayer->Name = L"Label_FinishLayer";
			this->Label_FinishLayer->Size = System::Drawing::Size(114, 17);
			this->Label_FinishLayer->TabIndex = 25;
			this->Label_FinishLayer->Text = L"�������� ����:";
			// 
			// Label_StartLayer
			// 
			this->Label_StartLayer->AutoSize = true;
			this->Label_StartLayer->Location = System::Drawing::Point(71, 431);
			this->Label_StartLayer->Name = L"Label_StartLayer";
			this->Label_StartLayer->Size = System::Drawing::Size(122, 17);
			this->Label_StartLayer->TabIndex = 24;
			this->Label_StartLayer->Text = L"��������� ����:";
			// 
			// Button_NextPart
			// 
			this->Button_NextPart->Enabled = false;
			this->Button_NextPart->Location = System::Drawing::Point(285, 492);
			this->Button_NextPart->Name = L"Button_NextPart";
			this->Button_NextPart->Size = System::Drawing::Size(176, 34);
			this->Button_NextPart->TabIndex = 19;
			this->Button_NextPart->Text = L"��������� ������ >>";
			this->Button_NextPart->UseVisualStyleBackColor = true;
			this->Button_NextPart->Click += gcnew System::EventHandler(this, &MainForm::Button_NextPart_Click);
			// 
			// Button_PreviousPart
			// 
			this->Button_PreviousPart->Enabled = false;
			this->Button_PreviousPart->Location = System::Drawing::Point(64, 492);
			this->Button_PreviousPart->Name = L"Button_PreviousPart";
			this->Button_PreviousPart->Size = System::Drawing::Size(176, 34);
			this->Button_PreviousPart->TabIndex = 18;
			this->Button_PreviousPart->Text = L"<< ���������� ������";
			this->Button_PreviousPart->UseVisualStyleBackColor = true;
			this->Button_PreviousPart->Click += gcnew System::EventHandler(this, &MainForm::Button_PreviousPart_Click);
			// 
			// TextBox_AlphaValue
			// 
			this->TextBox_AlphaValue->Enabled = false;
			this->TextBox_AlphaValue->Location = System::Drawing::Point(282, 230);
			this->TextBox_AlphaValue->Name = L"TextBox_AlphaValue";
			this->TextBox_AlphaValue->Size = System::Drawing::Size(50, 22);
			this->TextBox_AlphaValue->TabIndex = 17;
			this->TextBox_AlphaValue->Text = L"0";
			this->TextBox_AlphaValue->TextChanged += gcnew System::EventHandler(this, &MainForm::TextBox_AlphaValue_TextChanged);
			// 
			// TrackBar_Alpha
			// 
			this->TrackBar_Alpha->Enabled = false;
			this->TrackBar_Alpha->Location = System::Drawing::Point(338, 227);
			this->TrackBar_Alpha->Maximum = 1000;
			this->TrackBar_Alpha->Name = L"TrackBar_Alpha";
			this->TrackBar_Alpha->Size = System::Drawing::Size(177, 56);
			this->TrackBar_Alpha->TabIndex = 16;
			this->TrackBar_Alpha->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_Alpha_ValueChanged);
			// 
			// TrackBar_BrightnessMult
			// 
			this->TrackBar_BrightnessMult->Location = System::Drawing::Point(231, 177);
			this->TrackBar_BrightnessMult->Maximum = 100;
			this->TrackBar_BrightnessMult->Minimum = 1;
			this->TrackBar_BrightnessMult->Name = L"TrackBar_BrightnessMult";
			this->TrackBar_BrightnessMult->Size = System::Drawing::Size(284, 56);
			this->TrackBar_BrightnessMult->TabIndex = 15;
			this->TrackBar_BrightnessMult->Value = 30;
			this->TrackBar_BrightnessMult->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_BrightnessMult_ValueChanged);
			// 
			// Label_ThresholdAlphaValue
			// 
			this->Label_ThresholdAlphaValue->AutoSize = true;
			this->Label_ThresholdAlphaValue->Enabled = false;
			this->Label_ThresholdAlphaValue->Location = System::Drawing::Point(130, 232);
			this->Label_ThresholdAlphaValue->Name = L"Label_ThresholdAlphaValue";
			this->Label_ThresholdAlphaValue->Size = System::Drawing::Size(149, 17);
			this->Label_ThresholdAlphaValue->TabIndex = 14;
			this->Label_ThresholdAlphaValue->Text = L"��������� ��������:";
			// 
			// CheckBox_AlphaTest
			// 
			this->CheckBox_AlphaTest->AutoSize = true;
			this->CheckBox_AlphaTest->Location = System::Drawing::Point(18, 230);
			this->CheckBox_AlphaTest->Name = L"CheckBox_AlphaTest";
			this->CheckBox_AlphaTest->Size = System::Drawing::Size(100, 21);
			this->CheckBox_AlphaTest->TabIndex = 13;
			this->CheckBox_AlphaTest->Text = L"Alpha-����";
			this->CheckBox_AlphaTest->UseVisualStyleBackColor = true;
			this->CheckBox_AlphaTest->CheckedChanged += gcnew System::EventHandler(this, &MainForm::CheckBox_AlphaTest_CheckedChanged);
			// 
			// GroupBox_Methods
			// 
			this->GroupBox_Methods->Controls->Add(this->RadioButtonRaycasting);
			this->GroupBox_Methods->Controls->Add(this->RadioButtonTextures_3D);
			this->GroupBox_Methods->Controls->Add(this->RadioButtonTextures_2D);
			this->GroupBox_Methods->Location = System::Drawing::Point(18, 26);
			this->GroupBox_Methods->Name = L"GroupBox_Methods";
			this->GroupBox_Methods->Size = System::Drawing::Size(207, 125);
			this->GroupBox_Methods->TabIndex = 0;
			this->GroupBox_Methods->TabStop = false;
			this->GroupBox_Methods->Text = L"����� ������������";
			// 
			// RadioButtonRaycasting
			// 
			this->RadioButtonRaycasting->AutoSize = true;
			this->RadioButtonRaycasting->Location = System::Drawing::Point(18, 88);
			this->RadioButtonRaycasting->Name = L"RadioButtonRaycasting";
			this->RadioButtonRaycasting->Size = System::Drawing::Size(174, 21);
			this->RadioButtonRaycasting->TabIndex = 2;
			this->RadioButtonRaycasting->Text = L"Raycasting (�������)";
			this->RadioButtonRaycasting->UseVisualStyleBackColor = true;
			this->RadioButtonRaycasting->CheckedChanged += gcnew System::EventHandler(this, &MainForm::RadioButtonRaycasting_CheckedChanged);
			// 
			// RadioButtonTextures_3D
			// 
			this->RadioButtonTextures_3D->AutoSize = true;
			this->RadioButtonTextures_3D->Location = System::Drawing::Point(18, 61);
			this->RadioButtonTextures_3D->Name = L"RadioButtonTextures_3D";
			this->RadioButtonTextures_3D->Size = System::Drawing::Size(111, 21);
			this->RadioButtonTextures_3D->TabIndex = 1;
			this->RadioButtonTextures_3D->Text = L"3D-��������";
			this->RadioButtonTextures_3D->UseVisualStyleBackColor = true;
			this->RadioButtonTextures_3D->CheckedChanged += gcnew System::EventHandler(this, &MainForm::RadioButtonTextures_3D_CheckedChanged);
			// 
			// RadioButtonTextures_2D
			// 
			this->RadioButtonTextures_2D->AutoSize = true;
			this->RadioButtonTextures_2D->Checked = true;
			this->RadioButtonTextures_2D->Location = System::Drawing::Point(18, 34);
			this->RadioButtonTextures_2D->Name = L"RadioButtonTextures_2D";
			this->RadioButtonTextures_2D->Size = System::Drawing::Size(113, 21);
			this->RadioButtonTextures_2D->TabIndex = 0;
			this->RadioButtonTextures_2D->TabStop = true;
			this->RadioButtonTextures_2D->Text = L"2D-��������";
			this->RadioButtonTextures_2D->UseVisualStyleBackColor = true;
			this->RadioButtonTextures_2D->CheckedChanged += gcnew System::EventHandler(this, &MainForm::RadioButtonTextures_2D_CheckedChanged);
			// 
			// Localization
			// 
			this->Localization->BackColor = System::Drawing::SystemColors::Control;
			this->Localization->Controls->Add(this->CheckBox_Localization);
			this->Localization->Controls->Add(this->YPoint_TextBox);
			this->Localization->Controls->Add(this->XPoint_TextBox);
			this->Localization->Controls->Add(this->YPoint_Label);
			this->Localization->Controls->Add(this->XPoint_Label);
			this->Localization->Controls->Add(this->CheckBox_Gradients);
			this->Localization->Controls->Add(this->ButtonLocalize);
			this->Localization->Controls->Add(this->GroupBox_SnakeParameters);
			this->Localization->Location = System::Drawing::Point(4, 25);
			this->Localization->Name = L"Localization";
			this->Localization->Size = System::Drawing::Size(535, 940);
			this->Localization->TabIndex = 3;
			this->Localization->Text = L"�����������";
			// 
			// CheckBox_Localization
			// 
			this->CheckBox_Localization->AutoSize = true;
			this->CheckBox_Localization->Location = System::Drawing::Point(58, 372);
			this->CheckBox_Localization->Name = L"CheckBox_Localization";
			this->CheckBox_Localization->Size = System::Drawing::Size(282, 21);
			this->CheckBox_Localization->TabIndex = 11;
			this->CheckBox_Localization->Text = L"���������� ���������� �����������";
			this->CheckBox_Localization->UseVisualStyleBackColor = true;
			this->CheckBox_Localization->CheckedChanged += gcnew System::EventHandler(this, &MainForm::CheckBox_Localization_CheckedChanged);
			// 
			// YPoint_TextBox
			// 
			this->YPoint_TextBox->Location = System::Drawing::Point(298, 525);
			this->YPoint_TextBox->Name = L"YPoint_TextBox";
			this->YPoint_TextBox->Size = System::Drawing::Size(100, 22);
			this->YPoint_TextBox->TabIndex = 10;
			// 
			// XPoint_TextBox
			// 
			this->XPoint_TextBox->Location = System::Drawing::Point(298, 490);
			this->XPoint_TextBox->Name = L"XPoint_TextBox";
			this->XPoint_TextBox->Size = System::Drawing::Size(100, 22);
			this->XPoint_TextBox->TabIndex = 9;
			// 
			// YPoint_Label
			// 
			this->YPoint_Label->AutoSize = true;
			this->YPoint_Label->Location = System::Drawing::Point(173, 528);
			this->YPoint_Label->Name = L"YPoint_Label";
			this->YPoint_Label->Size = System::Drawing::Size(125, 17);
			this->YPoint_Label->TabIndex = 8;
			this->YPoint_Label->Text = L"������� ����� Y:";
			// 
			// XPoint_Label
			// 
			this->XPoint_Label->AutoSize = true;
			this->XPoint_Label->Location = System::Drawing::Point(173, 493);
			this->XPoint_Label->Name = L"XPoint_Label";
			this->XPoint_Label->Size = System::Drawing::Size(125, 17);
			this->XPoint_Label->TabIndex = 7;
			this->XPoint_Label->Text = L"������� ����� X:";
			// 
			// CheckBox_Gradients
			// 
			this->CheckBox_Gradients->AutoSize = true;
			this->CheckBox_Gradients->Enabled = false;
			this->CheckBox_Gradients->Location = System::Drawing::Point(58, 333);
			this->CheckBox_Gradients->Name = L"CheckBox_Gradients";
			this->CheckBox_Gradients->Size = System::Drawing::Size(212, 21);
			this->CheckBox_Gradients->TabIndex = 2;
			this->CheckBox_Gradients->Text = L"�������� ����� ����������";
			this->CheckBox_Gradients->UseVisualStyleBackColor = true;
			this->CheckBox_Gradients->CheckedChanged += gcnew System::EventHandler(this, &MainForm::CheckBox_Gradients_CheckedChanged);
			// 
			// ButtonLocalize
			// 
			this->ButtonLocalize->Enabled = false;
			this->ButtonLocalize->Location = System::Drawing::Point(143, 430);
			this->ButtonLocalize->Name = L"ButtonLocalize";
			this->ButtonLocalize->Size = System::Drawing::Size(282, 34);
			this->ButtonLocalize->TabIndex = 1;
			this->ButtonLocalize->Text = L"������������ ��������� �����������";
			this->ButtonLocalize->UseVisualStyleBackColor = true;
			this->ButtonLocalize->Click += gcnew System::EventHandler(this, &MainForm::ButtonLocalize_Click);
			// 
			// GroupBox_SnakeParameters
			// 
			this->GroupBox_SnakeParameters->Controls->Add(this->Label_IterationsNumber);
			this->GroupBox_SnakeParameters->Controls->Add(this->Label_GammaValue);
			this->GroupBox_SnakeParameters->Controls->Add(this->Label_BetaValue);
			this->GroupBox_SnakeParameters->Controls->Add(this->Label_AlphaValue);
			this->GroupBox_SnakeParameters->Controls->Add(this->TrackBar_IterationsNumber);
			this->GroupBox_SnakeParameters->Controls->Add(this->TrackBar_GammaValue);
			this->GroupBox_SnakeParameters->Controls->Add(this->TrackBar_BetaValue);
			this->GroupBox_SnakeParameters->Controls->Add(this->TrackBar_AlphaValue);
			this->GroupBox_SnakeParameters->Location = System::Drawing::Point(58, 22);
			this->GroupBox_SnakeParameters->Name = L"GroupBox_SnakeParameters";
			this->GroupBox_SnakeParameters->Size = System::Drawing::Size(425, 273);
			this->GroupBox_SnakeParameters->TabIndex = 0;
			this->GroupBox_SnakeParameters->TabStop = false;
			this->GroupBox_SnakeParameters->Text = L"��������� ������ ��������� �������";
			// 
			// Label_IterationsNumber
			// 
			this->Label_IterationsNumber->AutoSize = true;
			this->Label_IterationsNumber->Location = System::Drawing::Point(23, 207);
			this->Label_IterationsNumber->Name = L"Label_IterationsNumber";
			this->Label_IterationsNumber->Size = System::Drawing::Size(120, 17);
			this->Label_IterationsNumber->TabIndex = 7;
			this->Label_IterationsNumber->Text = L"����� ��������:";
			// 
			// Label_GammaValue
			// 
			this->Label_GammaValue->AutoSize = true;
			this->Label_GammaValue->Location = System::Drawing::Point(82, 146);
			this->Label_GammaValue->Name = L"Label_GammaValue";
			this->Label_GammaValue->Size = System::Drawing::Size(61, 17);
			this->Label_GammaValue->TabIndex = 6;
			this->Label_GammaValue->Text = L"Gamma:";
			// 
			// Label_BetaValue
			// 
			this->Label_BetaValue->AutoSize = true;
			this->Label_BetaValue->Location = System::Drawing::Point(100, 90);
			this->Label_BetaValue->Name = L"Label_BetaValue";
			this->Label_BetaValue->Size = System::Drawing::Size(41, 17);
			this->Label_BetaValue->TabIndex = 5;
			this->Label_BetaValue->Text = L"Beta:";
			// 
			// Label_AlphaValue
			// 
			this->Label_AlphaValue->AutoSize = true;
			this->Label_AlphaValue->Location = System::Drawing::Point(92, 38);
			this->Label_AlphaValue->Name = L"Label_AlphaValue";
			this->Label_AlphaValue->Size = System::Drawing::Size(48, 17);
			this->Label_AlphaValue->TabIndex = 4;
			this->Label_AlphaValue->Text = L"Alpha:";
			// 
			// TrackBar_IterationsNumber
			// 
			this->TrackBar_IterationsNumber->Location = System::Drawing::Point(164, 202);
			this->TrackBar_IterationsNumber->Maximum = 1000;
			this->TrackBar_IterationsNumber->Name = L"TrackBar_IterationsNumber";
			this->TrackBar_IterationsNumber->Size = System::Drawing::Size(255, 56);
			this->TrackBar_IterationsNumber->TabIndex = 3;
			this->TrackBar_IterationsNumber->Value = 30;
			this->TrackBar_IterationsNumber->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_IterationsNumber_ValueChanged);
			// 
			// TrackBar_GammaValue
			// 
			this->TrackBar_GammaValue->Location = System::Drawing::Point(164, 140);
			this->TrackBar_GammaValue->Maximum = 100;
			this->TrackBar_GammaValue->Minimum = 1;
			this->TrackBar_GammaValue->Name = L"TrackBar_GammaValue";
			this->TrackBar_GammaValue->Size = System::Drawing::Size(255, 56);
			this->TrackBar_GammaValue->TabIndex = 2;
			this->TrackBar_GammaValue->Value = 15;
			this->TrackBar_GammaValue->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_GammaValue_ValueChanged);
			// 
			// TrackBar_BetaValue
			// 
			this->TrackBar_BetaValue->Location = System::Drawing::Point(164, 84);
			this->TrackBar_BetaValue->Maximum = 100;
			this->TrackBar_BetaValue->Minimum = 1;
			this->TrackBar_BetaValue->Name = L"TrackBar_BetaValue";
			this->TrackBar_BetaValue->Size = System::Drawing::Size(255, 56);
			this->TrackBar_BetaValue->TabIndex = 1;
			this->TrackBar_BetaValue->Value = 2;
			this->TrackBar_BetaValue->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_BetaValue_ValueChanged);
			// 
			// TrackBar_AlphaValue
			// 
			this->TrackBar_AlphaValue->Location = System::Drawing::Point(164, 33);
			this->TrackBar_AlphaValue->Maximum = 100;
			this->TrackBar_AlphaValue->Minimum = 1;
			this->TrackBar_AlphaValue->Name = L"TrackBar_AlphaValue";
			this->TrackBar_AlphaValue->Size = System::Drawing::Size(255, 56);
			this->TrackBar_AlphaValue->TabIndex = 0;
			this->TrackBar_AlphaValue->Value = 1;
			this->TrackBar_AlphaValue->ValueChanged += gcnew System::EventHandler(this, &MainForm::TrackBar_AlphaValue_ValueChanged);
			// 
			// Segmentation
			// 
			this->Segmentation->BackColor = System::Drawing::SystemColors::Control;
			this->Segmentation->Controls->Add(this->GroupBoxSegmentsInfo);
			this->Segmentation->Controls->Add(this->Button_VisualizeSelectedClusters);
			this->Segmentation->Controls->Add(this->Button_Clusterization);
			this->Segmentation->Controls->Add(this->GroupBoxSegmentation);
			this->Segmentation->Location = System::Drawing::Point(4, 25);
			this->Segmentation->Name = L"Segmentation";
			this->Segmentation->Padding = System::Windows::Forms::Padding(3);
			this->Segmentation->Size = System::Drawing::Size(535, 940);
			this->Segmentation->TabIndex = 1;
			this->Segmentation->Text = L"�����������";
			// 
			// Reconstruction
			// 
			this->Reconstruction->BackColor = System::Drawing::SystemColors::Control;
			this->Reconstruction->Controls->Add(this->CheckBox_Reconstruction);
			this->Reconstruction->Controls->Add(this->ButtonReconstructionData);
			this->Reconstruction->Controls->Add(this->ButtonReconstructionSegments);
			this->Reconstruction->Location = System::Drawing::Point(4, 25);
			this->Reconstruction->Name = L"Reconstruction";
			this->Reconstruction->Size = System::Drawing::Size(535, 940);
			this->Reconstruction->TabIndex = 2;
			this->Reconstruction->Text = L"3D-�������������";
			// 
			// CheckBox_Reconstruction
			// 
			this->CheckBox_Reconstruction->AutoSize = true;
			this->CheckBox_Reconstruction->Location = System::Drawing::Point(98, 128);
			this->CheckBox_Reconstruction->Name = L"CheckBox_Reconstruction";
			this->CheckBox_Reconstruction->Size = System::Drawing::Size(295, 21);
			this->CheckBox_Reconstruction->TabIndex = 3;
			this->CheckBox_Reconstruction->Text = L"���������� ���������� �������������";
			this->CheckBox_Reconstruction->UseVisualStyleBackColor = true;
			this->CheckBox_Reconstruction->CheckedChanged += gcnew System::EventHandler(this, &MainForm::CheckBox_Reconstruction_CheckedChanged);
			// 
			// ButtonReconstructionData
			// 
			this->ButtonReconstructionData->Enabled = false;
			this->ButtonReconstructionData->Location = System::Drawing::Point(127, 21);
			this->ButtonReconstructionData->Name = L"ButtonReconstructionData";
			this->ButtonReconstructionData->Size = System::Drawing::Size(236, 38);
			this->ButtonReconstructionData->TabIndex = 1;
			this->ButtonReconstructionData->Text = L"������������� �������� ������";
			this->ButtonReconstructionData->UseVisualStyleBackColor = true;
			this->ButtonReconstructionData->Click += gcnew System::EventHandler(this, &MainForm::ButtonReconstructionData_Click);
			// 
			// ButtonReconstructionSegments
			// 
			this->ButtonReconstructionSegments->Enabled = false;
			this->ButtonReconstructionSegments->Location = System::Drawing::Point(128, 71);
			this->ButtonReconstructionSegments->Name = L"ButtonReconstructionSegments";
			this->ButtonReconstructionSegments->Size = System::Drawing::Size(236, 40);
			this->ButtonReconstructionSegments->TabIndex = 2;
			this->ButtonReconstructionSegments->Text = L"������������� ���������";
			this->ButtonReconstructionSegments->UseVisualStyleBackColor = true;
			// 
			// MainForm
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Inherit;
			this->ClientSize = System::Drawing::Size(1273, 1002);
			this->Controls->Add(this->TabControlOptions);
			this->Controls->Add(this->ProgressBar_Layers);
			this->Controls->Add(this->ProgressBar_Iterations);
			this->Controls->Add(this->Label_Status);
			this->Controls->Add(this->RenderingPanel);
			this->Controls->Add(this->MenuStrip);
			this->MainMenuStrip = this->MenuStrip;
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Neocortex: ������������, �����������, 3D-������������� ������ ������������ ������" 
				L"���� � ����������� �����������";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->Resize += gcnew System::EventHandler(this, &MainForm::MainForm_Resize);
			this->MenuStrip->ResumeLayout(false);
			this->MenuStrip->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_ClustersCount))->EndInit();
			this->GroupBoxSegmentation->ResumeLayout(false);
			this->GroupBoxSegmentation->PerformLayout();
			this->GroupBoxAlgorithm->ResumeLayout(false);
			this->GroupBoxAlgorithm->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_IterationsCount))->EndInit();
			this->GroupBoxDiapason->ResumeLayout(false);
			this->GroupBoxDiapason->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->DataGridView_Clusters))->EndInit();
			this->GroupBoxSegmentsInfo->ResumeLayout(false);
			this->TabControlOptions->ResumeLayout(false);
			this->Information->ResumeLayout(false);
			this->Information->PerformLayout();
			this->Visualization->ResumeLayout(false);
			this->Visualization->PerformLayout();
			this->GroupBox_Scales->ResumeLayout(false);
			this->GroupBox_Scales->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_ScaleZ))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_ScaleY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_ScaleX))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_LayersDistance))->EndInit();
			this->GroupBox_Modes->ResumeLayout(false);
			this->GroupBox_Modes->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NumericUpDown_Finish))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NumericUpDown_Start))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_Layers))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_Alpha))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_BrightnessMult))->EndInit();
			this->GroupBox_Methods->ResumeLayout(false);
			this->GroupBox_Methods->PerformLayout();
			this->Localization->ResumeLayout(false);
			this->Localization->PerformLayout();
			this->GroupBox_SnakeParameters->ResumeLayout(false);
			this->GroupBox_SnakeParameters->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_IterationsNumber))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_GammaValue))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_BetaValue))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TrackBar_AlphaValue))->EndInit();
			this->Segmentation->ResumeLayout(false);
			this->Reconstruction->ResumeLayout(false);
			this->Reconstruction->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		private: System::String^ GetStringOfColor(TColor Color)
				 {
					 return L"Color[R = "+((unsigned char)(Color.R*255)).ToString()+L", "+
						          L"G = "+((unsigned char)(Color.G*255)).ToString()+L", "+
								  L"B = "+((unsigned char)(Color.B*255)).ToString()+L"]";
				 }

		private: System::UInt32 MinDensityOfSegment_2D(size_t LayerIndex, size_t SegmentIndex)
			 {
			  UInt32 result = UInt32::MaxValue;
			  for (size_t i = 0; i < InputData->TotalSize; ++i)
				  if ((VoxelsData[i].SegmentIndex_2D == SegmentIndex)&&(InputData->data[i]<result))
					  result = InputData->data[i];
			  
			  return result;

			 }

	 private: System::UInt32 MaxDensityOfSegment_2D(size_t LayerIndex, size_t SegmentIndex)
			  {
			   UInt32 result = 0;
			   for (size_t i = 0; i < InputData->TotalSize; ++i)
				   if ((VoxelsData[i].SegmentIndex_2D == SegmentIndex)&&(InputData->data[i]>result))
					   result = InputData->data[i];
			  
			   return result;
			  }

   private: System::UInt32 MinDensityOfSegment_3D(size_t StartLayerIndex, size_t SegmentIndex)
			{
			  UInt32 result = UInt32::MaxValue;
			  for (size_t i = 0; i < InputData->TotalSize; ++i)
				  if ((VoxelsData[i].SegmentIndex_3D == SegmentIndex)&&(InputData->data[i]<result))
					  result = InputData->data[i];

			  return result;
			}

	private: System::UInt32 MaxDensityOfSegment_3D(size_t StartLayerIndex, size_t SegmentIndex)
			 {
			  UInt32 result = 0;
			  for (size_t i = 0; i < InputData->TotalSize; ++i)
				  if ((VoxelsData[i].SegmentIndex_3D == SegmentIndex)&&(InputData->data[i]>result))
					  result = InputData->data[i];

			  return result;
			 }

	private: System::Void DrawGLScene() 
			 {
			  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
              glLoadIdentity();

              if (!RadioButtonRaycasting->Checked) 
			  {
               glTranslatef(distance_x, distance_y, -distance_z);
              }
              else 
			  {
               glTranslatef(0.0f, 0.0f, -3.0f);
              }
              
			  glRotatef(90.0f, 0.0f, 0.0, 1.0f);
			  if (!RadioButtonRaycasting->Checked) 
			  {
			   glRotatef(angleXRotation, 1.0f, 0.0f, 0.0f);
               glRotatef(angleYRotation, 0.0f, 1.0f, 0.0f);
              }
                                
			  if (CheckBox_Transparency->Checked) 
			  {
               glEnable(GL_BLEND);
               if (CheckBox_DepthTest->Checked) 
			   {
                glDisable(GL_DEPTH_TEST);
               }
              }

			  if (CheckBox_AlphaTest->Checked) 
			  {
               glEnable(GL_ALPHA_TEST);
               glAlphaFunc(GL_GREATER, AlphaTestValue);
			  }

			  if (InputData && InputData->data) 
			  { 
				  if (this->CheckBox_Reconstruction->Checked && qw)
				  {
					int N=10;
					int RazmX_By_N=qw->razm_x*N;
					int RazmY_By_N=qw->razm_y*N;
					int RazmZ_By_N=qw->razm_z*N;

					int HalfCountX=InputData->sizeX/(2*qw->STEP);
					int HalfCountY=InputData->sizeY/(2*qw->STEP);
					int HalfCountZ=InputData->sizeZ/(2*qw->STEP);

		 
				    glMatrixMode(GL_MODELVIEW);
				    glClear(GL_COLOR_BUFFER_BIT);

					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glPushMatrix(); 				  
					  
				   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				   
				   glBegin(GL_TRIANGLES);
				   
				   for(int i=0; i<qw->TRIANGLES_COUNT; i++)
				   {
				    glVertex3f((qw->all_points[qw->triangles_res[i].point1].getX()-HalfCountX)*qw->scale.x, (qw->all_points[qw->triangles_res[i].point1].getY()-HalfCountY)*qw->scale.y, (qw->all_points[qw->triangles_res[i].point1].getZ()-HalfCountZ)*qw->scale.z);
					glVertex3f((qw->all_points[qw->triangles_res[i].point2].getX()-HalfCountX)*qw->scale.x, (qw->all_points[qw->triangles_res[i].point2].getY()-HalfCountY)*qw->scale.y, (qw->all_points[qw->triangles_res[i].point2].getZ()-HalfCountZ)*qw->scale.z);
					glVertex3f((qw->all_points[qw->triangles_res[i].point3].getX()-HalfCountX)*qw->scale.x, (qw->all_points[qw->triangles_res[i].point3].getY()-HalfCountY)*qw->scale.y, (qw->all_points[qw->triangles_res[i].point3].getZ()-HalfCountZ)*qw->scale.z);
				   }
					
				   glEnd();

				   qw->drawAxises();
				   glPopMatrix();
				   //glutSwapBuffers();
				  }

				  else 
				  {

				  

			   GLenum target = RadioButtonTextures_3D->Checked ? GL_TEXTURE_3D : GL_TEXTURE_2D;
			   glColor4f(BrightnessMult / (float)(TrackBar_BrightnessMult->Maximum), BrightnessMult / (float)(TrackBar_BrightnessMult->Maximum), BrightnessMult / (float)(TrackBar_BrightnessMult->Maximum), 1.0f);

			   if (this->RadioButton_3D->Checked)
			   {
			    float localDepth = -((float)(NumericUpDown_Finish->Value-NumericUpDown_Start->Value) * InputData->scaleZ * this->TrackBar_LayersDistance->Value / 2.0f);
				float width = (float)(InputData->sizeX / 2) * InputData->scaleX;
                float height = (float)(InputData->sizeY / 2) * InputData->scaleY;

			    glEnable(target);
                glBindTexture(target, layerTextures[0]);

            	for (size_t iLayer = (size_t)NumericUpDown_Start->Value; iLayer < NumericUpDown_Finish->Value + 1; ++iLayer) 
				{
				 glTranslatef(0.0f, 0.0f, -localDepth * 2 / ((size_t)(NumericUpDown_Finish->Value - NumericUpDown_Start->Value + 1)));
				 if (target==GL_TEXTURE_2D)
				 {
                  glBindTexture(GL_TEXTURE_2D, layerTextures[iLayer]);
                  glBegin(GL_QUADS); 
				  {
                   glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, -height, localDepth);
                   glTexCoord2f(1.0f, 0.0f); glVertex3f( width, -height, localDepth);
                   glTexCoord2f(0.0f, 0.0f); glVertex3f( width,  height, localDepth);
                   glTexCoord2f(0.0f, 1.0f); glVertex3f(-width,  height, localDepth);
				  }
				  glEnd();
				 }
				      
				 else 
				 if (target==GL_TEXTURE_3D)
				 {
				  float texZcoord = iLayer / (float)(InputData->sizeZ - 1);
				  glBegin(GL_QUADS); 
				  {
                   glTexCoord3f(1.0f, 1.0f, texZcoord); glVertex3f(-width, -height, localDepth);
                   glTexCoord3f(1.0f, 0.0f, texZcoord); glVertex3f( width, -height, localDepth);
                   glTexCoord3f(0.0f, 0.0f, texZcoord); glVertex3f( width,  height, localDepth);
                   glTexCoord3f(0.0f, 1.0f, texZcoord); glVertex3f(-width,  height, localDepth);
				  }    
				  glEnd();
				 }
				}
				glBindTexture(target, 0);
				glDisable(target);

			   } 
			   
			   else if (this->RadioButton_2D->Checked)
			
			   {
    		     // ������������ �� �����
					 float width = (float)(InputData->sizeX / 2) * InputData->scaleX;
                     float height = (float)(InputData->sizeY / 2) * InputData->scaleY;
					 glTranslatef(0.0f, 0.0f, 0.0f);
					 glEnable(target);


					  //glColor4f(BrightnessMult / (float)(TrackBar_BrightnessMult->Maximum), BrightnessMult / (float)(TrackBar_BrightnessMult->Maximum), BrightnessMult / (float)(TrackBar_BrightnessMult->Maximum), 1.0f);
         
					 if (target == GL_TEXTURE_2D)
					 {
					  glBindTexture(GL_TEXTURE_2D, layerTextures[this->TrackBar_Layers->Value]);
					  glBegin(GL_QUADS);
					  {
					   glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, -height, 0.0f);
					   glTexCoord2f(1.0f, 0.0f); glVertex3f( width, -height, 0.0f);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f( width,  height, 0.0f);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(-width,  height, 0.0f);
					  }
					  glEnd();
					 }

					 else if (target == GL_TEXTURE_3D)
					 {
						float texZCoord = this->TrackBar_Layers->Value / (float)(InputData->sizeZ - 1);
                                 glBegin(GL_QUADS); {
                                     glTexCoord3f(1.0f, 1.0f, texZCoord); glVertex3f(-width, -height, 0.0f);
                                     glTexCoord3f(1.0f, 0.0f, texZCoord); glVertex3f(width, -height, 0.0f);
                                     glTexCoord3f(0.0f, 0.0f, texZCoord); glVertex3f(width, height, 0.0f);
                                     glTexCoord3f(0.0f, 1.0f, texZCoord); glVertex3f(-width, height, 0.0f);
                                 } glEnd();
					 }

					 glBindTexture(target, 0);
                     glDisable(target);
					} 
					  
				  }
				  
				  if (CheckBox_Transparency->Checked) 
				  {
                   glDisable(GL_BLEND);
                   if (CheckBox_DepthTest->Checked) 
				   {
                    glEnable(GL_DEPTH_TEST);
                   }
                  }
				   
				  if (CheckBox_AlphaTest->Checked) {
                     glDisable(GL_ALPHA_TEST);
                 }

				  }
					

				  
				  
			 }

	
	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
			  GLuint PixelFormat;

			  hWnd = (HWND)(intptr_t)this->RenderingPanel->Handle;
                 hDC = GetDC(hWnd);
                 if (!hDC) {
                     this->Label_Status->Text = L"Can't Create A GL Device Context";
                     return;
                 }

                 BYTE colorBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
                 static PIXELFORMATDESCRIPTOR pfd= {            // pfd Tells Windows How We Want Things To Be
                     sizeof(PIXELFORMATDESCRIPTOR),                // Size Of This Pixel Format Descriptor
                     1,                                            // Version Number
                     PFD_DRAW_TO_WINDOW |                          // Format Must Support Window
                     PFD_SUPPORT_OPENGL |                          // Format Must Support OpenGL
                     PFD_DOUBLEBUFFER,                             // Must Support Double Buffering
                     PFD_TYPE_RGBA,                                // Request An RGBA Format
                     colorBits,                                    // Select Our Color Depth
                     0, 0, 0, 0, 0, 0,                             // Color Bits Ignored
                     0,                                            // No Alpha Buffer
                     0,                                            // Shift Bit Ignored
                     0,                                            // No Accumulation Buffer
                     0, 0, 0, 0,                                   // Accumulation Bits Ignored
                     24,                                           // 24Bit Z-Buffer (Depth Buffer)  
                     0,                                            // No Stencil Buffer
                     0,                                            // No Auxiliary Buffer
                     PFD_MAIN_PLANE,                               // Main Drawing Layer
                     0,                                            // Reserved
                     0, 0, 0                                       // Layer Masks Ignored
                 };

                 LoadLibrary(L"opengl32.dll");
                 PixelFormat = ChoosePixelFormat(hDC, &pfd);
                 if (!PixelFormat) {
                     this->Label_Status->Text = L"Can't Find A Suitable PixelFormat.";
                     return;
                 }
                 if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
                     this->Label_Status->Text = L"Can't Set The PixelFormat.";
                     return;
                 }
                 hRC = wglCreateContext(hDC);
                 if (!hRC) {
                     this->Label_Status->Text = L"Can't Create A GL Rendering Context. GetLastError: " + GetLastError();
                     return;
                 }
                 if (!wglMakeCurrent(hDC, hRC)) {
                     this->Label_Status->Text = L"Can't Activate The GL Rendering Context.";
                     return;
                 }
                 this->Label_Status->Text = L"";

                 MainForm_Resize(sender, e);

                 if (!InitGL()) {
                     this->Label_Status->Text = L"Failed to init GL";
                 }

                 depth = 10.0f;
                 theBox = glGenLists(1);
                 glNewList(theBox, GL_COMPILE);
                 glBegin(GL_QUADS); {
                     // front face
                     //glNormal3f(0.0f, 0.0f, 1.0f);
                     glVertex3f(0.5f, 0.5f, 0.5f * depth);
                     glVertex3f(0.5f, -0.5f, 0.5f * depth);
                     glVertex3f(-0.5f, -0.5f, 0.5f * depth);
                     glVertex3f(-0.5f, 0.5f, 0.5f * depth);

                     // left face
                     //glNormal3f(-1.0f, 0.0f, 0.0f);
                     glVertex3f(-0.5f, 0.5f, 0.5f * depth);
                     glVertex3f(-0.5f, -0.5f, 0.5f * depth);
                     glVertex3f(-0.5f, -0.5f, -0.5f * depth);
                     glVertex3f(-0.5f, 0.5f, -0.5f * depth);

                     // back face
                     //glNormal3f(0.0f, 0.0f, -1.0f);
                     glVertex3f(-0.5f, 0.5f, -0.5f * depth);
                     glVertex3f(-0.5f, -0.5f, -0.5f * depth);
                     glVertex3f(0.5f, -0.5f, -0.5f * depth);
                     glVertex3f(0.5f, 0.5f, -0.5f * depth);

                     // right face
                     //glNormal3f(1.0f, 0.0f, 0.0f);
                     glVertex3f(0.5f, 0.5f, -0.5f * depth);
                     glVertex3f(0.5f, -0.5f, -0.5f * depth);
                     glVertex3f(0.5f, -0.5f, 0.5f * depth);
                     glVertex3f(0.5f, 0.5f, 0.5f * depth);

                     // top face
                     //glNormal3f(0.0f, 1.0f, 0.0f);
                     glVertex3f(-0.5f, 0.5f, -0.5f * depth);
                     glVertex3f(0.5f, 0.5f, -0.5f * depth);
                     glVertex3f(0.5f, 0.5f, 0.5f * depth);
                     glVertex3f(-0.5f, 0.5f, 0.5f * depth);

                     // bottom face
                     //glNormal3f(0.0f, -1.0f, 0.0f);
                     glVertex3f(-0.5f, -0.5f, 0.5f * depth);
                     glVertex3f(0.5f, -0.5f, 0.5f * depth);
                     glVertex3f(0.5f, -0.5f, -0.5f * depth);
                     glVertex3f(-0.5f, -0.5f, -0.5f * depth);
                 }
                 glEnd();
                 glEndList();

				 this->TrackBar_Layers->Enabled = false;
				 this->NumericUpDown_Start->Enabled = false;
				 this->NumericUpDown_Finish->Enabled = false;
				 this->TrackBar_LayersDistance->Enabled = false;
				
				 this->TextBox_LowBorder->Enabled = false;
				 this->TextBox_HighBorder->Enabled = false;

				 this->Button_Clusterization->Enabled = false;
				 this->DataGridView_Clusters->Enabled = false;
				 this->Button_VisualizeSelectedClusters->Enabled = false;

				 this->CheckBox_DepthTest->Enabled = false;
				 
				 this->Label_LayersDistance->Text = L"���������� ����� ������: "+this->TrackBar_LayersDistance->Value.ToString();
				 this->Label_LayersDistance->Enabled = false;

				 this->Label_ClustersCount->Text = L"������������ ����� ���������: "+this->TrackBar_ClustersCount->Value.ToString();
				 this->Label_IterationsCount->Text = L"���������� �������� ���������: "+this->TrackBar_IterationsCount->Value.ToString();

				 StartLayerIndex_3D = 0;
				 
				 CellValueChanging = false;
				 				 
                 InputData = new ScanData();
				 				 
				 Segments_2D = NULL; Segments_3D = NULL; VoxelsData = NULL;

				 SnakeParameters = new TSnakeParameters();

				 this->Label_AlphaValue->Text = L"Alpha: " + SnakeParameters->alpha;
				 this->Label_BetaValue->Text = L"Beta: " + SnakeParameters->beta;
				 this->Label_GammaValue->Text = L"Gamma: " + SnakeParameters->gamma;
				 this->Label_IterationsNumber->Text = L"����� ��������: " + SnakeParameters->iterations_number;

				 layerTextures = NULL;
			 }
	private: System::Void ToolStripMenuItem_DownloadFile_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
			  if (this->OpenDataDialog->ShowDialog() == Windows::Forms::DialogResult::OK) 
			  {
			   System::String ^pathToDataFile = OpenDataDialog->FileName;
               if (System::IO::File::Exists(pathToDataFile)) 
			   {
               pathToDataFile = pathToDataFile->Replace("\\","\\\\");

			   InputData->ReleaseResources();
 
			   /*InputData = new short [nLayers*sizeX*sizeY];
 
				 ifstream fs(pathToBinFile, std::ios::in | std::ios::binary);

					if (!fs) return false;

					fs.read((char*)&sizeX, sizeof(int));
					fs.read((char*)&sizeY, sizeof(int));
					fs.read((char*)&sizeZ, sizeof(int));
 
					TotalSize = nLayers*sizeX*sizeY;

					fs.read((char*)&scaleX, sizeof(float));
					fs.read((char*)&scaleY, sizeof(float));
					fs.read((char*)&scaleZ, sizeof(float));
 
					data = new short [TotalSize]; */


               if (InputData->LoadData((char*)Runtime::InteropServices::Marshal::StringToHGlobalAnsi(pathToDataFile).ToPointer())) 
			   {
               this->Label_Status->Text = L"������ �� bin-����� ���������.";

			   float FileSize = (float)(IO::FileInfo(pathToDataFile).Length >> 20);

			   this->Label_FileSize->Text = L"������ ����� �������� ������: " + FileSize + L" MB";

			   this->Label_LayerWidth->Text = L"������ ������� ����: " + InputData->sizeX + " ��������";
			   this->Label_LayerHeight->Text = L"������ ������� ����: " + InputData->sizeY + " ��������";
			   this->Label_LayersNumber->Text = L"���������� ����������� ����: " + InputData->sizeZ;

			   this->Label_InputScaleX->Text = L"������������ ������� ����� ��� X: " + InputData->scaleX;
			   this->Label_InputScaleY->Text = L"������������ ������� ����� ��� Y: " + InputData->scaleY;
			   this->Label_InputScaleZ->Text = L"������������ ������� ����� ��� Z: " + InputData->scaleZ;

               float fileSize = (float)(IO::FileInfo(pathToDataFile).Length >> 20);
                
			   delete [] VoxelsData; VoxelsData = NULL; VoxelsData = new TVoxel [InputData->TotalSize];

			   MaxDensity = 0; MinDensity = UINT::MaxValue;

			   for (size_t i = 0; i < InputData->TotalSize; ++i) 
			   {
			   // VoxelsData[i].Color_2D = VoxelsData[i].Color_3D = TColor(0.0f, 0.0f, 0.0f, 0.0f);

			   //	VoxelsData[i].IsVisible_2D = VoxelsData[i].IsVisible_3D = true;

                if (MaxDensity < InputData->data[i]) MaxDensity = InputData->data[i];
				if (MinDensity > InputData->data[i]) MinDensity = InputData->data[i];
			   }

			   if (MinDensity < 0) MinDensity = 0;

			   ProgressBar_Layers->Maximum = InputData->sizeZ;
			   TrackBar_Layers->Maximum = InputData->sizeZ-1;
			   TrackBar_Layers->Value = 0;
			   NumericUpDown_Start->Maximum = InputData->sizeZ-1;
			   NumericUpDown_Start->Value = 0;
			   NumericUpDown_Finish->Maximum = InputData->sizeZ-1;
			   NumericUpDown_Finish->Value = InputData->sizeZ-1;

			   this->Label_LayerInfo->Text = L"������� ����: "+(this->TrackBar_Layers->Value+1).ToString()+"/"+(this->TrackBar_Layers->Maximum+1).ToString();


			   this->Button_VisualizeSelectedClusters->Enabled = false;

			   delete [] Segments_2D; delete [] Segments_3D; 
			   
			   Segments_2D = NULL; Segments_3D = NULL; 
			   
			   this->DataGridView_Clusters->RowCount = 0;

			   this->DataGridView_Clusters->Enabled = false;

			   if (this->RadioButton_2D->Checked)
			   {
				this->TrackBar_Layers->Enabled = true;
			   }

			   if (this->RadioButton_3D->Checked)
			   {
			    this->NumericUpDown_Start->Enabled = true;
			    this->NumericUpDown_Finish->Enabled = true;
				this->TrackBar_LayersDistance->Enabled = true;
			   }
					
			   this->TextBox_BrightnessMult->Enabled = true;

			   this->TextBox_LowBorder->Text = MinDensity.ToString();
			   this->TextBox_HighBorder->Text = MaxDensity.ToString();

			   this->TextBox_LowBorder->Enabled = true;
			   this->TextBox_HighBorder->Enabled = true;

			   this->TextBox_ScaleX->Enabled = true;
			   this->TextBox_ScaleY->Enabled = true;
			   this->TextBox_ScaleZ->Enabled = true;

			   this->TrackBar_ScaleX->Enabled = true;
			   this->TrackBar_ScaleY->Enabled = true;
			   this->TrackBar_ScaleZ->Enabled = true;

			   this->TrackBar_ScaleX->Value = (int)(InputData->scaleX*this->TrackBar_ScaleX->Maximum);
			   this->TrackBar_ScaleY->Value = (int)(InputData->scaleY*this->TrackBar_ScaleY->Maximum);
			   this->TrackBar_ScaleZ->Value = (int)(InputData->scaleZ*this->TrackBar_ScaleZ->Maximum);

			   this->Button_Clusterization->Enabled = true;

			   this->ButtonReconstructionData->Enabled = true;

			   angleXRotation = angleYRotation = 0.0f;
               distance_x = distance_y = 0.0f;
			   distance_z = 600.0f;
			   
			   TextBox_BrightnessMult->Text = L"30,0";
               BrightnessMult = 30.f;

			   StartLayerIndex_3D = 0;

			   CellValueChanging = false;

			   GenerateTextures();
                    
			  }

			 } else {
                     this->Label_Status->Text = "������! ������������ ������ �����.";
                 }
			  } else {
                  this->Label_Status->Text = "������! ���� �� ������.";
			  } 

			 }

// ������������ ������� ��� ����������			
private: System::Void GenerateTextures()
		 {
		  ReleaseTextures();

		  bool is3Dtexture = !this->RadioButtonTextures_2D->Checked;

          size_t numTextures = is3Dtexture ? 1 : InputData->sizeZ;

		  layerTextures = new GLuint[numTextures];
          
		  if (!this->RadioButtonRaycasting->Checked) 
		  glGenTextures(numTextures, layerTextures);
          else 
		  {
           GLuint tmpTex;
           glGenTextures(numTextures, &tmpTex);
           shaderTexture = tmpTex;
          }

		  size_t dataSize = 4*InputData->sizeX*InputData->sizeY;

          if (RadioButtonRaycasting->Checked) dataSize >>= 1;
          if (is3Dtexture)  dataSize *= InputData->sizeZ;
		  
		  float *tmp = new float [dataSize];

		  GLenum target = is3Dtexture ? GL_TEXTURE_3D : GL_TEXTURE_2D;

		  size_t numLayers = is3Dtexture ? 1 : InputData->sizeZ;
          float additionalMult = 1.5;
          if (InputData->sizeZ < 30) additionalMult = 10;
		  
		  for (size_t iLayer = 0; iLayer < numLayers; ++iLayer) 
		  {
		   for (size_t i = 0; i < dataSize; i += 4)  
		   {
		   
		    size_t reducedIndex = i / 4 + iLayer * InputData->sizeX * InputData->sizeY;
		   
		    if (!RadioButtonRaycasting->Checked)
		    {
			 if (CheckBoxClusters->Checked)			// ���� �������� ������������ 
			 {
			  if (this->RadioButton_2D->Checked && Segments_2D)
			  {
			   TSegment tmpSegment = Segments_2D[iLayer].at(VoxelsData[reducedIndex].SegmentIndex_2D);
     		   if (tmpSegment.Visible && (InputData->data[reducedIndex]>=low_density) && (InputData->data[reducedIndex]<=high_density))
			   {
			    TColor tmpColor = tmpSegment.Color;
			    tmp[i] = tmpColor.R;
			    tmp[i+1] = tmpColor.G;
			    tmp[i+2] = tmpColor.B;
			    tmp[i+3] = tmpColor.A;
			   }
			  }
			  else if (this->RadioButton_3D->Checked && Segments_3D)
			  {
			   TSegment tmpSegment = Segments_3D->at(VoxelsData[reducedIndex].SegmentIndex_3D);
			   if (tmpSegment.Visible && (InputData->data[reducedIndex]>=low_density) && (InputData->data[i]<=high_density))
			   {
			    TColor tmpColor = tmpSegment.Color;
			    tmp[i] = tmpColor.R;
			    tmp[i+1] = tmpColor.G;
			    tmp[i+2] = tmpColor.B;
				tmp[i+3] = tmpColor.A;
			   }
			  }
			  else tmp[i+3] = tmp[i+2] = tmp[i+1] = tmp[i] = 0.0f;
			 } 
			 else tmp[i+3] = tmp[i+2] = tmp[i+1] = tmp[i] = InputData->data[reducedIndex] * 5.0*additionalMult / MaxDensity; 
			}
		   
		    else

		    {
			 if (CheckBoxClusters->Checked)
			 {
			  if (this->RadioButton_2D->Checked && Segments_2D &&
				 Segments_2D[iLayer].at(VoxelsData[reducedIndex].SegmentIndex_2D).Visible &&
				 (InputData->data[reducedIndex]>=low_density) && (InputData->data[i]<=high_density))
			  {
			   tmp[i] = Segments_2D[iLayer].at(VoxelsData[reducedIndex].SegmentIndex_2D).Color.R;
			   tmp[i+1] = Segments_2D[iLayer].at(VoxelsData[reducedIndex].SegmentIndex_2D).Color.G;
			   tmp[i+2] = Segments_2D[iLayer].at(VoxelsData[reducedIndex].SegmentIndex_2D).Color.B;
			   //tmp[i+3] = Segments_2D[iLayer].at(VoxelsData[reducedIndex].SegmentIndex_2D).Color.A; 
			   --i;
			  }
			  else if (this->RadioButton_3D->Checked && Segments_3D &&
				       Segments_3D->at(VoxelsData[reducedIndex].SegmentIndex_3D).Visible &&
					   (InputData->data[reducedIndex]>=low_density) && (InputData->data[i]<=high_density))
			  {
			   tmp[i] = Segments_3D->at(VoxelsData[reducedIndex].SegmentIndex_3D).Color.R;
			   tmp[i+1] = Segments_3D->at(VoxelsData[reducedIndex].SegmentIndex_3D).Color.G;
			   tmp[i+2] = Segments_3D->at(VoxelsData[reducedIndex].SegmentIndex_3D).Color.B;
			   //tmp[i+3] = Segments_3D->at(VoxelsData[reducedIndex].SegmentIndex_3D).Color.A; 
			   --i;
			  }
			  else {tmp[i] = tmp[i+1] = tmp[i+2] = 0.0f; --i;}
			 }
		     else {tmp[i] = tmp[i+1] = tmp[i+2] = InputData->data[i] * 5.0 * additionalMult / MaxDensity; --i;}
			}
		  
		   
		   }

		   
		   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		   if (!RadioButtonRaycasting->Checked) {
                         glBindTexture(target, layerTextures[iLayer]);
                     }
                     else {
                         glBindTexture(target, shaderTexture);
                     }

					 glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
           
                     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

                     glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP);
                     glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP);
                     if (is3Dtexture) {
                         glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP);
                     }
                     glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                     glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                     if (is3Dtexture) {
                         if (RadioButtonRaycasting->Checked) {
                             glTexImage3D(GL_TEXTURE_3D, 0, 4, InputData->sizeX, InputData->sizeY, InputData->sizeZ, 0, GL_ALPHA, GL_FLOAT, tmp);
                         }
                         else {
                             glTexImage3D(GL_TEXTURE_3D, 0, 4, InputData->sizeX, InputData->sizeY, InputData->sizeZ, 0, GL_LUMINANCE_ALPHA, GL_FLOAT, tmp);
                         }
                     } 
                     else {
                         glTexImage2D(GL_TEXTURE_2D, 0, 4, InputData->sizeX, InputData->sizeY, 0, GL_RGBA, GL_FLOAT, tmp);
                     }
		  }
           
		  

          delete [] tmp; tmp = NULL;
		 }

private: System::Void ToolStripMenuItem_Exit_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 Close();
		 }
private: System::Void RenderTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
			 //GenerateTextures();
			 DrawGLScene();
			 SwapBuffers(hDC);
		 }
private: System::Void MainForm_Resize(System::Object^  sender, System::EventArgs^  e) {
			 GLsizei height = this->RenderingPanel->Height;
                 GLsizei width = this->RenderingPanel->Width;
                 if (height == 0) {
                     height = 1;
                 }

                 glViewport(0, 0, width, height);

                 glMatrixMode(GL_PROJECTION);
                 glLoadIdentity();

                 gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);

                 glMatrixMode(GL_MODELVIEW);
                 glLoadIdentity();

                 DrawGLScene();
                 SwapBuffers(hDC);

		 }
private: System::Void TrackBar_Layers_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  this->Label_LayerInfo->Text = L"������� ����: "+(this->TrackBar_Layers->Value+1).ToString()+"/"+(this->TrackBar_Layers->Maximum+1).ToString();
			 
		  if (this->RadioButton_2D->Checked && Segments_2D)
		  {
		   size_t z  = this->TrackBar_Layers->Value;

		   bool tmp_value = CellValueChanging; CellValueChanging = false;

		   this->DataGridView_Clusters->RowCount = Segments_2D[z].size();
		   for (size_t index = 0; index < Segments_2D[z].size(); ++index)
		   {
		    this->DataGridView_Clusters->Rows[index]->Cells[0]->Value = index.ToString();
			this->DataGridView_Clusters->Rows[index]->Cells[1]->Value = Segments_2D[z].at(index).Volume.ToString();
		    this->DataGridView_Clusters->Rows[index]->Cells[2]->Value = Segments_2D[z].at(index).MinDensity.ToString();
		    this->DataGridView_Clusters->Rows[index]->Cells[3]->Value = Segments_2D[z].at(index).MaxDensity.ToString();

			if (CheckBox_ColorsExport->Checked)
			{
			 //TColor tmpColor = VoxelsData[Segments_2D[z].at(index).Index.at(0)+z*InputData->sizeX*InputData->sizeY].Color_2D;
			 //bool tmpVisible = VoxelsData[Segments_2D[z].at(index).Index.at(0)+z*InputData->sizeX*InputData->sizeY].IsVisible_2D;

			 this->DataGridView_Clusters->Rows[index]->Cells[4]->Value = GetStringOfColor(Segments_2D[z].at(index).tmpColor);
			 this->DataGridView_Clusters->Rows[index]->Cells[5]->Value = Segments_2D[z].at(index).tmpTransparensy.ToString();
			 this->DataGridView_Clusters->Rows[index]->Cells[6]->Value = Segments_2D[z].at(index).tmpVisible;
			}
			else
			{
 		     this->DataGridView_Clusters->Rows[index]->Cells[4]->Value = this->GetStringOfColor(Segments_2D[z].at(index).Color);
			 this->DataGridView_Clusters->Rows[index]->Cells[5]->Value = Segments_2D[z].at(index).tmpTransparensy.ToString();
		     this->DataGridView_Clusters->Rows[index]->Cells[6]->Value = Segments_2D[z].at(index).Visible;
			}
		   }

		   CellValueChanging = tmp_value;
    	  }  
		 }
private: System::Void CheckBoxClusters_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (this->RadioButton_2D->Checked && Segments_2D || this->RadioButton_3D->Checked && Segments_3D)
			 {
			  DataGridView_Clusters->Enabled = CheckBoxClusters->Checked;
			 
		      Button_VisualizeSelectedClusters->Enabled = CheckBoxClusters->Checked;
			 }

		 }
private: System::Void Button_Clusterization_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
		  //this->StaticDelInst_Visible_2D = gcnew MyVisibleDelegate(this, &MainForm::ChangeVisible_2D);
		  //this->StaticDelInst_Visible_3D = gcnew MyVisibleDelegate(this, &MainForm::ChangeVisible_3D);
		  //this->StaticDelInst_Color_2D = gcnew MyColorDelegate(this, &MainForm::ChangeVoxelColor_2D);
		  //this->StaticDelInst_Color_3D = gcnew MyColorDelegate(this, &MainForm::ChangeVoxelColor_3D);

		  this->ProgressBar_Layers->Value = 0;
		  this->ProgressBar_Iterations->Value = 0;

		  low_density = SHORT::Parse(this->TextBox_LowBorder->Text);
		  high_density = SHORT::Parse(this->TextBox_HighBorder->Text);

		  bool tmp_value = ((low_density == 0)&&(high_density == MaxDensity));

		  this->Label_Status->Text = L"";

		  if (RadioButton_2D->Checked)					// ���� ������������� �� �����
		  {
		   delete [] Segments_2D; Segments_2D = NULL; 
		   
		   Segments_2D = new vector <TSegment> [InputData->sizeZ];

		   /*for (size_t index = 0; index<InputData->TotalSize; ++index) 
		   {
			   VoxelsData[index].Color_2D = TColor(0.0f, 0.0f, 0.0f, 0.0f);			
			   VoxelsData[index].IsVisible_2D = true;
		   }*/

		   float SumTime = 0.0f;
				   
		   for (size_t z = 0; z < InputData->sizeZ; ++z)
		   {
			ClusterizationMethod = RadioButton_Consequtive->Checked ? 
			(tmp_value ? new KmeansMethod(InputData->GetLayer(z)) : 
			(KmeansMethod*)new KmeansConditionalMethod(InputData->GetLayer(z), low_density, high_density)) :
			(tmp_value ? new KmeansOpenCLMethod(InputData->GetLayer(z)) :
			(KmeansMethod*)new KmeansConditionalOpenCLMethod(InputData->GetLayer(z), low_density, high_density));
			
			ClusterizationMethod->SetClustersNumber(TrackBar_ClustersCount->Value);
		    ClusterizationMethod->SetCountIterations(TrackBar_IterationsCount->Value);
						
			//Label_Status->Text = L"����������� ����������� ������. ����������, ���������...";
						 
			vector <vector <size_t> > TmpIndexVector = ClusterizationMethod->GetClusters(this->BackgroundWorker);

			SumTime += ClusterizationMethod->GetExecutionTime();

			size_t NumberOfSegments = TmpIndexVector.size();

			for (size_t i = 0; i < NumberOfSegments; ++i)
		    {
    		 vector <size_t> segment_indexes = TmpIndexVector.at(i);

			 // ���������� ������ ��������
			 vector <size_t>::iterator segment_indexes_iter = segment_indexes.begin();
			 vector <size_t>::iterator segment_indexes_iter_end = segment_indexes.end();
			 for (; segment_indexes_iter != segment_indexes_iter_end; ++segment_indexes_iter)
			 VoxelsData[*segment_indexes_iter + z*InputData->sizeX*InputData->sizeY].SegmentIndex_2D = i;

			 Segments_2D[z].push_back(TSegment()); 
			 
			 Segments_2D[z].at(i).Volume = InputData->scaleX*InputData->scaleY*InputData->scaleZ*segment_indexes.size();
			 Segments_2D[z].at(i).MinDensity = MinDensityOfSegment_2D(z, i);
			 Segments_2D[z].at(i).MaxDensity = MaxDensityOfSegment_2D(z, i);
			 
			 float grayIntensity = (float)(i+1)/NumberOfSegments; // ���������� ������������� ������� ������� �� ��������

			 Segments_2D[z].at(i).Color = TColor(grayIntensity, grayIntensity, grayIntensity, grayIntensity);

			 Segments_2D[z].at(i).Transparensy = (unsigned char)(255*grayIntensity);

			 Segments_2D[z].at(i).Visible = true;

			 Segments_2D[z].at(i).tmpColor = Segments_2D[z].at(i).Color;
			 Segments_2D[z].at(i).tmpTransparensy = Segments_2D[z].at(i).Transparensy;
			 Segments_2D[z].at(i).tmpVisible = true;

		     
			/* vector<size_t>::iterator iter = segment_indexes.begin();
			 vector<size_t>::iterator _end = segment_indexes.end();
			 for (; iter!=_end; ++iter) 
			 {
			  this->StaticDelInst_Color_2D(*iter+z*InputData->sizeX*InputData->sizeY, TColor(grayIntensity, grayIntensity, grayIntensity, grayIntensity));
     		 }*/
			} 

			/*for (size_t voxel_index = 0; voxel_index < InputData->sizeX*InputData->sizeY; ++voxel_index)
		    {
			 if ((InputData->data[voxel_index+z*InputData->sizeX*InputData->sizeY]<low_density)||
			     (InputData->data[voxel_index+z*InputData->sizeX*InputData->sizeY]>high_density))
			 this->StaticDelInst_Visible_2D(voxel_index+z*InputData->sizeX*InputData->sizeY, false);
		    }*/

			// ���������� ��� ���������� ��������
			this->ProgressBar_Iterations->Value = z==InputData->sizeZ-1 ? this->ProgressBar_Iterations->Maximum : 0;
			this->ProgressBar_Layers->Value++;
			
		   }

		   System::Windows::Forms::MessageBox::Show(L"������� ����� ����������� 1 ����: "+(SumTime/InputData->sizeZ).ToString()+L" �.", L"������� ����������� ��������", MessageBoxButtons::OK, MessageBoxIcon::Information, MessageBoxDefaultButton::Button1, MessageBoxOptions::DefaultDesktopOnly, false);
		   
		   Label_Status->Text = L"������ ��������������.";

		   size_t z = this->TrackBar_Layers->Value;

		   CellValueChanging = false;

		   this->DataGridView_Clusters->RowCount = Segments_2D[z].size();

		   //������� � ������� ���������� � ��������� ��� �������� ����
		   for (size_t index = 0; index < Segments_2D[z].size(); ++index)
		   {
			this->DataGridView_Clusters->Rows[index]->Cells[0]->Value = index.ToString();
			this->DataGridView_Clusters->Rows[index]->Cells[1]->Value = Segments_2D[z].at(index).Volume.ToString();
			this->DataGridView_Clusters->Rows[index]->Cells[2]->Value = Segments_2D[z].at(index).MinDensity.ToString();
			this->DataGridView_Clusters->Rows[index]->Cells[3]->Value = Segments_2D[z].at(index).MaxDensity.ToString();
			this->DataGridView_Clusters->Rows[index]->Cells[4]->Value = this->GetStringOfColor(Segments_2D[z].at(index).Color);
			this->DataGridView_Clusters->Rows[index]->Cells[5]->Value = Segments_2D[z].at(index).Transparensy.ToString();
			this->DataGridView_Clusters->Rows[index]->Cells[6]->Value = Segments_2D[z].at(index).Visible;
		   }

		   CellValueChanging = true;

		   this->DataGridView_Clusters->Enabled = this->CheckBoxClusters->Checked;
		  }

		  if (this->RadioButton_3D->Checked)				// ���� ����������� ���������� ���� ��� ������� ������
		  {
		   delete [] Segments_3D; Segments_3D = NULL; 
		   
		   Segments_3D = new vector <TSegment> [1];

		   /*for (size_t index = 0; index<InputData->TotalSize; ++index) 
		   {
			   VoxelsData[index].Color_3D = TColor(0.0f, 0.0f, 0.0f, 0.0f);			
			   VoxelsData[index].IsVisible_3D = true;
		   }*/

		   StartLayerIndex_3D = (size_t)this->NumericUpDown_Start->Value;
	
		   ScanData* Data = InputData->GetSubData(StartLayerIndex_3D, (size_t)this->NumericUpDown_Finish->Value);

		   ClusterizationMethod = RadioButton_Consequtive->Checked ? 
		   (tmp_value ? new KmeansMethod(Data) : 
		   (KmeansMethod*)new KmeansConditionalMethod(Data, low_density, high_density)) :
		   (tmp_value ? new KmeansOpenCLMethod(Data) :
		   (KmeansMethod*)new KmeansConditionalOpenCLMethod(Data, low_density, high_density));

		   ClusterizationMethod->SetClustersNumber(this->TrackBar_ClustersCount->Value);
		   ClusterizationMethod->SetCountIterations(this->TrackBar_IterationsCount->Value);

  		   //Label_Status->Text = L"����������� ����������� ������. ����������, ���������...";

		   vector <vector <size_t> > TmpIndexVector = ClusterizationMethod->GetClusters(this->BackgroundWorker);

		   size_t NumberOfSegments = TmpIndexVector.size();

		   for (size_t i = 0; i < NumberOfSegments; ++i)
		   {
			vector <size_t> segment_indexes = TmpIndexVector.at(i);

			vector <size_t>::iterator segment_indexes_iter = segment_indexes.begin();
			vector <size_t>::iterator segment_indexes_iter_end = segment_indexes.end();
			
			for (; segment_indexes_iter != segment_indexes_iter_end; ++segment_indexes_iter)
			VoxelsData[*segment_indexes_iter+StartLayerIndex_3D*InputData->sizeX*InputData->sizeY].SegmentIndex_3D = i;

			Segments_3D->push_back(TSegment());

			Segments_3D->at(i).MinDensity = MinDensityOfSegment_3D(StartLayerIndex_3D, i);
			Segments_3D->at(i).MaxDensity = MaxDensityOfSegment_3D(StartLayerIndex_3D, i);
			Segments_3D->at(i).Visible = true;
			
			float grayIntensity = (float)(i+1)/NumberOfSegments;

			Segments_3D->at(i).Color = TColor(grayIntensity, grayIntensity, grayIntensity, grayIntensity);

			Segments_3D->at(i).Transparensy = (unsigned char)(255*grayIntensity);

		    /*vector<size_t>::iterator iter = segment_indexes.begin();
		    vector<size_t>::iterator _end = segment_indexes.end();
		    for (; iter!=_end; ++iter) 
		    {
			 this->StaticDelInst_Color_3D(*iter+StartLayerIndex_3D*InputData->sizeX*InputData->sizeY, TColor(grayIntensity, grayIntensity, grayIntensity, grayIntensity));
			}*/
		   }

		   /*for (size_t voxel_index = 0; voxel_index < InputData->TotalSize; ++voxel_index)
		   {
			if ((InputData->data[voxel_index]<low_density)||
			    (InputData->data[voxel_index]>high_density))
			this->StaticDelInst_Visible_3D(voxel_index, false);
		   }*/

		   this->ProgressBar_Layers->Value = this->ProgressBar_Layers->Maximum;

		   System::Windows::Forms::MessageBox::Show(L"����� ����� �����������: "+ClusterizationMethod->GetExecutionTime().ToString()+L" �.", L"������� ����������� ��������", MessageBoxButtons::OK, MessageBoxIcon::Information, MessageBoxDefaultButton::Button1, MessageBoxOptions::DefaultDesktopOnly, false);

		   Label_Status->Text = L"������ ��������������.";

		   CellValueChanging = false;

		   this->DataGridView_Clusters->RowCount = Segments_3D->size();

		   for (size_t index = 0; index < Segments_3D->size(); ++index)
		   {
			this->DataGridView_Clusters->Rows[index]->Cells[0]->Value = index.ToString();
			this->DataGridView_Clusters->Rows[index]->Cells[1]->Value = Segments_3D->at(index).Volume.ToString();
			this->DataGridView_Clusters->Rows[index]->Cells[2]->Value = Segments_3D->at(index).MinDensity.ToString();
			this->DataGridView_Clusters->Rows[index]->Cells[3]->Value = Segments_3D->at(index).MaxDensity.ToString();
			this->DataGridView_Clusters->Rows[index]->Cells[4]->Value = this->GetStringOfColor(Segments_3D->at(index).Color);
			this->DataGridView_Clusters->Rows[index]->Cells[5]->Value =	Segments_3D->at(index).Transparensy.ToString(); 
			this->DataGridView_Clusters->Rows[index]->Cells[6]->Value = Segments_3D->at(index).Visible;
		   }

		   CellValueChanging = true;

		   this->DataGridView_Clusters->Enabled = this->CheckBoxClusters->Checked;
		  }

		  GenerateTextures();

		  if (this->CheckBoxClusters->Checked)
			  this->Button_VisualizeSelectedClusters->Enabled = true;
		 }
private: System::Void TrackBar_ClustersCount_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Label_ClustersCount->Text = L"������������ ����� ���������: "+
											   this->TrackBar_ClustersCount->Value;
		 }
private: System::Void TrackBar_IterationsCount_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Label_IterationsCount->Text = L"���������� �������� ���������: "+
												 this->TrackBar_IterationsCount->Value;
		 }
private: System::Void BackgroundWorker_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
		 {

		 }
public: System::Void BackgroundWorker_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) 
		 {
		  this->ProgressBar_Iterations->Value = e->ProgressPercentage;
		  this->Label_Status->Text = (this->ProgressBar_Iterations->Value == this->ProgressBar_Iterations->Maximum) ?
			  L"" : L"����������� ����������� ���� ������, "+
		  (100*this->ProgressBar_Iterations->Value/this->ProgressBar_Iterations->Maximum).ToString()+
		  L"% ���������";
		 }
private: System::Void BackgroundWorker_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
		 }

private: System::Void Button_VisualizeSelectedClusters_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
		  if (this->RadioButton_2D->Checked)
		  {
		   size_t z = (size_t)this->TrackBar_Layers->Value;

		   for (size_t i = 0; i<Segments_2D[z].size(); ++i)
				
		   {
			Segments_2D[z].at(i).Visible = (bool)this->DataGridView_Clusters->Rows[i]->Cells[6]->Value;

			/*vector<size_t>::iterator iter = Segments_2D[z].at(i).Index.begin();
			vector<size_t>::iterator _end = Segments_2D[z].at(i).Index.end();
			for (; iter!=_end; ++iter)
			{
		 	 if ((bool)this->DataGridView_Clusters->Rows[i]->Cells[6]->Value ==true)
			 this->StaticDelInst_Visible_2D(*iter+z*InputData->sizeX*InputData->sizeY, true);
			 else this->StaticDelInst_Visible_2D(*iter+z*InputData->sizeX*InputData->sizeY, false);
			}*/
		   }
		  }

		  if (this->RadioButton_3D->Checked)
		  {
		   for (size_t i = 0; i<Segments_3D->size(); ++i)
		   {
			Segments_3D->at(i).Visible = (bool)this->DataGridView_Clusters->Rows[i]->Cells[6]->Value;

			/*vector <size_t>::iterator iter = Segments_3D->at(i).Index.begin();
			vector <size_t>::iterator _end = Segments_3D->at(i).Index.end();
			for (; iter!=_end; ++iter)
			{
			 if (*iter+(size_t)StartLayerIndex_3D*InputData->sizeX*InputData->sizeY<InputData->TotalSize)
			 this->StaticDelInst_Visible_3D(*iter+(size_t)StartLayerIndex_3D*InputData->sizeX*InputData->sizeY, (bool)this->DataGridView_Clusters->Rows[i]->Cells[6]->Value);
   			}*/
		   }
		  }

		  GenerateTextures();
		 }
private: System::Void NumericUpDown_Start_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (this->NumericUpDown_Start->Value > this->NumericUpDown_Finish->Value)
				 this->NumericUpDown_Start->Value = this->NumericUpDown_Finish->Value; 
		 }
private: System::Void NumericUpDown_Finish_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (this->NumericUpDown_Finish->Value < this->NumericUpDown_Finish->Value)
				 this->NumericUpDown_Finish->Value = this->NumericUpDown_Start->Value;
		 }
private: System::Void RenderingPanel_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
		  if (e->Button == Windows::Forms::MouseButtons::Left)
		  {
		   System::Drawing::Point tmpPosition = mousePosition;
           mousePosition = e->Location;
           angleXRotation -= tmpPosition.X - mousePosition.X;
           angleYRotation += tmpPosition.Y - mousePosition.Y;
          }
		 }
private: System::Void RenderingPanel_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
		  if (e->Button == Windows::Forms::MouseButtons::Left) { mousePosition = e->Location; }
		 }
private: System::Void RenderingPanel_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			  distance_z *= (e->Delta < 0) ? (float)Math::Abs(0.66 * e->Delta / 120) :
                     (float)Math::Abs(1.33 * e->Delta / 120);
		 }
private: System::Void RenderingPanel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			this->RenderingPanel->Focus();
		 }
private: System::Void TextBox_BrightnessMult_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 try {
                     BrightnessMult = (float)Double::Parse(this->TextBox_BrightnessMult->Text);
                 } catch (...) 
			     {
					 /*MessageBox::Show("������������ �������� ������.", "������!", MessageBoxButtons::OK, MessageBoxIcon::Error,
						 MessageBoxDefaultButton::Button1, MessageBoxOptions::DefaultDesktopOnly, false); */
					 this->Label_Status->Text = L"������������ �������� ������.";
                 }
		 }
private: System::Void CheckBox_Transparency_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->CheckBox_DepthTest->Enabled = this->CheckBox_Transparency->Checked;
		 }


private: System::Void DataGridView_Clusters_CellClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			 if (e->ColumnIndex==4)
			 {
			  if (this->ColorDialog->ShowDialog()== Windows::Forms::DialogResult::OK)
			  {
			   System::Drawing::Color selectedColor = ColorDialog->Color;
			   
			   TColor tmpColor = TColor((float)selectedColor.R/255, (float)selectedColor.G/255, (float)selectedColor.B/255, (float)selectedColor.A/255);
			   this->DataGridView_Clusters->Rows[e->RowIndex]->Cells[4]->Value = this->GetStringOfColor(tmpColor);
			   this->DataGridView_Clusters->Rows[e->RowIndex]->Cells[5]->Value = selectedColor.A.ToString();
			   //this-DataGridView_Clusters->Rows[e->RowIndex]->Cells[4]->Style->FieldSetter(L"System::Drawing::Color", L"BackColor", (Object^)selectedColor); 

			   if (this->RadioButton_2D->Checked)
			   {
				size_t z = (size_t)this->TrackBar_Layers->Value; 
			    Segments_2D[z].at(e->RowIndex).Color = tmpColor;
				Segments_2D[z].at(e->RowIndex).Transparensy = selectedColor.A;

				/*vector<size_t>::iterator iter = Segments_2D[z].at(e->RowIndex).Index.begin();
				vector<size_t>::iterator _end = Segments_2D[z].at(e->RowIndex).Index.end();
			    for (; iter!=_end; ++iter)
			    {
				 this->StaticDelInst_Color_2D(*iter+z*InputData->sizeX*InputData->sizeY, tmpColor);
			    }*/
				
				if (CheckBox_ColorsExport->Checked)
				{
				 /*for (size_t layer_index = 0; layer_index < InputData->sizeZ; ++layer_index)
				 if ((layer_index != z)&&(e->RowIndex < Segments_2D[layer_index].size()))
				 {
				  vector <size_t>::iterator iter = Segments_2D[layer_index].at(e->RowIndex).Index.begin();
				  vector <size_t>::iterator _end = Segments_2D[layer_index].at(e->RowIndex).Index.end();
				  for (; iter!=_end; ++iter)
				  {
				   this->StaticDelInst_Color_2D(*iter+layer_index*InputData->sizeX*InputData->sizeY, tmpColor);
				  }
				 }*/
				}
				
			   }
			   
			   if (this->RadioButton_3D->Checked)
			   {
				Segments_3D->at(e->RowIndex).Color = TColor((float)selectedColor.R/255, (float)selectedColor.G/255, (float)selectedColor.B/255, (float)selectedColor.A/255);
				Segments_3D->at(e->RowIndex).Transparensy = selectedColor.A;

				/*vector<size_t>::iterator iter = Segments_3D->at(e->RowIndex).Index.begin();
				vector<size_t>::iterator _end = Segments_3D->at(e->RowIndex).Index.end();
			    for (; iter!=_end; ++iter)
			    {
				 if (*iter+(size_t)StartLayerIndex_3D*InputData->sizeX*InputData->sizeY<InputData->TotalSize)
				 this->StaticDelInst_Color_3D(*iter+(size_t)StartLayerIndex_3D*InputData->sizeX*InputData->sizeY, tmpColor);
			    }*/
			   }
			  
			   GenerateTextures();
			  }
			 }
		     
			 
         }
			
		 
private: System::Void DataGridView_Clusters_CellValueChanged(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			if ((e->ColumnIndex == 6) && InputData && CellValueChanging)
			{
			 int index = e->RowIndex;

			 if (this->RadioButton_2D->Checked)
			 {
			  size_t z = (size_t)this->TrackBar_Layers->Value;
			  Segments_2D[z].at(index).Visible = (bool)this->DataGridView_Clusters->Rows[index]->Cells[6]->Value;
			  /*vector <size_t>::iterator iter = Segments_2D[z].at(index).Index.begin();
			  vector <size_t>::iterator _end = Segments_2D[z].at(index).Index.end();
			  for (; iter!=_end; ++iter)
			  {
			   this->StaticDelInst_Visible_2D(*iter+z*InputData->sizeX*InputData->sizeY, Segments_2D[z].at(index).IsVisible);
			  }*/

			  if (CheckBox_ColorsExport->Checked)
			  {
			   for (size_t layer_index = 0; layer_index < InputData->sizeZ; ++layer_index)
			   if ((layer_index!=z)&&(index < Segments_2D[layer_index].size()))
			   {
			    /*vector <size_t>::iterator iter = Segments_2D[layer_index].at(index).Index.begin();
			    vector <size_t>::iterator _end = Segments_2D[layer_index].at(index).Index.end();
			    for (; iter!=_end; ++iter)
			    {
				 this->StaticDelInst_Visible_2D(*iter+layer_index*InputData->sizeX*InputData->sizeY, Segments_2D[z].at(index).IsVisible);
				}*/
			   }
			  }
					
			 }

			 if (this->RadioButton_3D->Checked)
			 {
			  Segments_3D->at(index).Visible = (bool)this->DataGridView_Clusters->Rows[index]->Cells[6]->Value;
			  /*vector <size_t>::iterator iter = Segments_3D->at(index).Index.begin();
			  vector <size_t>::iterator _end = Segments_3D->at(index).Index.end();
			  for (; iter!=_end; ++iter)
			  {
			   if (*iter+(size_t)StartLayerIndex_3D*InputData->sizeX*InputData->sizeY<InputData->TotalSize)
			   this->StaticDelInst_Visible_3D(*iter+(size_t)StartLayerIndex_3D*InputData->sizeX*InputData->sizeY, Segments_3D->at(index).IsVisible);
			  }*/
					
			 }
				
			 GenerateTextures();
			}
		 }

private: System::Void CheckBoxClusters_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 GenerateTextures();
			 this->CheckBox_Gradients->Checked = !this->CheckBoxClusters->Checked&&this->CheckBox_Gradients->Checked;
			 this->CheckBox_Localization->Checked = !this->CheckBoxClusters->Checked&&this->CheckBox_Localization->Checked;
			 this->CheckBox_Reconstruction->Checked = !this->CheckBoxClusters->Checked&&this->CheckBox_Reconstruction->Checked;
		 }
private: System::Void TrackBar_LayersDistance_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->Label_LayersDistance->Text = L"���������� ����� ������: "+
												this->TrackBar_LayersDistance->Value.ToString();
		 }

private: System::Void DataGridView_Clusters_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
			if ((e->ColumnIndex==5) && CellValueChanging)
			{
			 int index = e->RowIndex;

			 unsigned char tmp_value = 0;

			 try 
			 {
			  tmp_value = 
				  (unsigned char)System::Int32::Parse((System::String^)this->DataGridView_Clusters->Rows[index]->Cells[5]->Value);
			 } 
			 catch (...)
			 {
			  Label_Status->Text = L"������������ �������� ������.";
			 }

			 if (this->RadioButton_2D->Checked)
		  
			 {
			  size_t z = (size_t)this->TrackBar_Layers->Value;

			  Segments_2D[z].at(index).Transparensy = tmp_value;
		   
			  /*vector<size_t>::iterator iter = Segments_2D[z].at(index).Index.begin();
		      vector<size_t>::iterator _end = Segments_2D[z].at(index).Index.end();
			  for (; iter!=_end; ++iter)
			  {
			   TColor tmpColor = VoxelsData[*iter+z*InputData->sizeX*InputData->sizeY].Color_2D;
			   this->StaticDelInst_Color_2D(*iter+z*InputData->sizeX*InputData->sizeY, TColor(tmpColor.R, tmpColor.G, tmpColor.B, (float)tmp_value/255));
			  }*/

			  if (CheckBox_ColorsExport->Checked)
			  {
			   /* for (size_t layer_index = 0; layer_index < InputData->sizeZ; ++layer_index)
			   if ((layer_index != z)&&(index < Segments_2D[layer_index].size()))
			   {
				vector <size_t>::iterator iter = Segments_2D[layer_index].at(index).Index.begin();
				vector <size_t>::iterator _end = Segments_2D[layer_index].at(index).Index.end();
				for (; iter!=_end; ++iter)
				{
				 TColor tmpColor = VoxelsData[*iter+layer_index*InputData->sizeX*InputData->sizeY].Color_2D;
				 this->StaticDelInst_Color_2D(*iter+layer_index*InputData->sizeX*InputData->sizeY, TColor(tmpColor.R, tmpColor.G, tmpColor.B, (float)tmp_value/255));
				}
			   } */
			  }
			  
			  GenerateTextures();
			 }

			  if (this->RadioButton_3D->Checked)
			  {
			   Segments_3D->at(index).Transparensy = tmp_value;
			   /*vector<size_t>::iterator iter = Segments_3D->at(index).Index.begin();
			   vector<size_t>::iterator _end = Segments_3D->at(index).Index.end();
			   for (; iter!=_end; ++iter)
			   if (*iter+(size_t)StartLayerIndex_3D*InputData->sizeX*InputData->sizeY<InputData->TotalSize)
			   {
				TColor tmpColor = VoxelsData[*iter+(size_t)StartLayerIndex_3D*InputData->sizeX*InputData->sizeY].Color_3D;
				
				this->StaticDelInst_Color_3D(*iter+(size_t)StartLayerIndex_3D*InputData->sizeX*InputData->sizeY, TColor(tmpColor.R, tmpColor.G, tmpColor.B, (float)tmp_value/255));
			   }*/

			   GenerateTextures();
			  }
			  
			}
		 }
private: System::Void TextBox_LowBorder_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 try {
				  UINT::Parse(this->TextBox_LowBorder->Text);
                 } catch (...) 
			     {
					 /* MessageBox::Show("������������ �������� ������.", "������!", MessageBoxButtons::OK, MessageBoxIcon::Error,
						MessageBoxDefaultButton::Button1, MessageBoxOptions::DefaultDesktopOnly, false); */
					 this->Label_Status->Text = L"������������ �������� ������.";
                 }

		 }
private: System::Void TextBox_HighBorder_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 try {
				  UINT::Parse(this->TextBox_HighBorder->Text);
                 } catch (...) 
			     {
					 /*MessageBox::Show("������������ �������� ������.", "������!", MessageBoxButtons::OK, MessageBoxIcon::Error,
						 MessageBoxDefaultButton::Button1, MessageBoxOptions::DefaultDesktopOnly, false); */
					 this->Label_Status->Text = L"������������ �������� ������.";
                 }
		 }
private: System::Void CheckBox_ColorsExport_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (this->RadioButton_2D->Checked && Segments_2D)
			 {
				 switch (CheckBox_ColorsExport->Checked)
				 {

				 case true:
				 {
				  size_t z = (size_t)this->TrackBar_Layers->Value;

				  for (size_t layer_index = 0; layer_index < InputData->sizeZ; ++layer_index)
				  if (layer_index!=z)
				  {
				   for (size_t i = 0; i < min(Segments_2D[layer_index].size(), Segments_2D[z].size()); ++i)
				   {
				    TColor tmpColor = Segments_2D[z].at(i).Color;

					/*vector <size_t>::iterator iter = Segments_2D[layer_index].at(i).Index.begin();
					vector <size_t>::iterator _end = Segments_2D[layer_index].at(i).Index.end();
					for(; iter!=_end; ++iter)
					{
					 this->StaticDelInst_Color_2D(*iter+layer_index*InputData->sizeX*InputData->sizeY, 
					 TColor(tmpColor.R, tmpColor.G, tmpColor.B, (float)Segments_2D[z].at(i).transparensy/255));
					 this->StaticDelInst_Visible_2D(*iter+layer_index*InputData->sizeX*InputData->sizeY, Segments_2D[z].at(i).IsVisible);
					}*/
				   }
  				  }

				  break;
				 }

				 case false:
				 {
			      for (size_t z = 0; z < InputData->sizeZ; ++z)
				  {
				   for (size_t i = 0; i < Segments_2D[z].size(); ++i)
				   {
				    TColor tmpColor = Segments_2D[z].at(i).Color;

					/*vector <size_t>::iterator iter = Segments_2D[z].at(i).Index.begin();
					vector <size_t>::iterator _end = Segments_2D[z].at(i).Index.end();
					for(; iter!=_end; ++iter)
					{
					 this->StaticDelInst_Color_2D(*iter+z*InputData->sizeX*InputData->sizeY, 
					 TColor(tmpColor.R, tmpColor.G, tmpColor.B, (float)Segments_2D[z].at(i).transparensy/255));
					 this->StaticDelInst_Visible_2D(*iter+z*InputData->sizeX*InputData->sizeY, Segments_2D[z].at(i).IsVisible);
					}*/
				   }
  				  }

				  break;
				 }

				}

				GenerateTextures();
			 }

		 }
private: System::Void CheckBox_ColorsExport_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			if (Segments_2D)
			 {
				 switch (CheckBox_ColorsExport->Checked)
				 {

				 case true:
				 {
				  size_t z = (size_t)this->TrackBar_Layers->Value;

				  for (size_t layer_index = 0; layer_index < InputData->sizeZ; ++layer_index)
				  if (layer_index!=z)
				  {
				   for (size_t i = 0; i < min(Segments_2D[layer_index].size(), Segments_2D[z].size()); ++i)
				   {
				    TColor tmpColor = Segments_2D[z].at(i).Color;

					/*vector <size_t>::iterator iter = Segments_2D[layer_index].at(i).Index.begin();
					vector <size_t>::iterator _end = Segments_2D[layer_index].at(i).Index.end();
					for(; iter!=_end; ++iter)
					{
					 this->StaticDelInst_Color_2D(*iter+layer_index*InputData->sizeX*InputData->sizeY, 
					 TColor(tmpColor.R, tmpColor.G, tmpColor.B, (float)Segments_2D[z].at(i).transparensy/255));
					 this->StaticDelInst_Visible_2D(*iter+layer_index*InputData->sizeX*InputData->sizeY, Segments_2D[z].at(i).IsVisible);
					}*/
				   }
  				  }

				  break;
				 }

				 case false:
				 {
			      for (size_t z = 0; z < InputData->sizeZ; ++z)
				  {
				   for (size_t i = 0; i < Segments_2D[z].size(); ++i)
				   {
				    TColor tmpColor = Segments_2D[z].at(i).Color;

					/*vector <size_t>::iterator iter = Segments_2D[z].at(i).Index.begin();
					vector <size_t>::iterator _end = Segments_2D[z].at(i).Index.end();
					for(; iter!=_end; ++iter)
					{
					 this->StaticDelInst_Color_2D(*iter+z*InputData->sizeX*InputData->sizeY, 
					 TColor(tmpColor.R, tmpColor.G, tmpColor.B, (float)Segments_2D[z].at(i).transparensy/255));
					 this->StaticDelInst_Visible_2D(*iter+z*InputData->sizeX*InputData->sizeY, Segments_2D[z].at(i).IsVisible);
					}*/
				   }
  				  }

				  break;
				 }

				}

				GenerateTextures();
			 }
		 }

private: System::Void ScanFolder(LPWSTR path, vector <WCHAR*> &StrFiles)
{
 const short str_size = 260;
 WIN32_FIND_DATA fd;
 HANDLE hFind = FindFirstFile(path, &fd);
 StrFiles.clear();

 if(hFind != INVALID_HANDLE_VALUE)
 {
  do
  {
   if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
   {
    WCHAR* tmp = new WCHAR [str_size];
    for (int i = 0; i < str_size; ++i) tmp[i] = fd.cFileName[i];
    StrFiles.push_back(tmp);
   }
  }

  while(FindNextFile(hFind, &fd));

  FindClose(hFind);
 }

 std::sort(StrFiles.begin(), StrFiles.end()); 
}		

private: System::Void ToolStripMenuItem_DownloadFolder_Click(System::Object^ sender, System::EventArgs^  e) 
		 {
		  if (FolderBrowserDialog->ShowDialog() == Windows::Forms::DialogResult::OK)
		  {
		   FolderPath = FolderBrowserDialog->SelectedPath;
		   System::String ^pathToFolderName = FolderPath + "\\*.*";
		   pin_ptr <const WCHAR> str = PtrToStringChars(pathToFolderName);
		   const int str_size = pathToFolderName->Length;
		   WCHAR strFolder[260];
		   wcscpy_s(strFolder, str);

		   delete StrFiles;
		   StrFiles = new vector <WCHAR*> ();
		   ScanFolder(strFolder, *StrFiles);
		   int N = StrFiles->size();

		   vector <WCHAR*>::iterator iter = StrFiles->begin();
		   vector <WCHAR*>::iterator iter_end = StrFiles->end();

		   Mat tmp_img = imread((char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(FolderPath + "\\"+ gcnew System::String(StrFiles->at(0))).ToPointer()));
			
		   int MaxLayers = (MEMORY_LIMIT - 2*K_MAX*(9*sizeof(float)+2*sizeof(short)+4))/
						   ((sizeof(short)+2*sizeof(int)+4*sizeof(float))*tmp_img.rows*tmp_img.cols);

		   int nLayers = min(MaxLayers, N);  			  
		   
		   // ����������� ������ ��� ����� ������
		   InputData->ReleaseResources();

		   // ����� ����� ������� �� ��� ���������
		   InputData->sizeX = tmp_img.cols;
		   InputData->sizeY = tmp_img.rows;
		   InputData->sizeZ = nLayers;
			  
		   // ����� ������� �� ������ ���
		   InputData->scaleX = 1.0f;
		   InputData->scaleY = 1.0f; 
		   InputData->scaleZ = 1.0f;

		   InputData->TotalSize = tmp_img.rows*tmp_img.cols*nLayers;

		   InputData->data = new short [InputData->TotalSize];

		   delete [] VoxelsData; VoxelsData = new TVoxel [InputData->TotalSize];

		   PartPointer = 0;

		   int k = PartPointer; 

		   for (; iter!=iter_end, k<PartPointer+nLayers; ++iter, ++k)
 		   {
			{
		     System::String^ fullpath = FolderPath + "\\" + gcnew System::String(*iter);
		     Mat img = imread((char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(fullpath).ToPointer()), 0);
			 
			 int type_value = img.type();

			 for (int i = 0; i < img.rows; ++i)
  			 for (int j = 0; j < img.cols; ++j)
			 InputData->data[(k-PartPointer)*InputData->sizeX*InputData->sizeY+InputData->sizeX*i+j] = img.at <uchar> (i,j);
			}
		   }
		  
		   MinDensity = 0; MaxDensity = 255;

		   ProgressBar_Layers->Maximum = InputData->sizeZ;
		   TrackBar_Layers->Maximum = InputData->sizeZ-1;
		   TrackBar_Layers->Value = 0;
		   NumericUpDown_Start->Maximum = InputData->sizeZ-1;
		   NumericUpDown_Start->Value = 0;
		   NumericUpDown_Finish->Maximum = InputData->sizeZ-1;
		   NumericUpDown_Finish->Value = InputData->sizeZ-1;

		   this->Button_PreviousPart->Enabled = false;

		   if (N > nLayers) this->Button_NextPart->Enabled = true;

		   this->Button_VisualizeSelectedClusters->Enabled = false;

		   delete [] Segments_2D; delete [] Segments_3D; 
			   
		   Segments_2D = NULL; Segments_3D = NULL; 
			   
		   this->DataGridView_Clusters->RowCount = 0;

		   this->DataGridView_Clusters->Enabled = false;

		   if (this->RadioButton_2D->Checked) this->TrackBar_Layers->Enabled = true;
		   
		   if (this->RadioButton_3D->Checked)
		   {
		    this->NumericUpDown_Start->Enabled = true;
		    this->NumericUpDown_Finish->Enabled = true;
			this->TrackBar_LayersDistance->Enabled = true;
		   }
					
		   this->TextBox_BrightnessMult->Enabled = true;

		   this->TextBox_LowBorder->Text = MinDensity.ToString();
		   this->TextBox_HighBorder->Text = MaxDensity.ToString();

		   this->TextBox_LowBorder->Enabled = true;
		   this->TextBox_HighBorder->Enabled = true;

		   this->Button_Clusterization->Enabled = true;

		   angleXRotation = angleYRotation = 0.0f;
		   distance_x = distance_y = 0.0f;
           distance_z = 600.0f;

		   TextBox_BrightnessMult->Text = L"1,0";
           BrightnessMult = 1.0f;

		   StartLayerIndex_3D = 0;

		   CellValueChanging = false;

		   GenerateTextures();
		  }
		 }

private: System::Void RenderingPanel_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e) 
		 {
		  const float shift_value = 100.0f;

		  if (e->KeyCode == System::Windows::Forms::Keys::Up) distance_y += shift_value; else 
		  if (e->KeyCode == System::Windows::Forms::Keys::Down) distance_y -= shift_value; else 
		  if (e->KeyCode == System::Windows::Forms::Keys::Right) distance_x -= shift_value; else 
		  if (e->KeyCode == System::Windows::Forms::Keys::Left) distance_x += shift_value;	 
		 }
		 private: System::Void ButtonReconstructionData_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
		  delete qw;
		  qw = new ballPivot(InputData);
		  qw->buildMesh();
		 }
private: System::Void RadioButtonTextures_2D_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  if (RadioButtonTextures_2D->Checked) GenerateTextures();
		 }
private: System::Void RadioButtonTextures_3D_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  if (RadioButtonTextures_3D->Checked) GenerateTextures();
		 }
private: System::Void RadioButtonRaycasting_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (RadioButtonRaycasting->Checked) GenerateTextures();
		 }
private: System::Void TrackBar_Alpha_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 Double newAlpha = TrackBar_Alpha->Value / 1000.0f;
			 TextBox_AlphaValue->Text = newAlpha.ToString();
		 }
private: System::Void TextBox_AlphaValue_TextChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 Double curValue;
             try {
                 curValue = Double::Parse(this->TextBox_AlphaValue->Text);
             } catch (...) {
				 MessageBox::Show(L"������������ �������� ������.", L"������!", System::Windows::Forms::MessageBoxButtons::OK, 
					 System::Windows::Forms::MessageBoxIcon::Error, System::Windows::Forms::MessageBoxDefaultButton::Button1,
					 System::Windows::Forms::MessageBoxOptions::RightAlign, false);
				 this->TextBox_AlphaValue->Focus();
                 return;
			 }

			 if (curValue >= 0 && curValue <= 1) {
				AlphaTestValue = curValue;
				this->TrackBar_Alpha->Value = int(AlphaTestValue * TrackBar_Alpha->Maximum);
             } else {
                 this->Label_Status->Text = "Error. Aplha test value should be in range [0, 1].";
             }

			 
		 }
private: System::Void CheckBox_AlphaTest_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  bool state = CheckBox_AlphaTest->Enabled && CheckBox_AlphaTest->Checked;
		  Label_ThresholdAlphaValue->Enabled = state;
          TextBox_AlphaValue->Enabled = state;
          TrackBar_Alpha->Enabled = state;
		 }
private: System::Void TrackBar_BrightnessMult_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  TextBox_BrightnessMult->Text = TrackBar_BrightnessMult->Value.ToString();
		 }
private: System::Void RadioButton_2D_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  CellValueChanging = false;

	 	  if (InputData && InputData->data) 
		  {
		   this->ProgressBar_Layers->Maximum = InputData->sizeZ;

		   if (this->ProgressBar_Iterations->Value == this->ProgressBar_Iterations->Maximum)
		   this->ProgressBar_Layers->Value = this->ProgressBar_Layers->Maximum;
		   else this->ProgressBar_Layers->Value = 0;

		   this->NumericUpDown_Start->Enabled = false;
		   this->NumericUpDown_Finish->Enabled = false;
		   this->Label_LayersDistance->Enabled = false;
		   this->TrackBar_LayersDistance->Enabled = false;
		   this->TrackBar_Layers->Enabled = true;

		   this->CheckBox_ColorsExport->Enabled = true;

		   if (Segments_2D)										// ���� �������� ��� �������
		   {
   		    size_t z = (size_t)this->TrackBar_Layers->Value;

			this->DataGridView_Clusters->RowCount = Segments_2D[z].size();

		    for (size_t index = 0; index < Segments_2D[z].size(); ++index)
		    {
		     this->DataGridView_Clusters->Rows[index]->Cells[0]->Value = index.ToString();
			 this->DataGridView_Clusters->Rows[index]->Cells[1]->Value = Segments_2D[z].at(index).Volume.ToString();
		     this->DataGridView_Clusters->Rows[index]->Cells[2]->Value = Segments_2D[z].at(index).MinDensity.ToString();
			 this->DataGridView_Clusters->Rows[index]->Cells[3]->Value = Segments_2D[z].at(index).MaxDensity.ToString();
			 
			 if (CheckBox_ColorsExport->Checked)
			 {
			  // TColor tmpColor = VoxelsData[Segments_2D[z].at(index).Index.at(0)+z*InputData->sizeX*InputData->sizeY].Color_2D;
			  // bool tmpVisible = VoxelsData[Segments_2D[z].at(index).Index.at(0)+z*InputData->sizeX*InputData->sizeY].IsVisible_2D;

			  this->DataGridView_Clusters->Rows[index]->Cells[4]->Value = GetStringOfColor(Segments_2D[z].at(index).tmpColor);
			  this->DataGridView_Clusters->Rows[index]->Cells[5]->Value = Segments_2D[z].at(index).tmpTransparensy.ToString();
			  this->DataGridView_Clusters->Rows[index]->Cells[6]->Value = Segments_2D[z].at(index).tmpVisible;
			 }

			 else

			 { 
			  this->DataGridView_Clusters->Rows[index]->Cells[4]->Value = GetStringOfColor(Segments_2D[z].at(index).Color);
			  this->DataGridView_Clusters->Rows[index]->Cells[5]->Value = Segments_2D[z].at(index).Transparensy.ToString();
			  this->DataGridView_Clusters->Rows[index]->Cells[6]->Value = Segments_2D[z].at(index).Visible;
			 }
			} 

			if (CheckBoxClusters->Checked) 
			{
			 this->DataGridView_Clusters->Enabled = true; 
			 this->Button_VisualizeSelectedClusters->Enabled = true; 
			}
		   }
		   else 
		   {
			this->DataGridView_Clusters->RowCount = 0;
		    this->DataGridView_Clusters->Enabled = false; 
			this->Button_VisualizeSelectedClusters->Enabled = false; 
		   }

		   GenerateTextures();
		  }
		  else this->ProgressBar_Layers->Maximum = 0;

		  CellValueChanging = true;
		 }
private: System::Void RadioButton_3D_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 CellValueChanging = false;

			 if (InputData && InputData->data) 
			 {
			  this->ProgressBar_Layers->Maximum = 1;

			  if (this->ProgressBar_Iterations->Value == this->ProgressBar_Iterations->Maximum)
			  this->ProgressBar_Layers->Value = this->ProgressBar_Layers->Maximum;
			  else this->ProgressBar_Layers->Value = 0;

     		  this->NumericUpDown_Start->Enabled = true;
			  this->NumericUpDown_Finish->Enabled = true;
			  this->Label_LayersDistance->Enabled = true;
			  this->TrackBar_LayersDistance->Enabled = true;
			  this->TrackBar_Layers->Enabled = false;

			  this->CheckBox_ColorsExport->Enabled = false;

			  if (Segments_3D)  // ���� �������� ��� �������
			  {
			   this->DataGridView_Clusters->RowCount = Segments_3D->size();

			   for (size_t index = 0; index < Segments_3D->size(); ++index)
			   {
			    this->DataGridView_Clusters->Rows[index]->Cells[0]->Value = index.ToString();
				this->DataGridView_Clusters->Rows[index]->Cells[1]->Value = Segments_3D->at(index).Volume.ToString();
			    this->DataGridView_Clusters->Rows[index]->Cells[2]->Value = Segments_3D->at(index).MinDensity.ToString();
			    this->DataGridView_Clusters->Rows[index]->Cells[3]->Value = Segments_3D->at(index).MaxDensity.ToString();
                this->DataGridView_Clusters->Rows[index]->Cells[4]->Value = this->GetStringOfColor(Segments_3D->at(index).Color);
			    this->DataGridView_Clusters->Rows[index]->Cells[5]->Value = Segments_3D->at(index).Transparensy.ToString();
			    this->DataGridView_Clusters->Rows[index]->Cells[6]->Value = Segments_3D->at(index).Visible; 
			   }

			   if (CheckBoxClusters->Checked) 
			   {
			    this->DataGridView_Clusters->Enabled = true; 
			    this->Button_VisualizeSelectedClusters->Enabled = true; 
			   }
			  }
			  
			 
			  else 
				  
			  {
				  this->DataGridView_Clusters->RowCount = 0; 
				  this->DataGridView_Clusters->Enabled = false;
				  this->Button_VisualizeSelectedClusters->Enabled = false; 
			  }
			  
			  GenerateTextures();
			 }
			 else this->ProgressBar_Layers->Maximum = 0;

			 CellValueChanging = true;
		 }
 private: System::Void TrackBar_AlphaValue_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		  {
			 SnakeParameters->alpha = (float)TrackBar_AlphaValue->Value;
			 Label_AlphaValue->Text = L"Alpha: " + SnakeParameters->alpha;
		  }
private: System::Void TrackBar_BetaValue_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 SnakeParameters->beta = (float)TrackBar_BetaValue->Value;
			 Label_BetaValue->Text = L"Beta: " + SnakeParameters->beta;
		 }
private: System::Void TrackBar_GammaValue_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 SnakeParameters->gamma = (float)TrackBar_GammaValue->Value;
			 Label_GammaValue->Text = L"Gamma: " + SnakeParameters->gamma;
		 }
private: System::Void TrackBar_IterationsNumber_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 SnakeParameters->iterations_number = TrackBar_IterationsNumber->Value;
			 Label_IterationsNumber->Text = L"����� ��������: " + SnakeParameters->iterations_number;
		 }
private: System::Void Button_NextPart_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
		  delete [] InputData->data; 
		  
		  PartPointer +=InputData->sizeZ;

		  InputData->sizeZ = min(InputData->sizeZ, StrFiles->size()-PartPointer);

		  InputData->TotalSize = InputData->sizeX*InputData->sizeY*InputData->sizeZ;

		  InputData->data = new short [InputData->TotalSize];
	      
		  delete [] VoxelsData; VoxelsData = new TVoxel [InputData->TotalSize];

		  for (int k = PartPointer; k<PartPointer+InputData->sizeZ; ++k)
 		  {
		   {
		     System::String^ fullpath = FolderPath + "\\" + gcnew System::String(StrFiles->at(k));
		     Mat img = imread((char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(fullpath).ToPointer()), 0);
			 
			 for (int i = 0; i < img.rows; ++i)
  			 for (int j = 0; j < img.cols; ++j)
			 {
			  InputData->data[(k-PartPointer)*InputData->sizeX*InputData->sizeY+InputData->sizeX*i+j] = img.at <uchar> (i,j);
			 }
			}
		   }
		  

		  if (PartPointer+InputData->sizeZ>=StrFiles->size()) 
		  {
			 this->Button_NextPart->Enabled = false;
		  }
		  if (!this->Button_PreviousPart->Enabled &&(StrFiles->size()>InputData->sizeZ)) 
			  this->Button_PreviousPart->Enabled = true;

		  GenerateTextures();
		 }
private: System::Void Button_PreviousPart_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
		  delete [] InputData->data;

		  PartPointer-=InputData->sizeZ;

		  InputData->data = new short [InputData->TotalSize];
	      
		  delete [] VoxelsData; VoxelsData = new TVoxel [InputData->TotalSize];

		  for (int k = PartPointer; k<PartPointer+InputData->sizeZ; ++k)
 		  {
		   {
		     System::String^ fullpath = FolderPath + "\\" + gcnew System::String(StrFiles->at(k));
		     Mat img = imread((char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(fullpath).ToPointer()), 0);
			 
			 int type_value = img.type();

			 for (int i = 0; i < img.rows; ++i)
  			 for (int j = 0; j < img.cols; ++j)
			 {
			  InputData->data[(k-PartPointer)*InputData->sizeX*InputData->sizeY+InputData->sizeX*i+j] = img.at <uchar> (i,j);
			 }
			}
		   }
		  

		  if (PartPointer==0) 
		  {
			  this->Button_PreviousPart->Enabled = false;
		  }

		  if (!this->Button_NextPart->Enabled && (StrFiles->size()>InputData->sizeZ)) 
			  this->Button_NextPart->Enabled = true;

		  GenerateTextures();
		 }
private: System::Void TabControlOptions_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (TabControlOptions->SelectedIndex == 2)
			 {
			 /* int AIV, BIV, GIV, SITERS;		//��������� ������ ��� ���������
			  int LXmod=0, LYmod=0;				//������ ��� �� ����
			  Mat window;						//������� ����� �����������
			  Mat src_1ch, src_3ch, fake_src_3ch;
			  char* winName=L"����� ��������� �������";
			  char Pressed;						//������� �������
			  Texture2D *img;					//
			  TextureData2D *data; 
			  ShaderSnake *Snake;

			  glfwInit();
			  glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, 1);
			  if(!glfwOpenWindow(1, 1, 0, 0, 0, 0, 16, 0, GLFW_WINDOW))
			  {
				 glfwTerminate(); 
				 MessageBoxA(0, lpszGlfwError, "Fatal", 0);
				 return 1;
				}
				glfwSetWindowTitle("Output window");

				AIV=(int)SnakeParameters->alpha;
				BIV=(int)SnakeParameters->beta;
				GIV=(int)SnakeParameters->gamma;
				SITERS=30;

				
				while (1)
			    {
				 src_3ch=imread(fnames[0],1);

				 //������� ���� ���������
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


		//������������� ��������� �����������
		//�� ������ �� ����������� Nick Govier � David Newman
		//�������� ��������� ������������� �����������
		//�.�. �������� ����� ������������ � ����
		//	preprocess(src_1ch);

		//������ ���������
		Parameters params(fake_src_3ch, "Source", window, LXmod, LYmod, numfiles, fnames);
		refresh(params);

		//���������� �������� �����������
		namedWindow("Source",CV_GUI_EXPANDED|CV_WINDOW_NORMAL|CV_WINDOW_KEEPRATIO);
		setWindowProperty("Source",CV_WND_PROP_FULLSCREEN,CV_WINDOW_FULLSCREEN);
		imshow("Source", params.wind);

		//������ �� �������� �����
		cvSetMouseCallback(winName, onMouseEvent, &params);

		attachTB("Source", AIV, BIV, GIV, SITERS);
		attachSliders("Source", params);

		//���� �����
		Pressed=-1;
		while ((Pressed!=27)&&(Pressed!=13))
			Pressed=waitKey(0);
		if(Pressed==27)
			break;

		//������� ������
		src_3ch.~Mat();
		fake_src_3ch.~Mat();

		for (int l=0; l < paramsnumslices; l++)
		{

			src_1ch=imread(params.FNames[l], 0);
			preprocess(src_1ch);
			//�������� ������������������
			data = new TextureData2D(src_1ch.cols, src_1ch.rows, 3);
			if (!data)
			{
				MessageBoxA(0, lpszInternalError, "Error", 0);
				return 1;
			}

			//�������� ����������� � ���������� ������
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
			//���������� �������� ������
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

			//������� �����
			Texture2D* ResPoints=Snake->Output();
			int ResSize=Snake->GetSize();
			vector <Point> OCV_ResPoints;

			for (int i=0; i<ResSize; i++)
			{
				Vector3D temp;
				temp=ResPoints->Data->Pixel<Vector3D>(i,0);
				OCV_ResPoints.push_back(Point(temp.X, abs(temp.Y-src_1ch.rows+1)));
			}
			//���������� �� ����������
			params.ResPoints.push_back(OCV_ResPoints);
			//�������� ������������������
			src_1ch.~Mat();
			Snake->~ShaderSnake();
			data->~TextureData2D();
			img->~Texture2D();
			//			ResPoints=NULL;
		}

		//�����
		attachSliceSlider("Source", params);

		params.image=imread(params.FNames[0], 1);
		for (int i=0; i<params.ResPoints[0].size(); i++)
		{
			circle(params.image, params.ResPoints[0][i], 3, Scalar(0, 0, 255), -1);
		}
		refresh(params);
		imshow("Source", params.wind);

		//���� �����
		Pressed=-1;
		while ((Pressed!=27)&&(Pressed!=13))
			Pressed=waitKey(0);
		if(Pressed==27)
			break;

		params.ResPoints.clear();
		params.image.~Mat();
		LXmod=params.Xmod;
		LYmod=params.Ymod;
			 }gluUnProject(winx, winy, winz,*/
		 }

		  
			 }


			 
private: System::Void RenderingPanel_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if (e->Button==Windows::Forms::MouseButtons::Right)
			 {
			  float distance_z;

			  /* ������� ��������� ����� ��� �������� */
			  int mouse_x=e->Location.X, mouse_y=e->Location.Y;
			  GLint viewport[4];								// ��������� viewport-a
			  GLdouble projection [16];							// ������� ��������
			  GLdouble modelview [16];							// ������� �������
			  GLfloat vx, vy, vz;								// ���������� ������� ���� � ������� ��������� viewport-a.
			  GLdouble wx, wy, wz;								// ������������ ������� ����������.
			  glGetIntegerv(GL_VIEWPORT,viewport);				// ����� ��������� viewport-a.
			  glGetDoublev(GL_PROJECTION_MATRIX,projection);	// ����� ������� ��������.
			  glGetDoublev(GL_MODELVIEW_MATRIX,modelview);		// ����� ������� �������.
					 
			  vx=RenderingPanel->Width-mouse_x-1+viewport[0];
					
			  vy=mouse_y+viewport[1];
			  glReadPixels(vx, vy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &vz);
			  float p1[3], p2[3];
			  p1[0]=vx;
			  p1[1]=vy;
			  p1[2]=vz;
			  gluUnProject(vx, vy, (GLdouble)vz, modelview, projection, viewport, &wx, &wy, &wz);
			  //������ ��� ��� ���������� ����� ����������� � ��������� � ������������
			  p2[0]=wx;
			  p2[1]=wy;
			  p2[2]=wz;
			  //��������� � �������� �������

			  // ���� ������������ ������� �� ��������
			  float width = (float)(InputData->sizeX / 2) * InputData->scaleX;
			  float height = (float)(InputData->sizeY / 2) * InputData->scaleY;

			  float dx,dy;
			  dx=wy+height+2*distance_x;
			  dy=wx+width-2*distance_y;

			  int xvox, yvox;
			  xvox=dx/InputData->scaleX;
			  yvox=dy/InputData->scaleY;

			  //������ �� ��������
			  float *texel=new float [4];
			  texel[0]=1.0f;
			  texel[1]=0.0f;
			  texel[2]=0.0f;
			  texel[3]=1.0f;
			  glBindTexture(GL_TEXTURE_2D, layerTextures[TrackBar_Layers->Value]);
			  glTexSubImage2D(GL_TEXTURE_2D, 0, xvox, yvox, 1, 1, GL_RGBA, GL_FLOAT, texel);
			 }
		 }
private: System::Void ButtonLocalize_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
       	  GenerateTextures();

		  ShaderSnake *Snake=new ShaderSnake();
	  	  TSnakeParameters sparams;
		  sparams.alpha=TrackBar_AlphaValue->Value;
		  sparams.beta=TrackBar_BetaValue->Value;
		  sparams.gamma=TrackBar_GammaValue->Value;
		  sparams.iterations_number=TrackBar_IterationsNumber->Value;
		  RenderTimer->Stop();
		  Snake->Init();
		  Snake->GetReady();

		  Snake->FixParams(layerTextures[TrackBar_Layers->Value], 512,512,4, sparams);
				 
		  Snake->AddSeed(Int32::Parse(XPoint_TextBox->Text), Int32::Parse(YPoint_TextBox->Text));

		  for (int i=0; i<TrackBar_IterationsNumber->Value; i++) Snake->Iterate();

		  //������� �����
		  Texture2D* ResPoints=Snake->Output();
		  int ResSize=Snake->GetSize();
  		  int *IResPoints=new int [ResSize*2];

		  for (int i=0; i<ResSize; i++)
		  {
		   Vector4D temp=ResPoints->Data->Pixel<Vector4D>(i,0);
		   IResPoints[2*i]=temp.X;
		   IResPoints[2*i+1]=temp.Y;
		  }
		  
		  ResPoints=NULL;
		  Snake->~ShaderSnake();
		  //������� �� ��������
		  float *tex=new float [4];

		  tex[0]=0.0;
		  tex[1]=1.0;
		  tex[2]=0.0;
		  tex[3]=1.0;

		  glBindTexture(GL_TEXTURE_2D,layerTextures[TrackBar_Layers->Value]);
		  for (int i=0; i<ResSize; i++)
		  glTexSubImage2D(GL_TEXTURE_2D,0, IResPoints[2*i], IResPoints[2*i+1], 1,1, GL_RGBA, GL_FLOAT, tex);
		  delete [] IResPoints; IResPoints=NULL;
		  RenderTimer->Start();
		 }
private: System::Void TrackBar_ScaleX_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  this->TextBox_ScaleX->Text = ((float)this->TrackBar_ScaleX->Value/this->TrackBar_ScaleX->Maximum).ToString();
		 }
private: System::Void TrackBar_ScaleY_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  this->TextBox_ScaleY->Text = ((float)this->TrackBar_ScaleY->Value/this->TrackBar_ScaleY->Maximum).ToString();
		 }
private: System::Void TrackBar_ScaleZ_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  this->TextBox_ScaleZ->Text = ((float)this->TrackBar_ScaleZ->Value/this->TrackBar_ScaleZ->Maximum).ToString();
		 }
private: System::Void TextBox_ScaleX_TextChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  InputData->scaleX = Double::Parse(this->TextBox_ScaleX->Text);
		 }
private: System::Void TextBox_ScaleY_TextChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  InputData->scaleY = Double::Parse(this->TextBox_ScaleY->Text);
		 }
private: System::Void TextBox_ScaleZ_TextChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  InputData->scaleZ = Double::Parse(this->TextBox_ScaleZ->Text);
		 }
private: System::Void CheckBox_Reconstruction_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  this->CheckBox_Gradients->Checked = !this->CheckBox_Reconstruction->Checked&&this->CheckBox_Gradients->Checked;
		  this->CheckBox_Localization->Checked = !this->CheckBox_Reconstruction->Checked&&this->CheckBox_Localization->Checked;
		  this->CheckBoxClusters->Checked = !this->CheckBox_Reconstruction->Checked&&this->CheckBoxClusters->Checked;
		 }
private: System::Void CheckBox_Gradients_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  this->CheckBox_Localization->Checked = !this->CheckBox_Gradients->Checked&&this->CheckBox_Localization->Checked;
          this->CheckBoxClusters->Checked = !this->CheckBox_Gradients->Checked&&this->CheckBoxClusters->Checked;
		  this->CheckBox_Reconstruction->Checked = !this->CheckBox_Gradients->Checked&&this->CheckBox_Reconstruction->Checked;
		 }
private: System::Void CheckBox_Localization_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		  this->CheckBox_Gradients->Checked = !this->CheckBox_Localization->Checked&&this->CheckBox_Gradients->Checked;
          this->CheckBoxClusters->Checked = !this->CheckBox_Localization->Checked&&this->CheckBoxClusters->Checked;
		  this->CheckBox_Reconstruction->Checked = !this->CheckBox_Localization->Checked&&this->CheckBox_Reconstruction->Checked;
		 }
};
}

