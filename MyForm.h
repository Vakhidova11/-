#pragma once

#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

#include <windows.h>
#include <string>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::IO;

public ref class MyForm : public Form
{
private:
    Button^ buttonCalculate;
    Button^ buttonOpenFile;
    TextBox^ textBox;
    Label^ labelResult;

public:
    MyForm()
    {
        this->Text = L"\u0412\u0430\u0440\u0438\u0430\u043d\u0442 2 - \u041f\u043e\u0438\u0441\u043a \u0441\u0443\u043c\u043c\u044b \u043d\u0430\u0438\u043c\u0435\u043d\u044c\u0448\u0438\u0445 \u044d\u043b\u0435\u043c\u0435\u043d\u0442\u043e\u0432";
        this->Size = System::Drawing::Size(600, 400);
        this->StartPosition = FormStartPosition::CenterScreen;

        textBox = gcnew TextBox();
        textBox->Location = Point(20, 20);
        textBox->Size = System::Drawing::Size(550, 150);
        textBox->Multiline = true;
        textBox->Font = gcnew System::Drawing::Font("Consolas", 12);
        this->Controls->Add(textBox);

        buttonOpenFile = gcnew Button();
        buttonOpenFile->Text = L"\u041e\u0442\u043a\u0440\u044b\u0442\u044c \u0444\u0430\u0439\u043b (.txt)";
        buttonOpenFile->Location = Point(20, 190);
        buttonOpenFile->Size = System::Drawing::Size(150, 30);
        buttonOpenFile->Click += gcnew EventHandler(this, &MyForm::ButtonOpenFile_Click);
        this->Controls->Add(buttonOpenFile);

        buttonCalculate = gcnew Button();
        buttonCalculate->Text = L"\u041d\u0430\u0439\u0442\u0438 \u0441\u0443\u043c\u043c\u0443";
        buttonCalculate->Location = Point(190, 190);
        buttonCalculate->Size = System::Drawing::Size(150, 30);
        buttonCalculate->Click += gcnew EventHandler(this, &MyForm::ButtonCalculate_Click);
        this->Controls->Add(buttonCalculate);

        labelResult = gcnew Label();
        labelResult->Text = L"\u0420\u0435\u0437\u0443\u043b\u044c\u0442\u0430\u0442 \u0431\u0443\u0434\u0435\u0442 \u0437\u0434\u0435\u0441\u044c...";
        labelResult->Location = Point(20, 240);
        labelResult->Size = System::Drawing::Size(550, 50);
        labelResult->Font = gcnew System::Drawing::Font("Segoe UI", 14, FontStyle::Bold);
        labelResult->ForeColor = Color::DarkBlue;
        this->Controls->Add(labelResult);
    }

private:
    void ButtonOpenFile_Click(Object^ sender, EventArgs^ e)
    {
        OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
        openFileDialog->Filter = L"\u0422\u0435\u043a\u0441\u0442\u043e\u0432\u044b\u0435 \u0444\u0430\u0439\u043b\u044b (*.txt)|*.txt|\u0412\u0441\u0435 \u0444\u0430\u0439\u043b\u044b (*.*)|*.*";

        if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            StreamReader^ sr = gcnew StreamReader(openFileDialog->FileName);
            textBox->Text = sr->ReadToEnd();
            sr->Close();
            labelResult->Text = L"\u0424\u0430\u0439\u043b \u0437\u0430\u0433\u0440\u0443\u0436\u0435\u043d!";
        }
    }

    void ButtonCalculate_Click(Object^ sender, EventArgs^ e)
    {
        try
        {
            if (String::IsNullOrWhiteSpace(textBox->Text))
            {
                MessageBox::Show(L"\u0421\u043d\u0430\u0447\u0430\u043b\u0430 \u0432\u0432\u0435\u0434\u0438\u0442\u0435 \u043c\u0430\u0441\u0441\u0438\u0432!");
                return;
            }

            String^ rawText = textBox->Text->Trim();

            array<String^>^ lines = rawText->Split(gcnew array<String^>{ L"\r\n", L"\n" }, StringSplitOptions::RemoveEmptyEntries);
            int rows = lines->Length;

            array<String^>^ firstRowNums = lines[0]->Split(gcnew array<Char>{ L' ', L'\t' }, StringSplitOptions::RemoveEmptyEntries);
            int cols = firstRowNums->Length;

            array<double, 2>^ matrix = gcnew array<double, 2>(rows, cols);
            for (int i = 0; i < rows; i++)
            {
                array<String^>^ nums = lines[i]->Trim()->Split(gcnew array<Char>{ L' ', L'\t' }, StringSplitOptions::RemoveEmptyEntries);
                for (int j = 0; j < nums->Length; j++)
                {
                    matrix[i, j] = Double::Parse(nums[j], System::Globalization::CultureInfo::InvariantCulture);
                }
            }

            double sumOfMins = 0.0;
            for (int i = 0; i < rows; i++)
            {
                double currentMin = matrix[i, 0];
                for (int j = 1; j < cols; j++)
                {
                    if (matrix[i, j] < currentMin)
                    {
                        currentMin = matrix[i, j];
                    }
                }
                sumOfMins += currentMin;
            }

            // ========== СОХРАНЯЕМ РЕЗУЛЬТАТ В ФАЙЛ ДЛЯ HTML ==========
            String^ resultString = "Результат Варианта 2: Сумма минимальных элементов = " + sumOfMins.ToString();
            StreamWriter^ sw = gcnew StreamWriter("result.txt", false, System::Text::Encoding::UTF8);
            sw->WriteLine(resultString);
            sw->Close();
            // ==========================================================

            labelResult->Text = L"\u0421\u0443\u043c\u043c\u0430 \u043c\u0438\u043d\u0438\u043c\u0430\u043b\u044c\u043d\u044b\u0445 \u044d\u043b\u0435\u043c\u0435\u043d\u0442\u043e\u0432: " + sumOfMins.ToString();
        }
        catch (Exception^ ex)
        {
            MessageBox::Show(L"\u041e\u0448\u0438\u0431\u043a\u0430 \u0432 \u0434\u0430\u043d\u043d\u044b\u0445! \u0423\u0431\u0435\u0434\u0438\u0442\u0435\u0441\u044c, \u0447\u0442\u043e \u0438\u0441\u043f\u043e\u043b\u044c\u0437\u0443\u0435\u0442\u0441\u044f \u0442\u043e\u0447\u043a\u0430 \u0432 \u043a\u0430\u0447\u0435\u0441\u0442\u0432\u0435 \u0440\u0430\u0437\u0434\u0435\u043b\u0438\u0442\u0435\u043b\u044f.");
        }
    }
};
