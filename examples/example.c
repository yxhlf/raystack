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
#include <stdio.h>
#include <ray_log.h>
#include <ray_errno.h>
#include <ray_string.h>
#include <ray_packet.h>
#include "ray_device.h"
#include <ray_devif_class.h>	

#include <pcap/pcap.h>

#define VIRT_DEVICE_SOURCE "./examples/pcap/test.pcap"


void dev_loop(ray_devif_t *devif)
{
	pcap_t *sources;
	ray_packet_t packet;
	ray_u8_t *data, errbuf[1024];
	ray_s32_t nonblock = 0;
	ray_u32_t count = 0;
	ray_devif_ops_t *ops;
	struct pcap_pkthdr header;

	ops = devif->ops;

	sources = pcap_open_offline(VIRT_DEVICE_SOURCE, errbuf);
	if (sources == NULL) {
		RAY_LOG(INFO, "%s\n", ray_strerror(errno))
		return;
	}
	pcap_setnonblock(sources, nonblock, errbuf);

	while(count++ < 100) {
		data = pcap_next(sources, &header);
		packet.data = data;
		packet.data_len  = header.len;
		packet.data_off  = 0;
		ops->if_input(devif, &packet);
	}
	pcap_close(sources);
}

int main()
{
	ray_devif_t *devif;
	ray_devif_ops_t *devops;
	ray_devif_class_t *dpdk_class = devif_class_get_byname("virt");
	if (dpdk_class == NULL) {
		RAY_LOG(ERR, "%s\n", ray_strerror(errno));
		return -1;
	}
	dpdk_class->init();

	devif = dpdk_class->create_dev();
	if (devif == NULL) {
		RAY_LOG(ERR, "Create dpdk device failed\n");
		return -1;
	}
	devops = devif->ops;
	devops->if_start(devif, 1, dev_loop);
	/* implement manager */
	while (1) {
		sleep(1);
	}
	return 0;
}