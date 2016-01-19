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
	public /*ref*/ class MyForm //: public System::Windows::Forms::Form
	{
	public:
		MyForm(/*void*/)
		{

			/*this->components = gcnew System::ComponentModel::Container();
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

			InitTrackBars();*/
		}
		int DetermenGesture(std::string windowName, cv::Mat& srcBGR);
		int main_photo();
		int main_video();
		/*void SetText(String^ str) {
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
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::TrackBar^  trackBar2;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TrackBar^  trackBar3;
	private: System::Windows::Forms::TrackBar^  trackBar4;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::TrackBar^  trackBar5;
	private: System::Windows::Forms::TrackBar^  trackBar6;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::TrackBar^  trackBar7;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::TrackBar^  trackBar8;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::Label^  label21;
			 /// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

	void TrackBar1_ValueChanged(Object^ sender, EventArgs^ e) {
		label11->Text = trackBar1->Value.ToString();
	}
	void TrackBar2_ValueChanged(Object^ sender, EventArgs^ e) {
		label12->Text = trackBar2->Value.ToString();
	}
	void TrackBar3_ValueChanged(Object^ sender, EventArgs^ e) {
		label13->Text = trackBar3->Value.ToString();
	}
	void TrackBar4_ValueChanged(Object^ sender, EventArgs^ e) {
		label14->Text = trackBar4->Value.ToString();
	}
	void TrackBar5_ValueChanged(Object^ sender, EventArgs^ e) {
		label15->Text = trackBar5->Value.ToString();
	}
	void TrackBar6_ValueChanged(Object^ sender, EventArgs^ e) {
		label16->Text = trackBar6->Value.ToString();
	}
	void TrackBar7_ValueChanged(Object^ sender, EventArgs^ e) {
		label17->Text = trackBar7->Value.ToString();
	}
	void TrackBar8_ValueChanged(Object^ sender, EventArgs^ e) {
		label18->Text = trackBar8->Value.ToString();
	}

	void InitTrackBars() {
		trackBar1->ValueChanged += gcnew System::EventHandler(this, &MyForm::TrackBar1_ValueChanged);
		this->Controls->Add(this->trackBar1);
		trackBar2->ValueChanged += gcnew System::EventHandler(this, &MyForm::TrackBar2_ValueChanged);
		this->Controls->Add(this->trackBar2);
		trackBar3->ValueChanged += gcnew System::EventHandler(this, &MyForm::TrackBar3_ValueChanged);
		this->Controls->Add(this->trackBar3);
		trackBar4->ValueChanged += gcnew System::EventHandler(this, &MyForm::TrackBar4_ValueChanged);
		this->Controls->Add(this->trackBar4);
		trackBar5->ValueChanged += gcnew System::EventHandler(this, &MyForm::TrackBar5_ValueChanged);
		this->Controls->Add(this->trackBar5);
		trackBar6->ValueChanged += gcnew System::EventHandler(this, &MyForm::TrackBar6_ValueChanged);
		this->Controls->Add(this->trackBar6);
		trackBar7->ValueChanged += gcnew System::EventHandler(this, &MyForm::TrackBar7_ValueChanged);
		this->Controls->Add(this->trackBar7);
		trackBar8->ValueChanged += gcnew System::EventHandler(this, &MyForm::TrackBar8_ValueChanged);
		this->Controls->Add(this->trackBar8);
	}
	delegate int GetBarDelegate();
		
	int GetBar1() {
		return trackBar1->Value;
	}
	int GetBar2() {
		return trackBar2->Value;
	}
	int GetBar3() {
		return trackBar3->Value;
	}
	int GetBar4() {
		return trackBar4->Value;
	}
	int GetBar5() {
		return trackBar5->Value;
	}
	int GetBar6() {
		return trackBar6->Value;
	}
	int GetBar7() {
		return trackBar7->Value;
	}
	int GetBar8() {
		return trackBar8->Value;
	}


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
	private: System::Void label5_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
	}*/
};
}
