/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */

#include "xml_parser/xml_parser.h"
#include "xml_parser/xml_tag.h"
#include "../../util/util.h"
#include "config_loader.h"
#include <string.h>
#include <stdlib.h>

struct Config_t
{
    Tag current_tag;
    char* name;
    char* type;
    char* path;
    char* full_path;
    float shot_speed;
    float speed;
    float enemy_shot_speed;
    float enemy_speed;
    float hurdle_percentage;
    float freq_shoot;
};

static Config* new_config(void)
{
    Config* new;
    get_memory(new, sizeof(Config));
    new->current_tag = NONE_TAG;
    new->name = NULL;
    new->type = NULL;
    new->path = NULL;
    new->full_path = NULL;
    return new;
}

void delete_config(Config** config)
{
    free((*config)->name);
    free((*config)->type);
    free((*config)->path);
    free((*config)->full_path);
    free(*config);
    *config = NULL;
}

int on_opentag(void* param, int depth, char* name)
{
    unused(depth);

    if(equal_str("aplicacao", name))
    {
        cast(param, Config)->current_tag = APP_TAG;
    }
    else if(equal_str("arquivoDaArena", name))
    {
        cast(param, Config)->current_tag = ARENA_PATH_TAG;
    }
    else if(equal_str("jogador", name))
    {
        cast(param, Config)->current_tag = PLAYER_TAG;
    }
    else if(equal_str("inimigo", name))
    {
        cast(param, Config)->current_tag = ENEMY_TAG;
    }
    else if(equal_str("obstaculo", name))
    {
        cast(param, Config)->current_tag = HURDLE_TAG;
    }
    else /* other unknown tag */
    {
        cast(param, Config)->current_tag = NONE_TAG;
    }

    return 0;
}

int on_attribute(void* param, int depth, char* name, char* value)
{
    unused(depth);

    switch(cast(param, Config)->current_tag)
    {
        case ARENA_PATH_TAG:

            if(equal_str("nome", name))
            {
                cast(param, Config)->name = strdup(value);           
            }
            else if(equal_str("tipo", name))
            {
                cast(param, Config)->type = strdup(value);     
            }
            else if(equal_str("caminho", name))
            {
                cast(param, Config)->path = strdup(value);           
            }
            break;

        case PLAYER_TAG:

            if(equal_str("velTiro", name))
            {
                cast(param, Config)->shot_speed = atof(value);           
            }
            else if(equal_str("vel", name))
            {
                cast(param, Config)->speed = atof(value);           
            }
            break;

        case ENEMY_TAG:

            if(equal_str("velTiro", name))
            {
                cast(param, Config)->enemy_shot_speed = atof(value);           
            }
            else if(equal_str("vel", name))
            {
                cast(param, Config)->enemy_speed = atof(value);           
            }
            else if(equal_str("freqTiro", name))
            {
                cast(param, Config)->freq_shoot = atof(value);           
            }
            break;

        case HURDLE_TAG:

            if(equal_str("altura", name))
            {
                cast(param, Config)->hurdle_percentage = atof(value);           
            }

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


static void build_full_path(Config* config)
{
    get_memory(config->full_path, sizeof(char) * (strlen(config->path) + strlen(config->name) + strlen(config->type) + 2));
    config->full_path[0] = '\0';
    strcat(config->full_path, config->path);
    strcat(config->full_path, config->name);
    strcat(config->full_path, ".");
    strcat(config->full_path, config->type);
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

const char* get_full_path(Config* config) { return config->full_path; }
float get_speed(Config* config) { return config->speed; }
float get_shot_speed(Config* config) { return config->shot_speed; }
float get_enemy_speed(Config* config) { return config->enemy_speed; }
float get_enemy_shot_speed(Config* config) { return config->enemy_shot_speed; }
float get_hurdle_percentage(Config* config) { return config->hurdle_percentage; }
float get_freq_shot(Config* config) { return config->freq_shoot; }

Config* get_config(int argc, char** argv)
{
    char* xml_file_name;
    Config* config = new_config();

    xml_file_name = get_xml_file_name(argc, argv);

    if(xml_parse(xml_file_name, config, on_opentag, on_closetag, on_attribute, on_endattribute, on_text))
        raise_error("The xml file is not in correct format.");

    free(xml_file_name);

    build_full_path(config);

    return config;
}


