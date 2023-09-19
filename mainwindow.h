#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "DataPlot.h"
#include "SwitchButton.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_setXYBtn_clicked();

    void on_setXY1Btn_clicked();

    void onSwitchToggled(bool checked);

private:
    Ui::MainWindow *ui;

    DataPlot m_datePlot;
};
#endif // MAINWINDOW_H
