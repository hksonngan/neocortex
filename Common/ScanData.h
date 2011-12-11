#pragma once

struct Layer  // слой данных (двумерное изображение)
{ 
 short *data;
 size_t sizeX, sizeY;  // ширина и высота слоя
 float scaleX, scaleY; // размеры слоя по осям

 // Конструкторы
 Layer() : data(0), sizeX(0), sizeY(0), scaleX(0), scaleY(0) {};
 Layer(short*_data, size_t _sizeX, size_t _sizeY, float _scaleX, float _scaleY): 
 data(_data), sizeX(_sizeX), sizeY(_sizeY), scaleX(_scaleX), scaleY(_scaleY) {};

 short GetDistination(size_t x1, size_t y1, size_t x2, size_t y2);
};

class ScanData 
{
 public:
		short *data;
		size_t sizeX, sizeY, sizeZ;
		float scaleX, scaleY, scaleZ;

		size_t TotalSize;

		ScanData();
		ScanData(const char *pathToBinFile);
		ScanData(Layer layer);
		~ScanData();

		void ReleaseResources();

		bool LoadData(const char *pathToBinFile);

		bool LoadData(const char *pathToBinFile, size_t z_start, size_t nLayers);

		ScanData* GetSubData(size_t z_first, size_t z_last);

		Layer GetLayer(size_t z); 
		// возвращает слой с данными от всего массива, при изменении этого слоя, изменятся и основные данные
		short& GetDensity(size_t x, size_t y, size_t z);
		short& GetDensity(Layer &layer, size_t x, size_t y);

		double GetDistination(size_t x1, size_t y1, size_t z1, size_t x2, size_t y2, size_t z2);

		
		
};

namespace surface_reconstruction {

	public delegate void SetScanData(ScanData* sData);
	public delegate void SetIndexVoxel(size_t x, size_t y, size_t z);
}