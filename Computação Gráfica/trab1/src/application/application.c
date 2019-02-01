/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a circle with drag-and-drop and resize functions.
 *
 */
 
#include "application.h"
#include "../util/get_memory.h"

Application* new_application(void)
{
    Application* new;
    get_memory(new, sizeof(Application));
    return new;
}

void delete_application(Application** app)
{
    free((*app)->title);
    (*app)->title = NULL;
    free(*app);
    app = NULL;
}

