/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a player and hurdles with collision detection and jump effect.
 *
 */

#ifndef XML_PATH_H
#define XML_PATH_H

typedef struct Path_t Path;

#define DEFAULT_XML_CONFIG_FILE "config.xml"

Path* get_path(int argc, char** argv);
void delete_path(Path** path);
const char* get_full_path(Path* path);

#endif
