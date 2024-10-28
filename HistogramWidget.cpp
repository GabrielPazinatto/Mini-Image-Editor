#include "HistogramWidget.h"
#include <iostream>

HistogramWidget::HistogramWidget(const QVector<double>& data, QWidget* parent = nullptr) : QWidget(parent) { 
// Cria o conjunto de barras e adiciona os dados
    QBarSet *set = new QBarSet("Values");
    for (long double value : data) {
        value /= *std::max_element(data.begin(), data.end()) / 100;
        *set << value;
    }

    set->setColor(QColor(0, 0, 0)); 

    // Cria a série de barras e adiciona o conjunto de dados
    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Cria o gráfico e adiciona a série de barras
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Histograma");

    // Configuração do eixo Y (valores)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Valores");
    axisY->setTickCount(8);
    axisY->setRange(0, 100);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    std::cout << std::endl  << *std::max_element(data.begin(), data.end()) << std::endl;

    // Configuração do eixo X (índices)
    QValueAxis *axisX = new QValueAxis();

    axisX->setRange(0, 255);
    axisX->setTitleText("Índices");
    axisX->setTickCount(16);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Configuração da visualização do gráfico
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Configura o layout do widget e adiciona o gráfico
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);

    chartView->show();

}
