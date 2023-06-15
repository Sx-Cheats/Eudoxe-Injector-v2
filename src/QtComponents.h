
#pragma once

#include "./NTapi/NTapi.h"
#include "Includes.h"

#define CONCACT_MACRO_CALL(c,_c) c  _c

#define  UI_RGB(r,g,b) "rgb("#r","#g","#b");\n"

#define  UI_RGB_ALPHA(r,g,b,a) "rgba(" #r "," #g "," #b "," #a ");\n"

#define TEXT_COLOR(color) ((QString)(CONCACT_MACRO_CALL("color : ", color)))

#define UI_MARGIN(top,right,bottom,left) "margin : " #top "px " #right "px " #bottom "px " #left "px;\n"

#define UI_TOP_MARGIN(top)       ((QString)("margin-top     : "  #top    "px;\n"))
#define UI_RIGHT_MARGIN(right)   ((QString)("margin-right   : "  #right  "px;\n"))
#define UI_BOTTOM_MARGIN(bottom) ((QString)("margin-bottom  : "  #bottom "px;\n"))
#define UI_LEFT_MARGIN(left)     ((QString)("margin-left    : "  #left   "px;\n"))

#define UI_PADDING(top,right,bottom,left) ((QString)("padding : " #top "px " #right "px " #bottom "px " #left "px;\n"))

#define UI_TOP_PADDING(top)       ((QString)("padding-top     : "  #top    "px;\n"))
#define UI_RIGHT_PADDING(right)   ((QString)("padding-right   : "  #right  "px;\n"))
#define UI_BOTTOM_PADDING(bottom) ((QString)("padding-bottom  : "  #bottom "px;\n"))
#define UI_LEFT_PADDING(left)     ((QString)("padding-left    : "  #left   "px;\n"))


#define UI_BACKGROUND_COLOR(color)  ((QString)(CONCACT_MACRO_CALL("background-color : ", color)))

#define LETTER_SPACING(spacing) ((QString)("letter-spacing : "  #spacing "px;\n"))


#define BORDER(width,color) ((QString)(CONCACT_MACRO_CALL("border : " #width "px solid ", color)))

#define BORDER_TOP(width,color) (((QString) CONCACT_MACRO_CALL("border-top       : " #width "px solid ", color)))
#define BORDER_RIGHT(width,color) (((QString)CONCACT_MACRO_CALL("border-right   : " #width "px solid ", color)))
#define BORDER_BOTTOM(width,color) (((QString)CONCACT_MACRO_CALL("border-bottom : " #width "px solid ", color)))
#define BORDER_LEFT(width,color) (((QString) CONCACT_MACRO_CALL("border-left     : " #width "px solid ", color)))

#define BORDER_NULL ((QString)"border : none;border-top:none;border-right:none;border-bottom:none;border-left:none;")


#define BORDER_RADIUS(radius)   ((QString)("border-radius  : "  #radius  "px;\n"))

#define BORDER_RADIUS_TOP_LEFT(radius)      ((QString)("border-top-left-radius      : "  #radius  "px;\n"))
#define BORDER_RADIUS_TOP_RIGHT(radius)     ((QString)("border-top-right-radius     : "  #radius  "px;\n"))
#define BORDER_RADIUS_BOTTOM_RIGHT(radius)  ((QString)("border-bottom-right-radius  : "  #radius  "px;\n"))
#define BORDER_RADIUS_BOTTOM_LEFT(radius)   ((QString)("border-bottom-left-radius   : "  #radius  "px;\n"))

#define MAX_SIZE 16777215


struct QButton
{
	QPushButton * Button;
	QButton(QPushButton* bp) : Button(bp) {};

	template<typename cevent>
	QButton* click(cevent callback)
	{
		QObject::connect(Button, &QAbstractButton::clicked, callback);

		return this;
	}

	QButton * SetFont(BYTE size, bool bold)
	{
		QFont font;
		font.setPointSize(size);
		font.setBold(bold);
		Button->setFont(font);

		return this;
	}
};


struct QInputBox
{
	QLineEdit * Input;

	QInputBox(QLineEdit* input) : Input(input) {};

	template<typename cevent>
	QInputBox* WhenInput(cevent callback)
	{
		QObject::connect(Input, &QLineEdit::textChanged, callback);
		return this;
	}

	template<typename cevent>
	QInputBox* WhenTextEdited(cevent callback)
	{
		QObject::connect(Input, &QLineEdit::textEdited, callback);
		return this;
	}

	template<typename cevent>
	QInputBox* WhenTextChanged(cevent callback)
	{
		QObject::connect(Input, &QLineEdit::textChanged, callback);
		return this;
	}

	template<typename cevent>
	QInputBox* WhenEditFinish(cevent callback)
	{
		QObject::connect(Input, &QLineEdit::editingFinished, callback);

		return this;
	}

}; 


struct QDoubleNumberBox
{
	QDoubleSpinBox* SpinBox;

	QDoubleNumberBox(QDoubleSpinBox* spinbox) : SpinBox(spinbox) {};

	template<typename cevent>
	QDoubleNumberBox* WhenValueChanged(cevent callback)
	{
		QObject::connect(dynamic_cast<QDoubleSpinBox*>(SpinBox), &QDoubleSpinBox::valueChanged, callback);

		return this;
	}
};

struct QtDoubleSpinBox
{
	QDoubleSpinBox *  SpinBox;

	QtDoubleSpinBox(QDoubleSpinBox *  spinbox) : SpinBox(spinbox) {};

	template<typename cevent>
	QtDoubleSpinBox * WhenValueChanged(cevent callback)
	{
		QObject::connect(SpinBox, &QDoubleSpinBox::valueChanged, callback);

		return this;
	}
};


struct QtCheckBox
{
	QCheckBox* CheckBox;
	QtCheckBox(QCheckBox* checkbox) : CheckBox(checkbox) {};

	template<typename cevent>
	QtCheckBox* WhenStateChanged(cevent callback)
	{
		QObject::connect(CheckBox, &QCheckBox::stateChanged, callback);
		return this;
	}
};


struct QDropdownBox
{
	QComboBox * ComboBox;
	QDropdownBox(QComboBox * combobox) : ComboBox(combobox) {};

	template<typename cevent>
	QDropdownBox * WhenIndexChanged(cevent callback)
	{
		QObject::connect(ComboBox, &QComboBox::currentIndexChanged, callback);
		return this;
	}

	template<typename cevent>
	QDropdownBox * WhenTextChanged(cevent callback)
	{
		QObject::connect(ComboBox, &QComboBox::currentTextChanged, callback);
		return this;
	}

	QDropdownBox * AddItem(QString item)
	{
		ComboBox->addItem(item);
		return this;
	}

	QDropdownBox * AddItems(QStringList items)
	{
		ComboBox->addItems(items);
		return this;
	}
};

struct QBIcon : public QButton
{
	QBIcon(QPushButton* bp) : QButton(bp) {};

	QBIcon* SwitchIcon(QString Path)
	{
		Button->setIcon(QIcon(Path));
		return this;
	}
};

QT_BEGIN_NAMESPACE

class QtComponents : public QObject
{
public:

	static QFont CurrentFont(WORD Size = 12,bool IsBold = false,bool IsItalic=false,QString Font = "Bahnschrift SemiLight Condensed")
	{
		QFont f;
		f.setFamilies({Font});
		f.setPointSize(Size);
		f.setBold(IsBold);
		f.setItalic(IsItalic);

		return f;
	}


	static QLabel* AddText(QWidget* parent,QString text,WORD Size=12, bool Bold=false,bool IsItalic = false, QRect Geometry = { 0,0,0,0 }, QSize MinSize = { 0,0 }, QSize MaxSize = { MAX_SIZE , MAX_SIZE })
	{
		QLabel* t = new QLabel(parent);
		t->setText(text);
		t->setFont(QtComponents::CurrentFont(Size, Bold, IsItalic));
		t->setGeometry(Geometry);
		t->setMinimumSize(MinSize);
		t->setMaximumSize(MaxSize);
		t->show();

		return t;
	}

	static QWidget* AddWidget(QWidget* parent, QRect Geometry = { 0,0,0,0 }, QSize MinSize = { 0,0 }, QSize MaxSize = { MAX_SIZE , MAX_SIZE })
	{
		QWidget* widget = new QWidget(parent);
		widget->setGeometry(Geometry);
		widget->setMinimumSize(MinSize);
		widget->setMaximumSize(MaxSize);
		widget->show();
		return widget;
	}	

	static QAction* AddActionMenu(QMenu* parent, QString ActionText, bool Checkable = false, QString IconPath = "")
	{
		QAction* action = new QAction(parent);
		action->setText(ActionText);
		if (Checkable)
			action->setCheckable(Checkable);
		else  if (!Checkable)
			action->setIcon(QIcon(IconPath));

		parent->addAction(action);

		return action;
	}

	static QButton * AddButton(QWidget* parent,QString Text,WORD TSize=12, bool TBold=false, QRect Geometry = { 0,0,0,0 }, QSize MinSize = { 0,0 }, QSize MaxSize = { MAX_SIZE , MAX_SIZE })
	{
		QPushButton * button = new QPushButton(parent);
		button->setGeometry(Geometry);
		button->setMinimumSize(MinSize);
		button->setMaximumSize(MaxSize);
		button->setFont(QtComponents::CurrentFont());
		button->setText(Text);
		button->setFocusPolicy(Qt::StrongFocus);
		button->setFont(QtComponents::CurrentFont(TSize, TBold));
		button->show();
		return new QButton(button);

	}

	static QtCheckBox * AddCheckBox(QWidget* parent,QString Text, bool IsChecked = false, QRect Geometry = { 0,0,0,0 }, QSize MinSize = { 0,0 }, QSize MaxSize = { MAX_SIZE , MAX_SIZE })
	{
		QCheckBox * checkbox = new QCheckBox(parent);
		checkbox->setGeometry(Geometry);
		checkbox->setMinimumSize(MinSize);
		checkbox->setMaximumSize(MaxSize);
		checkbox->setChecked(IsChecked);
		checkbox->setText(Text);
		checkbox->setFont(QtComponents::CurrentFont(12,true,false));

		return new QtCheckBox(checkbox);

	}

	static QDropdownBox * AddComboBox(QWidget* parent, QRect Geometry = { 0,0,0,0 }, QSize MinSize = { 0,0 }, QSize MaxSize = { MAX_SIZE , MAX_SIZE })
	{
		QComboBox* ComboBox = new QComboBox(parent);
		ComboBox->setGeometry(Geometry);
		ComboBox->setMinimumSize(MinSize);
		ComboBox->setMaximumSize(MaxSize);
		ComboBox->setFont(QtComponents::CurrentFont());
		ComboBox->view()->window()->setWindowFlags(ComboBox->view()->window()->windowFlags() | Qt::NoDropShadowWindowHint);
		ComboBox->show();
		return new QDropdownBox(ComboBox);
	}

	static QInputBox * AddLineEdit(QWidget* parent,bool IsReadOnly = false, QRect Geometry = { 0,0,0,0 }, QSize MinSize = { 0,0 }, QSize MaxSize = { MAX_SIZE , MAX_SIZE })
	{
		QLineEdit * LineEdit = new QLineEdit(parent);
		LineEdit->setGeometry(Geometry);
		LineEdit->setMinimumSize(MinSize);
		LineEdit->setMaximumSize(MaxSize);
		LineEdit->setReadOnly(IsReadOnly);
		LineEdit->setFont(QtComponents::CurrentFont());
		LineEdit->show();
		return new QInputBox(LineEdit);
	}


	static QTabWidget * AddTabWidget(QWidget* parent, QRect Geometry = { 0,0,0,0 }, QSize MinSize = { 0,0 }, QSize MaxSize = { MAX_SIZE , MAX_SIZE })
	{
		QTabWidget* tabwidget = new QTabWidget(parent);
		tabwidget->setGeometry(Geometry);
		tabwidget->setMinimumSize(MinSize);
		tabwidget->setMaximumSize(MaxSize);
		tabwidget->setFont(QtComponents::CurrentFont(12, false,false,"Bahnschrift SemiBold"));
		tabwidget->show();

		return tabwidget;
	}

	static void ProcessView(QListView * listview,BYTE arch = 3)
	{
		if (listview == nullptr)
			return;

		EnumObj<PSYSTEM_PROCESS_INFORMATION> * SysProcess = NTapi::GetAllProcess();

		QFileIconProvider qq{};

		QSortFilterProxyModel * sortprocess = (QSortFilterProxyModel*)listview->model();

		QStandardItemModel * model = (QStandardItemModel*)sortprocess->sourceModel();

		model->clear();
		
		do

		{
			if ((DWORD)SysProcess->element->UniqueProcessId == (DWORD)NTapi::Teb.ClientId.UniqueProcess)
				continue;

			
			if(arch != 3)
				if (NTapi::IsWow64((DWORD)SysProcess->element->UniqueProcessId) != arch)
					continue;

			std::string ProcessPath = NTapi::GetProcessPath((DWORD)SysProcess->element->UniqueProcessId);
			
			QIcon icon =  qq.icon(QFileInfo(ProcessPath.c_str()));

			QString process_name = QString::number((DWORD)SysProcess->element->UniqueProcessId, 10);
			
			process_name += (" - " + (NTapi::UTF16toUTF8(SysProcess->element->ImageName.Buffer))).c_str();

			QStandardItem* item = new QStandardItem(icon, process_name);

			item->setTextAlignment(Qt::AlignCenter);

			model->appendRow(item);

			

		} while ((SysProcess = SysProcess->Next)->Next); 

		sortprocess->sortOrder();
	}

	static QListView * AddListView(QWidget* parent, QRect Geometry = { 0,0,0,0 }, QSize MinSize = { 0,0 }, QSize MaxSize = { MAX_SIZE , MAX_SIZE })
	{
		QListView * listview = new QListView(parent);
		listview->setGeometry(Geometry);
		listview->setMinimumSize(MinSize);
		listview->setMaximumSize(MaxSize);

		QStandardItemModel* model = new QStandardItemModel(listview);
		listview->setModel(model);

		QSortFilterProxyModel * proxyModel = new QSortFilterProxyModel();
		proxyModel->setSourceModel(model);

		// Enable sorting on the proxy model
		proxyModel->setSortCaseSensitivity(Qt::CaseSensitive);
		proxyModel->setSortRole(Qt::DisplayRole);
		proxyModel->sort(0, Qt::AscendingOrder);

		listview->setModel(proxyModel);

		listview->setEditTriggers(QAbstractItemView::NoEditTriggers);
		listview->show();

		return listview;
	}


	static QtDoubleSpinBox* AddDoubleSpinBox(QWidget* parent, double MinValue = 0.000, double MaxValue = 99.000, double StartValue = 1.00, double StepValue = 1.000, QRect Geometry = { 0,0,0,0 }, QSize MinSize = { 0,0 }, QSize MaxSize = { MAX_SIZE , MAX_SIZE })
	{
		QDoubleSpinBox * SpinBox = new QDoubleSpinBox(parent);
		SpinBox->setGeometry(Geometry);
		SpinBox->setValue(StartValue);
		SpinBox->setMinimumSize(MinSize);
		SpinBox->setMaximumSize(MaxSize);
		SpinBox->setMinimum(MinValue);
		SpinBox->setMaximum(MaxValue);
		SpinBox->setSingleStep(StepValue);
		SpinBox->setDecimals(3);
		SpinBox->setValue(StartValue);

		return new QtDoubleSpinBox(SpinBox);
		
	}

	static QBIcon* BpIcon(QWidget* parent, QString Path, QSize IconSize = { 32,32 }, QPoint Position = { 0,0 })
	{
		QPushButton* Icon = new QPushButton(parent);
		Icon->setObjectName(QString::fromUtf8("IconButton"));
		Icon->setGeometry(QRect(Position.x(), Position.y(), IconSize.width(), IconSize.height()));
		Icon->setMinimumSize(IconSize);
		Icon->setMaximumSize(IconSize);
		Icon->setIcon(QIcon(Path));
		Icon->setIconSize(IconSize - QSize(12, 12));
		Icon->setCursor(QCursor(Qt::PointingHandCursor));
		Icon->setFlat(false);
		Icon->show();
		return new QBIcon(Icon);
	}

	static QBIcon* BpIcon(QWidget* parent, QIcon Icon, QSize IconSize = { 32,32 }, QPoint Position = { 0,0 })
	{
		QPushButton* IIcon = new QPushButton(parent);
		IIcon->setObjectName(QString::fromUtf8("IconButton"));
		IIcon->setGeometry(QRect(Position.x(), Position.y(), IconSize.width(), IconSize.height()));
		IIcon->setMinimumSize(IconSize);
		IIcon->setMaximumSize(IconSize);
		IIcon->setIcon(Icon);
		IIcon->setIconSize(IconSize - QSize(10, 10));
		IIcon->setCursor(QCursor(Qt::PointingHandCursor));
		IIcon->setFlat(false);
		IIcon->show();
		return new QBIcon(IIcon);
	}

	static QLabel* Icon(QWidget* parent, QString Path, QSize IconSize = { 32,32 }, QPoint Position = { 0,0 })
	{
		QLabel * Icon = new QLabel(parent);
		Icon->setObjectName(QString::fromUtf8("IconButton"));
		Icon->setGeometry(QRect(Position.x(), Position.y(), IconSize.width(), IconSize.height()));
		Icon->setMinimumSize(IconSize);
		Icon->setMaximumSize(IconSize);
		Icon->setPixmap(QPixmap(Path));
		Icon->setScaledContents(true);
		Icon->show();
		return Icon;
	}

	static QLabel* Icon(QWidget* parent, QIcon Icon, QSize IconSize = { 32,32 }, QPoint Position = { 0,0 })
	{
		QLabel* IIcon = new QLabel(parent);
		IIcon->setObjectName(QString::fromUtf8("IconButton"));
		IIcon->setGeometry(QRect(Position.x(), Position.y(), IconSize.width(), IconSize.height()));
		IIcon->setMinimumSize(IconSize);
		IIcon->setMaximumSize(IconSize);
		IIcon->setPixmap(Icon.pixmap(Icon.availableSizes(QIcon::Normal)[0]));
		IIcon->setScaledContents(true);
		IIcon->show();
		return IIcon;
	}

};





#define INJECTOR_STYLESHEET R"(
#centralwidget
{
background-color: rgb(17, 12, 24);
border: 1px solid rgb(246, 179, 194);
}

*>*
{
color: rgb(87, 71, 109);
selection-background-color: rgba(189, 122, 169,0.85);
}

#TitleBar
{
background-color: rgb(20, 15, 27);
}

#Title
{
 color: rgba(246, 179, 194,0.8);
}

#TitleBar QPushButton
{
border:none;

padding-bottom:4px;
}
QPushButton
{
   border:none;
}


#BPMinimize
{
padding-top: 7px;
border-radius:6px;
background-color: rgb(225, 225, 68);
}

#BPClose
{
padding-top: 7px;
border-radius:6px;
background-color: rgb(255, 68, 68);
}

#BPClose::hover
{
   
	background-color: rgb(240, 53, 53);
}
#BPMinimize::hover
{
   background-color: rgb(210, 210, 53);
}

#BPSaveOption::hover
{
   border-bottom : 1px solid rgb(87, 71, 109);
}

#DllHistory
{
padding: 2px 10px 2px 0;
}

#BPRestartAdmin, #DllHistory
{
    border: 1px solid rgb(87, 71, 109);
}

#BPRestartAdmin::hover, #DllHistory::hover
{
     color:rgb(17, 12, 24);
     background-color: rgb(87, 71, 109);
}


QTabWidget  QWidget
{     
	background-color: transparent;
}

QTabWidget::pane {
    border:none;
	background-color: transparent;
}

QTabWidget::tab-bar {
background-color: rgb(62, 187, 255);
}

QTabBar::tab {
     background-color: rgb(20, 15, 27);
     padding: 5px 15px 5px 15px;
     font-size:14px;
     color:rgba(255,255,255,0.7);
}

 QTabBar::tab:hover {
        background-color: rgb(22, 17, 29);
}

QTabBar::tab:selected {
  border-bottom: 2px solid rgba(255,255,255,0.15);
  background-color: rgb(22, 17, 29);
}


#BPSearchProcess
{
padding:7px;
}


QComboBox
{
color:rgb(246, 179, 194);
border-radius:0px;
border-bottom:   1px solid  rgb(87, 71, 109);
padding:0 0 0 3px;
}


QComboBox::drop-down {
    width: 30px;
}

QComboBox::down-arrow {
    width:15px;
	image: url(./Logos/down-arrow.png);
}

QComboBox QAbstractItemView {
    color: rgba(246, 179, 194,0.8);
	background-color: rgb(27, 22, 34);
    border:none;

}

QComboBox QAbstractItemView::item
{
   letter-spacing:1px;
   height:25px;
}

QLineEdit {
    border:none;
    border-bottom: 1px solid rgb(87, 71, 109);
	background:transparent;
    padding:0px 5px 1px 5px;
	color: rgb(246, 179, 194);
}

#BackgroundSelectProcess
{		 

  background-color: rgb(17, 12, 24);
}


#OpenProcess,#CancelProcess,#RefreshProcess
{
border: 1px solid rgb(87, 71, 109);
border-radius:0px;
color:rgba(246, 179, 194,0.85);
}

#ListProcess
{
   background:transparent;
   border:none;
   color:rgba(246, 179, 194,0.85);
}

QScrollBar::vertical {
    background:   rgb(17, 12, 24);

    width:10px;
}

QScrollBar::handle:vertical {

    background: rgba(246, 179, 194,0.85);
  
}

QScrollBar::sub-page:vertical,QScrollBar::add-page:vertical {
 background: rgb(46, 34, 64);
}

QScrollBar::sub-line:vertical,QScrollBar::add-line:vertical {
 background: transparent;
}

#OpenProcess::hover,#CancelProcess::hover,#RefreshProcess::hover
{
     color:rgb(17, 12, 24);
     background-color: rgb(87, 71, 109);
}


QCheckBox {
    letter-spacing: 1px;
}

QCheckBox::indicator {
    width: 17px;
    height: 17px;
    image: url(./Logos/uncheckbox.png);
}

QCheckBox::indicator:unchecked {
   		    image: url(./Logos/uncheckbox.png);
}

QCheckBox::indicator:checked {
  image: url(./Logos/checkbox.png);
}


QDoubleSpinBox {
    selection-background-color: transparent;
    padding:2px 0 3px 5px;
    border:1px solid rgb(87, 71, 109);
    color:rgb(246, 179, 194);

   
}


QDoubleSpinBox::up-button {
	  image: url(./Logos/up.png) ;
	  margin:-3px;
      margin-right:0px;
	  width:20px;
}

QDoubleSpinBox::down-button {
      image: url(./Logos/down.png);
	  margin:-3px;
      margin-right:0px;
	  width:20px;
      
}

#BPInject
{
border: 1px solid rgb(87, 71, 109);
border-radius:0px;
color:rgba(246, 179, 194,0.85);
padding: 2px 10px 2px 0;
}

#BPInject::hover
{
     color:rgb(17, 12, 24);
     background-color: rgb(87, 71, 109);
}

#BPSearchDll,#BPSearchProcess 
{
  border-radius:4px;
}

#BPSearchDll::hover,#BPSearchProcess::hover
{
     background-color:  rgb(29, 24, 36);
}


)"


#define QMENU_STYESHEET R"(
QMenu
{
    background: rgb(25, 20, 32);
    color:rgb(246, 179, 194);
}

QMenu::item {
    padding: 2px 25px 2px 20px;
    border: 1px solid transparent; 
}

QMenu::item:selected {
    background: rgb(33, 28, 40);
}


QMenu::separator {
    height: 1px;
    background: rgb(45, 45, 45);
    margin-left: 10px;
    margin-right: 5px;
}



)"


QT_END_NAMESPACE
