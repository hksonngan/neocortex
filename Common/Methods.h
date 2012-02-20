#include "Structures.h"

System::String^ GetStringOfColor(TColor Color);																			// �������� ��������� ������������� �����

cv::Point_<short> MinMaxDensityOfSegment_2D(TVoxelsData* Data, size_t LayerIndex, int SegmentIndex);					// ����������� � ������������ �������� ��������� 2D-��������
cv::Point_<short> MinMaxDensityOfSegment_3D(TVoxelsData* Data, size_t StartLayerIndex, int SegmentIndex);				// ����������� � ������������ �������� ��������� 3D-��������
cv::Point2f DensityMeanDevOfSegment_2D(TVoxelsData* Data, size_t LayerIndex, int SegmentIndex);							// ���. �������� � ��������� ��������� �������� � 2D-��������
cv::Vec4f MeanDevXYOfSegment_2D(TVoxelsData* Data, size_t LayerIndex, int SegmentIndex);								// ���. �������� � ��������� ��������� �������� � 2D-��������

void SegmentsSort(const TVoxelsData* Data, std::vector <TSegment>& Segments, size_t LayerIndex, int left, int right);	// ���������� ��������� �� ����������� ���������� �������� ���������
