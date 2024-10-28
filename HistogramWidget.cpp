#include "HistogramWidget.h"
#include <iostream>

HistogramWidget::HistogramWidget(const QVector<double>& data, QWidget* parent = nullptr) : QWidget(parent) { 
// Cria o conjunto de barras e adiciona os dados 
    long double total = 0;

    QBarSet *set = new QBarSet("Values");
    for (int i = 0 ; i < data.size(); i++){
        *set << data[i];

        if(i % 2 == 0)
            set->setColor(QColor(255, 0, 0));
        else
            set->setColor(QColor(0, 0, 255));

        total += data[i];
    }

    for(int i = 0; i < data.size(); i++){
        set->replace(i, set->at(i) / total);
    }


    // Cria a série de barras e adiciona o conjunto de dados
    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->setBarWidth(1);

    // Cria o gráfico e adiciona a série de barras
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Histograma");

    // Configuração do eixo Y (valores)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Valores");
    axisY->setTickCount(8);
    axisY->setRange(0, *std::max_element(data.begin(), data.end())/total);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Configuração do eixo X (índices)
    QValueAxis *axisX = new QValueAxis();

    axisX->setRange(0, 255);
    axisX->setTitleText("Índices");
    axisX->setTickCount(18);
    axisX->setTickInterval(16);
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
