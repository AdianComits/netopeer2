/**
 * @file netconf_yang_push.h
 * @author Michal Vasko <mvasko@cesnet.cz>
 * @brief ietf-yang-push callbacks header
 *
 * Copyright (c) 2021 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef NP2SRV_NETCONF_YANG_PUSH_H_
#define NP2SRV_NETCONF_YANG_PUSH_H_

#include <libyang/libyang.h>
#include <sysrepo.h>

int np2srv_rpc_establish_yang_push_cb(sr_session_ctx_t *session, const char *op_path, const struct lyd_node *input,
        sr_event_t event, uint32_t request_id, struct lyd_node *output, void *private_data);

int np2srv_rpc_modify_yang_push_cb(sr_session_ctx_t *session, const char *op_path, const struct lyd_node *input,
        sr_event_t event, uint32_t request_id, struct lyd_node *output, void *private_data);

#endif /* NP2SRV_NETCONF_YANG_PUSH_H_ */