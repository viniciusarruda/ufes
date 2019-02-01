/*
 * Author: Vinicius Ferraco Arruda
 * Email:  viniciusferracoarruda@gmail.com
 * Website: viniciusarruda.github.io
 * Professor: Thiago Oliveira dos Santos
 * Subject: Computer Graphics
 * Assignment: Implementation of a gameplay.
 *
 */

#include "../enemies/enemies.h"


int player_collided(void* arg, float cx, float cy, float radius)
{
	return collided_with_enemies(arg, cx, cy, radius);
}

