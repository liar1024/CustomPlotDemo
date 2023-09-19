#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_datePlot.initDataPlot(ui->qcustomPlot);
    m_datePlot.setDataLabel(ui->xAxisLabel);
    ui->qcustomPlot->setMouseTracking(true);
    connect(ui->switchControl, &SwitchControl::toggled, this, &MainWindow::onSwitchToggled);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_setXYBtn_clicked()
{
    // 生成数据，画出的是抛物线
    QVector<double> x(101), y(101); //初始化向量x和y
    for (int i = 0; i < 101; ++i)
    {
      x[i] = i/50.0 - 1; // x范围[-1,1]
      y[i] = x[i]*x[i]; // y=x*x
    }
    m_datePlot.addDataGraph(x, y, "x_y坐标", YAxisZoomMode::LeftYAxisZoom);
}

void MainWindow::on_setXY1Btn_clicked()
{
    QVector<double> x(101), y(101); //初始化向量x和y
    for (int i = 0; i < 101; ++i)
    {
      x[i] = i; // x范围[-1,1]
      y[i] = i; // y=x*x
    }
    m_datePlot.addDataGraph(x, y, "x_y1坐标", YAxisZoomMode::RightYAxisZoom);
}

void MainWindow::onSwitchToggled(bool checked)
{
    if (checked)
    {
        m_datePlot.setZoomInteractionMode(YAxisZoomMode::RightYAxisZoom);
    }
    else
    {
        m_datePlot.setZoomInteractionMode(YAxisZoomMode::LeftYAxisZoom);
    }

}
