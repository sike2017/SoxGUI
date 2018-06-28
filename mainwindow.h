#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProcess>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPushButton *ptrChoiceFileButton;
    QPushButton *ptrRaisePitchButton;
    QPushButton *ptrDropPitchButton;
    QLineEdit *ptrFilePathLineEdit;
    QPushButton *ptrExportButton;
    QLabel *ptrPitchLabel;
    QPushButton *ptrPlayButton;
    QPushButton *ptrStopButton;
private slots:
    void clickedChoiceFileButton();
    void clickedRaisePitchButton();
    void clickedDropPitchButton();
    void clickedPlayButton();
    void clickedStopButton();
    void clickedExportButton();
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    QString filePath = "";
    int pitch = 0;
    QProcess *ptrPlayProcess;
    QProcess *ptrExportProcess;
};

#endif // MAINWINDOW_H
