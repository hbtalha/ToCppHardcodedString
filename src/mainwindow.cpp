#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icons/app.png"));
    this->setWindowTitle("ToCppHardcodedString");

    ui->copy_button->setIcon(QIcon(":/icons/edit-copy.png"));
    connect(ui->copy_button, &QToolButton::clicked, this, [this]
    {
        auto clipboard = QApplication::clipboard();
        auto text = ui->usableString_textEdit->toPlainText();
        if( ! text.isEmpty())
            clipboard->setText(text);
    });
    connect(ui->rawString_textEdit, &QTextEdit::textChanged, this, &MainWindow::translateString);
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
        lines[i].push_back(( (i == (len - 1)) ? "\"" : "\\n\""));
    }

    ui->usableString_textEdit->setText(lines.join("\n"));
}

