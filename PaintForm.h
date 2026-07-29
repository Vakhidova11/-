#pragma once

#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::IO;

public ref class PaintForm : public Form
{
private:
    PictureBox^ canvas;
    Button^ btnColor;
    Button^ btnClear;
    Button^ btnSave;
    Button^ btnOpenImage;
    ComboBox^ cmbSize;
    Label^ lblStatus;

    bool isDrawing;
    Point lastPoint;
    Color currentColor;
    int penSize;

public:
    PaintForm()
    {
        this->Text = L"\u0420\u0438\u0441\u043e\u0432\u0430\u043d\u0438\u0435 (\u041f\u0443\u043d\u043a\u0442 4.3)";
        this->Size = System::Drawing::Size(800, 600);
        this->StartPosition = FormStartPosition::CenterScreen;

        isDrawing = false;
        currentColor = Color::Black;
        penSize = 5;

        canvas = gcnew PictureBox();
        canvas->Location = Point(10, 50);
        canvas->Size = System::Drawing::Size(760, 500);
        canvas->BackColor = Color::White;
        canvas->BorderStyle = BorderStyle::FixedSingle;
        canvas->Image = gcnew Bitmap(canvas->Width, canvas->Height);
        canvas->MouseDown += gcnew MouseEventHandler(this, &PaintForm::Canvas_MouseDown);
        canvas->MouseMove += gcnew MouseEventHandler(this, &PaintForm::Canvas_MouseMove);
        canvas->MouseUp += gcnew MouseEventHandler(this, &PaintForm::Canvas_MouseUp);
        this->Controls->Add(canvas);

        btnColor = gcnew Button();
        btnColor->Text = L"\u0426\u0432\u0435\u0442";
        btnColor->Location = Point(10, 10);
        btnColor->Size = System::Drawing::Size(80, 30);
        btnColor->Click += gcnew EventHandler(this, &PaintForm::BtnColor_Click);
        this->Controls->Add(btnColor);

        btnClear = gcnew Button();
        btnClear->Text = L"\u041e\u0447\u0438\u0441\u0442\u0438\u0442\u044c";
        btnClear->Location = Point(100, 10);
        btnClear->Size = System::Drawing::Size(80, 30);
        btnClear->Click += gcnew EventHandler(this, &PaintForm::BtnClear_Click);
        this->Controls->Add(btnClear);

        btnOpenImage = gcnew Button();
        btnOpenImage->Text = L"\u041e\u0442\u043a\u0440\u044b\u0442\u044c PNG";
        btnOpenImage->Location = Point(190, 10);
        btnOpenImage->Size = System::Drawing::Size(100, 30);
        btnOpenImage->Click += gcnew EventHandler(this, &PaintForm::BtnOpenImage_Click);
        this->Controls->Add(btnOpenImage);

        btnSave = gcnew Button();
        btnSave->Text = L"\u0421\u043e\u0445\u0440\u0430\u043d\u0438\u0442\u044c PNG";
        btnSave->Location = Point(300, 10);
        btnSave->Size = System::Drawing::Size(100, 30);
        btnSave->Click += gcnew EventHandler(this, &PaintForm::BtnSave_Click);
        this->Controls->Add(btnSave);

        cmbSize = gcnew ComboBox();
        cmbSize->Location = Point(420, 10);
        cmbSize->Size = System::Drawing::Size(80, 25);
        cmbSize->DropDownStyle = ComboBoxStyle::DropDownList;
        cmbSize->Items->Add(L"\u041c\u0430\u043b\u0430\u044f");
        cmbSize->Items->Add(L"\u0421\u0440\u0435\u0434\u043d\u044f\u044f");
        cmbSize->Items->Add(L"\u0411\u043e\u043b\u044c\u0448\u0430\u044f");
        cmbSize->SelectedIndex = 1;
        cmbSize->SelectedIndexChanged += gcnew EventHandler(this, &PaintForm::CmbSize_SelectedIndexChanged);
        this->Controls->Add(cmbSize);

        lblStatus = gcnew Label();
        lblStatus->Text = L"\u0420\u0438\u0441\u0443\u0439\u0442\u0435!";
        lblStatus->Location = Point(520, 15);
        lblStatus->Size = System::Drawing::Size(200, 20);
        this->Controls->Add(lblStatus);
    }

private:
    void Canvas_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        if (e->Button == System::Windows::Forms::MouseButtons::Left)
        {
            isDrawing = true;
            lastPoint = e->Location;
        }
    }

    void Canvas_MouseMove(Object^ sender, MouseEventArgs^ e)
    {
        if (isDrawing && e->Button == System::Windows::Forms::MouseButtons::Left)
        {
            Graphics^ g = Graphics::FromImage(canvas->Image);
            Pen^ p = gcnew Pen(currentColor, (float)penSize);
            p->StartCap = System::Drawing::Drawing2D::LineCap::Round;
            p->EndCap = System::Drawing::Drawing2D::LineCap::Round;
            g->DrawLine(p, lastPoint, e->Location);
            lastPoint = e->Location;
            canvas->Refresh();
            lblStatus->Text = L"\u0420\u0438\u0441\u0443\u044e...";
        }
    }

    void Canvas_MouseUp(Object^ sender, MouseEventArgs^ e)
    {
        if (e->Button == System::Windows::Forms::MouseButtons::Left)
        {
            isDrawing = false;
            lblStatus->Text = L"\u0413\u043e\u0442\u043e\u0432\u043e";
        }
    }

    void BtnColor_Click(Object^ sender, EventArgs^ e)
    {
        ColorDialog^ cd = gcnew ColorDialog();
        if (cd->ShowDialog() == System::Windows::Forms::DialogResult::OK) currentColor = cd->Color;
    }

    void BtnClear_Click(Object^ sender, EventArgs^ e)
    {
        if (canvas->Image != nullptr)
        {
            Graphics::FromImage(canvas->Image)->Clear(Color::White);
            canvas->Refresh();
            lblStatus->Text = L"\u041e\u0447\u0438\u0449\u0435\u043d\u043e";
        }
    }

    void CmbSize_SelectedIndexChanged(Object^ sender, EventArgs^ e)
    {
        String^ s = cmbSize->SelectedItem->ToString();
        if (s == L"\u041c\u0430\u043b\u0430\u044f") penSize = 2;
        else if (s == L"\u0421\u0440\u0435\u0434\u043d\u044f\u044f") penSize = 5;
        else penSize = 10;
    }

    void BtnSave_Click(Object^ sender, EventArgs^ e)
    {
        SaveFileDialog^ sfd = gcnew SaveFileDialog();
        sfd->Filter = L"PNG (*.png)|*.png";
        if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK && canvas->Image != nullptr)
        {
            canvas->Image->Save(sfd->FileName, Imaging::ImageFormat::Png);
            lblStatus->Text = L"\u0421\u043e\u0445\u0440\u0430\u043d\u0435\u043d\u043e";
        }
    }

    void BtnOpenImage_Click(Object^ sender, EventArgs^ e)
    {
        OpenFileDialog^ ofd = gcnew OpenFileDialog();
        ofd->Filter = L"PNG (*.png)|*.png";
        if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            delete canvas->Image;
            canvas->Image = Image::FromFile(ofd->FileName);
            lblStatus->Text = L"\u041e\u0442\u043a\u0440\u044b\u0442\u043e";
        }
    }
};
