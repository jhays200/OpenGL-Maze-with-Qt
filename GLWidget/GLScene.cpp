#include "GLScene.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "maze.h"
#include "camera.h"
#include <QtGui/QKeyEvent>
#include <QApplication>

CCamera Camera;

GLScene::GLScene ( QWidget* parent ):QGLWidget(parent), m_ambientColor(), m_background(), m_lastPoint()
{
	//ambient lighting color
	m_ambientColor.assign(0.2f);
	m_ambientColor[3] = 1.0f;
	
	//background initialization
	m_background.assign(0.0f); //make it a black bg
	m_background[3] = 1.0f;
	
	InitMaze(1.2f);
	
}

void GLScene::initializeGL()
{
    //setup some initial settings
	glEnable(GL_DEPTH_TEST);		// Compensate for depth
	glEnable(GL_COLOR_MATERIAL);	// Enable color
	glEnable(GL_LIGHTING);			// Enable lighting system
	glEnable(GL_LIGHT0);			// Enable light #0
	glEnable(GL_LIGHT1);			// Enable light #1
	glEnable(GL_NORMALIZE);			// Automatically normalize normals
	
	glClearColor(m_background[0], m_background[1], m_background[2], m_background[3]);
}


void GLScene::paintGL ()
{
	//set background color
	//glClearColor(m_background[0], m_background[1], m_background[2], m_background[3]);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//NOTE: 
	glMatrixMode(GL_MODELVIEW);		// Drawing perspective
	glLoadIdentity();				// Reset camera position
	
	//Camera Mode Lighting
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
	
	// Add ambient light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_ambientColor.c_array());
	
	// Adjust for camera position
	Camera.Render();
	
	//Add light
	const GLfloat lightColor0[] = {0.5f, 0.2f, 0.2f, 1.0f};
	const GLfloat lightPos0[] = {5.0f, 0.0f, -5.0f, 1.0f};
	//GLfloat lightPos0[] = {Camera.Position.x, Camera.Position.y, Camera.Position.z, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lightColor0);

	//Add light
	GLfloat lightColor1[] = {0.2f, 0.5f, 0.2f, 1.0f};
	GLfloat lightPos1[] = {10.0f, 10.0f, 5.0f, 1.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lightColor1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lightColor1);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0f);

	
	// Draw maze
	RenderMaze();
}

void GLScene::resizeGL ( int w, int h )
{
    if (h == 0)
		h = 1;
	
	// Adjust viewport settings
	glViewport(0,0,w,h);
	
	glMatrixMode(GL_PROJECTION);
	
	//Fix camera position
	glLoadIdentity();
	gluPerspective(65.0, (double)w / (double)h, 0.5, 200.0);
}

void GLScene::keyPressEvent ( QKeyEvent* event )
{
    switch(event->key())
	{
		case Qt::Key_Escape:
			QApplication::closeAllWindows();
			break;
			
		case Qt::Key_Left:
			Camera.RotateY(5.0f);
			break;
			
		case Qt::Key_Right:
			Camera.RotateY(-5.0f);
			break;
			
		case Qt::Key_Up:
			Camera.RotateX(5.0f);
			break;
			
		case Qt::Key_Down:
			Camera.RotateX(-5.0f);
			break;
			
		case 'A':
			Camera.StrafeRight(-0.2f);
			break;
 			
		case 'D':
			Camera.StrafeRight(0.2f);
			break;
			
		case 'W':
			Camera.MoveForwards(-0.1f);
			break;
			
		case 'S':
			Camera.MoveForwards(0.1f);
			break;
			
		default:
			//
			
			QGLWidget::keyPressEvent(event);
			return;
	}
	event->accept();
	update();
}

void GLScene::mousePressEvent ( QMouseEvent* event )
{
    m_lastPoint = event->posF();
	QGLWidget::mousePressEvent(event);
}


void GLScene::mouseMoveEvent ( QMouseEvent* event )
{
    const QPointF difference = (m_lastPoint - event->posF());
	
	//save last event
	m_lastPoint = event->posF();
	
	Camera.RotateX(difference.y());
	Camera.RotateY(difference.x());
	update();
	event->accept();
}