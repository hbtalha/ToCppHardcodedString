#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QClipboard>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icons/app.png"));
    this->setWindowTitle("ToCppHardcodedString");

    ui->rawString_textEdit->setAcceptRichText(false);

    ui->clear_button->setIcon(style()->standardIcon(QStyle::SP_LineEditClearButton));
    connect(ui->clear_button, &QToolButton::clicked,  ui->rawString_textEdit, &QTextEdit::clear);
    connect(ui->clear_button, &QToolButton::clicked,  ui->usableString_textEdit, &QTextEdit::clear);

    ui->paste_button->setIcon(QIcon(":/icons/edit-paste.png"));
    connect(ui->paste_button, &QToolButton::clicked,  ui->rawString_textEdit, &QTextEdit::paste);

    ui->copy_button->setIcon(QIcon(":/icons/edit-copy.png"));
    connect(ui->copy_button, &QToolButton::clicked, this, [this]
    {
        auto text = ui->usableString_textEdit->toPlainText();
        if( ! text.isEmpty())
            QApplication::clipboard()->setText(text);
    });
    connect(ui->rawString_textEdit, &QTextEdit::textChanged, this, &MainWindow::translateString);
    connect(ui->addNewLineCheckBox, &QCheckBox::stateChanged, this, &MainWindow::translateString);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::translateString()
{
    QString rawString = ui->rawString_textEdit->toPlainText();

    QStringList lines = rawString.split("\n");

    for(int i = 0, len = lines.size(); i < len; ++i)
    {
        lines[i].replace("\\", "\\\\");
        lines[i].replace("\"", "\\\"");
        lines[i].push_front("\"");
        lines[i].push_back(( (i != (len - 1) && ui->addNewLineCheckBox->isChecked() ) ? "\\n\"" : "\""));
    }

    ui->usableString_textEdit->setPlainText(lines.join("\n"));
}

