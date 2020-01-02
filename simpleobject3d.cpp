#include "simpleobject3d.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

SimpleObject3D::SimpleObject3D() :
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_texture(nullptr)
{

}

SimpleObject3D::SimpleObject3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture)
{
    init(vertData, indexes, texture);
}

SimpleObject3D::~SimpleObject3D()
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();
    if(m_texture != nullptr){
        if(m_texture->isCreated())
            m_texture->destroy();
    }
}

void SimpleObject3D::init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture)
{
    // если будет вызыватся повторно надо сначало удалить буферы
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();
    if(m_texture != nullptr){
        if(m_texture->isCreated())
            m_texture->destroy();
       m_texture = nullptr;
    }
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertData.constData(), static_cast<int>(static_cast<ulong>(vertData.size()) * sizeof (VertexData)) ); // выделение в видеопамяти нужное кол-во байт и скопирует указанные данные в видеопамять
    m_vertexBuffer.release(); // можно пока освободить

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), static_cast<int>(static_cast<ulong>(indexes.size()) * sizeof(GLuint)));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(texture.mirrored()); // текстуру надо зеркально отражать относит вертик оси тогда будет отображаться корректно

    // Фильтрация текстур:
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);

    m_modelMatrix.setToIdentity();



}

void SimpleObject3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if(!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated()) return;

    m_texture->bind(0);
    program->setUniformValue("u_modelMatrix", m_modelMatrix);
    program->setUniformValue("u_texture", 0); // 0 - номер структуры которая будет отрисовываться);


    m_vertexBuffer.bind();
    int offset = 0;

    // указываем шейдеру аттрибуты
    int vertLoc = program->attributeLocation("a_position");
    program->enableAttributeArray(vertLoc);
    program->setAttributeBuffer(vertLoc,GL_FLOAT, offset, 3, sizeof (VertexData));

    offset += sizeof(QVector3D);

    int textLoc = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(textLoc);
    program->setAttributeBuffer(textLoc, GL_FLOAT, offset, 2, sizeof (VertexData));

    offset += sizeof(QVector2D);

    int normLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normLoc);
    program->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof (VertexData));

    m_indexBuffer.bind();

    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr); //первы аргумент это примитивы которые рисовать, второй количество элементов для отрисовки далее тип и массив для отрисовки но так как мы используем буфер то здесь просто 0

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_texture->release();
}

void SimpleObject3D::translate(const QVector3D &translateVector)
{
    m_modelMatrix.translate(translateVector);
}
