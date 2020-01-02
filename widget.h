#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QVector>
//#include "simpleobject3d.h"

class SimpleObject3D;

//struct VertexData
//{
//    VertexData(){
//    }
//    VertexData(QVector3D p, QVector2D t, QVector3D n):
//        position(p), textCoord(t), normal(n)
//    {

//    }
//    QVector3D position;  // позиция точки
//    QVector2D textCoord; // текстурные координаты
//    QVector3D normal; // нормаль к данной точке

//};

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:  // вызываются только внутри этого класса
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();
    void initCube(float width);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_program;
    //QOpenGLTexture *m_texture;
    //QOpenGLBuffer m_arrayBuffer;
    //QOpenGLBuffer m_indexBuffer;
    QVector2D m_mousePosition;
    // Объект дял поворота содержит вектор и угол поворота
    QQuaternion m_rotation;

    QVector<SimpleObject3D *> m_objects;
    float m_z; // сдвиг по оси Z для камеры
};

#endif // WIDGET_H
