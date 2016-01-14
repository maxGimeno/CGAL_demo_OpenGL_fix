// Copyright (c) 2011 CNRS and LIRIS' Establishments (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>

#include <QApplication>
#include <QKeyEvent>

#include <QGLViewer/qglviewer.h>
#include <QKeyEvent>
#include <QOpenGLFunctions_2_1>
#include <QOpenGLVertexArrayObject>
#include <QGLBuffer>
#include <QOpenGLShaderProgram>
#include <CGAL/Qt/CreateOpenGLContext.h>

class EmptyViewerQt : public QGLViewer, public QOpenGLFunctions_2_1
{
public:
  // Constructor/Destructor
  EmptyViewerQt() :are_buffers_initialized(false)
  {
    setWindowTitle("Empty viewer");
    resize(500, 450);
    QOpenGLContext *context = new QOpenGLContext();
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(2,1);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    context->setFormat(format);
    QGLViewer(QGLContext::fromOpenGLContext(context));
    QGLFormat newFormat = this->format();
    newFormat.setSampleBuffers(true);
    newFormat.setSamples(16);
    this->setFormat(newFormat);
    if(!this->context()->isValid())
      qDebug()<<"ERROR : OpenGL context is not valid.";

  }

  ~EmptyViewerQt()
  {
   /* buffers[0].destroy();
    buffers[1].destroy();
    buffers[2].destroy();
    buffers[3].destroy();
    buffers[4].destroy();
    buffers[5].destroy();
    vao[0].destroy();
    vao[1].destroy();
    vao[2].destroy();
    vao[3].destroy();*/
  }

protected:
  void compile_shaders()
  {/*
    if(!buffers[0].create() || !buffers[1].create() || !buffers[2].create() ||
       !buffers[3].create() || !buffers[4].create() || !buffers[5].create())
    {
      std::cerr<<"VBO Creation FAILED"<<std::endl;
    }

    if(!vao[0].create() || !vao[1].create() || !vao[2].create() || !vao[3].create())
    {
      std::cerr<<"VAO Creation FAILED"<<std::endl;
    }

    //The Facets

    //Vertex source code
    const char vertex_source[] =
      {
        "#version 120 \n"
        "attribute highp vec4 vertex;\n"
        "attribute highp vec3 normal;\n"

        "uniform highp mat4 mvp_matrix;\n"
        "uniform highp mat4 mv_matrix; \n"

        "varying highp vec4 fP; \n"
        "varying highp vec3 fN; \n"
        "void main(void)\n"
        "{\n"
        "   fP = mv_matrix * vertex; \n"
        "   fN = mat3(mv_matrix)* normal; \n"
        "   gl_Position = mvp_matrix * vertex;\n"
        "}"
      };
    //Vertex source code
    const char fragment_source[] =
      {
        "#version 120 \n"
        "varying highp vec4 fP; \n"
        "varying highp vec3 fN; \n"
        "uniform highp vec4 color; \n"
        "uniform vec4 light_pos;  \n"
        "uniform vec4 light_diff; \n"
        "uniform vec4 light_spec; \n"
        "uniform vec4 light_amb;  \n"
        "uniform float spec_power ; \n"

        "void main(void) { \n"

        "   vec3 L = light_pos.xyz - fP.xyz; \n"
        "   vec3 V = -fP.xyz; \n"

        "   vec3 N = normalize(fN); \n"
        "   L = normalize(L); \n"
        "   V = normalize(V); \n"

        "   vec3 R = reflect(-L, N); \n"
        "   vec4 diffuse = max(dot(N,L), 0.0) * light_diff * color; \n"
        "   vec4 specular = pow(max(dot(R,V), 0.0), spec_power) * light_spec; \n"

        "gl_FragColor = light_amb*color + diffuse  ; \n"
        "} \n"
        "\n"
      };

    QOpenGLShader *vertex_shader = new QOpenGLShader(QOpenGLShader::Vertex);
    if(!vertex_shader->compileSourceCode(vertex_source))
    {
      std::cerr<<"Compiling vertex source FAILED"<<std::endl;
    }

    QOpenGLShader *fragment_shader= new QOpenGLShader(QOpenGLShader::Fragment);
    if(!fragment_shader->compileSourceCode(fragment_source))
    {
      std::cerr<<"Compiling fragmentsource FAILED"<<std::endl;
    }

    if(!rendering_program.addShader(vertex_shader))
    {
      std::cerr<<"adding vertex shader FAILED"<<std::endl;
    }
    if(!rendering_program.addShader(fragment_shader))
    {
      std::cerr<<"adding fragment shader FAILED"<<std::endl;
    }
    if(!rendering_program.link())
    {
      std::cerr<<"linking Program FAILED"<<std::endl;
    }
    rendering_program.bind();

    //Vertex source code
    const char vertex_source_p_l[] =
      {
        "#version 120 \n"
        "attribute highp vec4 vertex;\n"
        "uniform highp mat4 mvp_matrix;\n"
        "void main(void)\n"
        "{\n"
        "   gl_Position = mvp_matrix * vertex;\n"
        "}"
      };
    //Vertex source code
    const char fragment_source_p_l[] =
      {
        "#version 120 \n"
        "uniform highp vec4 color; \n"
        "void main(void) { \n"
        "gl_FragColor = color; \n"
        "} \n"
        "\n"
      };

    vertex_shader = new QOpenGLShader(QOpenGLShader::Vertex);
    if(!vertex_shader->compileSourceCode(vertex_source_p_l))
    {
      std::cerr<<"Compiling vertex source FAILED"<<std::endl;
    }

    fragment_shader= new QOpenGLShader(QOpenGLShader::Fragment);
    if(!fragment_shader->compileSourceCode(fragment_source_p_l))
    {
      std::cerr<<"Compiling fragmentsource FAILED"<<std::endl;
    }

    if(!rendering_program_p_l.addShader(vertex_shader))
    {
      std::cerr<<"adding vertex shader FAILED"<<std::endl;
    }
    if(!rendering_program_p_l.addShader(fragment_shader))
    {
      std::cerr<<"adding fragment shader FAILED"<<std::endl;
    }
    if(!rendering_program_p_l.link())
    {
      std::cerr<<"linking Program FAILED"<<std::endl;
    }
    rendering_program_p_l.bind();*/
  }

  void initialize_buffers()
  {
    //points of the facets
  /*  vao[0].bind();
    buffers[0].bind();
    buffers[0].allocate(pos_facets.data(),
                        static_cast<int>(pos_facets.size()*sizeof(float)));
    vertexLocation[0] = rendering_program.attributeLocation("vertex");
    rendering_program.bind();
    rendering_program.enableAttributeArray(vertexLocation[0]);
    rendering_program.setAttributeBuffer(vertexLocation[0],GL_FLOAT,0,3);
    rendering_program.release();
    buffers[0].release();

    //normals of the facets
    buffers[1].bind();
    buffers[1].allocate(flat_normals.data(),
                        static_cast<int>(flat_normals.size()*sizeof(float)));
    normalsLocation = rendering_program.attributeLocation("normal");
    rendering_program.bind();
    rendering_program.enableAttributeArray(normalsLocation);
    rendering_program.setAttributeBuffer(normalsLocation,GL_FLOAT,0,3);
    rendering_program.release();
    buffers[1].release();

    vao[0].release();

    vao[1].bind();

    //points of the facets
    buffers[2].bind();
    buffers[2].allocate(pos_facets.data(),
                        static_cast<int>(pos_facets.size()*sizeof(float)));
    vertexLocation[1] = rendering_program.attributeLocation("vertex");
    rendering_program.bind();
    rendering_program.enableAttributeArray(vertexLocation[1]);
    rendering_program.setAttributeBuffer(vertexLocation[1],GL_FLOAT,0,3);
    rendering_program.release();
    buffers[2].release();

    //normals of the facets
    buffers[3].bind();
    buffers[3].allocate(smooth_normals.data(),
                        static_cast<int>(smooth_normals.size()*sizeof(float)));
    normalsLocation = rendering_program.attributeLocation("normal");
    rendering_program.bind();
    rendering_program.enableAttributeArray(normalsLocation);
    rendering_program.setAttributeBuffer(normalsLocation,GL_FLOAT,0,3);
    rendering_program.release();
    buffers[3].release();

    vao[1].release();

    //The lines
    vao[2].bind();

    buffers[4].bind();
    buffers[4].allocate(pos_lines.data(), static_cast<int>(pos_lines.size()*sizeof(float)));
    vertexLocation[2] = rendering_program_p_l.attributeLocation("vertex");
    rendering_program_p_l.bind();
    rendering_program_p_l.enableAttributeArray(vertexLocation[2]);
    rendering_program_p_l.setAttributeBuffer(vertexLocation[2],GL_FLOAT,0,3);
    buffers[4].release();
    rendering_program_p_l.release();

    vao[2].release();

    //The points
    vao[3].bind();
    buffers[5].bind();
    buffers[5].allocate(pos_points.data(), static_cast<int>(pos_points.size()*sizeof(float)));
    vertexLocation[3] = rendering_program_p_l.attributeLocation("vertex");
    rendering_program_p_l.bind();
    rendering_program_p_l.enableAttributeArray(vertexLocation[3]);
    rendering_program_p_l.setAttributeBuffer(vertexLocation[3],GL_FLOAT,0,3);
    buffers[5].release();
    rendering_program_p_l.release();
    vao[3].release();

    are_buffers_initialized = true;*/
  }

  void attrib_buffers(QGLViewer* viewer)
  {
   /* QMatrix4x4 mvpMatrix;
    QMatrix4x4 mvMatrix;
    double mat[16];
    viewer->camera()->getModelViewProjectionMatrix(mat);
    for(int i=0; i < 16; i++)
    {
      mvpMatrix.data()[i] = (float)mat[i];
    }
    viewer->camera()->getModelViewMatrix(mat);
    for(int i=0; i < 16; i++)
    {
      mvMatrix.data()[i] = (float)mat[i];
    }
    // define material
    QVector4D ambient(0.4f, 0.4f, 0.4f, 0.4f);
    QVector4D diffuse( 0.9f,
                       0.9f,
                       0.9f,
                       0.9f );

    QVector4D specular( 0.0f,
                        0.0f,
                        0.0f,
                        1.0f );

    QVector4D position(10.0f, 10.0f, 10.0f, 0.0 );
    GLfloat shininess =  1.0f;

    rendering_program.bind();
    mvpLocation[0] = rendering_program.uniformLocation("mvp_matrix");
    mvLocation = rendering_program.uniformLocation("mv_matrix");
    lightLocation[0] = rendering_program.uniformLocation("light_pos");
    lightLocation[1] = rendering_program.uniformLocation("light_diff");
    lightLocation[2] = rendering_program.uniformLocation("light_spec");
    lightLocation[3] = rendering_program.uniformLocation("light_amb");
    lightLocation[4] = rendering_program.uniformLocation("spec_power");

    rendering_program.setUniformValue(lightLocation[0], position);
    rendering_program.setUniformValue(lightLocation[1], diffuse);
    rendering_program.setUniformValue(lightLocation[2], specular);
    rendering_program.setUniformValue(lightLocation[3], ambient);
    rendering_program.setUniformValue(lightLocation[4], shininess);
    rendering_program.setUniformValue(mvpLocation[0], mvpMatrix);
    rendering_program.setUniformValue(mvLocation, mvMatrix);

    colorLocation2 = rendering_program.uniformLocation("color");
    rendering_program.release();

    rendering_program_p_l.bind();
    mvpLocation[1] = rendering_program_p_l.uniformLocation("mvp_matrix");
    colorLocation = rendering_program_p_l.uniformLocation("color");
    rendering_program.setUniformValue(mvpLocation[1], mvpMatrix);
    rendering_program_p_l.release();*/
  }

  virtual void draw()
  {
    glEnable(GL_DEPTH_TEST);
    /*if(!are_buffers_initialized)
      initialize_buffers();

    QColor color;
    vao[0].bind();
    attrib_buffers(this);
    color.setRgbF(0.1f, 0.7f, 0.1f);
    rendering_program.bind();
    rendering_program.setUniformValue(colorLocation2,color);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(pos_facets.size()/3));
    rendering_program.release();
    vao[0].release();

    vao[2].bind();
    attrib_buffers(this);
    color.setRgbF(0.2f, 0.2f, 0.7f);
    rendering_program_p_l.bind();
    rendering_program_p_l.setAttributeValue(colorLocation,color);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(pos_lines.size()/3));
    rendering_program_p_l.release();
    vao[2].release();

    ::glPointSize(7.f);
    vao[3].bind();
    attrib_buffers(this);
    color.setRgbF(.2f,.2f,.6f);
    rendering_program_p_l.bind();
    rendering_program_p_l.setAttributeValue(colorLocation,color);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(pos_points.size()/3));
    rendering_program_p_l.release();
    vao[3].release();*/
  }

  virtual void init()
  {
    // Restore previous viewer state.
    restoreStateFromFile();

    // Define 'Control+Q' as the new exit shortcut (default was 'Escape')
    setShortcut(EXIT_VIEWER, Qt::CTRL+Qt::Key_Q);

    // Light default parameters
   /* ::glLineWidth(2.4f);
    ::glPointSize(7.f);
    ::glEnable(GL_POLYGON_OFFSET_FILL);
    ::glPolygonOffset(1.f,1.f);
    ::glClearColor(1.0f,1.0f,1.0f,0.0f);
    ::glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    ::glEnable(GL_LIGHTING);

    ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    ::glShadeModel(GL_FLAT);
    ::glDisable(GL_BLEND);
    ::glDisable(GL_LINE_SMOOTH);
    ::glDisable(GL_POLYGON_SMOOTH_HINT);
    ::glBlendFunc(GL_ONE, GL_ZERO);
    ::glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);*/

    initializeOpenGLFunctions();
    compile_shaders();

    this->showEntireScene();
  }

private:
  bool are_buffers_initialized;

  //Shaders elements
  int vertexLocation[4];
  int normalsLocation;
  int mvpLocation[2];
  int mvLocation;
  int colorLocation;
  int colorLocation2;
  int lightLocation[5];

  std::vector<float> pos_points;
  std::vector<float> pos_lines;
  std::vector<float> pos_facets;
  std::vector<float> smooth_normals;
  std::vector<float> flat_normals;

  QGLBuffer buffers[6];
  QOpenGLVertexArrayObject vao[4];

  QOpenGLShaderProgram rendering_program;
  QOpenGLShaderProgram rendering_program_p_l;
};

int main()
{
  int argc=1;

  const char* argv[2]={"emptyviewer","\0"};
  QApplication app(argc,const_cast<char**>(argv));

  EmptyViewerQt mainwindow;
  mainwindow.show();

  app.exec();
  return EXIT_SUCCESS;
}

