#pragma once

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
	};
}
