#include "MyForm.h"

#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"
#include "Math.h"

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
	// MyForm
	// 
	this->ClientSize = System::Drawing::Size(652, 533);
	this->Controls->Add(this->label1);
	this->Controls->Add(this->panel1);
	this->Name = L"MyForm";
	this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
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
	adaptiveHSVSkinColorFilter(srcHSV, srcBinair);

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
