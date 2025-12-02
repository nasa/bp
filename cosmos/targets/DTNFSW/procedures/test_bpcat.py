#######################################################################
## BPCat Test
#######################################################################
from dtntools.dtncla.udp import UdpTxSocket, UdpRxSocket
from dtntools.dtngen.bundle import Bundle
load_utility ("DTNFSW-1/procedures/dtngen_utils.py")

# Configure and connect data sender and receiver
dest_ip = ask("Enter bpcat IP address: ")
dest_port = 4501
local_ip = "0.0.0.0"
local_port = 4551

data_sender = UdpTxSocket(dest_ip, dest_port)
data_receiver = UdpRxSocket(local_ip, local_port)
data_sender.connect()
data_receiver.connect()

# Generate bundles
dest_node    = 200
dest_service = 64

num_bundles = 50
payload = b'\xAA'*100
DTNGenUtils.generate_bundles(dest_node, dest_service, num_bundles, payload)

# Send bundles to bpcat
print("Sending bundles to bpcat ...")
num_loops=2
with disable_instrumentation():
    for _ in range(num_loops):
        DTNGenUtils.send_bundles(num_bundles, dest_node, data_sender)
    
# Receive bundles from bpcat
print("Receiving bundles from bpcat ...")
for n in range(num_loops*num_bundles):
    received_bundle = Bundle.from_bytes(data_receiver.read())
    print(f"Received bundle {n+1}: {received_bundle.to_json()}")

# Disconnect data sender/receiver
data_receiver.disconnect()
data_sender.disconnect()
