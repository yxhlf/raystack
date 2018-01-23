/*-
 * Copyright (c) 2018, Honglin Xiao
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <ray_log.h>
#include <ray_errno.h>
#include <ray_device.h>
#include <ray_string.h>
#include <ray_devif_class.h>


static struct 

static struct ray_tailq_devclass_head devclasses_head =
					TAILQ_HEAD_INITIALIZER(devclasses_head);

ray_s32_t devclass_exist(ray_devif_class_t *devif_class)
{
	ray_s32_t ret = 0;
	struct ray_tailq_devclass_elem *devclass = NULL;

	TAILQ_FOREACH(devclass, &devclasses_head, next) {
		if (ray_strcmp(devclass->type, devif_class->type) == 0) {
			ret = 1;
		}
	}
	return ret;
}

ray_devif_class_t *
get_devif_class(ray_consts8_t *class_name)
{
	struct ray_tailq_devclass_elem *devclass = NULL;

	TAILQ_FOREACH(devclass, &devclasses_head, next) {
		RAY_LOG("","", "%s\n", devclass->type);
	}
}

void register_devif_class(ray_devif_class_t *devif_class)
{
	struct ray_tailq_devclass_elem *new_elem;
	/* Invalid parameter check */
	if(devif_class == NULL) {
		RAY_LOG("", "", "Invalid Parameter!\n");
		return -EINVAL;
	}

	if (devclass_exist(devif_class))
		return -EEXIST;

	new_elem = ray_malloc();
}