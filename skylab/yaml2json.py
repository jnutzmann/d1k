#!/usr/bin/python

import yaml
import json
import pprint
import sys
import canParser

packet_enum_prefix="CAN_PACKET_";
packet_enum_name="CanPacketId";

yaml_file = canParser.loadAsList(expandRepeatPackets = True)

sys.stdout.write(str(json.dumps(yaml_file, indent=4)))

