
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QVector>
#include <QWidget>
#include <QVBoxLayout>

class HistogramWidget : public QWidget {
    Q_OBJECT

public:
    HistogramWidget(const QVector<double>& data, QWidget* parent); 
};

