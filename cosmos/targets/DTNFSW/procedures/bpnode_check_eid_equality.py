
from dtntools.dtngen.types import (
    EID
)

def bpnode_check_eid_equality(eid_a, eid_b):
    # If scheme is IPN
    if hasattr(eid_a, "uri"):
        check_expression(f"{eid_a.uri} == {eid_b.uri}")
        check_expression(f"{eid_a.ssp['node_num']} == {eid_b.ssp['node_num']}")
        check_expression(f"{eid_a.ssp['service_num']} == {eid_b.ssp['service_num']}")

    # If scheme is DTN
    else:
        check_expression(f"{eid_a[0]} == {eid_b[0]}")
        check_expression(f"{eid_a[1]} == {eid_b[1]}")
