#include <vector>
#include <CL/opencl.h>
#include "ScanData.h"

using namespace System::ComponentModel;

#define MinGroupSize 1
#define MaxGroupSize 330

class KmeansMethod									// ������� ����� ��� ������ k-�������
{
 protected:
		   size_t DataSize;							// ������ ������ ��� �������������
		   size_t NClusters;						// ������������ ����� ��������� 
		   size_t CountIterations;					// ���������� �������� ������
			
		   ScanData* InputData;						// �������� ������ ��� ������������� 
		   float* means;							// ������ ������� ��������� 
		   std::vector <size_t>* Clusters;			// ��������, ����� �������� �������������� ������ 
		
		   float time;								// ����� ������ ���������

		   float GetMean(size_t i);					// ���������� ������ ������� ��� i-��� ��������
		   void ReleaseResources();					// ������������ ���������� ��������

 public:
		// ������������ � ����������
		KmeansMethod(ScanData* DataValue);
		KmeansMethod(Layer layer);
		~KmeansMethod();
	 
		void SetClustersNumber(size_t value);		// ���������� ����� ���������
		void SetCountIterations(size_t value);		// ���������� ����� ��������
		
		// ��������� ��������� �� ���������
		virtual std::vector <std::vector <size_t> > GetClusters(BackgroundWorker^ worker);
		// ��������� ������� ������ ���������
		float GetExecutionTime();				
};

class KmeansOpenCLMethod : public KmeansMethod						// ����� k-������� �� GPU (OpenCL)
{
 protected: 
		   size_t* Labels;											// ����� ��������� 
		   cl_device_id device;										// ���������� ����������
		   cl_context context;										// �������� ����������
		   cl_command_queue queue;									// ������� ������

           void InitOpenCL();										// ������������� ��������� OpenCL
		   void CalcWorkGroupSize(cl_kernel kernel, size_t &group);	// ���������� ������� ������� ������
 public:
		//������������ 
		KmeansOpenCLMethod(ScanData* DataValue); 
		KmeansOpenCLMethod(Layer layer);		 
	
		//������������� ������� k-������� � �������������� OpenCL
		std::vector <std::vector <size_t> > GetClusters(BackgroundWorker^ worker);	
};

class KmeansConditionalMethod: public KmeansMethod			// ������������� ������� k-������� � �������� ���������
{
 private:
		 short MinDensityRange, MaxDensityRange;			// ������� ���������
		 std::vector <size_t> VoxelsInRange;				// ������ �������� �������� � ���������� �� ��������� 

 public:
		// ������������
		KmeansConditionalMethod(ScanData* DataValue, short LowBorder, short HighBorder);
		KmeansConditionalMethod(Layer layer, short LowBorder, short HighBorder);

	    void SetRangesOfDensity(short left, short right);	// ���������� �������� �������� ���������

		// �������� ������������� ������� k-������� 
        std::vector <std::vector <size_t> > GetClusters(BackgroundWorker^ worker);	
};

class KmeansConditionalOpenCLMethod: public KmeansOpenCLMethod   // ������������� ������� k-������� � �������� ��������� �� OpenCL
{
 private: 
		 short MinDensityRange, MaxDensityRange;				 // ������� ���������
		 std::vector <size_t> VoxelsInRange;					 // ������ �������� �������� � ���������� �� ��������� 
	 
 public:
	    // ������������
	    KmeansConditionalOpenCLMethod(ScanData* DataValue, short LowBorder, short HighBorder);
	    KmeansConditionalOpenCLMethod(Layer layer, short LowBorder, short HighBorder);

		void SetRangesOfDensity(short left, short right);		 // ���������� �������� �������� ���������

		// �������� ������������� ������� k-������� �� OpenCL
	    std::vector <std::vector <size_t> > GetClusters(BackgroundWorker^ worker);		
}; 