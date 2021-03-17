/**
 * @file subscribed_notifications.h
 * @author Michal Vasko <mvasko@cesnet.cz>
 * @brief ietf-subscribed-notifications sub-ntf callbacks header
 *
 * Copyright (c) 2021 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef NP2SRV_SUBSCRIBED_NOTIFICATIONS_H_
#define NP2SRV_SUBSCRIBED_NOTIFICATIONS_H_

#include <stdint.h>

#include <libyang/libyang.h>
#include <sysrepo.h>

#include "common.h"

struct np2srv_sub_ntf_sr;
struct np2srv_sub_ntf;

/**
 * @brief Type-specific data for these subscriptions.
 */
struct np2srv_sub_ntf_data {
    char *stream_filter_name;
    struct lyd_node *stream_subtree_filter;
    char *stream_xpath_filter;
    char *stream;
    time_t replay_start_time;
    ATOMIC_T denied_count;  /* notifications denied by NACM */
};

/**
 * @brief Called on establish-subscription RPC, should create any required sysrepo subscriptions and type-specific data.
 *
 * @param[in] ev_sess Event session.
 * @param[in] rpc RPC data.
 * @param[in] stop Subscription stop time.
 * @param[out] sub_ids Array of sub IDs of sysrepo subscriptions.
 * @param[out] sub_id_count Length of @p sub_ids.
 * @param[out] data Type-specific data.
 * @return Sysrepo error value.
 */
int sub_ntf_rpc_establish_sub(sr_session_ctx_t *ev_sess, const struct lyd_node *rpc, time_t stop, uint32_t **sub_ids,
        uint32_t *sub_id_count, void **data);

/**
 * @brief Called on modify-subscription RPC, should update sysrepo subscriptions and type-specific data accordingly.
 * sub-ntf lock held.
 *
 * @param[in] ev_sess Event session.
 * @param[in] rpc RPC data.
 * @param[in] stop Subscription stop time.
 * @param[in,out] sub sub-ntf subscription to update.
 * @return Sysrepo error value.
 */
int sub_ntf_rpc_modify_sub(sr_session_ctx_t *ev_sess, const struct lyd_node *rpc, time_t stop, struct np2srv_sub_ntf *sub);

/**
 * @brief Called on subscription-modified notification, should append type-specific YANG nodes.
 * sub-ntf lock held.
 *
 * @param[in] ntf Notification to append to.
 * @param[in] data Type-specific data.
 * @return Sysrepo error value.
 */
int sub_ntf_notif_modified_append_data(struct lyd_node *ntf, void *data);

/**
 * @brief Called for every configuration change in type-specific filters.
 * sub-ntf lock held.
 *
 * @param[in] filter Changed filter node.
 * @param[in] op Sysrepo operation.
 * @return Sysrepo error value.
 */
int sub_ntf_config_filters(const struct lyd_node *filter, sr_change_oper_t op);

/**
 * @brief Should append type-specific operational YANG nodes to "subscription" node.
 * sub-ntf lock held.
 *
 * @param[in] subscription Subscription to append to.
 * @param[in] data Type-specific data.
 * @return Sysrepo error value.
 */
int sub_ntf_oper_subscription(struct lyd_node *subscription, void *data);

/**
 * @brief Get excluded event count for a subscription.
 * sub-ntf lock held.
 *
 * @param[in] sub sub-ntf subscription to read from.
 * @return Number of excluded events because of non-matching fliter or denied NACM permission.
 */
uint32_t sub_ntf_oper_receiver_excluded(struct np2srv_sub_ntf *sub);

/**
 * @brief Terminate a type-specific sysrepo subscription, should delete the sub ID from sub-ntf information data.
 * sub-ntf lock held.
 *
 * @param[in] sub_id Sysrepo subscription ID.
 * @return Sysrepo error value.
 */
int sub_ntf_terminate_sr_sub(uint32_t sub_id);

/**
 * @brief Free type-specific data.
 *
 * @param[in] data Type-specific data to free.
 */
void sub_ntf_data_destroy(void *data);

#endif /* NP2SRV_SUBSCRIBED_NOTIFICATIONS_H_ */
