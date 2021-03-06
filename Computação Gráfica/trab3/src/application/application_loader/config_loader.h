/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of shooting and improvements on moving.
 *
 */

#ifndef XML_CONFIG_H
#define XML_CONFIG_H

typedef struct Config_t Config;

#define DEFAULT_XML_CONFIG_FILE "config.xml"

Config* get_config(int argc, char** argv);
void delete_config(Config** config);
const char* get_full_path(Config* config);
float get_speed(Config* config);
float get_shot_speed(Config* config);

#endif
