#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QApplication>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("keyboard");
    setupKeyboardUi();
    resize(780, 340);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QPushButton *MainWindow::createKeyButton(const QString &normal, const QString &shifted, int widthFactor)
{
    QPushButton *btn = new QPushButton(normal, ui->centralwidget);
    btn->setMinimumHeight(40);
    btn->setMinimumWidth(40 * widthFactor);
    btn->setFocusPolicy(Qt::NoFocus);
    m_charKeys.insert(btn, qMakePair(normal, shifted));
    connect(btn, &QPushButton::clicked, this, &MainWindow::onKeyButtonClicked);
    return btn;
}

QPushButton *MainWindow::createFunctionButton(const QString &label, int widthFactor)
{
    QPushButton *btn = new QPushButton(label, ui->centralwidget);
    btn->setMinimumHeight(40);
    btn->setMinimumWidth(40 * widthFactor);
    btn->setFocusPolicy(Qt::NoFocus);
    return btn;
}

void MainWindow::setupKeyboardUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralwidget);

    // ---- 상단: 텍스트 표시창 + Copy / Clear ----
    QHBoxLayout *topLayout = new QHBoxLayout();
    m_display = new QLineEdit(ui->centralwidget);
    m_display->setMinimumHeight(40);

    m_copyBtn = new QPushButton("Copy", ui->centralwidget);
    m_clearBtn = new QPushButton("Clear", ui->centralwidget);
    m_copyBtn->setMinimumWidth(70);
    m_clearBtn->setMinimumWidth(70);

    QVBoxLayout *rightBtnLayout = new QVBoxLayout();
    rightBtnLayout->addWidget(m_copyBtn);
    rightBtnLayout->addWidget(m_clearBtn);

    topLayout->addWidget(m_display, 1);
    topLayout->addLayout(rightBtnLayout);
    mainLayout->addLayout(topLayout);

    connect(m_copyBtn, &QPushButton::clicked, this, &MainWindow::onCopyClicked);
    connect(m_clearBtn, &QPushButton::clicked, this, &MainWindow::onClearClicked);

    // ---- 키보드 영역 ----
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(4);

    struct KeyDef { QString normal, shifted; };

    // Row 0: ` 1 2 3 4 5 6 7 8 9 0 - = Backspace
    const QVector<KeyDef> row0 = {
        {"`", "~"}, {"1", "!"}, {"2", "@"}, {"3", "#"}, {"4", "$"},
        {"5", "%"}, {"6", "^"}, {"7", "&"}, {"8", "*"}, {"9", "("},
        {"0", ")"}, {"-", "_"}, {"=", "+"}
    };
    int col = 0;
    for (const auto &k : row0) {
        grid->addWidget(createKeyButton(k.normal, k.shifted), 0, col);
        col++;
    }
    QPushButton *backspaceBtn = createFunctionButton("Backspace", 2);
    grid->addWidget(backspaceBtn, 0, col, 1, 2);
    connect(backspaceBtn, &QPushButton::clicked, this, &MainWindow::onBackspaceClicked);

    // Row 1: Tab Q W E R T Y U I O P [ ] (backslash)
    QPushButton *tabBtn = createFunctionButton("Tab", 1);
    grid->addWidget(tabBtn, 1, 0);
    connect(tabBtn, &QPushButton::clicked, this, &MainWindow::onTabClicked);

    const QVector<KeyDef> row1 = {
        {"q", "Q"}, {"w", "W"}, {"e", "E"}, {"r", "R"}, {"t", "T"},
        {"y", "Y"}, {"u", "U"}, {"i", "I"}, {"o", "O"}, {"p", "P"},
        {"[", "{"}, {"]", "}"}, {"\\", "|"}
    };
    col = 1;
    for (const auto &k : row1) {
        grid->addWidget(createKeyButton(k.normal, k.shifted), 1, col);
        col++;
    }

    // Row 2: Caps A S D F G H J K L ; ' Enter
    QPushButton *capsBtn = createFunctionButton("Caps", 1);
    grid->addWidget(capsBtn, 2, 0);
    connect(capsBtn, &QPushButton::clicked, this, &MainWindow::onCapsClicked);

    const QVector<KeyDef> row2 = {
        {"a", "A"}, {"s", "S"}, {"d", "D"}, {"f", "F"}, {"g", "G"},
        {"h", "H"}, {"j", "J"}, {"k", "K"}, {"l", "L"},
        {";", ":"}, {"'", "\""}
    };
    col = 1;
    for (const auto &k : row2) {
        grid->addWidget(createKeyButton(k.normal, k.shifted), 2, col);
        col++;
    }
    QPushButton *enterBtn = createFunctionButton("Enter", 2);
    grid->addWidget(enterBtn, 2, col, 1, 2);
    connect(enterBtn, &QPushButton::clicked, this, &MainWindow::onEnterClicked);

    // Row 3: Shift Z X C V B N M , . / Shift
    QPushButton *shiftBtnLeft = createFunctionButton("Shift", 1);
    grid->addWidget(shiftBtnLeft, 3, 0);
    connect(shiftBtnLeft, &QPushButton::clicked, this, &MainWindow::onShiftClicked);

    const QVector<KeyDef> row3 = {
        {"z", "Z"}, {"x", "X"}, {"c", "C"}, {"v", "V"}, {"b", "B"},
        {"n", "N"}, {"m", "M"}, {",", "<"}, {".", ">"}, {"/", "?"}
    };
    col = 1;
    for (const auto &k : row3) {
        grid->addWidget(createKeyButton(k.normal, k.shifted), 3, col);
        col++;
    }
    QPushButton *shiftBtnRight = createFunctionButton("Shift", 2);
    grid->addWidget(shiftBtnRight, 3, col, 1, 2);
    connect(shiftBtnRight, &QPushButton::clicked, this, &MainWindow::onShiftClicked);

    // Row 4: Ctrl Win Alt Space Kor Win Menu Ctrl
    QPushButton *ctrlBtn1 = createFunctionButton("Ctrl", 1);
    QPushButton *winBtn1  = createFunctionButton("Win", 1);
    QPushButton *altBtn1  = createFunctionButton("Alt", 1);
    QPushButton *spaceBtn = createFunctionButton("Space", 6);
    QPushButton *korBtn   = createFunctionButton("Kor", 1);
    QPushButton *winBtn2  = createFunctionButton("Win", 1);
    QPushButton *menuBtn  = createFunctionButton("Menu", 1);
    QPushButton *ctrlBtn2 = createFunctionButton("Ctrl", 1);

    grid->addWidget(ctrlBtn1, 4, 0);
    grid->addWidget(winBtn1, 4, 1);
    grid->addWidget(altBtn1, 4, 2);
    grid->addWidget(spaceBtn, 4, 3, 1, 6);
    grid->addWidget(korBtn, 4, 9);
    grid->addWidget(winBtn2, 4, 10);
    grid->addWidget(menuBtn, 4, 11);
    grid->addWidget(ctrlBtn2, 4, 12);

    // 토글 성격의 버튼(Ctrl/Alt/Kor/Menu)은 체크만 되는 표시용 버튼
    for (QPushButton *b : { ctrlBtn1, altBtn1, korBtn, menuBtn, ctrlBtn2 }) {
        b->setCheckable(true);
    }

    connect(spaceBtn, &QPushButton::clicked, this, &MainWindow::onSpaceClicked);
    connect(winBtn1, &QPushButton::clicked, this, &MainWindow::onWinClicked);
    connect(winBtn2, &QPushButton::clicked, this, &MainWindow::onWinClicked);

    mainLayout->addLayout(grid);
}

void MainWindow::updateKeyLabels()
{
    const bool upper = m_shiftActive != m_capsActive; // 둘 중 하나만 켜졌을 때 대문자/기호
    for (auto it = m_charKeys.begin(); it != m_charKeys.end(); ++it) {
        QPushButton *btn = it.key();
        const QString &normal = it.value().first;
        const QString &shifted = it.value().second;
        btn->setText(upper ? shifted : normal);
    }
}

void MainWindow::onKeyButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn) return;

    m_display->insert(btn->text());

    // 문자를 입력한 뒤에는 Shift는 자동 해제(Caps는 유지)
    if (m_shiftActive) {
        m_shiftActive = false;
        updateKeyLabels();
    }
}

void MainWindow::onBackspaceClicked()
{
    m_display->backspace();
}

void MainWindow::onEnterClicked()
{
    emit m_display->returnPressed();
}

void MainWindow::onSpaceClicked()
{
    m_display->insert(" ");
}

void MainWindow::onTabClicked()
{
    m_display->insert("\t");
}

void MainWindow::onShiftClicked()
{
    m_shiftActive = !m_shiftActive;
    updateKeyLabels();
}

void MainWindow::onCapsClicked()
{
    m_capsActive = !m_capsActive;
    updateKeyLabels();
}

void MainWindow::onWinClicked()
{
}

void MainWindow::onCopyClicked()
{
    QApplication::clipboard()->setText(m_display->text());
}

void MainWindow::onClearClicked()
{
    m_display->clear();
}