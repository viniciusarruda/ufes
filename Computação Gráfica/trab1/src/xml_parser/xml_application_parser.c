/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */

#include "xml_parser.h"
#include "../util/error.h"
#include "../application/application.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static int cmp(const char* a, const char* b)
{
    return strcmp(a, b) == 0;
}

int on_opentag(void* param, int depth, char* name)
{
    if(cmp("aplicacao", name))
    {
        CAST_APP(param)->current_tag = APP;
    }
    else if(cmp("janela", name))
    {
        CAST_APP(param)->current_tag = WINDOW;
    }
    else if(cmp("largura", name))
    {
        CAST_APP(param)->current_tag = WIDTH;
    }
    else if(cmp("altura", name))
    {
        CAST_APP(param)->current_tag = HEIGHT;
    }
    else if(cmp("fundo", name))
    {
        CAST_APP(param)->current_tag = BACKGROUND;
    }
    else if(cmp("titulo", name))
    {
        CAST_APP(param)->current_tag = TITLE;
    }
    else if(cmp("circulo", name))
    {
        CAST_APP(param)->current_tag = CIRCLE;
    }

    return 0;
}

int on_closetag(void* param, int depth, char* name)
{
    return 0;
}

int on_attribute(void* param, int depth, char* name, char* value)
{
    switch(CAST_APP(param)->current_tag)
    {
        case BACKGROUND:

            if(cmp("corR", name))
            {
                CAST_APP(param)->bg_r = atof(value);           
            }
            else if(cmp("corG", name))
            {
                CAST_APP(param)->bg_g = atof(value);           
            }
            else if(cmp("corB", name))
            {
                CAST_APP(param)->bg_b = atof(value);                      
            } 
            break;

        case CIRCLE:

            if(cmp("raio", name))
            {
                CAST_APP(param)->radius = atof(value);           
            }
            else if(cmp("corR", name))
            {
                CAST_APP(param)->circle_r = atof(value);           
            }
            else if(cmp("corG", name))
            {
                CAST_APP(param)->circle_g = atof(value);           
            }
            else if(cmp("corB", name))
            {
                CAST_APP(param)->circle_b = atof(value);                      
            } 
            break;

        default:
            break;
    }

    return 0;
}

int on_endattribute(void* param, int depth)
{
    return 0;
}

int on_text(void* param, int depth, char* text)
{
    switch(CAST_APP(param)->current_tag)
    {
        case WIDTH:
            CAST_APP(param)->width = atoi(text);
            break;
        case HEIGHT:
            CAST_APP(param)->height = atoi(text);
            break;
        case TITLE:
            CAST_APP(param)->title = strdup(text);
            break;
        default:
            break;
    }

    return 0;
}


static char* read_xml(const char* filename)
{
    char* buffer = NULL;
    long length;
    FILE* f = fopen(filename, "rb");

    if(f == NULL)
        raise_error("Cannot open XML file.\n");

    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    if((buffer = malloc(length + 1)) == NULL)
        raise_error("Cannot allocates memory.\n");
    
    fread(buffer, 1, length, f);
    fclose(f);

    buffer[length] = '\0';

    return buffer;
}


Application* get_xml_content(const char* xml_file)
{
    int parseerr;
    char* buffer;
    Application* app = new_application();

    buffer = read_xml(xml_file);

    parseerr = xml_parse(buffer,
        app,
        on_opentag,
        on_closetag,
        on_attribute,
        on_endattribute,
        on_text);

    free(buffer);

    if(parseerr)
        raise_error("The xml file is not in correct format.\n");

    return app;
}


