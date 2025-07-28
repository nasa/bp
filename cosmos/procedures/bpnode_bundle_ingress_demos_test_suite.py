from openc3.script.suite import Suite, Group

# Common Utilities
load_utility("<%= target_name %>/procedures/bpnode_create_test_bundle_util.py")

# Test Groups
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_01_nominal.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_02_max_blk_err.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_03_prev_node.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_04_crc_none.py")
load_utility("<%= target_name %>/procedures/integration_test_groups/contact_directives/integration_test_bpnode_contact_ingress_05_crc_32.py")

class BPNode_Bundle_Ingress_Demos_Test_Suite(Suite):
  def __init__(self):
      # Add each test group
      # ------------------------------------------------------------------------
      # The order that we add these test groups affects the order
      # they're listed in the COSMOS Script Runner drop-down menu
      # To make it easier to find tests using the drop-down,
      # these are added in alphabetical order,
      # rather than by directive type (application, storage, etc.).
      # ------------------------------------------------------------------------
      self.add_group(integration_test_bpnode_contact_ingress_01)
      self.add_group(integration_test_bpnode_contact_ingress_02)
      self.add_group(integration_test_bpnode_contact_ingress_03)
      self.add_group(integration_test_bpnode_contact_ingress_04)
      self.add_group(integration_test_bpnode_contact_ingress_05)

  def setup(self):
      # Run when Suite Setup button is pressed
      # Run before all groups when Suite Start is pressed
      Group.print(f"Starting BPNode Bundle Ingress Demos Test Suite against the following target:")
      Group.print(f"DTNFSW Target Name (target_name): <%= target_name %>")

      curr_to_lab_pkt_count = tlm(f"<%= target_name %> TO_LAB_HK RECEIVED_COUNT")
      if curr_to_lab_pkt_count is None:
        curr_to_lab_pkt_count = 0        
      exp_to_lack_pkt_count = curr_to_lab_pkt_count + 1

      # Send the command, TO_LAB_CMD_ENABLE_OUT
      to_lab_dest_ip = "127.0.0.1"
      cmd(f"<%= target_name %> TO_LAB_CMD_ENABLE_OUTPUT with DEST_IP '{to_lab_dest_ip}'")
      Group.print(f"Sent TO_LAB_CMD_ENABLE_OUTPUT command to <%= target_name %> with DEST_IP '{to_lab_dest_ip}'")

      # Wait for one TO packet to be received
      wait_check(f"<%= target_name %> TO_LAB_HK RECEIVED_COUNT == {exp_to_lack_pkt_count}", 10)

  def teardown(self):
      # Run when Suite Teardown button is pressed
      # Run after all groups when Suite Start is pressed
      pass
