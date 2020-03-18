#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <chrono>
#include <QFileDialog>
#include <thread>
#include "movie.h"
#include "store.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionExit_triggered()      // exit
{    
    QApplication::exit();
}

void MainWindow::on_actionLoad_triggered()      // load
{
    QString fileName = QFileDialog::getOpenFileName(this, "", "", "*.mp4");
    ui->listWidget->addItem( "Имя файла = "+fileName );
}

void MainWindow::on_actionStart_triggered()     // start
{
    Store temp;
    temp.clear();
    std::cout << "Start !" << std::endl;
    auto thrFunc = [](const std::string &fileName)
    {
        Movie movie( fileName );
        movie.calc();
    };

    auto thrOutConsole = []()
    {
        Store s;
        while(true)
        {
            std::cout << "Size of detections = " << s.countBboxes() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };
    std::thread thrConsole( thrOutConsole );
    thrConsole.detach();

    std::vector< std::thread > arrThr;
    for(auto i=0; i < ui->listWidget->count(); ++i)
    {
        arrThr.emplace_back( std::thread( thrFunc, ui->listWidget->item(i)->text().toStdString() ) );
    }
    for(std::thread &thr: arrThr)
    {
        thr.join();
    }
    std::cout << "Finish !" << std::endl;
}
