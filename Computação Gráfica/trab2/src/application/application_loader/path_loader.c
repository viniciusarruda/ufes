/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a player and hurdles with collision detection and jump effect.
 *
 */

#include "xml_parser/xml_parser.h"
#include "xml_parser/xml_tag.h"
#include "../../util/util.h"
#include "path_loader.h"
#include <string.h>
#include <stdlib.h>

#define cast_path(a) ((Path *) (a))

struct Path_t
{
    Tag current_tag;
    char* name;
    char* type;
    char* path;
    char* full_path;
};

static Path* new_path(void)
{
    Path* new;
    get_memory(new, sizeof(Path));
    new->current_tag = NONE_TAG;
    new->name = NULL;
    new->type = NULL;
    new->path = NULL;
    new->full_path = NULL;
    return new;
}

void delete_path(Path** path)
{
    free((*path)->name);
    free((*path)->type);
    free((*path)->path);
    free((*path)->full_path);
    free(*path);
    *path = NULL;
}

int on_opentag(void* param, int depth, char* name)
{
    unused(depth);

    if(equal_str("aplicacao", name))
    {
        cast_path(param)->current_tag = APP_TAG;
    }
    else if(equal_str("arquivoDaArena", name))
    {
        cast_path(param)->current_tag = ARENA_PATH_TAG;
    }

    return 0;
}

int on_attribute(void* param, int depth, char* name, char* value)
{
    unused(depth);

    switch(cast_path(param)->current_tag)
    {
        case ARENA_PATH_TAG:

            if(equal_str("nome", name))
            {
                cast_path(param)->name = strdup(value);           
            }
            else if(equal_str("tipo", name))
            {
                cast_path(param)->type = strdup(value);     
            }
            else if(equal_str("caminho", name))
            {
                cast_path(param)->path = strdup(value);           
            }
            break;

        default:
            break;
    }

    return 0;
}

int on_closetag(void* param, int depth, char* name)
{ 
    unused(param);
    unused(depth);
    unused(name);
    return 0; 
}

int on_endattribute(void* param, int depth)
{ 
    unused(param);
    unused(depth);
    return 0; 
}

int on_text(void* param, int depth, char* text)
{ 
    unused(param);
    unused(depth);
    unused(text);
    return 0; 
}


static void build_full_path(Path* path)
{
    get_memory(path->full_path, sizeof(char) * (strlen(path->path) + strlen(path->name) + strlen(path->type) + 2));
    path->full_path[0] = '\0';
    strcat(path->full_path, path->path);
    strcat(path->full_path, path->name);
    strcat(path->full_path, ".");
    strcat(path->full_path, path->type);
}

static char* get_xml_file_name(int argc, char** argv)
{
    char* xml_file_name;

    if(argc > 1)
    {
        get_memory(xml_file_name, sizeof(char) * (strlen(DEFAULT_XML_CONFIG_FILE) + strlen(argv[1]) + 1));
        xml_file_name[0] = '\0';
        strcat(xml_file_name, argv[1]);
    }
    else
    {
        get_memory(xml_file_name, sizeof(char) * (strlen(DEFAULT_XML_CONFIG_FILE) + 1));
        xml_file_name[0] = '\0';
    }
    strcat(xml_file_name, DEFAULT_XML_CONFIG_FILE);

    return xml_file_name;
}

const char* get_full_path(Path* path) { return path->full_path; }

Path* get_path(int argc, char** argv)
{
    char* xml_file_name;
    Path* path = new_path();

    xml_file_name = get_xml_file_name(argc, argv);

    if(xml_parse(xml_file_name, path, on_opentag, on_closetag, on_attribute, on_endattribute, on_text))
        raise_error("The xml file is not in correct format.");

    free(xml_file_name);

    build_full_path(path);

    return path;
}


