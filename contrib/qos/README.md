### QoS (Quality of service) ###

This is a Linux bash script that will set up tc to limit the outgoing bandwidth for connections to the Quercus network. It limits outbound TCP traffic with a source or destination port of 24768, but not if the destination IP is within a LAN.

This means one can have an always-on quercusd instance running, and another local quercusd/quercus-qt instance which connects to this node and receives blocks from it.
