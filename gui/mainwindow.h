#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProcess>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void compileAndRun();
    void saveImage();
    void displayImage();
    void runGnuplot();

private:
    Ui::MainWindow *ui;
    QLabel *imageLabel;
    QProcess *compileProcess;
    QProcess *gnuplotProcess;
    QCheckBox *checkbox2D;
    QCheckBox *checkbox3D;
    QLineEdit *outputDirectoryLineEdit;
    QLineEdit *folderNameLineEdit;

    QHBoxLayout* createXmlButton(const QString &label);
    QHBoxLayout* createDatFileButton(); // Declaration for .dat file selection button
    QHBoxLayout* createOutputDirectoryButton(); // Declaration for output directory selection button
    void resetImageToBlack();
    void copyFilesFor3DPlot(const QString &destination);
};

#endif // MAINWINDOW_H
