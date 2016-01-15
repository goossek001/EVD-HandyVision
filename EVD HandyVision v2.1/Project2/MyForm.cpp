#include "MyForm.h"

#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"
#include "Math.h"

#include "VisionOperators.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace ASDF;

void MyForm::DoWork(Object^ sender, DoWorkEventArgs^ e) {
	main_video();
}

void MyForm::RunWorkerCompleted(Object^, RunWorkerCompletedEventArgs^ e) {
	
}

void MyForm::ProgressChanged(Object^, ProgressChangedEventArgs^ e) {
	
}

void MyForm::InitializeComponent(void) {
	this->panel1 = (gcnew System::Windows::Forms::Panel());
	this->label1 = (gcnew System::Windows::Forms::Label());
	this->label2 = (gcnew System::Windows::Forms::Label());
	this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
	this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
	this->label5 = (gcnew System::Windows::Forms::Label());
	this->label6 = (gcnew System::Windows::Forms::Label());
	this->label3 = (gcnew System::Windows::Forms::Label());
	this->label4 = (gcnew System::Windows::Forms::Label());
	this->trackBar3 = (gcnew System::Windows::Forms::TrackBar());
	this->trackBar4 = (gcnew System::Windows::Forms::TrackBar());
	this->label7 = (gcnew System::Windows::Forms::Label());
	this->label8 = (gcnew System::Windows::Forms::Label());
	this->label9 = (gcnew System::Windows::Forms::Label());
	this->trackBar5 = (gcnew System::Windows::Forms::TrackBar());
	this->trackBar6 = (gcnew System::Windows::Forms::TrackBar());
	this->label10 = (gcnew System::Windows::Forms::Label());
	this->label11 = (gcnew System::Windows::Forms::Label());
	this->label12 = (gcnew System::Windows::Forms::Label());
	this->label13 = (gcnew System::Windows::Forms::Label());
	this->label14 = (gcnew System::Windows::Forms::Label());
	this->label15 = (gcnew System::Windows::Forms::Label());
	this->label16 = (gcnew System::Windows::Forms::Label());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar3))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar4))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar5))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar6))->BeginInit();
	this->SuspendLayout();
	// 
	// panel1
	// 
	this->panel1->Location = System::Drawing::Point(12, 12);
	this->panel1->Name = L"panel1";
	this->panel1->Size = System::Drawing::Size(623, 462);
	this->panel1->TabIndex = 1;
	this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::panel1_Paint);
	// 
	// label1
	// 
	this->label1->AutoSize = true;
	this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label1->Location = System::Drawing::Point(245, 489);
	this->label1->Name = L"label1";
	this->label1->Size = System::Drawing::Size(109, 39);
	this->label1->TabIndex = 2;
	this->label1->Text = L"label1";
	this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
	// 
	// label2
	// 
	this->label2->AutoSize = true;
	this->label2->Location = System::Drawing::Point(641, 76);
	this->label2->Name = L"label2";
	this->label2->Size = System::Drawing::Size(18, 17);
	this->label2->TabIndex = 3;
	this->label2->Text = L"H";
	this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click);
	// 
	// trackBar1
	// 
	this->trackBar1->Location = System::Drawing::Point(747, 99);
	this->trackBar1->Maximum = 255;
	this->trackBar1->Name = L"trackBar1";
	this->trackBar1->Size = System::Drawing::Size(408, 56);
	this->trackBar1->TabIndex = 7;
	this->trackBar1->Value = 51;
	// 
	// trackBar2
	// 
	this->trackBar2->Location = System::Drawing::Point(747, 37);
	this->trackBar2->Maximum = 255;
	this->trackBar2->Name = L"trackBar2";
	this->trackBar2->Size = System::Drawing::Size(408, 56);
	this->trackBar2->TabIndex = 8;
	this->trackBar2->Value = 229;
	// 
	// label5
	// 
	this->label5->AutoSize = true;
	this->label5->Location = System::Drawing::Point(683, 37);
	this->label5->Name = L"label5";
	this->label5->Size = System::Drawing::Size(30, 17);
	this->label5->TabIndex = 9;
	this->label5->Text = L"min";
	this->label5->Click += gcnew System::EventHandler(this, &MyForm::label5_Click);
	// 
	// label6
	// 
	this->label6->AutoSize = true;
	this->label6->Location = System::Drawing::Point(683, 99);
	this->label6->Name = L"label6";
	this->label6->Size = System::Drawing::Size(33, 17);
	this->label6->TabIndex = 10;
	this->label6->Text = L"max";
	// 
	// label3
	// 
	this->label3->AutoSize = true;
	this->label3->Location = System::Drawing::Point(683, 247);
	this->label3->Name = L"label3";
	this->label3->Size = System::Drawing::Size(33, 17);
	this->label3->TabIndex = 15;
	this->label3->Text = L"max";
	// 
	// label4
	// 
	this->label4->AutoSize = true;
	this->label4->Location = System::Drawing::Point(683, 185);
	this->label4->Name = L"label4";
	this->label4->Size = System::Drawing::Size(30, 17);
	this->label4->TabIndex = 14;
	this->label4->Text = L"min";
	// 
	// trackBar3
	// 
	this->trackBar3->Location = System::Drawing::Point(747, 185);
	this->trackBar3->Maximum = 255;
	this->trackBar3->Name = L"trackBar3";
	this->trackBar3->Size = System::Drawing::Size(408, 56);
	this->trackBar3->TabIndex = 13;
	this->trackBar3->Value = 89;
	// 
	// trackBar4
	// 
	this->trackBar4->Location = System::Drawing::Point(747, 247);
	this->trackBar4->Maximum = 255;
	this->trackBar4->Name = L"trackBar4";
	this->trackBar4->Size = System::Drawing::Size(408, 56);
	this->trackBar4->TabIndex = 12;
	this->trackBar4->Value = 243;
	// 
	// label7
	// 
	this->label7->AutoSize = true;
	this->label7->Location = System::Drawing::Point(641, 224);
	this->label7->Name = L"label7";
	this->label7->Size = System::Drawing::Size(17, 17);
	this->label7->TabIndex = 11;
	this->label7->Text = L"S";
	// 
	// label8
	// 
	this->label8->AutoSize = true;
	this->label8->Location = System::Drawing::Point(683, 406);
	this->label8->Name = L"label8";
	this->label8->Size = System::Drawing::Size(33, 17);
	this->label8->TabIndex = 20;
	this->label8->Text = L"max";
	// 
	// label9
	// 
	this->label9->AutoSize = true;
	this->label9->Location = System::Drawing::Point(683, 344);
	this->label9->Name = L"label9";
	this->label9->Size = System::Drawing::Size(30, 17);
	this->label9->TabIndex = 19;
	this->label9->Text = L"min";
	// 
	// trackBar5
	// 
	this->trackBar5->Location = System::Drawing::Point(747, 344);
	this->trackBar5->Maximum = 255;
	this->trackBar5->Name = L"trackBar5";
	this->trackBar5->Size = System::Drawing::Size(408, 56);
	this->trackBar5->TabIndex = 18;
	this->trackBar5->Value = 38;
	// 
	// trackBar6
	// 
	this->trackBar6->Location = System::Drawing::Point(747, 406);
	this->trackBar6->Maximum = 255;
	this->trackBar6->Name = L"trackBar6";
	this->trackBar6->Size = System::Drawing::Size(408, 56);
	this->trackBar6->TabIndex = 17;
	this->trackBar6->Value = 192;
	// 
	// label10
	// 
	this->label10->AutoSize = true;
	this->label10->Location = System::Drawing::Point(641, 383);
	this->label10->Name = L"label10";
	this->label10->Size = System::Drawing::Size(17, 17);
	this->label10->TabIndex = 16;
	this->label10->Text = L"V";
	// 
	// label11
	// 
	this->label11->AutoSize = true;
	this->label11->Location = System::Drawing::Point(937, 63);
	this->label11->Name = L"label11";
	this->label11->Size = System::Drawing::Size(18, 17);
	this->label11->TabIndex = 21;
	this->label11->Text = L"H";
	// 
	// label12
	// 
	this->label12->AutoSize = true;
	this->label12->Location = System::Drawing::Point(937, 125);
	this->label12->Name = L"label12";
	this->label12->Size = System::Drawing::Size(18, 17);
	this->label12->TabIndex = 22;
	this->label12->Text = L"H";
	// 
	// label13
	// 
	this->label13->AutoSize = true;
	this->label13->Location = System::Drawing::Point(937, 269);
	this->label13->Name = L"label13";
	this->label13->Size = System::Drawing::Size(18, 17);
	this->label13->TabIndex = 24;
	this->label13->Text = L"H";
	// 
	// label14
	// 
	this->label14->AutoSize = true;
	this->label14->Location = System::Drawing::Point(937, 207);
	this->label14->Name = L"label14";
	this->label14->Size = System::Drawing::Size(18, 17);
	this->label14->TabIndex = 23;
	this->label14->Text = L"H";
	// 
	// label15
	// 
	this->label15->AutoSize = true;
	this->label15->Location = System::Drawing::Point(937, 430);
	this->label15->Name = L"label15";
	this->label15->Size = System::Drawing::Size(18, 17);
	this->label15->TabIndex = 26;
	this->label15->Text = L"H";
	// 
	// label16
	// 
	this->label16->AutoSize = true;
	this->label16->Location = System::Drawing::Point(937, 368);
	this->label16->Name = L"label16";
	this->label16->Size = System::Drawing::Size(18, 17);
	this->label16->TabIndex = 25;
	this->label16->Text = L"H";
	// 
	// MyForm
	// 
	this->ClientSize = System::Drawing::Size(1184, 533);
	this->Controls->Add(this->label15);
	this->Controls->Add(this->label16);
	this->Controls->Add(this->label13);
	this->Controls->Add(this->label14);
	this->Controls->Add(this->label12);
	this->Controls->Add(this->label11);
	this->Controls->Add(this->label8);
	this->Controls->Add(this->label9);
	this->Controls->Add(this->trackBar5);
	this->Controls->Add(this->trackBar6);
	this->Controls->Add(this->label10);
	this->Controls->Add(this->label3);
	this->Controls->Add(this->label4);
	this->Controls->Add(this->trackBar3);
	this->Controls->Add(this->trackBar4);
	this->Controls->Add(this->label7);
	this->Controls->Add(this->label6);
	this->Controls->Add(this->label5);
	this->Controls->Add(this->trackBar2);
	this->Controls->Add(this->trackBar1);
	this->Controls->Add(this->label2);
	this->Controls->Add(this->label1);
	this->Controls->Add(this->panel1);
	this->Name = L"MyForm";
	this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar3))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar4))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar5))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar6))->EndInit();
	this->ResumeLayout(false);
	this->PerformLayout();
}

MyForm::~MyForm() {
	if (components)
	{
		delete components;
	}
	delete backgroundWorker;
}
[STAThread]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	MyForm form;
	Application::Run(%form); 

	return 0;
}

int MyForm::main_photo() {
	cv::Mat srcBGR;
	// open image
	srcBGR = cv::imread("img11.jpg");
	if (!srcBGR.data)
		return -1;

	int r = DetermenGesture("MyVideo", srcBGR);

	//Wait until a key is pressed to kill the program
	//cv::waitKey(0);

	return r;
}

int MyForm::main_video() {
	cv::VideoCapture cap(0); // open the video camera no. 0

	if (!cap.isOpened()) 
		throw "Cannot open the video cam";

	cv::namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	while (1) {
		cv::Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess)  //if not success, break loop
			throw "Cannot read a frame from video stream";
		//cv::imshow("MyVideo", frame);

		DetermenGesture("MyVideo", frame);

		if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			break;
	}
	return 0;
}

int MyForm::DetermenGesture(std::string windowName, cv::Mat& srcBGR) {
	cv::Mat srcHSV, srcBinair, palmMask, fingerMask;

	initHashTable();
	cv::cvtColor(srcBGR, srcHSV, CV_BGR2HSV);

	// Skin color filter
	cv::GaussianBlur(srcHSV, srcHSV, cv::Size(7, 7), 0, 0);

	int H_min=0, H_max=100, S_min=0, S_max=100, V_min=0, V_max=100;
	if (this->trackBar1->InvokeRequired) {
		GetBarDelegate^ d = gcnew GetBarDelegate(this, &MyForm::GetBar1);
		H_min = (int) this->Invoke(d, gcnew array<Object^> { });
	} else 
		H_max = GetBar1();
	if (this->trackBar2->InvokeRequired) {
		GetBarDelegate^ d = gcnew GetBarDelegate(this, &MyForm::GetBar2);
		H_max = (int) this->Invoke(d, gcnew array<Object^> { });
	}
	else
		S_min = GetBar2();
	if (this->trackBar3->InvokeRequired) {
		GetBarDelegate^ d = gcnew GetBarDelegate(this, &MyForm::GetBar3);
		S_min = (int) this->Invoke(d, gcnew array<Object^> { });
	}
	else
		S_max = GetBar3();
	if (this->trackBar4->InvokeRequired) {
		GetBarDelegate^ d = gcnew GetBarDelegate(this, &MyForm::GetBar4);
		S_max = (int) this->Invoke(d, gcnew array<Object^> { });
	}
	else
		H_min = GetBar4();
	if (this->trackBar5->InvokeRequired) {
		GetBarDelegate^ d = gcnew GetBarDelegate(this, &MyForm::GetBar5);
		V_min = (int) this->Invoke(d, gcnew array<Object^> { });
	}
	else
		V_min = GetBar5();
	if (this->trackBar6->InvokeRequired) {
		GetBarDelegate^ d = gcnew GetBarDelegate(this, &MyForm::GetBar6);
		V_max = (int) this->Invoke(d, gcnew array<Object^> { });
	}
	else
		V_max = GetBar6();


	adaptiveHSVSkinColorFilter(srcHSV, srcBinair, H_min, H_max, S_min, S_max, V_min, V_max);

	Mat kernel = Mat::ones(cv::Point(5, 5), CV_8UC1);
	cv::morphologyEx(srcBinair, srcBinair, CV_MOP_CLOSE, kernel);
	cv::fillHoles(srcBinair, srcBinair);

	// find palm
	cv::Point palmCenter;
	float palmRadius;
	getPalmCenter(srcBinair, palmCenter, palmRadius);
	createPalmMask(srcBinair, palmMask, palmCenter, palmRadius);

	// find wrist
	cv::Line wristLine;
	bool foundWrist = false;
	findWrist(srcBinair, wristLine, foundWrist, palmCenter, palmRadius);
	if (!foundWrist)
		return 1;

	cv::Point wristCenter = wristLine.position + wristLine.direction / 2;

	if (palmCenter == wristCenter)
		return 1;

	// find the orientation of the hand
	cv::Point2f handOrientation = (cv::Point2f)palmCenter - (cv::Point2f)wristCenter;
	handOrientation /= std::pow(handOrientation.x*handOrientation.x + handOrientation.y*handOrientation.y, 0.5f);
	float handAngle = std::atan2(handOrientation.y, handOrientation.x);

	// find the fingers
	createFingerMask(srcBinair, fingerMask, palmMask, wristCenter, handOrientation);
	std::vector<cv::RotatedRect> boundingBoxesFingers = getBoundingBoxes(fingerMask);

	//TODO: determen the direction of the thumb
	ThumbDirection thumbDirection = ThumbDirection::Left;

	// find the thumb
	cv::RotatedRect* fingers[5];
	std::fill(fingers, fingers + 5, (cv::RotatedRect*)0);

	int thumbIndex = getFindThumb(boundingBoxesFingers, palmCenter, handAngle, thumbDirection);
	if (thumbIndex >= 0)
		fingers[0] = &boundingBoxesFingers[thumbIndex];

	// find the palm line
	cv::Line palmLine;
	bool foundPalm = true;
	findPalmLine(srcBinair, palmLine, foundPalm, wristLine, palmRadius, handOrientation, thumbIndex >= 0);
	if (!foundPalm)
		return 1;
	if (thumbDirection == ThumbDirection::Right) {
		palmLine.position = palmLine.lineEnd();
		palmLine.direction *= -1;
	}

	// find the 4 other fingers
	labelFingers(boundingBoxesFingers, fingers, wristCenter, handOrientation, palmLine);

	bool fingersStretch[5];
	areFingersStretched(fingers, fingersStretch, palmRadius);

	std::string gesture = deteremenGesture(GestureType::DutchCounting, fingersStretch);

	//cv::line(srcBinair, palmLine.lineStart(), palmLine.lineEnd(), cv::Scalar(150));
	//cv::line(srcBinair, wristLine.lineStart(), wristLine.lineEnd(), cv::Scalar(50));
	
	//Mat finalImage(srcBGR);
	//if (gesture.size() > 0)
	//	cv::putText(finalImage, gesture, cv::Point(0.05f*finalImage.cols, 0.95f*finalImage.rows), 2, 0.006f*finalImage.rows, cv::Scalar(100, 0, 0), 8);
	//imshow(windowName, finalImage);

	String^ str = gcnew System::String(gesture.c_str());
	if (this->label1->InvokeRequired) {
		SetTextDelegate^ d = gcnew SetTextDelegate(this, &MyForm::SetText);
		this->Invoke(d, gcnew array<Object^> { str });
	} else {
		SetText(str);
	}
	
	Bitmap^ img = gcnew System::Drawing::Bitmap(srcBGR.size().width,
		srcBGR.size().height,
		srcBGR.step,
		Drawing::Imaging::PixelFormat::Format24bppRgb,
		(IntPtr)srcBGR.data
	);
	if (this->panel1->InvokeRequired) {
		SetImageDelegate^ d = gcnew SetImageDelegate(this, &MyForm::SetImage);
		this->Invoke(d, gcnew array<Object^> { img });
	} else {
		SetImage(img);
	}

	return 0;
}
