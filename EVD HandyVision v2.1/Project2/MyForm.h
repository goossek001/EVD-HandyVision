#pragma once

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>

namespace ASDF {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{

			this->components = gcnew System::ComponentModel::Container();
			this->Size = System::Drawing::Size(300, 300);
			this->Text = L"MyForm";
			this->Padding = System::Windows::Forms::Padding(0);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;


			backgroundWorker = gcnew System::ComponentModel::BackgroundWorker;

			backgroundWorker->DoWork += gcnew DoWorkEventHandler(this, &MyForm::DoWork);
			backgroundWorker->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler(this, &MyForm::RunWorkerCompleted);
			backgroundWorker->ProgressChanged += gcnew ProgressChangedEventHandler(this, &MyForm::ProgressChanged);

			backgroundWorker->RunWorkerAsync();

			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		int DetermenGesture(std::string windowName, cv::Mat& srcBGR);
		int main_photo();
		int main_video();
		void SetText(String^ str) {
			label1->Text = str;
			this->label1->Font = (gcnew System::Drawing::Font(this->label1->Font->FontFamily, 20, this->label1->Font->Style, this->label1->Font->Unit,
				this->label1->Font->GdiCharSet));
		}
		delegate void SetTextDelegate(String^ str);
		void SetImage(Bitmap^ img) {
			panel1->BackgroundImage = img;
		}
		delegate void SetImageDelegate(Bitmap^ img);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm();

		virtual void DoWork(Object^ sender, DoWorkEventArgs^ e);
		void RunWorkerCompleted(Object^, RunWorkerCompletedEventArgs^ e);
		void ProgressChanged(Object^, ProgressChangedEventArgs^ e);

	private:
		BackgroundWorker^ backgroundWorker;

	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label1;
			 /// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void);
#pragma endregion
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {

	}
	private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
					
	}
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	};
}
