| No-Op                             |            0         | BPNODE_NOOP_CC                             | BPNODE_CMD_NOOP                           |
| Add All Applications              |            1         | BPNODE_ADD_ALL_APPLICATIONS_CC             | BPNODE_CMD_ADD_ALL_APPLICATIONS           |
| Start All Applications            |            2         | BPNODE_START_ALL_APPLICATIONS_CC           | BPNODE_CMD_START_ALL_APPLICATIONS         |
| Verify Bundle Storage             |            3         | BPNODE_VERIFY_BUNDLE_STORAGE_CC            | BPNODE_CMD_VERIFY_BUNDLE_STORAGE          |
| Initialize Bundle Storage         |            4         | BPNODE_INIT_BUNDLE_STORAGE_CC              | BPNODE_CMD_INIT_BUNDLE_STORAGE            |
| Verify Bundle Metadata            |            5         | BPNODE_VERIFY_BUNDLE_METADATA_CC           | BPNODE_CMD_VERIFY_BUNDLE_METADATA         |
| Rebuild Bundle Metadata           |            6         | BPNODE_REBUILD_BUNDLE_METADATA_CC          | BPNODE_CMD_REBUILD_BUNDLE_METADATA        |
| Clear Volatile                    |            7         | BPNODE_CLEAR_VOLATILE_CC                   | BPNODE_CMD_CLEAR_VOLATILE                 |
| Reload Saved Data                 |            8         | BPNODE_RELOAD_SAVED_DATA_CC                | BPNODE_CMD_RELOAD_SAVED_DATA              |
| Reset All Counters                |            9         | BPNODE_RESET_ALL_COUNTERS_CC               | BPNODE_CMD_RESET_ALL_COUNTERS             |
| Reset Counter                     |           10         | BPNODE_RESET_COUNTER_CC                    | BPNODE_CMD_RESET_COUNTER                  |
| Reset Source Counters             |           11         | BPNODE_RESET_SOURCE_COUNTERS_CC            | BPNODE_CMD_RESET_SOURCE_COUNTERS          |
| Reset Bundle Counters             |           12         | BPNODE_RESET_BUNDLE_COUNTERS_CC            | BPNODE_CMD_RESET_BUNDLE_COUNTERS          |
| Reset Error Counters              |           13         | BPNODE_RESET_ERROR_COUNTERS_CC             | BPNODE_CMD_RESET_ERROR_COUNTERS           |
| Add Application                   |           14         | BPNODE_ADD_APPLICATION_CC                  | BPNODE_CMD_ADD_APPLICATION                |
| Remove Application                |           15         | BPNODE_REMOVE_APPLICATION_CC               | BPNODE_CMD_REMOVE_APPLICATION             |
| Set Registration State            |           16         | BPNODE_SET_REGISTRATION_STATE_CC           | BPNODE_CMD_SET_REGISTRATION_STATE         |
| Start Application                 |           17         | BPNODE_START_APPLICATION_CC                | BPNODE_CMD_START_APPLICATION              |
| Stop Application                  |           18         | BPNODE_STOP_APPLICATION_CC                 | BPNODE_CMD_STOP_APPLICATION               |
| Add Authorized Sources            |           19         | BPNODE_ADD_AUTH_SOURCES_CC                 | BPNODE_CMD_ADD_AUTH_SOURCES               |
| Remove Authorized Sources         |           20         | BPNODE_REMOVE_AUTH_SOURCES_CC              | BPNODE_CMD_REMOVE_AUTH_SOURCES            |
| Add Authorized Custody Sources    |           21         | BPNODE_ADD_AUTH_CUSTODY_SOURCES_CC         | BPNODE_CMD_ADD_AUTH_CUSTODY_SOURCES       |
| Remove Authorized Custody Sources |           22         | BPNODE_REMOVE_AUTH_CUSTODY_SOURCES_CC      | BPNODE_CMD_REMOVE_AUTH_CUSTODY_SOURCES    |
| Add Authorized Custodians         |           23         | BPNODE_ADD_AUTH_CUSTODIANS_CC              | BPNODE_CMD_ADD_AUTH_CUSTODIANS            |
| Remove Authorized Custodians      |           24         | BPNODE_REMOVE_AUTH_CUSTODIANS_CC           | BPNODE_CMD_REMOVE_AUTH_CUSTODIANS         |
| Add Authorized Report-to-EID      |           25         | BPNODE_ADD_AUTH_REPORT_TO_EID_CC           | BPNODE_CMD_ADD_AUTH_REPORT_TO_EID         |
| Remove Authorized Report-to-EID   |           26         | BPNODE_REMOVE_AUTH_REPORT_TO_EID_CC        | BPNODE_CMD_REMOVE_AUTH_REPORT_TO_EID      |
| Add Latency                       |           27         | BPNODE_ADD_LATENCY_CC                      | BPNODE_CMD_ADD_LATENCY                    |
| Remove Latency                    |           28         | BPNODE_REMOVE_LATENCY_CC                   | BPNODE_CMD_REMOVE_LATENCY                 |
| Contact Setup                     |           29         | BPNODE_CONTACT_SETUP_CC                    | BPNODE_CMD_CONTACT_SETUP                  |
| Contact Start                     |           30         | BPNODE_CONTACT_START_CC                    | BPNODE_CMD_CONTACT_START                  |
| Contact Stop                      |           31         | BPNODE_CONTACT_STOP_CC                     | BPNODE_CMD_CONTACT_STOP                   |
| Contact Teardown                  |           32         | BPNODE_CONTACT_TEARDOWN_CC                 | BPNODE_CMD_CONTACT_TEARDOWN               |
| Add MIB Array Key                 |           33         | BPNODE_ADD_MIB_ARRAY_KEY_CC                | BPNODE_CMD_ADD_MIB_ARRAY_KEY              |
| Remove MIB Array Key              |           34         | BPNODE_REMOVE_MIB_ARRAY_KEY_CC             | BPNODE_CMD_REMOVE_MIB_ARRAY_KEY           |
| Set MIB Item                      |           35         | BPNODE_SET_MIB_ITEM_CC                     | BPNODE_CMD_SET_MIB_ITEM                   |
| Add Storage Allocation            |           36         | BPNODE_ADD_STORAGE_ALLOCATION_CC           | BPNODE_CMD_ADD_STORAGE_ALLOCATION         |
| Remove Storage Allocation         |           37         | BPNODE_REMOVE_STORAGE_ALLOCATION_CC        | BPNODE_CMD_REMOVE_STORAGE_ALLOCATION      |
| Perform Self Test                 |           38         | BPNODE_PERFORM_SELF_TEST_CC                | BPNODE_CMD_PERFORM_SELF_TEST              |
| Send Node MIB Config HK           |           40         | BPNODE_SEND_NODE_MIB_CONFIG_HK_CC          | BPNODE_CMD_SEND_NODE_MIB_CONFIG_HK        |
| Send Per-Source MIB Config HK     |           41         | BPNODE_SEND_SOURCE_MIB_CONFIG_HK_CC        | BPNODE_CMD_SEND_SOURCE_MIB_CONFIG_HK      |
| Send Node MIB Counter HK          |           42         | BPNODE_SEND_NODE_MIB_COUNTERS_HK_CC        | BPNODE_CMD_SEND_NODE_MIB_COUNTERS_HK      |
| Send Per-Source MIB Counter HK    |           43         | BPNODE_SEND_SOURCE_MIB_COUNTERS_HK_CC      | BPNODE_CMD_SEND_SOURCE_MIB_COUNTERS_HK    |
| Send Storage HK                   |           44         | BPNODE_SEND_STORAGE_HK_CC                  | BPNODE_CMD_SEND_STORAGE_HK                |
| Send Channel/Contact Status HK    |           45         | BPNODE_SEND_CHANNEL_CONTACT_STAT_HK_CC     | BPNODE_CMD_SEND_CHANNEL_CONTACT_STAT_HK   |

