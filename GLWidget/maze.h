#ifndef MAZE_H
#define MAZE_H

#include <GL/gl.h>

// Math for vectors
#include <cmath>


// 3d vector struct
struct Vec3D
{
	GLfloat x, y, z;

	// Constructor
	Vec3D(GLfloat a = 0, GLfloat b = 0, GLfloat c = 0) : x(a), y(b), z(c) {}

	// Manipulators
	void	set(GLfloat a, GLfloat b, GLfloat c) { x = a; y = b; z = c; }
	GLfloat	getLength(void) { return sqrt(x*x + y*y + z*z); }

	// Operations
	GLfloat	dot(Vec3D vec) { return ((x * vec.x) + (y * vec.y) + (z * vec.z)); }
	//Vec3D	cross(Vec3D vec) { return Vec3D(y * vec.z - ); }
	Vec3D	add(Vec3D vec) { return Vec3D(x + vec.x, y + vec.y, z + vec.z); }
	Vec3D	sub(Vec3D vec) { return Vec3D(x - vec.x, y - vec.y, z - vec.z); }
	Vec3D	mul(GLfloat scalar) { return Vec3D(x * scalar, y * scalar, z * scalar); }

	// Increase length by scalar amount
	Vec3D	extend(GLfloat scalar) { GLfloat len = getLength(); return Vec3D(x, y, z).mul((len + scalar) / len); }
};



// A cell with 4 corners
struct QuadCell
{
	Vec3D vertex[4];	// Corners vertexes
	Vec3D color[4];		// Colors of each vertex
	Vec3D normal;		// Normal to the plane
};

// A cube in 3-space (general cuboid, technically)
// struct Cube3D
// {
// 	QuadCell face[6]; // The six faces
// };


// Functions
void InitMaze(GLfloat scale);
void RenderMaze(void);
bool CheckCollision(Vec3D pos, Vec3D mov);


#endif // MAZE_H
