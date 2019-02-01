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
#include "../../util/list/list.h"
#include "../../util/util.h"
#include "../../shape/circle/circle.h"
#include "../../object/character/character.h"
#include "svg_loader.h"
#include "../application.h"

#define cast_svg(a) ((Svg *) (a))

typedef struct
{
    Tag current_tag;

    /* tmp */
    int id;
    float cx;
    float cy;
    float r;
    float red;
    float green;
    float blue;
    /*******/

    /* hold */
    int hold;
    int hold_id;
    float hold_cx;
    float hold_cy;
    float hold_r;
    float hold_red;
    float hold_green;
    float hold_blue;
    /*******/

    Application* app;
} Svg;

static Svg* new_svg(Application* app)
{
    Svg* new;
    get_memory(new, sizeof(Svg));
    new->current_tag = NONE_TAG;
    new->app = app;
    new->hold = 0;
    return new;
}

static void delete_svg(Svg** svg)
{
    free_memory(svg);
}

static int on_opentag(void* param, int depth, char* name)
{
    unused(depth);

    if(equal_str("circle", name))
    {
        cast_svg(param)->current_tag = CIRCLE_TAG;
    }
    else /* other unknown tag */
    {
        cast_svg(param)->current_tag = NONE_TAG;
    }

    return 0;
}

static int on_attribute(void* param, int depth, char* name, char* value)
{
    unused(depth);

    switch(cast_svg(param)->current_tag)
    {
        case CIRCLE_TAG:

            if(equal_str("id", name))
            {
                cast_svg(param)->id = atoi(value); 
            }
            else if(equal_str("cx", name))
            {
                cast_svg(param)->cx = atof(value);           
            }
            else if(equal_str("cy", name))
            {
                cast_svg(param)->cy = atof(value);     
            }
            else if(equal_str("r", name))
            {
                cast_svg(param)->r = atof(value);           
            }
            else if(equal_str("fill", name))
            {
                if(equal_str("red", value))
                {
                    cast_svg(param)->red = MAX_COLOR;
                    cast_svg(param)->green = MIN_COLOR;
                    cast_svg(param)->blue = MIN_COLOR;
                }
                else if(equal_str("green", value))
                {
                    cast_svg(param)->red = MIN_COLOR;
                    cast_svg(param)->green = MAX_COLOR;
                    cast_svg(param)->blue = MIN_COLOR;
                }
                else if(equal_str("blue", value))
                {
                    cast_svg(param)->red = MIN_COLOR;
                    cast_svg(param)->green = MIN_COLOR;
                    cast_svg(param)->blue = MAX_COLOR;
                }   
                else if(equal_str("white", value))
                {
                    cast_svg(param)->red = MAX_COLOR;
                    cast_svg(param)->green = MAX_COLOR;
                    cast_svg(param)->blue = MAX_COLOR;
                }
                else if(equal_str("black", value))
                {
                    cast_svg(param)->red = MIN_COLOR;
                    cast_svg(param)->green = MIN_COLOR;
                    cast_svg(param)->blue = MIN_COLOR;
                }
            }
            break;

        default:
            break;
    }

    return 0;
}

static void on_closetag_circle_tag(Svg* svg)
{ 
    static int done = 0;

    if(is_red(svg->red, svg->green, svg->blue))
    {
        app_add_enemy(svg->app, svg->id, svg->r, svg->cx, svg->cy, svg->red, svg->green, svg->blue);
    }
    else if(is_green(svg->red, svg->green, svg->blue))
    {
        set_player(svg->app, new_character(PLAYER, svg->id, svg->r, svg->cx, svg->cy, svg->red, svg->green, svg->blue));
    }
    else if(is_blue(svg->red, svg->green, svg->blue))
    {
        if(!svg->hold)
        {
            svg->hold = 1;
            svg->hold_id = svg->id;
            svg->hold_cx = svg->cx;
            svg->hold_cy = svg->cy;
            svg->hold_r = svg->r;
            svg->hold_red = svg->red;
            svg->hold_green = svg->green;
            svg->hold_blue = svg->blue;
        }
        else if(!done)
        {
            set_arena(svg->app, new_arena(svg->hold_r, svg->hold_cx, svg->hold_cy, svg->hold_red, svg->hold_green, svg->hold_blue,
                 svg->r, svg->cx, svg->cy, svg->red, svg->green, svg->blue));
            done = 1;
        }
    }
    else if(is_white(svg->red, svg->green, svg->blue))
    {
        if(!svg->hold)
        {
            svg->hold = 1;
            svg->hold_id = svg->id;
            svg->hold_cx = svg->cx;
            svg->hold_cy = svg->cy;
            svg->hold_r = svg->r;
            svg->hold_red = svg->red;
            svg->hold_green = svg->green;
            svg->hold_blue = svg->blue;
        }
        else if(!done)
        {
            set_arena(svg->app, new_arena(svg->r, svg->cx, svg->cy, svg->red, svg->green, svg->blue,
                 svg->hold_r, svg->hold_cx, svg->hold_cy, svg->hold_red, svg->hold_green, svg->hold_blue));
            done = 1;
        }
    }
    else if(is_black(svg->red, svg->green, svg->blue))
    {
        app_add_hurdle(svg->app, svg->r, svg->cx, svg->cy, svg->red, svg->green, svg->blue);
    }
    else
    {
        raise_error("Unexpected color for circle in svg file");
    }
}


static int on_closetag(void* param, int depth, char* name)
{ 
    unused(depth);
    unused(name);

    switch(cast_svg(param)->current_tag)
    {
        case CIRCLE_TAG:
            on_closetag_circle_tag(cast_svg(param));
            break;

        default:
            break;
    }

    cast_svg(param)->current_tag = NONE_TAG;

    return 0; 
}


static int on_endattribute(void* param, int depth)
{ 
    unused(param);
    unused(depth);
    return 0; 
}

static int on_text(void* param, int depth, char* text)
{ 
    unused(param);
    unused(depth);
    unused(text);
    return 0; 
}


void set_app_with_svg(Application* app, const char* svg_file_name)
{
    Svg* svg;

    svg = new_svg(app);

    if(xml_parse(svg_file_name, svg, on_opentag, on_closetag, on_attribute, on_endattribute, on_text))
        raise_error("The xml file is not in correct format.");

    delete_svg(&svg);
}
