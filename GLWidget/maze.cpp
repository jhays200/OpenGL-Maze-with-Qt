#include "maze.h"

//#include <Windows.h>
//#include <cstdio>
#include <QDebug>

// Quad arrays contain vertex, color and normals for all cells
QuadCell g_maze_floor[16];
QuadCell g_maze_walls[23];
QuadCell g_maze_ceil[16];


// Create a wall cell around a center point
// side determines whether the cell plane is perpendicular (true) or parallel (false) to the screen
QuadCell CreateWallCell(Vec3D pos, GLfloat scale, bool side)
{
	QuadCell ret;

	// Colors same regardless of side
	ret.color[0] = Vec3D(0.5f, 0.3f, 0.1f);
	ret.color[1] = Vec3D(0.6f, 0.4f, 0.2f);
	ret.color[2] = Vec3D(0.7f, 0.5f, 0.3f);
	ret.color[3] = Vec3D(0.8f, 0.6f, 0.4f);

	if (side)
	{
		ret.vertex[0] = Vec3D(pos.x, pos.y - scale, pos.z + scale);
		ret.vertex[1] = Vec3D(pos.x, pos.y - scale, pos.z - scale);
		ret.vertex[2] = Vec3D(pos.x, pos.y + scale, pos.z + scale);
		ret.vertex[3] = Vec3D(pos.x, pos.y + scale, pos.z - scale);
		ret.normal = Vec3D(1, 0, 0);
	}
	else
	{
		ret.vertex[0] = Vec3D(pos.x - scale, pos.y - scale, pos.z);
		ret.vertex[1] = Vec3D(pos.x + scale, pos.y - scale, pos.z);
		ret.vertex[2] = Vec3D(pos.x - scale, pos.y + scale, pos.z);
		ret.vertex[3] = Vec3D(pos.x + scale, pos.y + scale, pos.z);
		ret.normal = Vec3D(0, 0, 1);
	}

	return ret;
}



// Initialize the maze at the specified scale
// Populates global quad arrays with vertex and color data
void InitMaze(GLfloat scale)
{
	Vec3D	color;
	GLfloat x, y, z;
	int		i;

	// ******************
	// *** Init floor ***
	x = 0.0f;
	z = 0.0f;
	y = -0.5f;
	for (i = 0; i < 16; ++i)
	{
		// Vertexes
		g_maze_floor[i].vertex[0] = Vec3D(x - scale, y, z + scale);
		g_maze_floor[i].vertex[1] = Vec3D(x + scale, y, z + scale);
		g_maze_floor[i].vertex[2] = Vec3D(x - scale, y, z - scale);
		g_maze_floor[i].vertex[3] = Vec3D(x + scale, y, z - scale);

		// Colors
		g_maze_floor[i].color[0] = Vec3D(0.5f, 0.0f, 0.8f);
		g_maze_floor[i].color[1] = Vec3D(0.0f, 0.9f, 0.8f);
		g_maze_floor[i].color[2] = Vec3D(1.0f, 0.0f, 0.0f);
		g_maze_floor[i].color[3] = Vec3D(0.0f, 0.6f, 0.6f);

		// Normal
		g_maze_floor[i].normal = Vec3D(0,1,0);

		// Move to next block
		x += (scale * 2);
		if (x >= (scale * 8))
		{
			x  = 0.0f;
			z -= (scale * 2);
		}
	}

	// ********************
	// *** Init ceiling ***
	x = 0.0f;
	z = 0.0f;
	y = (2 * scale) - 0.5f;
	for (i = 0; i < 16; ++i)
	{
		// Vertexes
		g_maze_ceil[i].vertex[0] = Vec3D(x - scale, y, z + scale);
		g_maze_ceil[i].vertex[1] = Vec3D(x + scale, y, z + scale);
		g_maze_ceil[i].vertex[2] = Vec3D(x - scale, y, z - scale);
		g_maze_ceil[i].vertex[3] = Vec3D(x + scale, y, z - scale);

		// Colors
		g_maze_ceil[i].color[0] = Vec3D(0.5f, 0.5f, 0.5f);
		g_maze_ceil[i].color[1] = Vec3D(0.6f, 0.6f, 0.6f);
		g_maze_ceil[i].color[2] = Vec3D(0.4f, 0.4f, 0.4f);
		g_maze_ceil[i].color[3] = Vec3D(0.3f, 0.3f, 0.3f);

		// Normal
		g_maze_ceil[i].normal = Vec3D(0,-1,0);

		// Move to next block
		x += (scale * 2);
		if (x >= (scale * 8))
		{
			x  = 0.0f;
			z -= (scale * 2);
		}
	}

	// Counter for wall cell array
	i = 0;

	// left outside wall
	for (z = 0.0f; z > -(scale*8); z -= (scale*2), ++i)
		g_maze_walls[i] = CreateWallCell(Vec3D(-scale, scale - 0.5f, z), scale, true);

	// back outside wall
	for (x = 0.0f; x < (scale*8); x += (scale*2), ++i)
		g_maze_walls[i] = CreateWallCell(Vec3D(x, (scale) - 0.5f, -(scale*7)), scale, false);

	// right outside wall (missing exit point)
	for (z = -(2*scale); z > -(scale*8); z -= (scale*2), ++i)
		g_maze_walls[i] = CreateWallCell(Vec3D((scale*7), scale - 0.5f, z), scale, true);

	// inside walls (missing pathways through maze)
	for (z = 0.0f; z > -(scale*6); z -= (scale*2), ++i)
		g_maze_walls[i] = CreateWallCell(Vec3D((scale*5), scale - 0.5f, z), scale, true);
	for (z = 0.0f; z > -(scale*6); z -= (scale*2), ++i)
		g_maze_walls[i] = CreateWallCell(Vec3D(scale, scale - 0.5f, z), scale, true);
	for (z = -(scale*2); z > -(scale*8); z -= (scale*2), ++i)
		g_maze_walls[i] = CreateWallCell(Vec3D((scale*3), scale - 0.5f, z), scale, true);

	// front outside wall (missing entry point)
	for (x = (scale*2); x < (scale*8); x += (scale*2), ++i)
		g_maze_walls[i] = CreateWallCell(Vec3D(x, scale - 0.5f, scale), scale, false);
}



// Check a given quad array for collisions in case of a movement from pos to mov
// Margin is there to account for OpenGLs zNEAR cutoff for the camera
bool CheckCollision(QuadCell* quads, int count, Vec3D pos, Vec3D mov, GLfloat margin = 0.6f)
{
	Vec3D	newpos;		// New position to move to
	Vec3D	intpos;		// Intersection point
	Vec3D	side[4];	// Side vectors
	Vec3D	intvec[2];	// Intersection vectors relative to two corner points of rect
	int i;
	bool collision = false;
	bool ret = false;
	GLfloat oldside;
	GLfloat newside;
	//char buffer[30];

	// Calculate new, potential position
	newpos = pos.add(mov);

	// Debug
        //OutputDebugString("Collision: ");
        qDebug("Collision: ");

	// Loop through all quads
	for (i = 0; i < count; ++i)
	{
		// Which side of the plane are the old and new positions?
		oldside = quads[i].normal.dot(pos.sub(quads[i].vertex[0]));
		newside = quads[i].normal.dot(newpos.sub(quads[i].vertex[0]));

		// Adjust based on move direction to "give camera some room"
		if (quads[i].normal.dot(mov) < 0) 
		{
			if (!i) 
				qDebug("towards ");
			
			newside -= margin;
		}
		else
		{
			if (!i) 
				qDebug("away ");
			
			newside += margin;
		}

		// Debug
		if (i == 0)
		{
                        qDebug("old: %.2f ", oldside);
                        //OutputDebugString(buffer);
                        qDebug("new: %.2f ", newside);
                        //OutputDebugString(buffer);
		}

		// Check if movement put us on the other side of plane
		if ((oldside > 0 && newside > 0) || (oldside < 0 && newside < 0))
			continue; // No! Skip it.

		// Estimate the intersection point
		intpos = newpos.extend(-margin);
		//midpos = newpos.sub(quads[i].vertex[0]);
		//midpos = midpos.extend(margin);
// 		midpos = newpos.sub(pos);
// 		midpos.set(midpos.x / 1.25, midpos.y / 1.25, midpos.z / 1.25);
// 		midpos = midpos.add(pos);

		// Debug
		if (!i)
		{
                        qDebug("intpos: %.2f,%.2f,%.2f ", intpos.x, intpos.y, intpos.x);
                        //OutputDebugString(buffer);
		}

		// Rect looks like this:
		//   
		// [2] ------- [3]
		//     |     |    
		//     |  *  |
		//     |     |    
		// [0] ------- [1]
		// * = intersection
		// [0] -> [1] = side[0]
		// [0] -> [2] = side[1]
		// [0] ->  *  = intvec[0]
		// [3] -> [2] = side[2]
		// [3] -> [1] = side[3]
		// [3] ->  *  = intvec[1]

		// Calculate side vectors
		side[0] = quads[i].vertex[1].sub(quads[i].vertex[0]);
		side[1] = quads[i].vertex[2].sub(quads[i].vertex[0]);
		side[2] = quads[i].vertex[2].sub(quads[i].vertex[3]);
		side[3] = quads[i].vertex[1].sub(quads[i].vertex[3]);
		
		// Calculate intersection vectors
		intvec[0] = intpos.sub(quads[i].vertex[0]);
		intvec[1] = intpos.sub(quads[i].vertex[3]);

		// Check if intersection is in rect
		collision = true;
		if (side[0].dot(intvec[0]) < 0) collision = false;
		if (side[1].dot(intvec[0]) < 0) collision = false;
		if (side[2].dot(intvec[1]) < 0) collision = false;
		if (side[3].dot(intvec[1]) < 0) collision = false;
		if (collision) ret = true;

		// Debug
                if (collision) 
					qDebug("collision!");
	}
        qDebug("\n");
	//DbgPrint("test");

	return ret;
}


bool CheckCollision(Vec3D pos, Vec3D mov)
{
	bool ret = false;

	ret =  CheckCollision(g_maze_floor, 16, pos, mov, 0.6f)
		|| CheckCollision(g_maze_ceil,  16, pos, mov, 0.6f)
		|| CheckCollision(g_maze_walls, sizeof(g_maze_walls) / sizeof(QuadCell), pos, mov, 0.65f);

	return ret;
}

void RenderQuads(QuadCell* quads, int count)
{
	int i, j; 

	for (i = 0; i < count; ++i)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (j = 0; j < 4; ++j)
		{
			glNormal3f(quads[i].normal.x, quads[i].normal.y, quads[i].normal.z);
			glColor3f(quads[i].color[j].x, quads[i].color[j].y, quads[i].color[j].z);
			glVertex3f(quads[i].vertex[j].x, quads[i].vertex[j].y, quads[i].vertex[j].z);
		}
		glEnd();
	}
}

void RenderMaze(void)
{
	RenderQuads(g_maze_floor, 16);
	RenderQuads(g_maze_walls, sizeof(g_maze_walls) / sizeof(QuadCell));
	RenderQuads(g_maze_ceil, 16);
}

