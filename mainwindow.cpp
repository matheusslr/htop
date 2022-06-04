#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sched.h>
#include <QFile>
#include <string>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->button_stop,SIGNAL(clicked(bool)),this,SLOT(stop()));
    connect(ui->button_kill,SIGNAL(clicked(bool)),this,SLOT(kill_process()));
    connect(ui->button_cont,SIGNAL(clicked(bool)),this,SLOT(cont()));
    connect(ui->input_filtro, &QLineEdit::textChanged, this, &MainWindow::filtro); // na forma antiga nao funciona
    connect(ui->button_prioridade,SIGNAL(clicked(bool)),this,SLOT(prioridade()));
    connect(ui->button_cpu,SIGNAL(clicked(bool)),this,SLOT(cpu()));

    QString grep = "";
    get_process(grep);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::get_process(QString grep)
{
    char path[256];
    getcwd(path,256);
    QString comando;
    if(grep==""){
        comando = "ps -aut > output.txt";
    }else{
        comando = "ps -aut | grep "+grep+" >output.txt";
    }
    std::string comandoString;
    comandoString= comando.toUtf8().constData();
    system(comandoString.c_str());
    QString caminho = QString(path) + "/output.txt";

    QFile file(caminho);
    QString output;

    if(file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        while(!in.atEnd()){
            QString line = in.readLine();
            output.append(line);
            output.append("\n");
        }
        file.close();
    }
    ui->textBrowser->setText(output);
}

void MainWindow::stop()
{
    QString PID =ui->input_PID->text();
    if(PID!=0){
        kill(PID.toInt(),SIGSTOP);
        qDebug() <<"ENTROU NO STOP"<<endl;
    }
}
void MainWindow::kill_process()
{
    QString PID =ui->input_PID->text();
    if(PID!=0){
        kill(PID.toInt(),SIGKILL);
        qDebug() <<"ENTROU NO KILL"<<endl;
    }
}
void MainWindow::cont()
{
    QString PID =ui->input_PID->text();
    if(PID!=0){
        kill(PID.toInt(),SIGCONT);
        qDebug() <<"ENTROU NO CONT"<<endl;
    }
}
void MainWindow::filtro()
{
    QString value =ui->input_filtro->text();
    get_process(value);
    qDebug() <<value<<endl;
}
void MainWindow::cpu()
{
    QString value =ui->input_cpu->text();
    QString PID =ui->input_PID->text();
    cpu_set_t  mask;  // máscara para habilitação de CPUs
    CPU_ZERO(&mask);
    CPU_SET(value.toInt(), &mask);    // alocar na CPU 0
    sched_setaffinity(PID.toInt(), sizeof(mask), &mask);
    qDebug() <<value<<endl;
}
void MainWindow::prioridade()
{
    QString value =ui->input_prioridade->text();
    QString PID =ui->input_PID->text();
    if(PID!=0){
        setpriority(PRIO_PROCESS,PID.toInt(),value.toInt());
        qDebug() <<value<<endl;
    }
}
