''' write_event_log.py
 
    - Writes events to an eventlog file
    - Run as a background job in startup script: 
      - script_run("<%= target_name %>/procedures/write_event_log.py")
         - eventlog filename stashed with key 'eventlog' 

'''

id = subscribe_packets([['<%= target_name %>', 'CFE_EVS_LONG_EVENT_MSG']])
wait(0.1)

EventType = {'1':'DEBUG', '2':'INFO', '3':'ERROR', '4':'CRIT'}

eventlogfile = stash_get('eventlog')
print ("Event log file name: " + eventlogfile)

f = open(eventlogfile, "w")

while True:
  id, packets = get_packets(id, block=1000, count=1)
  for packet in packets:
    ptime = f"{packet['PACKET_TIME']}"
    app   = f"{packet['PACKET_ID_APP_NAME']}"
    eid   = f"{packet['PACKET_ID_EVENT_ID']}"
    etype = f"{packet['PACKET_ID_EVENT_TYPE']}"
    emsg  = f"{packet['MESSAGE']}"
    
    # Log event message received
    event = ptime[0:-6] + " " + app + " " + eid + " " + EventType[etype] + " " + emsg
    print ("Event: " + event)

    f.write(event + "\n")
    f.flush()  # so we can see current log during runtime
