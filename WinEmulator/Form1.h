#pragma once
#include "RegisterDataBinder.h"
#include "MemoryDataBinder.h"
#include "AboutForm.h"

namespace WinEmulator {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
		/// <summary>
		/// The main processor
		/// </summary>
	private: Processor^ processor;
	private: bool steppingStarted;
	private: String^ AppName;
	private: String^ FileName;

	private: DataGridViewCellStyle^ memoryAccessedCellStyle;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataGridView^  dataGridView2;

	private: BindingSource ^registersBindingSource, ^memoryBindingSource;
	private: System::Windows::Forms::ToolStripMenuItem^  memoryToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  gotoToolStripMenuItem;
	private: System::Windows::Forms::ToolStripTextBox^  toolStripTextBox1;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripMenuItem^  breakToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  stepToolStripMenuItem;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  continueToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  clearToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  newToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  ofd;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveAsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::SaveFileDialog^  sfd;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;

	private: System::Windows::Forms::TextBox^  textBox2;

	public:
		Form1(void)
		{
			InitializeComponent();

			processor = gcnew Processor();
			// Important
			processor->initializeOperations();
			processor->onMemoryWrittenAt = gcnew Processor::MemoryAccessedAt(this, &Form1::UpdateMemoryViewAt);
			processor->updateAfterStep = gcnew Processor::InstructionExecuted(this, &Form1::UpdateAfterStep);

			registersBindingSource = gcnew BindingSource();
			memoryBindingSource = gcnew BindingSource();

			AppName = this->Text;
			FileName = "";
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  parseToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  runToolStripMenuItem;
	private: System::Windows::Forms::TextBox^  textBox1;

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
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->newToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->parseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->stepToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->continueToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->runToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->breakToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->memoryToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->gotoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripTextBox1 = (gcnew System::Windows::Forms::ToolStripTextBox());
			this->clearToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->ofd = (gcnew System::Windows::Forms::OpenFileDialog());
			this->sfd = (gcnew System::Windows::Forms::SaveFileDialog());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->BeginInit();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->fileToolStripMenuItem1, 
				this->fileToolStripMenuItem, this->memoryToolStripMenuItem, this->aboutToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(971, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem1
			// 
			this->fileToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->newToolStripMenuItem, 
				this->openToolStripMenuItem, this->toolStripSeparator3, this->saveToolStripMenuItem, this->saveAsToolStripMenuItem, this->toolStripSeparator4, 
				this->exitToolStripMenuItem});
			this->fileToolStripMenuItem1->Name = L"fileToolStripMenuItem1";
			this->fileToolStripMenuItem1->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem1->Text = L"&File";
			// 
			// newToolStripMenuItem
			// 
			this->newToolStripMenuItem->Name = L"newToolStripMenuItem";
			this->newToolStripMenuItem->Size = System::Drawing::Size(121, 22);
			this->newToolStripMenuItem->Text = L"&New";
			this->newToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::newToolStripMenuItem_Click);
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(121, 22);
			this->openToolStripMenuItem->Text = L"&Open...";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(118, 6);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(121, 22);
			this->saveToolStripMenuItem->Text = L"&Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
			// 
			// saveAsToolStripMenuItem
			// 
			this->saveAsToolStripMenuItem->Name = L"saveAsToolStripMenuItem";
			this->saveAsToolStripMenuItem->Size = System::Drawing::Size(121, 22);
			this->saveAsToolStripMenuItem->Text = L"S&ave as...";
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			this->toolStripSeparator4->Size = System::Drawing::Size(118, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(121, 22);
			this->exitToolStripMenuItem->Text = L"E&xit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->parseToolStripMenuItem, 
				this->toolStripSeparator1, this->stepToolStripMenuItem, this->continueToolStripMenuItem, this->runToolStripMenuItem, this->toolStripSeparator2, 
				this->breakToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(59, 20);
			this->fileToolStripMenuItem->Text = L"&Execute";
			// 
			// parseToolStripMenuItem
			// 
			this->parseToolStripMenuItem->Name = L"parseToolStripMenuItem";
			this->parseToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F5;
			this->parseToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->parseToolStripMenuItem->Text = L"&Assemble";
			this->parseToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::parseToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(141, 6);
			// 
			// stepToolStripMenuItem
			// 
			this->stepToolStripMenuItem->Enabled = false;
			this->stepToolStripMenuItem->Name = L"stepToolStripMenuItem";
			this->stepToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F6;
			this->stepToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->stepToolStripMenuItem->Text = L"&Step";
			this->stepToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::stepToolStripMenuItem_Click);
			// 
			// continueToolStripMenuItem
			// 
			this->continueToolStripMenuItem->Enabled = false;
			this->continueToolStripMenuItem->Name = L"continueToolStripMenuItem";
			this->continueToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F7;
			this->continueToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->continueToolStripMenuItem->Text = L"C&ontinue";
			this->continueToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::continueToolStripMenuItem_Click);
			// 
			// runToolStripMenuItem
			// 
			this->runToolStripMenuItem->Enabled = false;
			this->runToolStripMenuItem->Name = L"runToolStripMenuItem";
			this->runToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F8;
			this->runToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->runToolStripMenuItem->Text = L"&Run";
			this->runToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::runToolStripMenuItem_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(141, 6);
			// 
			// breakToolStripMenuItem
			// 
			this->breakToolStripMenuItem->Name = L"breakToolStripMenuItem";
			this->breakToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F12;
			this->breakToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->breakToolStripMenuItem->Text = L"&Break";
			this->breakToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::breakToolStripMenuItem_Click);
			// 
			// memoryToolStripMenuItem
			// 
			this->memoryToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->gotoToolStripMenuItem, 
				this->clearToolStripMenuItem});
			this->memoryToolStripMenuItem->Name = L"memoryToolStripMenuItem";
			this->memoryToolStripMenuItem->Size = System::Drawing::Size(64, 20);
			this->memoryToolStripMenuItem->Text = L"&Memory";
			// 
			// gotoToolStripMenuItem
			// 
			this->gotoToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripTextBox1});
			this->gotoToolStripMenuItem->Name = L"gotoToolStripMenuItem";
			this->gotoToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::G));
			this->gotoToolStripMenuItem->Size = System::Drawing::Size(151, 22);
			this->gotoToolStripMenuItem->Text = L"Goto...";
			this->gotoToolStripMenuItem->DropDownOpened += gcnew System::EventHandler(this, &Form1::gotoToolStripMenuItem_DropDownOpened);
			// 
			// toolStripTextBox1
			// 
			this->toolStripTextBox1->MaxLength = 4;
			this->toolStripTextBox1->Name = L"toolStripTextBox1";
			this->toolStripTextBox1->Size = System::Drawing::Size(50, 23);
			this->toolStripTextBox1->TextBoxTextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->toolStripTextBox1->TextChanged += gcnew System::EventHandler(this, &Form1::toolStripTextBox1_TextChanged);
			// 
			// clearToolStripMenuItem
			// 
			this->clearToolStripMenuItem->Name = L"clearToolStripMenuItem";
			this->clearToolStripMenuItem->Size = System::Drawing::Size(151, 22);
			this->clearToolStripMenuItem->Text = L"&Clear";
			this->clearToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::clearToolStripMenuItem_Click);
			// 
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Courier New", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->textBox1->Location = System::Drawing::Point(12, 30);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(208, 390);
			this->textBox1->TabIndex = 1;
			this->textBox1->Text = L"MVI A, 23h\r\nMVI B, 55h\r\nLXI H, 20h\r\nMOV M, A\r\nINX H\r\nINR B\r\nDCR A\r\nJNZ 0007\r\nHLT";
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AllowUserToResizeColumns = false;
			this->dataGridView1->AllowUserToResizeRows = false;
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::ColumnHeader;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->EditMode = System::Windows::Forms::DataGridViewEditMode::EditOnEnter;
			this->dataGridView1->Location = System::Drawing::Point(373, 30);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->ScrollBars = System::Windows::Forms::ScrollBars::None;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
			this->dataGridView1->Size = System::Drawing::Size(119, 390);
			this->dataGridView1->TabIndex = 2;
			// 
			// dataGridView2
			// 
			this->dataGridView2->AllowUserToAddRows = false;
			this->dataGridView2->AllowUserToDeleteRows = false;
			this->dataGridView2->AllowUserToResizeColumns = false;
			this->dataGridView2->AllowUserToResizeRows = false;
			dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Info;
			dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::ControlText;
			this->dataGridView2->AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
			this->dataGridView2->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::ColumnHeader;
			this->dataGridView2->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView2->EditMode = System::Windows::Forms::DataGridViewEditMode::EditOnEnter;
			this->dataGridView2->Location = System::Drawing::Point(498, 30);
			this->dataGridView2->MultiSelect = false;
			this->dataGridView2->Name = L"dataGridView2";
			this->dataGridView2->RowHeadersVisible = false;
			this->dataGridView2->RowTemplate->DefaultCellStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
			this->dataGridView2->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->dataGridView2->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
			this->dataGridView2->Size = System::Drawing::Size(461, 390);
			this->dataGridView2->TabIndex = 3;
			// 
			// textBox2
			// 
			this->textBox2->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->textBox2->Font = (gcnew System::Drawing::Font(L"Courier New", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->textBox2->Location = System::Drawing::Point(226, 30);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->ReadOnly = true;
			this->textBox2->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->textBox2->Size = System::Drawing::Size(141, 390);
			this->textBox2->TabIndex = 4;
			this->textBox2->WordWrap = false;
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripStatusLabel1});
			this->statusStrip1->Location = System::Drawing::Point(0, 431);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(971, 22);
			this->statusStrip1->TabIndex = 5;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(0, 17);
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(52, 20);
			this->aboutToolStripMenuItem->Text = L"&About";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::aboutToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(971, 453);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->dataGridView2);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->menuStrip1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"8085 Emulator";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->EndInit();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void parseToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 textBox2->Text = "";
				 toolStripStatusLabel1->Text = "Assembling...";

				 try {
					 if(!processor->instructions) {
						 processor->instructions = gcnew InstructionList(MAX_INSTRUCTIONS, processor->BASE_ADDRESS);
					 }
					 processor->instructions->clear();
					 processor->parse(textBox1->Lines);
					 processor->RESET();
					 processor->jumpTo(processor->BASE_ADDRESS);
					 
					 //Advanced check:
					 /*
					 toolStripStatusLabel1->Text = "Checking for errors...use File->Break to stop.";
					 processor->execute();
					 processor->RESET();
					 */

					 this->runToolStripMenuItem->Enabled = true;
					 this->stepToolStripMenuItem->Enabled = true;
					 toolStripStatusLabel1->Text = "Assembling...done.";
				 }
				 catch(ArgumentException^ e) {
					 String^ error = gcnew String(e->Message);

					 // Move to beginning of line causing error
					 if(e->Data["LineNumber"]) {
						 int linestart = 0, linenumber = Convert::ToInt32(e->Data["LineNumber"]);
						 selectLineNumber(linenumber);
						 error = "Line " + linenumber + ": " + error;
					 }

					 toolStripStatusLabel1->Text = error;
					 return;
				 }
				 finally {
					 array<String^>^ lines = gcnew array<String^>(textBox1->Lines->Length);

					 for(int i = 0; i < processor->instructions->count; i++) {
						 lines[processor->instructions->instructions[i]->lineNumber-1] = Convert::ToString(processor->instructions->addresses[i],16)->ToUpper()->PadLeft(4,'0');
					 }

					 textBox2->Lines = lines;
				 }
			 }
	private: System::Void runToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 try {
					 toolStripStatusLabel1->Text = "Executing...use File->Break to stop.";
					 processor->PC = processor->BASE_ADDRESS;
					 processor->RESET();
					 processor->execute();
					 toolStripStatusLabel1->Text = "Executed successfully.";
				 } catch(ArgumentException^ e) {
					 MessageBox::Show(e->Message, "Execution Error");
					 this->runToolStripMenuItem->Enabled = false;
				 }
			 }
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 /* Registers */
				 for(int i = 0; i < Executor::RegisterNames->Length; i++) {
					 registersBindingSource->Add(gcnew RegisterDataBinder(Convert::ToString(Executor::RegisterNames[i]), processor));
				 }
				 registersBindingSource->Add(gcnew RegisterDataBinder("BC", processor));
				 registersBindingSource->Add(gcnew RegisterDataBinder("DE", processor));
				 registersBindingSource->Add(gcnew RegisterDataBinder("HL", processor));
				 registersBindingSource->Add(gcnew RegisterDataBinder("PC", processor));
				 registersBindingSource->Add(gcnew RegisterDataBinder("SP", processor));

				 dataGridView1->AutoGenerateColumns = false;
				 dataGridView1->DataSource = registersBindingSource;
				 dataGridView1->Name = "Registers";

				 DataGridViewColumn^ column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Name";
				 column->Name = "Register";
				 dataGridView1->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Value";
				 column->Name = "Value";
				 dataGridView1->Columns->Add(column);

				 dataGridView1->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

				 /* Memory */
				 for(Int32 add=0x0000; add < 0xFFFF; add += 16)
					 memoryBindingSource->Add(gcnew MemoryDataBinder((WORD)add, processor));

				 dataGridView2->AutoGenerateColumns = false;
				 dataGridView2->DataSource = memoryBindingSource;

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Address";
				 column->Name = "Address";
				 column->Width = 15;
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Zero";
				 column->Name = "0";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "One";
				 column->Name = "1";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Two";
				 column->Name = "2";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Three";
				 column->Name = "3";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Four";
				 column->Name = "4";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Five";
				 column->Name = "5";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Six";
				 column->Name = "6";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Seven";
				 column->Name = "7";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Eight";
				 column->Name = "8";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Nine";
				 column->Name = "9";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Ten";
				 column->Name = "A";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Eleven";
				 column->Name = "B";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Twelve";
				 column->Name = "C";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Thirteen";
				 column->Name = "D";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Fourteen";
				 column->Name = "E";
				 dataGridView2->Columns->Add(column);

				 column = gcnew DataGridViewTextBoxColumn();
				 column->DataPropertyName = "Fifteen";
				 column->Name = "F";
				 dataGridView2->Columns->Add(column);

				 dataGridView2->Columns[0]->FillWeight = 160;
				 dataGridView2->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
				 memoryAccessedCellStyle = gcnew DataGridViewCellStyle(dataGridView2->DefaultCellStyle);
				 memoryAccessedCellStyle->Font = gcnew Drawing::Font(dataGridView2->Font, FontStyle::Bold);
			 }
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				 runToolStripMenuItem->Enabled = false;
				 stepToolStripMenuItem->Enabled = false;
				 continueToolStripMenuItem->Enabled = false;
				 saveToolStripMenuItem->Enabled = true;
				 this->Text = AppName + " " + FileName + "*";
			 }
	private: System::Void toolStripTextBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				 toolStripStatusLabel1->Text = "Enter the address to scroll down to.";
				 toolStripTextBox1->Text = toolStripTextBox1->Text->Trim();
				 if(toolStripTextBox1->Text != "") {
					 try {
						 UInt16 address = (UInt16)Convert::ToUInt32(toolStripTextBox1->Text, 16);
						 dataGridView2->FirstDisplayedScrollingRowIndex = address >> 4;
						 dataGridView2->Rows[(int)(address >> 4)]->Cells[1+(address & 15)]->Selected = true;
					 } catch(Exception^) {
						 toolStripStatusLabel1->Text = "Invalid address.";
					 }
				 }
			 }
	private: System::Void toolStripTextBox1_KeyPress(System::Object^  sender, KeyPressEventArgs^  e) {
				 e->KeyChar = Char::ToUpper(e->KeyChar);

				 if(!Char::IsControl(e->KeyChar) && (e->KeyChar < '0' || e->KeyChar > '9') && (e->KeyChar < 'A' || e->KeyChar > 'F')) {
					 e->Handled = true;
				 }
				 if(e->KeyChar == (char)13) {
					 Form1::memoryToolStripMenuItem->DropDown->Visible = false;
					 Form1::dataGridView2->Focus();
				 }
			 }
	private: System::Void gotoToolStripMenuItem_DropDownOpened(System::Object^  sender, System::EventArgs^  e) {
				 toolStripStatusLabel1->Text = "Enter the address to scroll down to.";
			 }
	private: System::Void breakToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 processor->HALTED = 1;
			 }
	private: System::Void UpdateMemoryViewAt(WORD w) {
				 int index = w >> 4;
				 memoryBindingSource->ResetItem(index);
				 //dataGridView2->Rows[index]->Cells[1+(w & 15)]->Style->ForeColor = Color::Green;
				 dataGridView2->Rows[index]->Cells[1+(w & 15)]->Selected = true;
			 }
	private: System::Void UpdateAfterStep(Instruction^ i) {
				 registersBindingSource->ResetBindings(false);
				 selectLineNumber(i->lineNumber);
				 this->Update();
				 toolStripStatusLabel1->Text = "Executed: " + i;
			 }
	private: System::Void stepToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(!steppingStarted) {
					 // Start stepping
					 processor->PC = processor->BASE_ADDRESS;
					 processor->RESET();
					 steppingStarted = true;
				 }
				 if(processor->HALTED) {
					toolStripStatusLabel1->Text = "Processor has been halted.";
					steppingStarted = false;
					return;
				 }
				 this->processor->executeOneStep();
			 }
	private: System::Void selectLineNumber(int linenumber) {
				 int linestart = 0;

				 for(int i = 0; i < linenumber-1; i++)
					 linestart += textBox1->Lines[i]->Length+2;
				 
				 textBox1->Select(linestart, textBox1->Lines[linenumber-1]->Length+1);
			 }
private: System::Void continueToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 try {
					 toolStripStatusLabel1->Text = "Executing...use File->Break to stop.";
					 //processor->PC = BASE_ADDRESS;
					 processor->RESET();
					 processor->execute();
					 toolStripStatusLabel1->Text = "Executed successfully.";
				 } catch(ArgumentException^ e) {
					 MessageBox::Show(e->Message, "Execution Error");
					 this->runToolStripMenuItem->Enabled = false;
				 }
		 }
private: System::Void clearToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 Array::Clear(processor->memory, 0, 0xFFFF);
			 memoryBindingSource->ResetBindings(false);
		 }
private: System::Void newToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 textBox1->Text = "";
			 processor->RESET();

			 this->Text = AppName; 
			 FileName = "";
		 }
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(ofd->ShowDialog() == ::DialogResult::OK && ofd->FileName != "") {
				 textBox1->Text = IO::File::ReadAllText(ofd->FileName);
				 FileName = ofd->FileName;
				 this->Text = AppName + " " + FileName;
			 }
		 }
private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(FileName == "") {
				 if(sfd->ShowDialog() == ::DialogResult::OK && sfd->FileName != "") {
					 FileName = sfd->FileName;
				 } else {
					 return;
				 }
			 }

			 IO::File::WriteAllText(FileName, textBox1->Text);
			 saveToolStripMenuItem->Enabled = false;
			 this->Text = AppName + " " + FileName;
		 }
private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 AboutForm^ form = gcnew AboutForm();
			 form->Show();
		 }
};
}

