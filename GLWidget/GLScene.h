#ifndef GLSCENE_H
#define GLSCENE_H

#include <QGLWidget>
#include <boost/array.hpp>

class GLScene: public QGLWidget
{
	Q_OBJECT
	
public:
	GLScene(QWidget * parent = 0);
	
protected:
	//overloaded QGLWidget functions
	virtual void initializeGL();
	virtual void paintGL();
	virtual void resizeGL(int w, int h);
	
	//overloaded QWidget functions eventually to move to a overall class
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);

private:
	boost::array<GLfloat, 4> m_ambientColor;
	boost::array<GLfloat, 4> m_background;
	
	QPointF m_lastPoint; 
};



#endif //GLSCENE_H
