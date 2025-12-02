from openc3.script.suite import Suite, Group

# Common Utilities
load_utility("<%= target_name %>/procedures/bpnode_create_test_bundle_utils.py")
load_utility("<%= target_name %>/procedures/bpnode_channel_mgmt_utils.py")
load_utility("<%= target_name %>/procedures/bpnode_check_eid_equality.py")
load_utility("<%= target_name %>/procedures/load_new_table.py")

# Application Tests
load_utility("<%= target_name %>/procedures/integration_test_groups/application_directives/integration_test_bpnode_auto_adu_ingest.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/application_directives/integration_test_bpnode_application_commanding.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/application_directives/integration_test_bpnode_add_all_applications.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/application_directives/integration_test_bpnode_set_registration_state.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/application_directives/integration_test_bpnode_start_all_applications.py")

# Contact Directives
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_flow.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_setup.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_start.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_stop.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_teardown.py")

# Other Contact Ingress Tests
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_01_nominal.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_02_prev_node.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_03_hop_count.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_04_age.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_05_crc_none.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_06_crc_32.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_07_max_blk_err.py")
# load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_sb_contact_flow.py")

# Counter Directives
load_utility("<%= target_name %>/procedures/integration_test_groups/counter_directives/integration_test_bpnode_reset_all_counters.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/counter_directives/integration_test_bpnode_reset_bundle_counters.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/counter_directives/integration_test_bpnode_reset_counter.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/counter_directives/integration_test_bpnode_reset_error_counters.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/counter_directives/integration_test_bpnode_reset_source_counters.py")

# MIB Directives
load_utility("<%= target_name %>/procedures/integration_test_groups/mib_directives/integration_test_bpnode_add_mib_array_key.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/mib_directives/integration_test_bpnode_remove_mib_array_key.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/mib_directives/integration_test_bpnode_set_mib_item.py")

# Policy Directives
load_utility("<%= target_name %>/procedures/integration_test_groups/policy_directives/integration_test_bpnode_add_authorized_sources.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/policy_directives/integration_test_bpnode_remove_authorized_sources.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/policy_directives/integration_test_bpnode_add_authorized_custody_sources.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/policy_directives/integration_test_bpnode_remove_authorized_custody_sources.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/policy_directives/integration_test_bpnode_add_authorized_custodians.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/policy_directives/integration_test_bpnode_remove_authorized_custodians.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/policy_directives/integration_test_bpnode_add_authorized_report_to_eid.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/policy_directives/integration_test_bpnode_remove_authorized_report_to_eid.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/policy_directives/integration_test_bpnode_add_latency.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/policy_directives/integration_test_bpnode_remove_latency.py")

# Routine Directives
load_utility("<%= target_name %>/procedures/integration_test_groups/routine_directives/integration_test_bpnode_noop.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/routine_directives/integration_test_bpnode_wakeup.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/routine_directives/integration_test_bpnode_configuration_update.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/routine_directives/integration_test_bpnode_send_channel_contact_status_hk.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/routine_directives/integration_test_bpnode_send_node_mib_config_hk.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/routine_directives/integration_test_bpnode_send_node_mib_counters_hk.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/routine_directives/integration_test_bpnode_send_node_mib_reports_hk.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/routine_directives/integration_test_bpnode_send_per_source_mib_config_hk.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/routine_directives/integration_test_bpnode_send_per_source_mib_counters_hk.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/routine_directives/integration_test_bpnode_send_storage_hk.py")

# Storage Directives
load_utility("<%= target_name %>/procedures/integration_test_groups/storage_directives/integration_test_bpnode_add_storage_allocation.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/storage_directives/integration_test_bpnode_clear_volatile.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/storage_directives/integration_test_bpnode_initialize_bundle_storage.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/storage_directives/integration_test_bpnode_rebuild_bundle_metadata.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/storage_directives/integration_test_bpnode_remove_storage_allocation.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/storage_directives/integration_test_bpnode_verify_bundle_storage.py")

# Other Integration Tests
load_utility("<%= target_name %>/procedures/integration_test_groups/integration_test_bpnode_restart.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/integration_test_fsw_aliveness.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/integration_test_dtn_time.py")

class FSW_Integration_Test_Suite(Suite):
  def __init__(self):
      # Add each test group
      # ------------------------------------------------------------------------
      # The order that we add these test groups affects the order
      # they're listed in the COSMOS Script Runner drop-down menu
      # To make it easier to find tests using the drop-down,
      # these are added in alphabetical order,
      # rather than by directive type (application, storage, etc.).
      # ------------------------------------------------------------------------
      self.add_group(integration_test_bpnode_auto_adu_ingest)
      self.add_group(integration_test_bpnode_application_commanding)
      self.add_group(integration_test_bpnode_add_all_applications)
      self.add_group(integration_test_bpnode_add_auth_custodians)
      self.add_group(integration_test_bpnode_add_auth_custody_sources)
      self.add_group(integration_test_bpnode_add_auth_sources)
      self.add_group(integration_test_bpnode_add_latency)
      self.add_group(integration_test_bpnode_add_mib_array_key)
      self.add_group(integration_test_bpnode_add_storage_allocation)
      self.add_group(integration_test_bpnode_clear_volatile)
      self.add_group(integration_test_bpnode_contact_flow)
      self.add_group(integration_test_bpnode_contact_ingress_01)
      self.add_group(integration_test_bpnode_contact_ingress_02)
      self.add_group(integration_test_bpnode_contact_ingress_03)
      self.add_group(integration_test_bpnode_contact_ingress_04)
      self.add_group(integration_test_bpnode_contact_ingress_05)
      self.add_group(integration_test_bpnode_contact_ingress_06)
      self.add_group(integration_test_bpnode_contact_ingress_07)
      self.add_group(integration_test_bpnode_contact_setup)
      self.add_group(integration_test_bpnode_contact_start)
      self.add_group(integration_test_bpnode_contact_stop)
      self.add_group(integration_test_bpnode_contact_teardown)
      self.add_group(integration_test_bpnode_init_bundle_storage)
      self.add_group(integration_test_bpnode_noop)
      self.add_group(integration_test_bpnode_rebuild_bundle_metadata)
      self.add_group(integration_test_bpnode_remove_auth_custodians)
      self.add_group(integration_test_bpnode_remove_auth_custody_sources)
      self.add_group(integration_test_bpnode_remove_auth_report_to_eid)
      self.add_group(integration_test_bpnode_remove_auth_sources)
      self.add_group(integration_test_bpnode_remove_latency)
      self.add_group(integration_test_bpnode_remove_mib_array_key)
      self.add_group(integration_test_bpnode_remove_storage_allocation)
      self.add_group(integration_test_bpnode_reset_all_counters)
      self.add_group(integration_test_bpnode_reset_bundle_counters)
      self.add_group(integration_test_bpnode_reset_counter)
      self.add_group(integration_test_bpnode_reset_error_counters)
      self.add_group(integration_test_bpnode_reset_source_counters)
#       self.add_group(integration_test_bpnode_sb_contact_flow)
      self.add_group(integration_test_bpnode_send_channel_contact_stat_hk)
      self.add_group(integration_test_bpnode_send_node_mib_config)
      self.add_group(integration_test_bpnode_send_node_mib_counters_hk)
      self.add_group(integration_test_bpnode_send_node_mib_reports_hk)
      self.add_group(integration_test_bpnode_send_source_mib_config_hk)
      self.add_group(integration_test_bpnode_send_source_mib_counters_hk)
      self.add_group(integration_test_bpnode_send_storage_hk)
      self.add_group(integration_test_bpnode_set_mib_item)
      self.add_group(integration_test_bpnode_set_registration_state)
      self.add_group(integration_test_bpnode_start_all_applications)
      self.add_group(integration_test_bpnode_verify_bundle_storage)
      self.add_group(integration_test_bpnode_wakeup)
      self.add_group(integration_test_bpnode_configuration_update)
      self.add_group(integration_test_bpnode_restart)
      self.add_group(integration_test_fsw_aliveness)
      self.add_group(integration_test_dtn_time)

  def setup(self):
      # Run when Suite Setup button is pressed
      # Run before all groups when Suite Start is pressed
      Group.print(f"Starting FSW Integration Test Suite against the following target:")
      Group.print(f"DTNFSW Target Name (target_name): <%= target_name %>")

      curr_to_lab_pkt_count = tlm(f"<%= target_name %> TO_LAB_HK RECEIVED_COUNT")
      if curr_to_lab_pkt_count is None:
        curr_to_lab_pkt_count = 0        
      exp_to_lab_pkt_count = curr_to_lab_pkt_count + 1

      # Send the command, TO_LAB_CMD_ENABLE_OUT
      to_lab_dest_ip = "127.0.0.1"
      cmd(f"<%= target_name %> TO_LAB_CMD_ENABLE_OUTPUT with DEST_IP '{to_lab_dest_ip}'")
      Group.print(f"Sent TO_LAB_CMD_ENABLE_OUTPUT command to <%= target_name %> with DEST_IP '{to_lab_dest_ip}'")

      # Wait for one TO packet to be received
      wait_check(f"<%= target_name %> TO_LAB_HK RECEIVED_COUNT == {exp_to_lab_pkt_count}", 10)

      # Set up and start contacts 1 and 2
      for contact_num in range(<%= $dtnfsw_globals_num_contacts %>):
        # Get current state
        cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK RECEIVED_COUNT >= 0', 10)
        run_state = tlm(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num}')

        # Store the previous counter val, used to calculated expected/next value
        current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
        expected_valid_cmd_count = current_valid_cmd_count + 1

        # Get contact state to TORNDOWN
        if run_state == "SETUP":
          # Tear down contact
          cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID {contact_num}')
          wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
          expected_valid_cmd_count += 1

          # Check the run state for the contact
          cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
          wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "TORNDOWN"', 10)
        elif run_state == "STARTED":
          # Stop contact
          cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_STOP with CONTACT_ID {contact_num}')
          wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
          expected_valid_cmd_count += 1

          # Check the run state for the contact
          cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
          wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "STOPPED"', 10)

          # Tear down contact
          cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID {contact_num}')
          wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
          expected_valid_cmd_count += 1

          # Check the run state for the contact
          cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
          wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "TORNDOWN"', 10)
        elif run_state == "STOPPED":
          # Tear down contact
          cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID {contact_num}')
          wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
          expected_valid_cmd_count += 1

          # Check the run state for the contact
          cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
          wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "TORNDOWN"', 10)

        # Set up contact
        cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_SETUP with CONTACT_ID {contact_num}')
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
        expected_valid_cmd_count += 1

        # Check the run state for the contact
        cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "SETUP"', 10)

        # Start contact
        cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_START with CONTACT_ID {contact_num}')
        wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
        expected_valid_cmd_count += 1

        # Check the run state for the contact
        cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
        wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "STARTED"', 10)

  def teardown(self):
      # Run when Suite Teardown button is pressed
      # Run after all groups when Suite Start is pressed
      # Store the previous counter val, used to calculated expected/next value

    for contact_num in range(<%= $dtnfsw_globals_num_contacts %>):
      # Collect pre-command information
      current_valid_cmd_count = tlm(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT")
      expected_valid_cmd_count = current_valid_cmd_count + 1

      # Stop contact
      cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_STOP with CONTACT_ID {contact_num}')
      wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
      expected_valid_cmd_count += 1

      # Check the run state for the contact
      cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
      wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "STOPPED"', 10)

      # Tear down contact
      cmd(f'<%= target_name %> BPNODE_CMD_CONTACT_TEARDOWN with CONTACT_ID {contact_num}')
      wait_check(f"<%= target_name %> BPNODE_NODE_MIB_COUNTERS_HK BUNDLE_AGENT_ACCEPTED_DIRECTIVE_COUNT == {expected_valid_cmd_count}", 10)
      expected_valid_cmd_count += 1

      # Check the run state for the contact
      cmd("<%= target_name %> BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK")
      wait_check(f'<%= target_name %> BPNODE_CHAN_CON_STAT_HK CON_STAT_RUN_STATE_{contact_num} == "TORNDOWN"', 10)
