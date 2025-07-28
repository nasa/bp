#---------------------------------------
#Program: Enable_telemetry_and_checks.py
#Language: Python/COSMOS
#Description: Send a TO_LAB_CMD_ENABLE_OUTPUT commmand and verify Telemetry broadcasting
#Author: Stuart Bozievich
#---------------------------------------
#do we need a global statement to Python libarary here?
#variables
Current_Packet_Count = 0

#print(type(Current_Packet_Count))
#First startup procedure and Telemetry chackout

#display save screens for checking TO_LAB_CMD_ENABLE_OUTPUT command
display_screen("DTNFSW", "CFE_BUS_APS_EVENT_MESSAGES_SCREEN", 600, 0)
display_screen("DTNFSW", "TO_LAB_HOUSEKEEPING_DATA", 1000, 0)
display_screen("DTNFSW", "CI_LAB_HOUSEKEEPING_DATA", 1335, 0)

#send the command, TO_LAB_CMD_ENABLE_OUT
cmd("DTNFSW TO_LAB_CMD_ENABLE_OUTPUT with CCSDS_STREAMID 6272, CCSDS_SEQUENCE 49152, CCSDS_LENGTH 17, CCSDS_FC 6, CCSDS_CHECKSUM 0, DEST_IP '10.2.21.195'")
print("Sent TO_LAB_CMD_ENABLE_OUTPUT Command")

#check TO_LAB_Events message which is first telemetry enabled.
wait("DTNFSW CFE_EVS_LONG_EVENT_MSG PACKET_ID_APP_NAME == 'TO_LAB_APP'", 3)
if tlm("DTNFSW CFE_EVS_LONG_EVENT_MSG PACKET_ID_APP_NAME") == 'TO_LAB_APP':
  print (tlm("DTNFSW CFE_EVS_LONG_EVENT_MSG RECEIVED_TIMEFORMATTED")+' '+tlm("DTNFSW CFE_EVS_LONG_EVENT_MSG PACKET_ID_APP_NAME")+' '+tlm("DTNFSW CFE_EVS_LONG_EVENT_MSG MESSAGE"))
else:
  print("No Message generated or TO_LAB_CMD_ENABLE_OUT failed")

#Check TO_LAB_HK Telemetry.
Current_Packet_Count = tlm("DTNFSW TO_LAB_HK RECEIVED_COUNT")
print(type(Current_Packet_Count))
Current_Packet_Count = Current_Packet_Count + 1
wait("DTNFSW TO_LAB_HK RECEIVED_COUNT == (Current_Packet_Count)", 6)
if tlm("DTNFSW TO_LAB_HK RECEIVED_COUNT") > Current_Packet_Count:
  print("TO_LAB_HK telemetry is enabled")
else:
  print("TO_LAB_HK telemetry is still disabled")


#Check CI_LAB_HK Telemetry.

#close screens
clear_all_screens()