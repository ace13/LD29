#include "OBJLoader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <fstream>

#ifdef _WIN32
#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

OBJFile::OBJFile() : mTime(0)
{
	mTexture.loadFromFile("ace.png");
}

OBJFile::~OBJFile()
{

}

bool OBJFile::loadFromFile(const std::string& file)
{
	std::ifstream ifs(file);

	if (!ifs.is_open())
		return false;

	while (!ifs.eof())
	{
		std::string line;
		std::getline(ifs, line);

		switch (line[0])
		{
		case 'v':
		{
			float x, y, z;
			sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);

			mVerts.push_back(sf::Vector3f(x, y, z));
		} break;

		case 'u':
		{
			float x, y;
			sscanf(line.c_str(), "u %f %f", &x, &y);

			mUV.push_back(sf::Vector2f(x, y));
		} break;

		case 'f':
		{
			unsigned int x1, y1, z1, x2, y2, z2;

			sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &x1, &x2, &y1, &y2, &z1, &z2);

			mFaces1.push_back(sf::Vector3i(x1, y1, z1));
			mFaces2.push_back(sf::Vector3i(x2, y2, z2));
		} break;
		}
	}

	return true;
}

void OBJFile::update(double dt)
{
	mTime += dt * 36;
}

void OBJFile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.pushGLStates();

	glViewport(0, 0, (GLsizei)target.getSize().x, (GLsizei)target.getSize().y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)target.getSize().x / (GLfloat)target.getSize().y, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

	GLfloat pos[] = { 1, 1, 0, 0 };
	GLfloat amb[] = { 0, 0, 0, 1 };
	GLfloat spe[] = { 1, 1, 1, 1 };
	GLfloat dif[] = { 0.2, 0.2, 0.2, 1 };

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	sf::Texture::bind(&mTexture);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, -3.0, 0.0, 0.0, 1.0, 0.0);

	glRotatef(std::cos(mTime / 36) * 50, 0, 0, 1);
	glTranslatef(0, 1 - mTime / 15, 0);
	//glColor3f(0.8f, 0.8f, 0.2f);

	glBegin(GL_TRIANGLES);
	const sf::Vector3i* faces = &mFaces1.front();
	const sf::Vector3i* faceu = &mFaces2.front();
	const sf::Vector3f* verts = &mVerts.front();
	const sf::Vector2f* uvs = &mUV.front();

	for (int i = 0; i < mFaces1.size(); i++)
	{
		sf::Vector3i tri = faces[i];
		sf::Vector3i nor = faceu[i];

		sf::Vector3f vert = verts[tri.x - 1];
		sf::Vector2f uv   = uvs[nor.x - 1];

		glNormal3d(0, 0, 1);
		glTexCoord2f(uv.x, uv.y);
		glVertex3f(vert.x, vert.y, vert.z);

		vert = verts[tri.y - 1];
		uv = uvs[nor.y - 1];

		glNormal3d(0, 0, 1);
		glTexCoord2f(uv.x, uv.y);
		glVertex3f(vert.x, vert.y, vert.z);

		vert = verts[tri.z - 1];
		uv = uvs[nor.z - 1];

		glNormal3d(0, 0, 1);
		glTexCoord2f(uv.x, uv.y);
		glVertex3f(vert.x, vert.y, vert.z);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);

	target.popGLStates();
	target.resetGLStates();
}
