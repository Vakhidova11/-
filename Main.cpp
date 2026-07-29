// Main.cpp
// Точка входа и простое меню для запуска двух форм: задания и рисовалки.

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#include "MyForm.h"
#include "PaintForm.h"
#include "ReportForm.h" // ДОБАВЛЕНА НОВАЯ СТРОКА

// Убирает консоль
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(linker, "/ENTRY:mainCRTStartup")

using namespace System;
using namespace System::Windows::Forms;

// Класс Меню
public ref class MenuForm : public Form
{
private:
    Button^ btnTask2;
    Button^ btnPaint;
    Button^ btnReport; // Новая кнопка для HTML

public:
    MenuForm()
    {
        this->Text = L"\u041b\u0430\u0431\u043e\u0440\u0430\u0442\u043e\u0440\u043d\u0430\u044f \u0440\u0430\u0431\u043e\u0442\u0430";
        this->Size = System::Drawing::Size(400, 300);
        this->StartPosition = FormStartPosition::CenterScreen;

        btnTask2 = gcnew Button();
        btnTask2->Text = L"\u0417\u0430\u0434\u0430\u043d\u0438\u0435 2 (\u0421\u0443\u043c\u043c\u0430)";
        btnTask2->Location = Point(50, 40);
        btnTask2->Size = System::Drawing::Size(280, 40);
        btnTask2->Font = gcnew System::Drawing::Font("Segoe UI", 12);
        btnTask2->Click += gcnew EventHandler(this, &MenuForm::RunTask2);
        this->Controls->Add(btnTask2);

        btnPaint = gcnew Button();
        btnPaint->Text = L"\u0420\u0438\u0441\u043e\u0432\u0430\u043d\u0438\u0435 (Paint)";
        btnPaint->Location = Point(50, 100);
        btnPaint->Size = System::Drawing::Size(280, 40);
        btnPaint->Font = gcnew System::Drawing::Font("Segoe UI", 12);
        btnPaint->Click += gcnew EventHandler(this, &MenuForm::RunPaint);
        this->Controls->Add(btnPaint);

        // Новая кнопка для формы с HTML
        btnReport = gcnew Button();
        btnReport->Text = L"HTML \u041e\u0442\u0447\u0435\u0442 (WebBrowser)";
        btnReport->Location = Point(50, 160);
        btnReport->Size = System::Drawing::Size(280, 40);
        btnReport->Font = gcnew System::Drawing::Font("Segoe UI", 12);
        btnReport->Click += gcnew EventHandler(this, &MenuForm::RunReport);
        this->Controls->Add(btnReport);
    }

private:
    void RunTask2(Object^ sender, EventArgs^ e) { (gcnew MyForm())->ShowDialog(); }
    void RunPaint(Object^ sender, EventArgs^ e) { (gcnew PaintForm())->ShowDialog(); }
    void RunReport(Object^ sender, EventArgs^ e) { (gcnew ReportForm())->ShowDialog(); } // Вызов новой формы
};

[STAThreadAttribute]
int main()
{
    FreeConsole(); // Принудительно закрываем консоль (для надежности)
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MenuForm());
    return 0;
}
