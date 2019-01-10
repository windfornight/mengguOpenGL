#include "sprite.h"
#include "utils.h"

void Sprite2D::SetImage(const char *imagePath)
{
	//mTexture = CreateTexture2DFromBMP(imagePath);
	mTexture = CreateTexture2DFromPNG(imagePath);
}

void Sprite2D::SetRect(float x, float y, float width, float height)
{
	mVertexes[0].x = x;
	mVertexes[0].y = y;
	mVertexes[1].x = x + width;
	mVertexes[1].y = y;
	mVertexes[2].x = x + width;
	mVertexes[2].y = y + height;
	mVertexes[3].x = x;
	mVertexes[3].y = y + height;
}

void Sprite2D::Draw()
{
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glDisable(GL_LIGHTING);
	glColor4ub(255, 255, 255, 255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(mVertexes[0].x, mVertexes[0].y, mVertexes[0].z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(mVertexes[1].x, mVertexes[1].y, mVertexes[1].z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(mVertexes[2].x, mVertexes[2].y, mVertexes[2].z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(mVertexes[3].x, mVertexes[3].y, mVertexes[3].z);
	glEnd();

	glDisable(GL_BLEND);
}