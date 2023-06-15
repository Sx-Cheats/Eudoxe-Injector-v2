#pragma once

#ifndef UI_SLNINJECTOR_H
#define UI_SLNINJECTOR_H

#include "Includes.h"


QT_BEGIN_NAMESPACE

class QAppMovable : public QWidget
{
    QMainWindow* MainWindow;
    QPoint Base;

public:

    QAppMovable(QMainWindow* _MainWindow, QWidget* parent, QRect Geometry) : QWidget(parent), MainWindow(_MainWindow) {
        Base = pos(); setFocusPolicy(Qt::ClickFocus); setGeometry(Geometry); installEventFilter(this);};

protected :

    void mousePressEvent(QMouseEvent* event)
    {
        if (event->buttons() == Qt::MouseButton::LeftButton)
            Base = event->globalPos();
    }

    void mouseMoveEvent(QMouseEvent* event)
    {
        if (event->buttons() == Qt::MouseButton::LeftButton)
        {
            MainWindow->move((MainWindow->pos() + event->globalPos()) - Base);
            Base = event->globalPos();

        }
    }
}; 



class Ui_SlnInjectorClass
{
public:

    QSettings * settings;
    QButton   * BPInject;

    typedef struct __op__options_injector
    {
        bool HiJackHandle = false;
        bool RandomFileName = false;
        bool UseCodeCave = false;
        bool RemoveFromPEB = false;
        int Method = 0;
        int Status = 0;
        int FunctionToUse = 0;
        float Timeout = 5.0f;
        DWORD Pid = 0;

    } Option_Injector;

     struct eudoxe_injector_options
    {
         Option_Injector InjectorOptions;
         bool AutoInject;
         std::string DllPath;

    } EudoxeInjectorOption;

    Ui_SlnInjectorClass() 
    {
        settings  = new QSettings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    };

    enum Settings
    {
         AUTO_INJECT,
         LOAD_DLL_COPY,
         HIJACK_HANDLE,
         RANDOM_FILE_NAME,
         CLOAK_THREAD,
         REMOVE_FROM_PEB,
         USE_CODECAVE,
         LOADER,
         DLL_PATH,
         PROCESS,
         INJECT_METHOD,
         TIMEOUT,
    };

    template<typename T>
    void SaveSettings(Settings  type, T value)
    {
        settings->setValue(QString::number((int)type),value);
    };

    QVariant GetSettings(Settings type)
    {
      return settings->value(QString::number((int)type));
    }

    QString ToCorrectFormat(QString str)
    {
        if (str.length() > 1)
            return str;
        else
            return "";
    }

    void setupUi(QMainWindow* MainWindow)
    {
        bool IsAdmin = (bool)(RunAs::IsRunningLikeWho() == RunAs::Acc::Admin);
       
        MainWindow->resize(370, 510);
        MainWindow->setWindowFlag(Qt::FramelessWindowHint);
        MainWindow->setAttribute(Qt::WA_TranslucentBackground);

        QWidget* CentralWidget = QtComponents::AddWidget(MainWindow);
     
        MainWindow->setCentralWidget(CentralWidget);
        MainWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        CentralWidget->setObjectName("centralwidget");
        CentralWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
        CentralWidget->setMinimumWidth(366);

        QAppMovable * titlebar = new QAppMovable (MainWindow,CentralWidget, { 2,2,366,35 });
        titlebar->setObjectName("TitleBar");

        if (!IsAdmin)
        {
            QtComponents::Icon(titlebar, "./Logos/syringe.png", {17,17}, {10,6});
            QtComponents::AddText(titlebar, "Eudoxe Injector", 15, true, false, { 40 ,6,170,21 })->setObjectName("Title");

        } else if (IsAdmin)
        {
            QtComponents::Icon(titlebar,  "./Logos/shield.png", {17,17}, {10,6});
            QtComponents::AddText(titlebar, "Eudoxe Injector", 15, true, false, { 40 ,6,170,21 })->setObjectName("Title");
       
        }
        
        QtComponents::AddButton(titlebar, "", 20, true, { 340,10,13,13 })->click([&] {qApp->exit(); })->Button->setObjectName("BPClose");
        QtComponents::AddButton(titlebar, "", 20, true, { 316,10,13,13 })->click([&, MainWindow] {; MainWindow->showMinimized(); })->Button->setObjectName("BPMinimize");


        QTabWidget* tabwidget = QtComponents::AddTabWidget(CentralWidget, { 2,35,370,477 });
        tabwidget->setObjectName("tabWidget");


        QWidget* InjectorSection = QtComponents::AddWidget(tabwidget);
        InjectorSection->setObjectName("injector");
        tabwidget->addTab(InjectorSection, QIcon("./Logos/syringe.png"), "Injector");



        QWidget* OptionsSection = QtComponents::AddWidget(tabwidget);
        tabwidget->addTab(OptionsSection, QIcon("./Logos/settings.png"), "Options");

        // Injection Section

        QtComponents::AddText(InjectorSection, "dll path", 12, false, true, { 35,30,49,16 });
        auto DllPath = QtComponents::AddLineEdit(InjectorSection, false, { 40, 50, 250, 30 });

        QString dllpathf = ToCorrectFormat(GetSettings(Settings::DLL_PATH).toString());

        DllPath->Input->setText(dllpathf);
        EudoxeInjectorOption.DllPath = dllpathf.toStdString();

        DllPath->WhenEditFinish([&, DllPath] {

            EudoxeInjectorOption.DllPath = DllPath->Input->text().toStdString();

            SaveSettings(Settings::DLL_PATH, DllPath->Input->text());

            });
        
        QtComponents::BpIcon(InjectorSection, "./Logos/folder.png", { 35,35 }, { 300,40 })->click([&, DllPath, MainWindow] {
            QString directorypath(NTapi::GetDirPathFromPathA(DllPath->Input->text().toStdString()).c_str());

            if(!QDir(directorypath).exists())
                directorypath  = QDir::currentPath();

            QString dllpath = QFileDialog::getOpenFileName((QWidget*)DllPath->Input, QObject::tr("Open File"),directorypath,QObject::tr("Library (*.dll)"));

            if (!dllpath.isEmpty())
            {
                EudoxeInjectorOption.DllPath = dllpath.toStdString();

                DllPath->Input->setText(dllpath);

                SaveSettings(Settings::DLL_PATH, dllpath);
            }

        })->Button->setObjectName("BPSearchDll");


        QtComponents::AddText(InjectorSection, "process", 12, false, true, { 35,110,49,16 });
        auto Process = QtComponents::AddLineEdit(InjectorSection, true, { 40, 130, 250, 25 });
        
        
       QBIcon* BPGetProcess = QtComponents::BpIcon(InjectorSection, "./Logos/feature.png", { 35,35 }, { 300,120 });

       BPGetProcess->Button->setObjectName("BPSearchProcess");

       QRect mrec = MainWindow->geometry();
       
       QWidget* PanelSearchProcess = QtComponents::AddWidget(CentralWidget, { 2,mrec.y() + 35,mrec.width()-4,mrec.height()-37});

       PanelSearchProcess->hide();

       PanelSearchProcess->setObjectName("BackgroundSelectProcess");

       QtComponents::AddText(PanelSearchProcess, "process name", 12, false, true, { 35,10,80,16 });

       QInputBox * ProcessName = QtComponents::AddLineEdit(PanelSearchProcess, false, { 48, 30, 250, 30 });
     
       QtComponents::AddButton(PanelSearchProcess, "Cancel", 16, true, { 195,mrec.height() - 100,110,30 })->click([&, PanelSearchProcess] {
          
           PanelSearchProcess->hide();

           })->Button->setObjectName("CancelProcess");


       QListView* ListProcess = QtComponents::AddListView(PanelSearchProcess, { 48,75,255,315 });
       ListProcess->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       ListProcess->setFont(QtComponents::CurrentFont(9,false,false,"Segoe UI"));
       ListProcess->setObjectName("ListProcess");

       ListProcess->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
       ListProcess->setMovement(QListView::Static);
       ListProcess->setLayoutMode(QListView::Batched);
       ListProcess->setViewMode(QListView::ListMode);
       ListProcess->setResizeMode(QListView::Fixed);
       ListProcess->setResizeMode(QListView::Adjust);
       ListProcess->setSelectionRectVisible(false);
       ListProcess->setBatchSize(650);
                                                                                                                                                                      
       QtComponents::AddButton(PanelSearchProcess, "Open", 16, true, { 50,mrec.height() - 100,110,30 })->click([&, Process, ListProcess, PanelSearchProcess]{

           QString ProcessSelected = ListProcess->currentIndex().data().toString();

           EudoxeInjectorOption.InjectorOptions.Pid = ListProcess->currentIndex().data().toString().split(" -")[0].toShort(); 
           
           Process->Input->setText(ProcessSelected);

           PanelSearchProcess->hide();

           })->Button->setObjectName("OpenProcess");

       BPGetProcess->click([&, PanelSearchProcess, ListProcess] {

           QtComponents::ProcessView(ListProcess,0);
           PanelSearchProcess->show();

           });
                                                          
       ProcessName->WhenTextEdited([&, ListProcess, ProcessName](const QString& text)
           {
               QSortFilterProxyModel* sortprocess = (QSortFilterProxyModel*)ListProcess->model();
               sortprocess->setFilterRegularExpression(ProcessName->Input->text());
           });

        QtComponents::AddText(InjectorSection, "Inject Method", 12, false, true, { 35,195,80,16 });
        QtComponents::AddComboBox(InjectorSection, { 45,220,250,30 })->AddItems({"CreateRemoteThread","NtCreateThreadEx","QueueUserAPC","ThreadHiJacking","RtlCreateUserThread"})->WhenIndexChanged([&](int index) {
            
            EudoxeInjectorOption.InjectorOptions.Method = index;

            SaveSettings(Settings::INJECT_METHOD, index);

            })->ComboBox->setCurrentIndex(GetSettings(Settings::INJECT_METHOD).toInt());

        QtComponents::AddText(InjectorSection, "Timeout (second)", 12, false, true, { 35,275,100,16 });

        QtComponents::AddDoubleSpinBox(InjectorSection, 5.0f, 30.0f, 5.0f, 1, { 45,305,120,24 })->WhenValueChanged([&](double value) {
           
            EudoxeInjectorOption.InjectorOptions.Timeout = value;
            
            SaveSettings(Settings::TIMEOUT, value);
            
            })->SpinBox->setValue(GetSettings(Settings::TIMEOUT).toDouble());

        BPInject = QtComponents::AddButton(InjectorSection, "  Inject", 20, true, { 110,360,150,45 });
        BPInject->Button->setIcon(QIcon("./Logos/syringe.png"));
        BPInject->Button->setIconSize({ 25, 25 });
        BPInject->Button->setObjectName("BPInject");

        if (!IsAdmin)
        {
            QButton* DBRestartAsAdmin = QtComponents::AddButton(OptionsSection, " Restart as Admin", 12, false, { 215,205,130,30 });
            DBRestartAsAdmin->Button->setIcon(QIcon("./Logos/shield.png"));
            DBRestartAsAdmin->Button->setObjectName("BPRestartAdmin");
            DBRestartAsAdmin->click([&] {
                if (RunAs::StartAs(RunAs::Acc::Admin));
                exit(0);
                });
        };

        // Options Section

        QtComponents::AddText(OptionsSection, "Loader", 12, false, true, { 55,25,100,16 });

        QtComponents::AddComboBox(OptionsSection, { 60,50,250,30 })->AddItems({ "LoadLibraryExA","LoadLibraryExW"})->WhenIndexChanged([&](int index) {
            EudoxeInjectorOption.InjectorOptions.FunctionToUse = index;
            SaveSettings(Settings::LOADER, index);
            })->ComboBox->setCurrentIndex(GetSettings(Settings::LOADER).toInt());



        QtComponents::AddCheckBox(OptionsSection, "Random File Name", false, { 20, 125, 155, 20 })->WhenStateChanged([&](int state) {
            EudoxeInjectorOption.InjectorOptions.RandomFileName = (bool)state;
            SaveSettings(Settings::LOAD_DLL_COPY, state);
            })->CheckBox->setChecked(GetSettings(Settings::LOAD_DLL_COPY).toBool());

        QtComponents::AddCheckBox(OptionsSection, "Hijack Handle", false,    { 215, 125, 138, 20 })->WhenStateChanged([&](int state) {
            EudoxeInjectorOption.InjectorOptions.HiJackHandle = (bool)state;
            SaveSettings(Settings::HIJACK_HANDLE, state);
            })->CheckBox->setChecked(GetSettings(Settings::HIJACK_HANDLE).toBool());


        QtComponents::AddCheckBox(OptionsSection, "Remove from PEB", false, { 20, 165, 165, 20 })->WhenStateChanged([&](int state) {
            EudoxeInjectorOption.InjectorOptions.RemoveFromPEB = (bool)state;
            SaveSettings(Settings::REMOVE_FROM_PEB, state);
            })->CheckBox->setChecked(GetSettings(Settings::REMOVE_FROM_PEB).toBool());

           QtComponents::AddCheckBox(OptionsSection, "Use CodeCave", false, { 215, 165, 155, 20 })->WhenStateChanged([&](int state) {
                EudoxeInjectorOption.InjectorOptions.UseCodeCave = (bool)state;
                SaveSettings(Settings::USE_CODECAVE, state);
                })->CheckBox->setChecked(GetSettings(Settings::USE_CODECAVE).toBool());


                QtComponents::Icon(OptionsSection, "./Logos/Eudoxe.jpg", { 155,185 }, { 20,205 });

        tabwidget->setStyleSheet(QMENU_STYESHEET);
        CentralWidget->setStyleSheet(INJECTOR_STYLESHEET);
    }
};


QT_END_NAMESPACE

#endif

