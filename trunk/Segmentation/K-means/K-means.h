#include <vector>
#include <CL/opencl.h>
#include "ScanData.h"

using namespace System::ComponentModel;

#define MinGroupSize 1
#define MaxGroupSize 330

class KmeansMethod									// базовый класс для метода k-средних
{
 protected:
		   size_t DataSize;							// размер данных для кластеризации
		   size_t NClusters;						// максимальное число кластеров 
		   size_t CountIterations;					// количество итераций метода
			
		   ScanData* InputData;						// исходные данные для кластеризации 
		   float* means;							// центры тяжести кластеров 
		   std::vector <size_t>* Clusters;			// кластеры, между которыми распределяются данные 
		
		   float time;								// время работы алгоритма

		   float GetMean(size_t i);					// вычисление центра тяжести для i-ого кластера
		   void ReleaseResources();					// освобождение занимаемых ресурсов

 public:
		// Конструкторы и деструктор
		KmeansMethod(ScanData* DataValue);
		KmeansMethod(Layer layer);
		~KmeansMethod();
	 
		void SetClustersNumber(size_t value);		// установить число кластеров
		void SetCountIterations(size_t value);		// установить число итераций
		
		// Получение кластеров по алгоритму
		virtual std::vector <std::vector <size_t> > GetClusters(BackgroundWorker^ worker);
		// Получение времени работы алгоритма
		float GetExecutionTime();				
};

class KmeansOpenCLMethod : public KmeansMethod						// метод k-средних на GPU (OpenCL)
{
 protected: 
		   size_t* Labels;											// метки кластеров 
		   cl_device_id device;										// устройство исполнения
		   cl_context context;										// контекст устройства
		   cl_command_queue queue;									// очередь команд

           void InitOpenCL();										// Инициализация контекста OpenCL
		   void CalcWorkGroupSize(cl_kernel kernel, size_t &group);	// Вычисление размера рабочей группы
 public:
		//Конструкторы 
		KmeansOpenCLMethod(ScanData* DataValue); 
		KmeansOpenCLMethod(Layer layer);		 
	
		//Кластеризация методом k-средних с использованием OpenCL
		std::vector <std::vector <size_t> > GetClusters(BackgroundWorker^ worker);	
};

class KmeansConditionalMethod: public KmeansMethod			// кластеризация методом k-средних в заданном диапазоне
{
 private:
		 short MinDensityRange, MaxDensityRange;			// границы диапазона
		 std::vector <size_t> VoxelsInRange;				// вектор индексов вокселов с плотностью из диапазона 

 public:
		// Конструкторы
		KmeansConditionalMethod(ScanData* DataValue, short LowBorder, short HighBorder);
		KmeansConditionalMethod(Layer layer, short LowBorder, short HighBorder);

	    void SetRangesOfDensity(short left, short right);	// установить диапазон значений плотности

		// Условная кластеризация методом k-средних 
        std::vector <std::vector <size_t> > GetClusters(BackgroundWorker^ worker);	
};

class KmeansConditionalOpenCLMethod: public KmeansOpenCLMethod   // кластеризация методом k-средних в заданном диапазоне на OpenCL
{
 private: 
		 short MinDensityRange, MaxDensityRange;				 // границы диапазона
		 std::vector <size_t> VoxelsInRange;					 // вектор индексов вокселов с плотностью из диапазона 
	 
 public:
	    // Конструкторы
	    KmeansConditionalOpenCLMethod(ScanData* DataValue, short LowBorder, short HighBorder);
	    KmeansConditionalOpenCLMethod(Layer layer, short LowBorder, short HighBorder);

		void SetRangesOfDensity(short left, short right);		 // установить диапазон значений плотности

		// Условная кластеризация методом k-средних на OpenCL
	    std::vector <std::vector <size_t> > GetClusters(BackgroundWorker^ worker);		
}; 