#-----------------------------------------------------------------------------------------------
# Program Name: test_python_Script.py
# Descrition: The program is test adding event messages to the script log, by monitoring the loong messages counter inceasing.
# the program will start with TO_LAB_enable telemetry and move onto the entire list of CTS AP NOOPS.
# Create Date: 05/31/2024
# Author: Stuart Bozievich
#------------------------------------------------------------------------------------------------
# declare variables for sucessful counters increamenting and Noop Type Event messages.
noop_command_counter_check = 0
noop_command_message_check = 0
previous_telemetry_count = 0

#send TO_LAB
print("Send TO_LAB_CMD_ENABLE_OUTPUT to turn on telemtry")

cmd("DTNFSW TO_LAB_CMD_ENABLE_OUTPUT with CCSDS_STREAMID 6272, CCSDS_SEQUENCE 49152, CCSDS_LENGTH 17, CCSDS_FC 6, CCSDS_CHECKSUM 0, DEST_IP '10.2.21.195'")

wait 1

previous_telemetry_count = tlm("DTNFSW TO_LAB_HK RECEIVED_COUNT")

wait("tlm('DTNFSW TO_LAB_HK RECEIVED_COUNT' ==  previous_telemetry_count + 1",8)
