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

#include "ImageManager.h"
#include "ImageEditing.h"

/*-------------------------------------
              CSS STYLES
-------------------------------------*/

const QString MAIN_WINDOW_CSS_STYLE = QString::fromUtf8("background-color: #3b3b3b");
const QString IMAGE_LABEL_CSS_SYLE = QString::fromUtf8("background-color: #222222; border:5px solid #141414; border-style:ridge");
const QString ACTION_BUTTON_CSS_STYLE = QString::fromUtf8("background-color: #636363; border:2px solid #141414; border-style:ridge; padding:10px;");
const QString SPECIAL_ACTION_BUTTON_CSS_STYLE = QString::fromUtf8("background-color: #969696; border:2px solid #141414; border-style:ridge; padding:10px;");

/*-------------------------------------
                MAIN
-------------------------------------*/

int main(int argc, char* argv[])
{
    ImageManager editor = ImageManager("test_images/Space_187k.jpg");

    QApplication app(argc, argv);

    QWidget main_window;
 
    QHBoxLayout* main_layout = new QHBoxLayout();
    QHBoxLayout* image_layout = new QHBoxLayout();
    QVBoxLayout* editor_layout = new QVBoxLayout();

    QGridLayout* push_button_layout = new QGridLayout();
    QGridLayout* special_button_layout = new QGridLayout();
    QGridLayout* misc_button_layout = new QGridLayout();

    QSlider* slider_brightness = new QSlider(Qt::Horizontal);
    QSpinBox* spinbox_quantization = new QSpinBox;

    QPushButton* button_vertical_mirror = new QPushButton("Vertical Mirror");
    QPushButton* button_horizontal_mirror = new QPushButton("Horizontal Mirror");
    QPushButton* button_grayscale = new QPushButton("Grayscale");
    QPushButton* button_save_new_image = new QPushButton("Save As");
    QPushButton* button_load_image = new QPushButton("Open File");
    QPushButton* button_reset_new_image = new QPushButton("Reset");


    QLabel* source_image_label = new QLabel();
    QLabel* new_image_label = new QLabel();
    QLabel* slider_brightness_label = new QLabel();
    QLabel* spinbox_quantization_label = new QLabel();

    slider_brightness_label->setAlignment(Qt::AlignLeft);
    slider_brightness->setMinimumSize(QSize(300, 10));
    slider_brightness->setMaximumSize(QSize(900, 15));

    QIcon* icon_mirror_vertical = new QIcon("icons/mirror-vertical.svg");
    QIcon* icon_mirror_horizontal = new QIcon("icons/mirror-horizontal.svg");
    QIcon* icon_grayscale = new QIcon("icons/grayscale.svg");

    /*-------------------------------------
              INITIALIZE BUTTONS
    -------------------------------------*/

    slider_brightness->setMaximum(255);
    slider_brightness->setMinimum(-255);
    slider_brightness->setValue(0);

    spinbox_quantization->setEnabled(false);
    spinbox_quantization->setMaximum(255);
    spinbox_quantization->setMinimum(1);
    spinbox_quantization->setValue(255);

    /*-------------------------------------
               BUTTONS LAMBDAS
    -------------------------------------*/

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

    QObject::connect(slider_brightness, &QSlider::valueChanged, [&](){
        int slider_value = slider_brightness->value();
        std::string label_text = "Brightness: ";

        if(slider_value >= 0) label_text += "+";
        label_text += std::to_string(slider_value);
        slider_brightness_label->setText(QString(label_text.c_str()));
        
        editor.setBrightnessModifier(slider_value);
        editor.applyChanges();
        new_image_label->setPixmap(QPixmap::fromImage(editor.convertNewImageToQImage()));        
    });

    QObject::connect(button_reset_new_image, &QPushButton::clicked, [&](){
        slider_brightness->setValue(0);
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
             ADD WIDGETS TO LAYOUTS
    -------------------------------------*/

    push_button_layout->addWidget(button_vertical_mirror, 0, 0);
    push_button_layout->addWidget(button_horizontal_mirror, 0, 1);
    push_button_layout->addWidget(button_grayscale, 0, 2);
    
    special_button_layout->addWidget(spinbox_quantization, 0, 0);
    special_button_layout->addWidget(spinbox_quantization_label, 0, 1);
    special_button_layout->addWidget(slider_brightness, 1, 0);
    special_button_layout->addWidget(slider_brightness_label, 1, 1);
    
    misc_button_layout->addWidget(button_save_new_image, 0, 0);
    misc_button_layout->addWidget(button_load_image, 0, 1);
    misc_button_layout->addWidget(button_reset_new_image, 0, 2);

    image_layout->addWidget(source_image_label);
    image_layout->addWidget(new_image_label);

    /*-------------------------------------
             ADD LAYOUTS TO WINDOW 
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
    slider_brightness_label->setText("Brightness: +0");
    spinbox_quantization_label->setText("Quantization");
        
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
    button_grayscale->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    button_vertical_mirror->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    button_horizontal_mirror->setStyleSheet(ACTION_BUTTON_CSS_STYLE);
    button_save_new_image->setStyleSheet(SPECIAL_ACTION_BUTTON_CSS_STYLE);
    button_reset_new_image->setStyleSheet(SPECIAL_ACTION_BUTTON_CSS_STYLE);
    button_load_image->setStyleSheet(SPECIAL_ACTION_BUTTON_CSS_STYLE);

    // labels

    source_image_label->setStyleSheet(IMAGE_LABEL_CSS_SYLE);
    new_image_label->setStyleSheet(IMAGE_LABEL_CSS_SYLE);
    
    /*-------------------------------------
              INITIALIZE WINDOW
    -------------------------------------*/

    main_window.setWindowTitle("Photoshop");
    main_window.setLayout(main_layout);
    main_window.adjustSize();
    main_window.show();

    return app.exec();

}