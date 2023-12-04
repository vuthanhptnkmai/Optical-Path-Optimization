#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    imageLabel(new QLabel(this)),
    compileProcess(new QProcess(this)),
    gnuplotProcess(new QProcess(this)),
    checkbox2D(new QCheckBox("2D Image", this)),
    checkbox3D(new QCheckBox("3D Plot", this)),
    outputDirectoryLineEdit(new QLineEdit(this)),
    folderNameLineEdit(new QLineEdit(this))
{
    ui->setupUi(this);
    this->setFixedSize(1000, 500);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addLayout(createXmlButton("Select XML File"));
    leftLayout->addLayout(createOutputDirectoryButton());
    leftLayout->addSpacing(50);

    QPushButton *startButton = new QPushButton("Start Process", this);
    leftLayout->addWidget(startButton);
    leftLayout->addSpacing(50);

    QHBoxLayout *saveLayout = new QHBoxLayout();
    folderNameLineEdit->setPlaceholderText("Enter folder name");
    QPushButton *saveButton = new QPushButton("Save", this);
    saveLayout->addWidget(folderNameLineEdit);
    saveLayout->addWidget(saveButton);
    leftLayout->addLayout(saveLayout);
    leftLayout->addSpacing(1);

    QHBoxLayout *checkboxLayout = new QHBoxLayout();
    checkboxLayout->addWidget(checkbox2D);
    checkboxLayout->addWidget(checkbox3D);
    leftLayout->addLayout(checkboxLayout);

    QLabel *Description = new QLabel(
        "click replot in gnuplot for reload the data from new simulation\n"
        "\nManual for saving results \n"
        "Select Output Directory where you want to store the result of the simulation \n"
        "check the boxes whether you want to save 2D and/or 3D plot data \n"
        "enter a name and click save \n"
        "checked plot datas will be stored in the folder under your selected output directory \n"
        "──Select Output Directory\n"
        "      └─Folder name\n"
        "            ├─2D Image\n"
        "            └─3D Plot\n"
    , this);
    leftLayout->addWidget(Description);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::compileAndRun);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);

    // Image label setup
    imageLabel->setFixedSize(512, 512);
    resetImageToBlack();

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(imageLabel);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);




    // Debugging: Capture output and errors from compile process
    connect(compileProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        qDebug() << "Compile Output:" << compileProcess->readAllStandardOutput();
    });
    connect(compileProcess, &QProcess::readyReadStandardError, this, [this]() {
        qDebug() << "Compile Error:" << compileProcess->readAllStandardError();
    });

    // Debugging: Capture output and errors from Gnuplot process
    connect(gnuplotProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        qDebug() << "Gnuplot Output:" << gnuplotProcess->readAllStandardOutput();
    });
    connect(gnuplotProcess, &QProcess::readyReadStandardError, this, [this]() {
        qDebug() << "Gnuplot Error:" << gnuplotProcess->readAllStandardError();
    });

    // Debugging: Check when processes start and finish
    connect(compileProcess, &QProcess::started, []() {
        qDebug() << "Compile process started.";
    });
    connect(compileProcess, &QProcess::finished, this, &MainWindow::displayImage);
    connect(gnuplotProcess, &QProcess::started, []() {
        qDebug() << "Gnuplot process started.";
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete imageLabel;
    delete compileProcess;
    delete gnuplotProcess;
}

QHBoxLayout* MainWindow::createXmlButton(const QString &label) {
    QPushButton *button = new QPushButton(label, this);
    QLineEdit *lineEdit = new QLineEdit(this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(button);
    layout->addWidget(lineEdit);
    connect(button, &QPushButton::clicked, this, [this, lineEdit]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Select XML File", "", "XML Files (*.xml)");
        lineEdit->setText(fileName);
        if (!fileName.isEmpty()) {
            QString destination = "../../data/components.xml";
            if (QFile::exists(destination)) {
                QFile::remove(destination);
            }
            if (!QFile::copy(fileName, destination)) {
                QMessageBox::warning(this, "Copy Failed", "Failed to copy the selected file.");
            }
        }
    });
    return layout;
}

QHBoxLayout* MainWindow::createOutputDirectoryButton() {
    QPushButton *button = new QPushButton("Select Output Directory", this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(button);
    layout->addWidget(outputDirectoryLineEdit);
    connect(button, &QPushButton::clicked, this, [this]() {
        QString dir = QFileDialog::getExistingDirectory(this, "Select Directory", "");
        outputDirectoryLineEdit->setText(dir);
    });
    return layout;
}

void MainWindow::saveImage() {
    QString outputDir = outputDirectoryLineEdit->text();
    QString folderName = folderNameLineEdit->text();
    if (!outputDir.isEmpty() && !folderName.isEmpty()) {
        QString newFolderPath = outputDir + "/" + folderName;
        QDir().mkdir(newFolderPath);

        if (checkbox2D->isChecked()) {
            QFile::copy("../../visualization/detector_output.png", newFolderPath + "/detector_output.png");
        }
        if (checkbox3D->isChecked()) {
            copyFilesFor3DPlot(newFolderPath);
        }
    }
}

void MainWindow::resetImageToBlack()
{
    QPixmap blackPixmap(512, 512);
    blackPixmap.fill(Qt::black);
    imageLabel->setPixmap(blackPixmap);
}

void MainWindow::copyFilesFor3DPlot(const QString &destination)
{
    QStringList filesToCopy = {"plot.gp", "components.dat", "rays.dat", "circle_data.txt"};
    for (const QString &file : filesToCopy) {
        QFile::copy("../../visualization/" + file, destination + "/" + file);
    }
}

void MainWindow::compileAndRun()
{
    resetImageToBlack();
    // Start the compilation process

    compileProcess->start("g++", QStringList() << "-fopenmp" << "../../thirdParty/tinyxml2/tinyxml2.cpp" << "../../main.cpp" << "-o" << "../../a.exe");
    //compileProcess->start("make", QStringList() <<"run");

    // Connect signals to handle the output and completion of the process
    connect(compileProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        qDebug() << "Compile Output:" << compileProcess->readAllStandardOutput();
    });
    connect(compileProcess, &QProcess::readyReadStandardError, this, [this]() {
        qDebug() << "Compile Error:" << compileProcess->readAllStandardError();
    });
    connect(compileProcess, &QProcess::finished, this, &MainWindow::displayImage);


}

void MainWindow::displayImage()
{
    // Display the image after compilation
    QPixmap pixmap("../../visualization/detector_output.png");
    imageLabel->setPixmap(pixmap);
    imageLabel->setScaledContents(true);

    // Run the Gnuplot script
    runGnuplot();
}

void MainWindow::runGnuplot()
{
    QString gnuplotScriptPath = "../../visualization/plot.gp";
    QString workingDirectory = QFileInfo(gnuplotScriptPath).absolutePath();

    gnuplotProcess->setWorkingDirectory(workingDirectory);
    gnuplotProcess->start("\"C:/Program Files/gnuplot/bin/gnuplot.exe\"", QStringList() << QFileInfo(gnuplotScriptPath).fileName());

    // Debugging: Check if the process starts and capture any errors
    connect(gnuplotProcess, &QProcess::started, []() {
        qDebug() << "Gnuplot process started.";
    });
    connect(gnuplotProcess, &QProcess::readyReadStandardError, this, [this]() {
        qDebug() << "Gnuplot Error:" << gnuplotProcess->readAllStandardError();
    });
}


