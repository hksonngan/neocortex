// Contains common stuff for all project units
#include "common_defs.h"

void RunShader(int width, int height, int handle)
{
	/*	glViewport(0, 0, width, height);    
	glMatrixMode(GL_PROJECTION );
	glLoadIdentity(); 
	glOrtho(0.0F, 1.0F, 0.0F, 1.0F, -1.0F, 1.0F);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();				
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	{
	glTexCoord2f(0.0F, 0.0F );  
	glVertex2f(0.0F, 0.0F);

	glTexCoord2f(0.0F, height);
	glVertex2f(0.0F, 1.0F);

	glTexCoord2f(width, height); 
	glVertex2f(1.0F, 1.0F);

	glTexCoord2f(width, 0.0F ); 
	glVertex2f(1.0F, 0.0F);
	}
	glEnd();
	*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0, 0, -10.0);
	glRotatef(90.0f, 0.0f, 0.0, 1.0f);
	glRotatef(1, 1.0f, 0.0f, 0.0f);
	glRotatef(1, 0.0f, 1.0f, 0.0f);


			// визуализация по слоям
//			float width = (float)(InputData->sizeX / 2) * InputData->scaleX;
//			float height = (float)(InputData->sizeY / 2) * InputData->scaleY;
			glTranslatef(0.0f, 0.0f, 0.0f);
			glBindTexture(GL_TEXTURE_2D, handle);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, -height, 0.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( width, -height, 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( width,  height, 0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-width,  height, 0.0f);
			}
			glEnd();

//			glBindTexture(GL_TEXTURE_2D, 0);
}