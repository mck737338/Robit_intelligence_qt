#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    // link1 ~ link3 스핀박스 값이 바뀔 때 호출
    void on_spinBox_link1_valueChanged(double value);
    void on_spinBox_link2_valueChanged(double value);
    void on_spinBox_link3_valueChanged(double value);

    // Generate Arm 버튼 클릭 시 호출
    void on_pushButton_generate_clicked();

    // joint1 ~ joint3 슬라이더 값이 바뀔 때 호출 (옆의 값 표시창 갱신)
    void on_slider_joint1_valueChanged(int value);
    void on_slider_joint2_valueChanged(int value);
    void on_slider_joint3_valueChanged(int value);

    // Reset 버튼 클릭 시 호출
    void on_pushButton_reset_clicked();

private:
    Ui::MainWindow *ui;

    // 링크 길이 값 저장
    double m_link1;
    double m_link2;
    double m_link3;

    // Generate Arm 버튼을 눌러 팔이 생성되었는지 여부
    bool m_armGenerated;

    // 팔을 실제로 그리는 함수 (QGraphicsScene 이용)
    void drawArm();
};
#endif // MAINWINDOW_H
