#include "mainwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    ptrPlayProcess = new QProcess(0);
    ptrExportProcess = new QProcess(0);

    this->resize(600, 240);
    this->setAcceptDrops(true);

    ptrChoiceFileButton = new QPushButton(QStringLiteral("选择音频文件"), this);
    ptrRaisePitchButton = new QPushButton(QStringLiteral("升一调"), this);
    ptrDropPitchButton = new QPushButton(QStringLiteral("降一调"), this);
    ptrFilePathLineEdit = new QLineEdit(this);
    ptrExportButton = new QPushButton(QStringLiteral("导出"), this);
    ptrPitchLabel = new QLabel("0", this);
    ptrPlayButton = new QPushButton(QStringLiteral("播放"), this);
    ptrStopButton = new QPushButton(QStringLiteral("停止"), this);

    ptrPitchLabel->setAlignment(Qt::AlignCenter);
    ptrPitchLabel->setStyleSheet("QLabel {"
                                 "  background-color: rgb(192, 192, 192);"
                                 "}");

    ptrChoiceFileButton->setGeometry(10, 5, 100, 25);
    ptrFilePathLineEdit->setGeometry(150, 5, 300, 25);
    ptrExportButton->setGeometry(470, 5, 75, 25);
    ptrRaisePitchButton->setGeometry(10, 40, 75, 25);
    ptrDropPitchButton->setGeometry(95, 40, 75, 25);
    ptrPitchLabel->setGeometry(190, 40, 75, 25);
    ptrPlayButton->setGeometry(10, 75, 160, 25);
    ptrStopButton->setGeometry(180, 75, 160, 25);

    ptrFilePathLineEdit->setFocusPolicy(Qt::NoFocus);

    connect(ptrChoiceFileButton, &QPushButton::clicked, this, &MainWindow::clickedChoiceFileButton);
    connect(ptrRaisePitchButton, &QPushButton::clicked, this, &MainWindow::clickedRaisePitchButton);
    connect(ptrDropPitchButton, &QPushButton::clicked, this, &MainWindow::clickedDropPitchButton);
    connect(ptrPlayButton, &QPushButton::clicked, this, &MainWindow::clickedPlayButton);
    connect(ptrStopButton, &QPushButton::clicked, this, &MainWindow::clickedStopButton);
    connect(ptrExportButton, &QPushButton::clicked, this, &MainWindow::clickedExportButton);
}

MainWindow::~MainWindow()
{
    ptrPlayProcess->close();
    ptrExportProcess->close();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        // 如果数据包含urls
        event->acceptProposedAction();    // 接收事件
    }
    else {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    qDebug() << "dropEvent";
    const QMimeData *mimeData = event->mimeData();    // 获取mime数据
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();    // 获取url列表
        filePath = urlList.at(0).toLocalFile();
        ptrFilePathLineEdit->setText(filePath);
        pitch = 0;
        ptrPitchLabel->setText(QString::number(pitch));
    }
}

void MainWindow::clickedChoiceFileButton()
{
    filePath = QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"), NULL, QStringLiteral("allFile (*.*)"));
    ptrFilePathLineEdit->setText(filePath);
}

void MainWindow::clickedRaisePitchButton()
{
    pitch += 1;
    ptrPitchLabel->setText(QString::number(pitch));
}

void MainWindow::clickedDropPitchButton()
{
    pitch -= 1;
    ptrPitchLabel->setText(QString::number(pitch));
}

void MainWindow::clickedPlayButton()
{
    if (filePath.isEmpty()) {
        QMessageBox::warning(NULL, QStringLiteral("警告"), QStringLiteral("未选择音频文件"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    else {
        ptrPlayProcess->start(QString("sox-14.4.1\\sox.exe %1 -d pitch %2").arg(filePath).arg(pitch * 200));    // pitch * 200: 音调换算成音分
    }
}

void MainWindow::clickedStopButton()
{
    ptrPlayProcess->close();
}

void MainWindow::clickedExportButton()
{
    QString tmpFilePath = filePath;
    tmpFilePath.insert(tmpFilePath.lastIndexOf("."), QString("2"));
    if (filePath.isEmpty()) {
        QMessageBox::warning(NULL, QStringLiteral("警告"), QStringLiteral("未选择音频文件"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
//    qDebug() << QString("sox-14.4.1\\sox -S %1 %2 pitch %3").arg(filePath).arg(tmpFilePath).arg(QString::number(pitch));
    qDebug() << pitch;
    ptrExportProcess->start(QString("sox-14.4.1\\sox -S %1 %2 pitch %3").arg(filePath).arg(tmpFilePath).arg(QString::number(pitch * 100)));    // pitch * 100: 将音调换算成音分
    ptrExportProcess->waitForFinished();    // stop here until finished the convert
    QMessageBox::information(NULL, QStringLiteral("信息"), QStringLiteral("导出完成"), QMessageBox::Yes, QMessageBox::Yes);
}
