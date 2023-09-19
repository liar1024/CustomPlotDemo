#ifndef DATAPLOT_H
#define DATAPLOT_H

#include <QWidget>
#include <QObject>
#include <QMouseEvent>

#include "qcustomplot.h"

enum YAxisZoomMode {
        LeftYAxisZoom,
        RightYAxisZoom
    };

class DataPlot;

class DataGraph : public QWidget
{
    friend class DataPlot;
public:
    explicit DataGraph(QWidget *parent = nullptr);

private:
    QCPGraph* graph;
    QCPItemTracer* tracer;
    QString yName;
};


class DataPlot : public QWidget
{
    Q_OBJECT
public:
    explicit DataPlot(QWidget *parent = nullptr);

    /**
     * @brief initDataPlot 初始化绘图控件，及绘图模式
     * @param customPlot 绘图控件
     * @param plotType 绘图模式
     */
    void initDataPlot(QCustomPlot* customPlotype);

    /**
     * @brief setDataLabel 初始化游标数据显示的控件
     * @param label 数据显示控件
     */
    void setDataLabel(QLabel* label);

    /**
     * @brief addDataGraph 添加绘制图层,传入数据点集并绘制
     * @param xAxisV x轴数据集
     * @param yAxisV y轴数据集
     */
    void addDataGraph(QVector<double>& xAxisV, QVector<double>& yAxisV, QString graphCode, YAxisZoomMode code);

    /**
     * @brief deleteGraph 移除指定绘制图层
     * @param graphCode 图层名称
     */
    void deleteGraph(QString graphCode);

    /**
     * @brief clearGraph 清空绘图控件
     */
    void clearGraph();

    /**
     * @brief handleMouseMovement 游标跟随鼠标移动，刷新游标数据
     * @param event 鼠标事件
     */
    void handleMouseMovement(QMouseEvent* event);

    void onMouseSelected(QMouseEvent* event);

    void setZoomInteractionMode(YAxisZoomMode mode);

signals:

private:
    QCustomPlot* m_customPlot; /**<绘图的控件 */

    QCPItemStraightLine* m_vTracerLine; /**<游标参考线 */

    double m_xKey; /**<鼠标移动时，横坐标轴的数值 */

    QString m_lablelText;

    QLabel* m_dataLabel; /**<游标数据显示的控件 */

    QMap<QString, DataGraph*> m_dataGraphMap; //QMap<图层名称， 图层指针>


};

#endif // DATAPLOT_H
