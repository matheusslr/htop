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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connect(ui->retry_btn,SIGNAL(clicked(bool)),this,SLOT(reniciar()));
    connect(ui->button_stop,SIGNAL(clicked(bool)),this,SLOT(stop()));
    connect(ui->button_kill,SIGNAL(clicked(bool)),this,SLOT(kill_process()));
    connect(ui->button_cont,SIGNAL(clicked(bool)),this,SLOT(cont()));
    //connect(ui->input_filtro,SIGNAL(textChanged(Qstring)),this,SLOT(cont()));
    connect(ui->input_filtro, &QLineEdit::textChanged, this, &MainWindow::filtro); // na forma antiga nao funciona
    //connect(ui->input_cpu, &QLineEdit::textChanged, this, &MainWindow::cpu);
    //connect(ui->input_prioridade, &QLineEdit::textChanged, this, &MainWindow::prioridade);
    connect(ui->button_prioridade,SIGNAL(clicked(bool)),this,SLOT(prioridade()));
    connect(ui->button_cpu,SIGNAL(clicked(bool)),this,SLOT(cpu()));

    get_process();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::get_process()
{
    char path[256];
    getcwd(path,256);
    system("ps -aut > output.txt");
    QString caminho = QString(path) + "/output.txt";
    QFile file(caminho);
    QTextStream saida_ps(&file);
    QString saida = saida_ps.readAll();

    qDebug()<<saida;
    system("pwd");
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
    //cpuset_t();
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
