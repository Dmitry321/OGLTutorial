#ifndef SIMPLEOBJECT3D_H
#define SIMPLEOBJECT3D_H
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>

//#include <QOpenGLTexture>

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

struct VertexData
{
    VertexData(){
    }
    VertexData(QVector3D p, QVector2D t, QVector3D n):
        position(p), textCoord(t), normal(n)
    {

    }
    QVector3D position;  // позиция точки
    QVector2D textCoord; // текстурные координаты
    QVector3D normal; // нормаль к данной точке

};

class SimpleObject3D
{
public:
    SimpleObject3D();
    //SimpleObject3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    SimpleObject3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    ~SimpleObject3D();

    //void init(const QVector<VertexData> &vertData, const QVector<GLushort> &indexes, const QImage &texture);
    void init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
    void translate(const QVector3D &translateVector);

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QMatrix4x4 m_modelMatrix;
    QOpenGLTexture *m_texture;
};

#endif // SIMPLEOBJECT3D_H
