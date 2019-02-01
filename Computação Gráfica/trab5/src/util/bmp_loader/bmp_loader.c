#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include "bmpread.h"


/* Load the specified bitmap file from disk and copy it into an OpenGL texture.
 * Return the GLuint representing the texture.
 */
GLuint LoadTexture(const char * bitmap_file)
{
    GLuint texture = 0;
    bmpread_t bitmap;

    if(!bmpread(bitmap_file, BMPREAD_ANY_SIZE, &bitmap))
    {
        fprintf(stderr, "%s: error loading bitmap file\n", bitmap_file);
        exit(1);
    }

    /* At this point, bitmap.width and .height hold the pixel dimensions of the
     * file, and bitmap.rgb_data holds the raw pixel data in RGB triplets.
     */

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);  

    // mudar o local disso abaixo
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE ); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // brincar com isso    .. tem o wrap tbm
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // testar com diferentes parametros.. mas somente no final..

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //////////
    

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data);

    bmpread_free(&bitmap);

    return texture;
}

