#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QPainter>
#include <QtMath>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_link1(1.0)
    , m_link2(1.0)
    , m_link3(1.0)
    , m_armGenerated(false)
{
    ui->setupUi(this);

    // graphicsView 에 사용할 scene 준비
    QGraphicsScene *scene = new QGraphicsScene(this);

    // joint1(원점, base)의 화면 위치를 고정하기 위해
    // scene의 좌표 범위를 매번 바뀌지 않는 고정 크기로 미리 지정한다.
    // (drawArm()에서 scene->clear()만 하고 setSceneRect()는 다시 호출하지 않음)
    scene->setSceneRect(-400, -400, 800, 800);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ------------------ link1 ~ link3 스핀박스 ------------------

void MainWindow::on_spinBox_link1_valueChanged(double value)
{
    m_link1 = value;
    if (m_armGenerated) {
        drawArm();
    }
}

void MainWindow::on_spinBox_link2_valueChanged(double value)
{
    m_link2 = value;
    if (m_armGenerated) {
        drawArm();
    }
}

void MainWindow::on_spinBox_link3_valueChanged(double value)
{
    m_link3 = value;
    if (m_armGenerated) {
        drawArm();
    }
}

// ------------------ Generate Arm 버튼 ------------------

void MainWindow::on_pushButton_generate_clicked()
{
    m_armGenerated = true;
    std::cout << "Generate Arm: link1=" << m_link1
              << " link2=" << m_link2
              << " link3=" << m_link3 << std::endl;
    drawArm();
}

// ------------------ joint1 ~ joint3 슬라이더 ------------------

void MainWindow::on_slider_joint1_valueChanged(int value)
{
    ui->lineEdit_joint1->setText(QString::number(value));
    if (m_armGenerated) {
        drawArm();
    }
}

void MainWindow::on_slider_joint2_valueChanged(int value)
{
    ui->lineEdit_joint2->setText(QString::number(value));
    if (m_armGenerated) {
        drawArm();
    }
}

void MainWindow::on_slider_joint3_valueChanged(int value)
{
    ui->lineEdit_joint3->setText(QString::number(value));
    if (m_armGenerated) {
        drawArm();
    }
}

// ------------------ Reset 버튼 ------------------

void MainWindow::on_pushButton_reset_clicked()
{
    // 링크 길이 초기화
    ui->spinBox_link1->setValue(1.0);
    ui->spinBox_link2->setValue(1.0);
    ui->spinBox_link3->setValue(1.0);

    // joint 슬라이더/값 표시창 초기화
    ui->slider_joint1->setValue(0);
    ui->slider_joint2->setValue(0);
    ui->slider_joint3->setValue(0);

    ui->lineEdit_joint1->setText("0");
    ui->lineEdit_joint2->setText("0");
    ui->lineEdit_joint3->setText("0");

    m_armGenerated = false;
    ui->graphicsView->scene()->clear();
}

// ------------------ 팔(로봇 암) 그리기 ------------------

void MainWindow::drawArm()
{
    QGraphicsScene *scene = ui->graphicsView->scene();

    // scene->clear()는 아이템만 지울 뿐, 생성자에서 지정한
    // 고정 sceneRect(-400,-400,800,800)는 그대로 유지된다.
    // -> joint1(base)은 항상 scene 좌표 (0,0), 즉 뷰의 항상 같은
    //    화면 위치에 고정된 채로 그려진다.
    scene->clear();

    // 화면에 보기 좋도록 실제 link 길이에 배율(scale)을 곱함
    const double scale = 50.0;

    // joint1의 좌표는 항상 원점(0, 0)으로 고정
    const QPointF joint1(0.0, 0.0);

    // 각 joint 각도(도 단위) -> 라디안
    double theta1 = qDegreesToRadians(static_cast<double>(ui->slider_joint1->value()));
    double theta2 = qDegreesToRadians(static_cast<double>(ui->slider_joint2->value()));
    double theta3 = qDegreesToRadians(static_cast<double>(ui->slider_joint3->value()));

    // 순차적으로 누적되는 각도 (2D 평면 순기구학)
    double angle1 = theta1;
    double angle2 = angle1 + theta2;
    double angle3 = angle2 + theta3;

    QPointF joint2 = joint1 + QPointF(m_link1 * scale * qCos(angle1),
                                      -m_link1 * scale * qSin(angle1));
    QPointF joint3 = joint2 + QPointF(m_link2 * scale * qCos(angle2),
                                      -m_link2 * scale * qSin(angle2));
    QPointF endEffector = joint3 + QPointF(m_link3 * scale * qCos(angle3),
                                           -m_link3 * scale * qSin(angle3));

    QPen linkPen(Qt::cyan, 4);
    QPen jointPen(Qt::yellow, 2);
    QBrush jointBrush(Qt::yellow);

    scene->addLine(QLineF(joint1, joint2), linkPen);
    scene->addLine(QLineF(joint2, joint3), linkPen);
    scene->addLine(QLineF(joint3, endEffector), linkPen);

    const double r = 5.0;
    scene->addEllipse(joint1.x() - r, joint1.y() - r, 2 * r, 2 * r, jointPen, jointBrush);
    scene->addEllipse(joint2.x() - r, joint2.y() - r, 2 * r, 2 * r, jointPen, jointBrush);
    scene->addEllipse(joint3.x() - r, joint3.y() - r, 2 * r, 2 * r, jointPen, jointBrush);
    scene->addEllipse(endEffector.x() - r, endEffector.y() - r, 2 * r, 2 * r, jointPen, jointBrush);
}