#pragma once

#include "gui.h"


class EudoxeInjector : public QMainWindow, public Ui_SlnInjectorClass
{
    Q_OBJECT

public:
    EudoxeInjector(QWidget* parent = nullptr)
        : QMainWindow(parent), Ui_SlnInjectorClass()
    {
        setupUi(this);
    }
};
