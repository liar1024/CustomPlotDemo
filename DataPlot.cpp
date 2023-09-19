#include "DataPlot.h"
DataGraph::DataGraph(QWidget *parent)
{

}

DataPlot::DataPlot(QWidget *parent) : QWidget(parent)
{
}

void DataPlot::initDataPlot(QCustomPlot *customPlotype)
{
    m_customPlot = customPlotype;
    m_customPlot->legend->setVisible(true);
    m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight | Qt::AlignTop);

    m_customPlot->xAxis->setLabel("X");
    m_customPlot->yAxis->setLabel("Y");
    m_customPlot->yAxis2->setLabel("Y1");
    m_customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    m_customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    m_customPlot->yAxis2->setUpperEnding(QCPLineEnding::esSpikeArrow);


    m_customPlot->yAxis2->setVisible(true);//显示y轴2

    connect(m_customPlot, &QCustomPlot::mouseMove, this, &DataPlot::handleMouseMovement);

    m_customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8)); //设置x坐标轴显示的字体
    m_customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8)); //设置y坐标轴显示的字体

    connect(m_customPlot, &QCustomPlot::mousePress, this, &DataPlot::onMouseSelected); //信号：鼠标按下，槽：绘图窗口的

    m_customPlot->replot(); //刷新绘图窗口
}

void DataPlot::setDataLabel(QLabel* label)
{
    m_dataLabel = label;

    m_vTracerLine = new QCPItemStraightLine(m_customPlot); //初始化游标参考线
    m_vTracerLine->setPen(QPen(Qt::DashLine)); //设置参考线样式为虚线
    m_vTracerLine->point1->setCoords(0, 1000.0); //两点一线，设置参考线初始位置点
    m_vTracerLine->point2->setCoords(0, -1000.0);
}

void DataPlot::addDataGraph(QVector<double> &xAxisV, QVector<double> &yAxisV, QString graphCode, YAxisZoomMode code)
{
    DataGraph* dataGraph;
    if (code == YAxisZoomMode::LeftYAxisZoom)
    {
        dataGraph = new DataGraph;
        dataGraph->graph = m_customPlot->addGraph(); /**<绘图控件的图层 */
    }
    else if(code == YAxisZoomMode::RightYAxisZoom)
    {
        dataGraph = new DataGraph;
        dataGraph->graph = m_customPlot->addGraph(m_customPlot->xAxis, m_customPlot->yAxis2); /**<绘图控件的图层 */
    }
    else
    {
        //TODO
    }
    dataGraph->graph->setName(graphCode);

    if (xAxisV.isEmpty() || yAxisV.isEmpty()) //若绘图点集数据为空则返回，不绘制
    {
        return;
    }

    QPen pen; /**<绘图画笔 */
    QColor randColor = QColor::fromHsv((rand()%359+rand()%359)%359, 255, 200); /**<随机颜色 */
    pen.setColor(randColor); //设置画笔颜色

    dataGraph->graph->setPen(pen); //向图层设置画笔

    dataGraph->graph->setLineStyle(QCPGraph::lsNone);
    dataGraph->graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));


    dataGraph->graph->addData(xAxisV, yAxisV); //添加绘图数据
    dataGraph->graph->rescaleAxes(); //依据数据范围，自适应绘制窗口大小

    m_customPlot->setMouseTracking(true); //设置鼠标跟随

    dataGraph->tracer = new QCPItemTracer(m_customPlot); /**<游标 */

    dataGraph->tracer->setPen(QPen(Qt::DashLine)); //设置游标画笔为虚线
    dataGraph->tracer->setStyle(QCPItemTracer::tsCircle); //设置游标样式为圆圈

    dataGraph->tracer->setInterpolating(false); //ture：游标插值，false：定位最近点
    dataGraph->tracer->position->setType(QCPItemPosition::ptPlotCoords); //设置游标按坐标系位置
    dataGraph->tracer->setGraph(dataGraph->graph); //向图层添加游标

    dataGraph->yName = graphCode;

    m_dataGraphMap.insert(graphCode, dataGraph);

    m_customPlot->replot();
}

void DataPlot::deleteGraph(QString graphCode)
{
    m_customPlot->removeGraph(m_dataGraphMap.value(graphCode)->graph);  //移除指定图层

    m_dataGraphMap.value(graphCode)->tracer->setGraph(nullptr); //释放指定图层的游标
    m_dataGraphMap.value(graphCode)->tracer->setStyle(QCPItemTracer::tsNone); //设置游标样式为不显示

    delete m_dataGraphMap.value(graphCode);
    m_dataGraphMap.remove(graphCode);

    m_customPlot->replot();
}

void DataPlot::clearGraph()
{
    if (m_customPlot->graphCount() != 0) //判断图层数量是否为0
    {
        m_customPlot->clearGraphs();

        QList<DataGraph*> graphList = m_dataGraphMap.values();
        for (int i = 0; i < graphList.length(); ++i) //移除全部游标
        {
            graphList[i]->tracer->setGraph(nullptr);
            graphList[i]->tracer->setStyle(QCPItemTracer::tsNone);
            delete  graphList[i];
        }

        m_dataGraphMap.clear();

        m_customPlot->replot();
    }
}

void DataPlot::handleMouseMovement(QMouseEvent *event)
{
    m_xKey = m_customPlot->xAxis->pixelToCoord(event->pos().x());

    m_vTracerLine->point1->setCoords(m_xKey, 1000.000);
    m_vTracerLine->point2->setCoords(m_xKey, -1000.000);

    m_lablelText = QString::number(m_xKey,'f',3) + "\n";
    m_dataLabel->setText(m_lablelText); //刷新显示控件的内容

    m_customPlot->replot();
}

void DataPlot::onMouseSelected(QMouseEvent *event)
{
    m_customPlot->axisRect()->setRangeZoomFactor(1.1); //设置窗口内横纵坐标同比例缩放

    if (event->button() == Qt::LeftButton) //判断鼠标按下的键值
    {
        m_customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmNone); //禁用框选放大
        m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables); //设置拖拽、放大、选择
    }
    else if (event->button() == Qt::RightButton)
    {
        m_customPlot->setInteraction(QCP::iRangeDrag,false); //禁用拖拽
        m_customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom); //设置框选放大
    }
}

void DataPlot::setZoomInteractionMode(YAxisZoomMode mode)
{
    if (mode == YAxisZoomMode::LeftYAxisZoom)
    {
        m_customPlot->axisRect()->setRangeZoomAxes(m_customPlot->xAxis, m_customPlot->yAxis); // 指定X轴和Y轴可以被选择
    }
    else
    {
        m_customPlot->axisRect()->setRangeZoomAxes(m_customPlot->xAxis, m_customPlot->yAxis2); // 指定X轴和Y轴可以被选择

    }
}
