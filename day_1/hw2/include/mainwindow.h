#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onKeyButtonClicked();
    void onBackspaceClicked();
    void onEnterClicked();
    void onSpaceClicked();
    void onTabClicked();
    void onShiftClicked();
    void onCapsClicked();
    void onCopyClicked();
    void onClearClicked();
    void onWinClicked();

private:
    void setupKeyboardUi();
    QPushButton *createKeyButton(const QString &normal, const QString &shifted, int widthFactor = 1);
    QPushButton *createFunctionButton(const QString &label, int widthFactor = 1);
    void updateKeyLabels();

    Ui::MainWindow *ui;

    QLineEdit *m_display = nullptr;
    QPushButton *m_copyBtn = nullptr;
    QPushButton *m_clearBtn = nullptr;

    bool m_shiftActive = false;
    bool m_capsActive = false;

    // 문자키 버튼 -> (기본문자, Shift 시 문자)
    QMap<QPushButton *, QPair<QString, QString>> m_charKeys;
};
#endif // MAINWINDOW_H