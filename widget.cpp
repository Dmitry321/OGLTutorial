#include "widget.h"
#include <QOpenGLContext>
#include <QMouseEvent>
#include <simpleobject3d.h>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_z = -5.0f;
}

Widget::~Widget()
{

}

// вызывается один раз в самом начале
void Widget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST); // включает буфер глубины
    glEnable(GL_CULL_FACE);  // отсечение задних граней

    initShaders();
    initCube(2.0f);
    m_objects[0]->translate(QVector3D(-0.2f, 0.0f, 0.0f));

    initCube(1.0f);
    m_objects[1]->translate(QVector3D(0.8f, 0.0f, 0.0f));
}

// при изменении размера окна
void Widget::resizeGL(int w, int h)
{
    float aspect = w/(static_cast<float>(h));
    m_projectionMatrix.setToIdentity(); // создать единичную матрицу (единицы на главной диагонали)
    m_projectionMatrix.perspective(45, aspect,0.01f, 100.0f);  //(угол усеченного конуса камеры в градусах, aspectratio, передняя и дальняя плоскости отсечения)

}

// вызывается при перерисовке содержимого
void Widget::paintGL()
{
    // вначале очищаем буфер цвета и буфер глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 viewMatrix;// модельно видовая матрица или матрица камеры
    viewMatrix.setToIdentity();
    viewMatrix.translate(0.0, 0.0, m_z); // отодвигаем камеру на 5 назад

//    modelViewMatrix.rotate(30, 1.0, 0.0, 0.0);
//    modelViewMatrix.rotate(30, 0.0, 1.0, 0.0);

    viewMatrix.rotate(m_rotation);

    m_program.bind();
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("u_viewMatrix", viewMatrix);
    m_program.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_program.setUniformValue("u_lightPower", 1.0f);

    for(auto *s3d: m_objects){
    //for(int i = 0; i < m_objects.size(); ++i){
      //  m_objects[i]->draw(&m_program, context()->functions());
        s3d->draw(&m_program, context()->functions());
    }

}

void Widget::initShaders()
{
    // компиляция шейдеров
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh"))
        close();
    // шейдер для андроид
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh"))
        close();
    // Слинковать все шейдеры в один
    if(!m_program.link())
        close();
    // Bind shader pipeline for use
    if (!m_program.bind())
           close();

}

void Widget::initCube(float width)
{
    float with_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;
    // добавляем точки куба в массив
    //  первая грань если смотрим в положит направлении оси z к токе 0,0 (все z положительные)
    //                                         x          y           z        textures               normal
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, with_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, with_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, with_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, with_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));  // правая нижняя вершина

    // вторая грань если смотрим в положит направлении оси x к токе 0,0 (все x положительные)
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, with_div_2), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, with_div_2), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, -with_div_2), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, -with_div_2), QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));  // правая нижняя вершина

    // третья грань если смотрим в положит направлении оси y к токе 0,0(все y положительные)
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, with_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, -with_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, with_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, -with_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));  // правая нижняя вершина

    // 4 грань если смотрим в отрицат направлении оси z к токе 0,0 (все z отрицат)
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, -with_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, -with_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, -with_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, -with_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));  // правая нижняя вершина

    // 5 грань если смотрим в отрицат направлении оси x к токе 0,0 (все x отрицат)
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, with_div_2), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, -with_div_2), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, with_div_2), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, -with_div_2), QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));  // правая нижняя вершина

    // 6 грань если смотрим в отрицат направлении оси y к токе 0,0(все y отрицат)
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, with_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, -with_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, with_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, -with_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));  // правая нижняя вершина

    // массив индексов. Вершин у нас 24
    QVector<GLuint> indexes;
    for(uint i = 0; i < 24; i += 4)
    {
        indexes.append(i+0);
        indexes.append(i+1);
        indexes.append(i+2);
        indexes.append(i+2);
        indexes.append(i+1);
        indexes.append(i+3);
    }

//    QPixmap pixmap;
//    pixmap.load(":/cube1.png");

//    QFile file("cube123.png");
//    file.open(QIODevice::WriteOnly);
//    pixmap.save(&file, "PNG");

    QImage img(":/cube1.png");
    m_objects.append(new SimpleObject3D());
    m_objects.last()->init(vertexes, indexes, QImage(":/cube1.png"));// = new SimpleObject3D(vertexes, indexes, img); //(vertexes, indexes, QImage(":/cube1.png"));

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton){
        m_mousePosition = QVector2D(event->localPos());
    }
    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton) return;

    QVector2D diffPos = QVector2D(event->localPos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->localPos());

    float angel = diffPos.length() / 2.0f; // угол поворота

    // Точка вокруг которой осуществлять поворот - ось вращения
    QVector3D axis = QVector3D(diffPos.y(), diffPos.x(), 0.0);

    m_rotation = QQuaternion::fromAxisAndAngle(axis, angel) * m_rotation; // умножаем на уже совершенные повороты

    update();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0){
        m_z += 0.25f;
    }
    else if(event->delta() < 0)
    {
        m_z -= 0.25f;
    }
    update();
}
