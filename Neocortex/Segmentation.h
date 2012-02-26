#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace Neocortex {

	/// <summary>
	/// Summary for SegmentationProperties
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class SegmentationProperties : public System::Windows::Forms::Form
	{
	public:
		SegmentationProperties(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			IsDensityCoords = false;
			IsClustersCoords = false;
			IsMatMorphology = false;
			IsConnectedRegions = false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SegmentationProperties()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::CheckBox^  CheckBox_DensityCoords;
	public: System::Windows::Forms::CheckBox^  CheckBox_ClustersCoords;
	public: System::Windows::Forms::CheckBox^  CheckBox_MatMorphology;
	public: System::Windows::Forms::CheckBox^  CheckBox_ConnectedRegions;

	public: bool IsDensityCoords;		// учитывать/не учитывать координаты при сегментации по плотности
	public: bool IsClustersCoords;		// кластеризовать/не кластеризовать найденные сегменты по координатам
	public: bool IsMatMorphology;		// использовать/не использовать морфологическую фильтрацию сегментов
	public: bool IsConnectedRegions;	// искать/не искать связные области в сегментах

	private: System::Windows::Forms::Button^  Button_Accept;
	private: System::Windows::Forms::Button^  Button_Cancel;

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->CheckBox_DensityCoords = (gcnew System::Windows::Forms::CheckBox());
			this->CheckBox_ClustersCoords = (gcnew System::Windows::Forms::CheckBox());
			this->CheckBox_MatMorphology = (gcnew System::Windows::Forms::CheckBox());
			this->CheckBox_ConnectedRegions = (gcnew System::Windows::Forms::CheckBox());
			this->Button_Accept = (gcnew System::Windows::Forms::Button());
			this->Button_Cancel = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// CheckBox_DensityCoords
			// 
			this->CheckBox_DensityCoords->AutoSize = true;
			this->CheckBox_DensityCoords->Location = System::Drawing::Point(51, 29);
			this->CheckBox_DensityCoords->Name = L"CheckBox_DensityCoords";
			this->CheckBox_DensityCoords->Size = System::Drawing::Size(301, 21);
			this->CheckBox_DensityCoords->TabIndex = 0;
			this->CheckBox_DensityCoords->Text = L"Учитывать координаты при сегментации";
			this->CheckBox_DensityCoords->UseVisualStyleBackColor = true;
			// 
			// CheckBox_ClustersCoords
			// 
			this->CheckBox_ClustersCoords->AutoSize = true;
			this->CheckBox_ClustersCoords->Location = System::Drawing::Point(51, 68);
			this->CheckBox_ClustersCoords->Name = L"CheckBox_ClustersCoords";
			this->CheckBox_ClustersCoords->Size = System::Drawing::Size(315, 21);
			this->CheckBox_ClustersCoords->TabIndex = 1;
			this->CheckBox_ClustersCoords->Text = L"Сегментировать кластеры по координатам";
			this->CheckBox_ClustersCoords->UseVisualStyleBackColor = true;
			// 
			// CheckBox_MatMorphology
			// 
			this->CheckBox_MatMorphology->AutoSize = true;
			this->CheckBox_MatMorphology->Location = System::Drawing::Point(51, 108);
			this->CheckBox_MatMorphology->Name = L"CheckBox_MatMorphology";
			this->CheckBox_MatMorphology->Size = System::Drawing::Size(307, 21);
			this->CheckBox_MatMorphology->TabIndex = 2;
			this->CheckBox_MatMorphology->Text = L"Морфологическая фильтрация сегментов";
			this->CheckBox_MatMorphology->UseVisualStyleBackColor = true;
			// 
			// CheckBox_ConnectedRegions
			// 
			this->CheckBox_ConnectedRegions->AutoSize = true;
			this->CheckBox_ConnectedRegions->Location = System::Drawing::Point(51, 149);
			this->CheckBox_ConnectedRegions->Name = L"CheckBox_ConnectedRegions";
			this->CheckBox_ConnectedRegions->Size = System::Drawing::Size(274, 21);
			this->CheckBox_ConnectedRegions->TabIndex = 3;
			this->CheckBox_ConnectedRegions->Text = L"Поиск связных областей в сегментах";
			this->CheckBox_ConnectedRegions->UseVisualStyleBackColor = true;
			// 
			// Button_Accept
			// 
			this->Button_Accept->Location = System::Drawing::Point(83, 198);
			this->Button_Accept->Name = L"Button_Accept";
			this->Button_Accept->Size = System::Drawing::Size(90, 38);
			this->Button_Accept->TabIndex = 4;
			this->Button_Accept->Text = L"Применить";
			this->Button_Accept->UseVisualStyleBackColor = true;
			this->Button_Accept->Click += gcnew System::EventHandler(this, &SegmentationProperties::Button_Accept_Click);
			// 
			// Button_Cancel
			// 
			this->Button_Cancel->Location = System::Drawing::Point(229, 198);
			this->Button_Cancel->Name = L"Button_Cancel";
			this->Button_Cancel->Size = System::Drawing::Size(90, 38);
			this->Button_Cancel->TabIndex = 5;
			this->Button_Cancel->Text = L"Отмена";
			this->Button_Cancel->UseVisualStyleBackColor = true;
			this->Button_Cancel->Click += gcnew System::EventHandler(this, &SegmentationProperties::Button_Cancel_Click);
			// 
			// SegmentationProperties
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(414, 260);
			this->Controls->Add(this->Button_Cancel);
			this->Controls->Add(this->Button_Accept);
			this->Controls->Add(this->CheckBox_ConnectedRegions);
			this->Controls->Add(this->CheckBox_MatMorphology);
			this->Controls->Add(this->CheckBox_ClustersCoords);
			this->Controls->Add(this->CheckBox_DensityCoords);
			this->MaximumSize = System::Drawing::Size(422, 300);
			this->MinimumSize = System::Drawing::Size(422, 300);
			this->Name = L"SegmentationProperties";
			this->Text = L"Neocortex - Настройки сегментации";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &SegmentationProperties::SegmentationProperties_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Button_Accept_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 IsDensityCoords = this->CheckBox_DensityCoords->Checked;
				 IsClustersCoords = this->CheckBox_ClustersCoords->Checked;
				 IsMatMorphology = this->CheckBox_MatMorphology->Checked;
				 IsConnectedRegions = this->CheckBox_ConnectedRegions->Checked;
				 Close();
			 }
private: System::Void SegmentationProperties_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
		 {
			 this->CheckBox_DensityCoords->Checked = IsDensityCoords;
			 this->CheckBox_ClustersCoords->Checked = IsClustersCoords;
			 this->CheckBox_MatMorphology->Checked = IsMatMorphology;
			 this->CheckBox_ConnectedRegions->Checked = IsConnectedRegions;
			 e->Cancel = true;
			 Hide();
		 }
private: System::Void Button_Cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 Close();
		 }
};
}
