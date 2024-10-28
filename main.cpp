#include <iostream>
#include <string>
#include <cstring>

#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSlider>
#include <QIcon>
#include <QSpinBox>
#include <QTableWidget>
#include <QHeaderView>

#include "HistogramWidget.h"
#include "ImageManager.h"

/*-------------------------------------
              CSS STYLES
-------------------------------------*/

const QString MAIN_WINDOW_CSS_STYLE = QString::fromUtf8("background-color: #3b3b3b");
const QString IMAGE_LABEL_CSS_SYLE = QString::fromUtf8("background-color: #222222; border:5px solid #141414; border-style:ridge");
const QString ACTION_BUTTON_CSS_STYLE = QString::fromUtf8("background-color: #636363; border:2px solid #141414; border-style:ridge; padding:10px;");
const QString SPECIAL_ACTION_BUTTON_CSS_STYLE = QString::fromUtf8("background-color: #969696; border:2px solid #141414; border-style:ridge; padding:10px;");
const QString TABLE_CSS_STYLE = QString::fromUtf8("QTableWidget { border: 2px solid black; }"
                                                  "QTableWidget::item { border: 1px solid black; background-color: #969696}");

/*-------------------------------------
                MAIN
-------------------------------------*/

int main(int argc, char* argv[])
{
    ImageManager editor = ImageManager("test_images/Space_187k.jpg");

    QApplication app(argc, argv);

    QWidget main_window;
    QWidget convolutions_window; 
    QWidget histogram_window;

    /*-------------------------------------
            CONV WINDOW LAYOUTS
    -------------------------------------*/

    QHBoxLayout* main_conv_layout = new QHBoxLayout();
    QVBoxLayout* kernel_select_layout = new QVBoxLayout();
    QGridLayout* predefined_kernel_layout = new QGridLayout();

    /*-------------------------------------
            MAIN WINDOW LAYOUTS
    -------------------------------------*/

    QHBoxLayout* main_layout = new QHBoxLayout();
    QHBoxLayout* image_layout = new QHBoxLayout();
    QVBoxLayout* editor_layout = new QVBoxLayout();

    QGridLayout* push_button_layout = new QGridLayout();
    QGridLayout* special_button_layout = new QGridLayout();
    QGridLayout* misc_button_layout = new QGridLayout();

    /*-------------------------------------
              MAIN WINDOW BUTTONS
    -------------------------------------*/

    QSpinBox* spinbox_brightness = new QSpinBox;
    QSpinBox* spinbox_quantization = new QSpinBox;
    QDoubleSpinBox* spinbox_contrast = new QDoubleSpinBox;
    QPushButton* button_clockwise_rotation = new QPushButton("Rotate 90");
    QPushButton* button_counter_clockwise_rotation = new QPushButton("Rotate -90");
    QPushButton* button_vertical_mirror = new QPushButton("Vertical Mirror");
    QPushButton* button_horizontal_mirror = new QPushButton("Horizontal Mirror");
    QPushButton* button_grayscale = new QPushButton("Grayscale");
    QPushButton* button_save_new_image = new QPushButton("Save As");
    QPushButton* button_load_image = new QPushButton("Open File");
    QPushButton* button_reset_new_image = new QPushButton("Reset");
    QPushButton* button_negative = new QPushButton("Negative");
    QPushButton* button_convolution_menu = new QPushButton("Convolutions");
    QPushButton* button_histogram = new QPushButton("Histogram");

    /*-------------------------------------
              CONV WINDOW BUTTONS
    -------------------------------------*/

    QPushButton* conv_gaussian_button = new QPushButton("Gaussian");
    QPushButton* conv_laplacian_n_button = new QPushButton("Laplacian N");
    QPushButton* conv_laplacian_p_button = new QPushButton("Laplacian P");
    QPushButton* conv_high_boost_button = new QPushButton("High Boost");
    QPushButton* conv_prewitt_h_button = new QPushButton("Prewitt H");
    QPushButton* conv_prewitt_v_button = new QPushButton("Prewitt V");
    QPushButton* conv_sobel_h_button = new QPushButton("Sobel H");
    QPushButton* conv_sobel_v_button = new QPushButton("Sobel V");
    QPushButton* conv_clear_button = new QPushButton("Clear");
    QPushButton* conv_apply_button = new QPushButton("Apply");

    QTableWidget* editable_kernel_table = new QTableWidget(DIM, DIM);

    /*-------------------------------------
              MAIN WINDOW LABELS
    -------------------------------------*/

    QLabel* source_image_label = new QLabel();
    QLabel* new_image_label = new QLabel();
    QLabel* spinbox_brightness_label = new QLabel();
    QLabel* spinbox_quantization_label = new QLabel();

    /*-------------------------------------
              CONV WINDOW LABELS
    -------------------------------------*/

    QLabel* editable_kernel_label = new QLabel();
    QLabel* convolution_image_label = new QLabel();  

    /*-------------------------------------
                  QICONS
    -------------------------------------*/

    QIcon* icon_mirror_vertical = new QIcon("icons/mirror-vertical.svg");
    QIcon* icon_mirror_horizontal = new QIcon("icons/mirror-horizontal.svg");
    QIcon* icon_grayscale = new QIcon("icons/grayscale.svg");

    /*-------------------------------------
              INITIALIZE BUTTONS
    -------------------------------------*/

    spinbox_contrast->setMinimum(0);
    spinbox_contrast->setMaximum(255);
    spinbox_contrast->setValue(1);
    spinbox_contrast->setMinimumSize(QSize(300, 30));
    spinbox_contrast->setMaximumSize(QSize(900, 30));
    spinbox_contrast->setSingleStep(0.1);

    spinbox_brightness_label->setAlignment(Qt::AlignLeft);
    spinbox_brightness->setMinimumSize(QSize(300, 30));
    spinbox_brightness->setMaximumSize(QSize(900, 30));
    spinbox_brightness->setMaximum(255);
    spinbox_brightness->setMinimum(-255);
    spinbox_brightness->setValue(0);

    spinbox_quantization_label->setAlignment(Qt::AlignLeft);
    spinbox_quantization->setMinimumSize(QSize(300, 30));
    spinbox_quantization->setMaximumSize(QSize(900, 30));
    spinbox_quantization->setEnabled(false);
    spinbox_quantization->setMaximum(255);
    spinbox_quantization->setMinimum(1);
    spinbox_quantization->setValue(255);

    /*-------------------------------------
             CONV BUTTONS LAMBDAS
    -------------------------------------*/

    QObject::connect(conv_clear_button, &QPushButton::clicked, [&](){
        editor.setConvolutionKernel(NONE);
        editor.applyChanges();
        convolution_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(conv_apply_button, &QPushButton::clicked, [&](){
        convolutions_window.close();
        std::vector<std::vector<double>> kernel(DIM, std::vector<double>(DIM, 0));
        bool kernel_was_modified = false;

        for(int i =0 ; i < DIM; i++){
            for(int j = 0; j < DIM; j++){
                 QTableWidgetItem *item = editable_kernel_table->item(i, j);
                    kernel[i][j] = item->text().toDouble();
                    if(kernel[i][j] != 0) kernel_was_modified = true;
            }
        }

        if(kernel_was_modified){
            editor.setConvolutionKernel(kernel);
            editor.applyChanges();
        }

        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(conv_gaussian_button, &QPushButton::clicked, [&](){
        editor.setConvolutionKernel(GAUSSIAN_LOW_PASS);
        editor.applyChanges();
        convolution_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
        main_window.adjustSize();
    });

    QObject::connect(conv_laplacian_n_button, &QPushButton::clicked, [&](){
        editor.setConvolutionKernel(LAPLACIAN_HIGH_PASS_NEGATIVE);
        editor.applyChanges();
        convolution_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
        main_window.adjustSize();
    });

    QObject::connect(conv_laplacian_p_button, &QPushButton::clicked, [&](){
        editor.setConvolutionKernel(LAPLACIAN_HIGH_PASS_POSITIVE);
        editor.applyChanges();
        convolution_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(conv_high_boost_button, &QPushButton::clicked, [&](){
        editor.setConvolutionKernel(HIGH_BOOST);
        editor.applyChanges();
        convolution_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(conv_prewitt_h_button, &QPushButton::clicked, [&](){
        editor.setConvolutionKernel(PREWITT_HORIZONTAL);
        editor.applyChanges();
        convolution_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(conv_prewitt_v_button, &QPushButton::clicked, [&](){
        editor.setConvolutionKernel(PREWITT_VERTICAL);
        editor.applyChanges();
        convolution_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(conv_sobel_h_button, &QPushButton::clicked, [&](){
        editor.setConvolutionKernel(SOBEL_HORIZONTAL);
        editor.applyChanges();
        convolution_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(conv_sobel_v_button, &QPushButton::clicked, [&](){
        editor.setConvolutionKernel(SOBEL_VERTICAL);
        editor.applyChanges();
        convolution_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    /*-------------------------------------
               BUTTONS LAMBDAS
    -------------------------------------*/

    QObject::connect(spinbox_contrast, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [&](){
        editor.setContrastModifier(spinbox_contrast->value());
        editor.applyChanges();
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(button_clockwise_rotation, &QPushButton::clicked, [&](){
        editor.addClockwiseRotation();
        editor.applyChanges();
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(button_counter_clockwise_rotation, &QPushButton::clicked, [&](){
        editor.addCounterClockwiseRotation();
        editor.applyChanges();
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(button_histogram, &QPushButton::clicked, [&](){
        editor.generateNewImageHistogram();
        Histogram hist = editor.getNewImageHistogram();

        auto lum = hist.getRedChannel();

        QVector<double> data(lum.begin(), lum.end());
        HistogramWidget* histogram_widget = new HistogramWidget(data, nullptr);

        histogram_widget->resize(1200, 600);
        histogram_widget->show();
    });

    QObject::connect(button_convolution_menu, &QPushButton::clicked, [&](){
        convolution_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
        convolutions_window.show();
        convolutions_window.adjustSize();
    });

    QObject::connect(button_negative, &QPushButton::clicked, [&](){
        editor.setNegative();
        editor.applyChanges();
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(button_load_image, &QPushButton::clicked, [&](){
        QString source_file_path = QFileDialog::getOpenFileName(&main_window, 
            "Open File", "image", "All Files (*);;JPEG Files (*.jpeg);;PNG Files (*.png);;JPG Files (*.jpg)");
        
        editor.loadImage(source_file_path.toStdString());
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));        
        source_image_label->setPixmap(QPixmap::fromImage(editor.convertSourceToQImage()));
        main_window.adjustSize();
        main_window.show();
    });

    QObject::connect(spinbox_quantization, QOverload<int>::of(&QSpinBox::valueChanged), [&](){
        editor.setQuantization(spinbox_quantization->value());
        editor.applyChanges();
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(spinbox_brightness, QOverload<int>::of(&QSpinBox::valueChanged), [&](){
        int spinbox_value = spinbox_brightness->value();
        std::string label_text = "Brightness";

        editor.setBrightnessModifier(spinbox_value);
        editor.applyChanges();
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));        
    });

    QObject::connect(button_reset_new_image, &QPushButton::clicked, [&](){
        spinbox_brightness->setValue(0);
        spinbox_quantization->setValue(255);
        spinbox_quantization->setEnabled(editor.sourceImageIsGrayScale());
        editor.reset();
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(button_save_new_image, &QPushButton::clicked, [&](){
        QImage new_image;
        QString new_file_name = QFileDialog::getSaveFileName(&main_window, 
            "Save File", "new_image", "JPEG Files (*.jpeg);;All Files (*)");

        if(!new_file_name.isEmpty()){
            if(!new_file_name.endsWith(".jpeg", Qt::CaseInsensitive)){
                new_file_name += ".jpeg";
            }

            new_image = editor.convertNewImageToQImage();
            new_image.save(new_file_name, "JPEG");

            QMessageBox::information(&main_window, "File Saved", "The modified image has been saved.");
        }
    });

    QObject::connect(button_vertical_mirror, &QPushButton::clicked, [&editor, new_image_label](){
        editor.changeVerticalFlip();
        editor.applyChanges();
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(button_horizontal_mirror, &QPushButton::clicked, [&editor, new_image_label](){
        editor.changeHorizontalFlip();
        editor.applyChanges();
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    QObject::connect(button_grayscale, &QPushButton::clicked, [&editor, new_image_label, spinbox_quantization](){
        editor.setGrayScale();
        spinbox_quantization->setEnabled(true);
        editor.applyChanges();
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    });

    /*-------------------------------------
        INITIALIZE CONV EDITABLE KERNEL
    -------------------------------------*/ 

    editable_kernel_table->setStyleSheet(TABLE_CSS_STYLE);
    editable_kernel_table->horizontalHeader()->hide();
    editable_kernel_table->verticalHeader()->hide();
    editable_kernel_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    editable_kernel_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    editable_kernel_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for(int i = 0; i < DIM; i++){
        for(int j = 0; j < DIM; j++){
            QTableWidgetItem* cell = new QTableWidgetItem("0");
            cell->setTextAlignment(Qt::AlignCenter); 
            editable_kernel_table->setItem(i, j, cell);
        }
    }

    /*-------------------------------------
           ADD WIDGETS TO LAYOUTS (CONV)
    -------------------------------------*/

    predefined_kernel_layout->addWidget(conv_gaussian_button, 0, 0);
    predefined_kernel_layout->addWidget(conv_high_boost_button, 0, 1);
    predefined_kernel_layout->addWidget(conv_laplacian_n_button, 1, 0);
    predefined_kernel_layout->addWidget(conv_laplacian_p_button, 1, 1);
    predefined_kernel_layout->addWidget(conv_prewitt_h_button, 2, 0);
    predefined_kernel_layout->addWidget(conv_prewitt_v_button, 2, 1);
    predefined_kernel_layout->addWidget(conv_sobel_h_button, 3, 0);
    predefined_kernel_layout->addWidget(conv_sobel_v_button, 3, 1);
    predefined_kernel_layout->addWidget(conv_apply_button, 4, 0);
    predefined_kernel_layout->addWidget(conv_clear_button, 4, 1);

    /*-------------------------------------
             ADD WIDGETS TO LAYOUTS
    -------------------------------------*/

    push_button_layout->addWidget(button_vertical_mirror, 0, 0);
    push_button_layout->addWidget(button_horizontal_mirror, 0, 1);
    push_button_layout->addWidget(button_grayscale, 0, 2);
    push_button_layout->addWidget(button_negative, 1, 0);
    push_button_layout->addWidget(button_clockwise_rotation, 1, 1);
    push_button_layout->addWidget(button_counter_clockwise_rotation, 1, 2);

    special_button_layout->addWidget(spinbox_quantization, 0, 0);
    special_button_layout->addWidget(spinbox_quantization_label, 0, 1);
    special_button_layout->addWidget(spinbox_brightness, 1, 0);
    special_button_layout->addWidget(spinbox_brightness_label, 1, 1);
    special_button_layout->addWidget(spinbox_contrast, 2, 0);
    special_button_layout->addWidget(button_convolution_menu, 3, 0);
    special_button_layout->addWidget(button_histogram, 3, 1);
    
    misc_button_layout->addWidget(button_save_new_image, 0, 0);
    misc_button_layout->addWidget(button_load_image, 0, 1);
    misc_button_layout->addWidget(button_reset_new_image, 0, 2);

    image_layout->addWidget(source_image_label);
    image_layout->addWidget(new_image_label);

    /*-------------------------------------
            ADD LAYOUTS TO CONV WINDOW 
    -------------------------------------*/
    
    main_conv_layout->addWidget(convolution_image_label);
    main_conv_layout->addLayout(kernel_select_layout);
    kernel_select_layout->addLayout(predefined_kernel_layout);
    kernel_select_layout->addWidget(editable_kernel_label);
    kernel_select_layout->addWidget(editable_kernel_table);

    /*-------------------------------------
            ADD LAYOUTS TO MAIN WINDOW 
    -------------------------------------*/

    editor_layout->addLayout(push_button_layout);
    editor_layout->addLayout(special_button_layout);
    editor_layout->addLayout(misc_button_layout);

    main_layout->addLayout(image_layout);
    main_layout->addLayout(editor_layout);

    /*-------------------------------------
               INITIALIZE LABELS
    -------------------------------------*/

    source_image_label->setPixmap(QPixmap::fromImage(editor.convertSourceToQImage()));
    new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));
    spinbox_brightness_label->setText("Brightness: +0");
    spinbox_quantization_label->setText("Quantization");
        
    editable_kernel_label->setText("Custom Kernel");
    editable_kernel_label->setAlignment(Qt::AlignCenter);

    /*-------------------------------------
                    ICONS
    -------------------------------------*/

    button_grayscale->setIcon(*icon_grayscale);
    button_vertical_mirror->setIcon(*icon_mirror_vertical);
    button_horizontal_mirror->setIcon(*icon_mirror_horizontal);

    /*-------------------------------------
                  STYLESHEETS
    -------------------------------------*/
    
    main_window.setStyleSheet(MAIN_WINDOW_CSS_STYLE);    
    
    button_convolution_menu->setStyleSheet(SPECIAL_ACTION_BUTTON_CSS_STYLE);
    button_histogram->setStyleSheet(SPECIAL_ACTION_BUTTON_CSS_STYLE);

    button_grayscale->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    button_vertical_mirror->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    button_horizontal_mirror->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    button_negative->setStyleSheet(ACTION_BUTTON_CSS_STYLE);

    button_save_new_image->setStyleSheet(SPECIAL_ACTION_BUTTON_CSS_STYLE);
    button_reset_new_image->setStyleSheet(SPECIAL_ACTION_BUTTON_CSS_STYLE);
    button_load_image->setStyleSheet(SPECIAL_ACTION_BUTTON_CSS_STYLE);

    // labels

    source_image_label->setStyleSheet(IMAGE_LABEL_CSS_SYLE);
    new_image_label->setStyleSheet(IMAGE_LABEL_CSS_SYLE);

    /*-------------------------------------
            CONV WINDOW STYLESHEETS
    -------------------------------------*/

    convolutions_window.setStyleSheet(MAIN_WINDOW_CSS_STYLE);

    conv_gaussian_button->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    conv_laplacian_n_button->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    conv_laplacian_p_button->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    conv_high_boost_button->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    conv_prewitt_h_button->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    conv_prewitt_v_button->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    conv_sobel_h_button->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    conv_sobel_v_button->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    conv_apply_button->setStyleSheet(SPECIAL_ACTION_BUTTON_CSS_STYLE);
    conv_clear_button->setStyleSheet(SPECIAL_ACTION_BUTTON_CSS_STYLE);

    //editable_kernel_table->setStyleSheet(TABLE_CSS_STYLE);

    convolution_image_label->setStyleSheet(IMAGE_LABEL_CSS_SYLE);

    /*-------------------------------------
                MODIFY LAYOUTS    
    -------------------------------------*/


    /*-------------------------------------
            INITIALIZE CONV WINDOW
    -------------------------------------*/

    convolutions_window.setLayout(main_conv_layout);
    convolutions_window.setWindowTitle("Convolutions");

    /*-------------------------------------
            INITIALIZE MAIN WINDOW
    -------------------------------------*/

    main_window.setWindowTitle("Photoshop");
    main_window.setLayout(main_layout);
    main_window.adjustSize();
    main_window.show();

    return app.exec();

}