/****************************************************************
 *
 *        Copyright 2013, Big Switch Networks, Inc. 
 * 
 * Licensed under the Eclipse Public License, Version 1.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * 
 *        http://www.eclipse.org/legal/epl-v10.html
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the
 * License.
 *
 ****************************************************************/

/* Copyright 2013, Big Switch Networks, Inc. */

/**
 * Test message validator
 *
 * Run the message validator on corrupt messages to ensure it catches them.
 */

#include "loci_log.h"

#include <locitest/test_common.h>
#include <loci/loci_validator.h>

static int
test_validate_fixed_length(void)
{
    of_table_stats_request_t *obj = of_table_stats_request_new(OF_VERSION_1_0);
    of_message_t msg = OF_OBJECT_TO_MESSAGE(obj);

    TEST_ASSERT(of_validate_message(msg, of_message_length_get(msg)) == 0);

    of_message_length_set(msg, of_message_length_get(msg) - 1);
    TEST_ASSERT(of_validate_message(msg, of_message_length_get(msg)) == -1);

    of_table_stats_request_delete(obj);
    return TEST_PASS;
}

static int
test_validate_fixed_length_list(void)
{
    of_table_stats_reply_t *obj = of_table_stats_reply_new(OF_VERSION_1_0);
    of_list_table_stats_entry_t list;
    of_table_stats_entry_t element;
    of_message_t msg;
    of_table_stats_reply_entries_bind(obj, &list);
    of_table_stats_entry_init(&element, OF_VERSION_1_0, -1, 1);
    of_list_table_stats_entry_append_bind(&list, &element);
    of_list_table_stats_entry_append_bind(&list, &element);
    msg = OF_OBJECT_TO_MESSAGE(obj);

    TEST_ASSERT(of_validate_message(msg, of_message_length_get(msg)) == 0);

    of_message_length_set(msg, of_message_length_get(msg) - 1);
    TEST_ASSERT(of_validate_message(msg, of_message_length_get(msg)) == -1);

    of_table_stats_reply_delete(obj);
    return TEST_PASS;
}

static int
test_validate_tlv16_list(void)
{
    of_flow_modify_t *obj = of_flow_modify_new(OF_VERSION_1_0);
    of_list_action_t list;
    of_action_set_tp_dst_t element1;
    of_action_output_t element2;
    of_message_t msg;
    of_flow_modify_actions_bind(obj, &list);
    of_action_set_tp_dst_init(&element1, OF_VERSION_1_0, -1, 1);
    of_list_action_append_bind(&list, (of_action_t *)&element1);
    of_action_output_init(&element2, OF_VERSION_1_0, -1, 1);
    of_list_action_append_bind(&list, (of_action_t *)&element2);
    msg = OF_OBJECT_TO_MESSAGE(obj);

    TEST_ASSERT(of_validate_message(msg, of_message_length_get(msg)) == 0);

    of_message_length_set(msg, of_message_length_get(msg) - 1);
    TEST_ASSERT(of_validate_message(msg, of_message_length_get(msg)) == -1);

    of_message_length_set(msg, of_message_length_get(msg) + 2);
    TEST_ASSERT(of_validate_message(msg, of_message_length_get(msg)) == -1);

    of_flow_modify_delete(obj);
    return TEST_PASS;
}

int
run_validator_tests(void)
{
    RUN_TEST(validate_fixed_length);
    RUN_TEST(validate_fixed_length_list);
    RUN_TEST(validate_tlv16_list);

    return TEST_PASS;
}
