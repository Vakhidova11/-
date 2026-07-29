#pragma once

#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

#include <windows.h>
#include <fstream>
#include <string>
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::IO;

public ref class ReportForm : public Form
{
private:
    WebBrowser^ webBrowser1;
    Button^ btnOpenReport;
    Button^ btnCreateHTML;
    Button^ btnOpenInBrowser; // Новая кнопка
    OpenFileDialog^ openFileDialog1;

public:
    ReportForm()
    {
        this->Text = L"\u041f\u0443\u043d\u043a\u0442 4.4 - HTML \u043e\u0442\u0447\u0435\u0442\u044b";
        this->Size = System::Drawing::Size(900, 700);
        this->StartPosition = FormStartPosition::CenterScreen;

        webBrowser1 = gcnew WebBrowser();
        webBrowser1->Location = Point(10, 50);
        webBrowser1->Size = System::Drawing::Size(860, 600);
        this->Controls->Add(webBrowser1);

        // Кнопка "Открыть отчет"
        btnOpenReport = gcnew Button();
        btnOpenReport->Text = L"\u041e\u0442\u043a\u0440\u044b\u0442\u044c \u043e\u0442\u0447\u0435\u0442 (.html)";
        btnOpenReport->Location = Point(10, 10);
        btnOpenReport->Size = System::Drawing::Size(150, 30);
        btnOpenReport->Click += gcnew EventHandler(this, &ReportForm::BtnOpenReport_Click);
        this->Controls->Add(btnOpenReport);

        // Кнопка "Создать HTML"
        btnCreateHTML = gcnew Button();
        btnCreateHTML->Text = L"\u0421\u043e\u0437\u0434\u0430\u0442\u044c HTML";
        btnCreateHTML->Location = Point(180, 10);
        btnCreateHTML->Size = System::Drawing::Size(150, 30);
        btnCreateHTML->Click += gcnew EventHandler(this, &ReportForm::BtnCreateHTML_Click);
        this->Controls->Add(btnCreateHTML);

        // НОВАЯ КНОПКА: "Открыть в браузере"
        btnOpenInBrowser = gcnew Button();
        btnOpenInBrowser->Text = L"\u041e\u0442\u043a\u0440\u044b\u0442\u044c \u0432 \u0431\u0440\u0430\u0443\u0437\u0435\u0440\u0435";
        btnOpenInBrowser->Location = Point(350, 10);
        btnOpenInBrowser->Size = System::Drawing::Size(180, 30);
        btnOpenInBrowser->Click += gcnew EventHandler(this, &ReportForm::BtnOpenInBrowser_Click);
        this->Controls->Add(btnOpenInBrowser);

        openFileDialog1 = gcnew OpenFileDialog();
    }

private:
    void BtnOpenReport_Click(Object^ sender, EventArgs^ e)
    {
        openFileDialog1->Title = L"\u041e\u0442\u043a\u0440\u044b\u0442\u044c HTML-\u043e\u0442\u0447\u0451\u0442";
        openFileDialog1->Filter = L"Html files (*.html)|*.html|All files (*.*)|*.*";

        if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            webBrowser1->Navigate(openFileDialog1->FileName);
        }
    }

    // Функция для превращения картинки в строку (Base64)
    String^ ImageToBase64(String^ path)
    {
        array<Byte>^ imageBytes = System::IO::File::ReadAllBytes(path);
        return System::Convert::ToBase64String(imageBytes);
    }

    void BtnCreateHTML_Click(Object^ sender, EventArgs^ e)
    {
        openFileDialog1->Title = L"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u0441\u043a\u0440\u0438\u043d\u0448\u043e\u0442 \u0438\u0437 \u0433\u0430\u043b\u0435\u0440\u0435\u0438";
        openFileDialog1->Filter = L"Изображения (*.png;*.jpg;*.jpeg)|*.png;*.jpg;*.jpeg|Все файлы (*.*)|*.*";

        if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            String^ imagePath = openFileDialog1->FileName;

            String^ resultContent = "";
            if (System::IO::File::Exists("result.txt"))
            {
                StreamReader^ sr = gcnew StreamReader("result.txt");
                resultContent = sr->ReadToEnd();
                sr->Close();
            }
            else
            {
                resultContent = "Сначала выполните задание в Варианте 2.";
            }

            String^ base64Image = ImageToBase64(imagePath);
            String^ extension = System::IO::Path::GetExtension(imagePath)->ToLower();
            String^ mimeType = "image/png";
            if (extension == ".jpg" || extension == ".jpeg") mimeType = "image/jpeg";

            String^ imageSrc = "data:" + mimeType + ";base64," + base64Image;

            std::ofstream myfile("report.html", std::ios::out | std::ios::binary);

            unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
            myfile.write((char*)bom, 3);

            std::string resStr = msclr::interop::marshal_as<std::string>(resultContent);
            std::string htmlContent = "";
            for (size_t i = 0; i < resStr.length(); i++)
            {
                if (resStr[i] == '\n') htmlContent += "<br>";
                else htmlContent += resStr[i];
            }

            std::string imgSrcStr = msclr::interop::marshal_as<std::string>(imageSrc);

            myfile << "<!DOCTYPE html><html><head><meta charset='utf-8'></head><body>";
            myfile << "<h1 style='color:blue;'>Отчет по учебной практике</h1>";
            myfile << "<div style='font-family: Consolas, monospace; font-size: 16px;'>";
            myfile << htmlContent;
            myfile << "</div>";
            myfile << "<br>";
            myfile << "<img src='" << imgSrcStr << "' alt='Скриншот результата' width='600' style='border: 2px solid #ccc;'>";
            myfile << "</body></html>";
            myfile.close();

            webBrowser1->Navigate("file:///" + System::IO::Path::GetFullPath("report.html"));
        }
    }

    // НОВАЯ КНОПКА: ОТКРЫВАЕТ ТОТ ЖЕ ФАЙЛ В СИСТЕМНОМ БРАУЗЕРЕ (Chrome/Edge)
    void BtnOpenInBrowser_Click(Object^ sender, EventArgs^ e)
    {
        String^ filePath = System::IO::Path::GetFullPath("report.html");
        if (System::IO::File::Exists(filePath))
        {
            System::Diagnostics::Process::Start(filePath);
        }
        else
        {
            MessageBox::Show(L"\u0421\u043d\u0430\u0447\u0430\u043b\u0430 \u0441\u043e\u0437\u0434\u0430\u0439\u0442\u0435 HTML (\u043d\u0430\u0436\u043c\u0438\u0442\u0435 '\u0421\u043e\u0437\u0434\u0430\u0442\u044c HTML')!");
        }
    }
};
