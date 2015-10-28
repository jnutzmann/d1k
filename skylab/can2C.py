#!/usr/bin/python

import yaml
import pprint
import sys

import canParser

packet_enum_prefix="CAN_PACKET_";
packet_enum_name="CANPacketId";

yaml_file = canParser.loadAsList()

send_prefix = 'orbit_'
receive_prefix = 'deorbit_'

c = open(sys.argv[1],'w')
h = open(sys.argv[2],'w')

h.write("/// @file \n")
h.write("/// File generated from can2C.py\n")
h.write("/// Simply run the python script everytime the yaml CAN specification is changed\n")
h.write("/// This is located at: centaurus3/Utility Programs/cangen/can2C.py\n\n");


h.write("#ifndef GENERATED_CAN_HEADER\n")
h.write("#define GENERATED_CAN_HEADER\n\n")

h.write('#include "d1k.h"\n')
h.write('#include "d1k_can.h"\n');

h.write("""
typedef union {
	uint8_t b[4];
    uint32_t i;
	float f;
} can_float_union_t;

""");

# Construct an enumeration for the CAN Packet ID
h.write("/// @brief enumeration for CAN packet IDs\n")
h.write("typedef enum {\n")
for packet in yaml_file['packets']:
    h.write("    ///"+
        str(packet['description'])+
        "\n    "+
        packet_enum_prefix+
        str(packet['name']).upper()+
        " = "+
        str(hex(packet['id']))+
        ",\n\n");
h.write("} "+packet_enum_name+";\n\n\n")

# output the packet Structures

for packet in yaml_file['packets']:
    h.write("///@brief "+packet['description']+"\n")
    h.write("typedef struct can_packet_"+packet['name']+"_struct{\n")
    for field in packet['data']:
        h.write(" "*8);
        if str(field['type']) == 'bitfield':
            h.write("struct {\n")
            for subfield in field['bits']:
                if 'values' in subfield:
                    for possible_value in subfield['values']:
                        h.write(" "*12+"/// "+hex(possible_value)+":"+
                          str(subfield['values'][possible_value])+"\\n\n")
                h.write(" "*12)
                if 'bitnum' in subfield:
                    h.write("uint8_t "+str(subfield['name'])+" :"+str(subfield['bitnum'])+";\n\n")
                else:
                    h.write("uint8_t "+str(subfield['name'])+" :1;\n\n")
            h.write(" "*8)
            h.write("};\n")
        else:
            h.write(str(field['type'])+" "+str(field['name'])+";\n")
    h.write("} can_packet_"+str(packet['name'])+";\n\n")
    h.write("/// the length of the "+str(packet['name'])+" packet\n")
    h.write("#define CAN_LENGTH_"+str(packet['name']).upper()+
              " sizeof(can_packet_"+str(packet['name'])+")\n\n")
    h.write("/** initial values for the "+str(packet['name'])+" packet\n")
    h.write(" *  To be used like:\n")
    h.write(" *  @code\n")
    h.write(" *  CANPacket my_"+str(packet['name'])+
              "_packet = CAN_INIT_"+str(packet['name']).upper()+";\n")
    h.write(" *  @endcode **/\n")
    h.write("#define CAN_INIT_"+str(packet['name']).upper()+
              " {.id="+packet_enum_prefix+str(packet['name']).upper()+
              ", .length=CAN_LENGTH_"+str(packet['name']).upper()+"}\n\n")

# Now output the packet structures
h.write("/// @brief the structure for a CAN packet\n");
h.write("typedef struct CANPacket_struct{\n\n")
h.write("    /// @brief number of bytes in the data of this packet\n");
h.write("    uint8_t length;\n\n");
h.write("    bool rtr;\n\n");
h.write("    /// @brief the id for this packet\n");
h.write("    "+packet_enum_name+" id;\n");
h.write("    union{\n");
h.write(" "*8 + "///@brief the data without any structure\n");
h.write(" "*8 + "uint8_t data[8];\n\n");
for packet in yaml_file['packets']:
    h.write(" "*8+"///@brief "+packet['description']+"\n")
    h.write(" "*8+"can_packet_"+str(packet['name'])+" "+str(packet['name'])+";\n\n")
h.write("    };\n")
h.write("} CANPacket;\n\n")


c.write("/// @file \n")
c.write("/// File generated from can2C.py\n")
c.write("/// Simply run the python script everytime the yaml CAN specification is changed\n")
c.write("/// This is located at: centaurus3/Utility Programs/cangen/can2C.py\n\n");
c.write('#include "orbit.h"\n\n');


# Output send packet function prototypes
for packet in yaml_file['packets']:
    prototype = ""
    prototype += 'void ' + send_prefix + packet['name'] + '('
    prototype += canParser.getPrototypeArgs(packet)
    prototype += ')'
    h.write(prototype + ";\n")
    c.write(prototype + "{\n")
    c.write('\tCanTxMsg c;\n')
    c.write('\tc.IDE = 0;\n')
    c.write('\tc.RTR = 0;\n')
    if 'repeat' in packet:
        c.write('\tc.StdId = _n*'+str(packet['offset'])+'+'+str(packet['id'])+';\n')
    else:
        c.write('\tc.StdId = ' + str(packet['id']) + ';\n')
    dlc = 0
    for field in packet['data']:
        for i in range(0,canParser.c_lengths[field['type']]):
            if field['type'] == 'bitfield':
                bits = []
                bitcount = 0
                for subfield in field['bits']:
                    if 'bitnum' not in subfield: subfield['bitnum'] = 1
                    mask = hex(2**subfield['bitnum']-1)
                    bits.append('((' + field['name'] + '_' + subfield['name'] + ' & '+mask+')<<'+str(bitcount)+')')
                    bitcount += subfield['bitnum']
                    if bitcount > 8:
                        print "ERROR: too many bits in " + field['name'] + '_' + subfield['name']
                        sys.exit(1)
                c.write('\tc.Data['+str(dlc)+'] =   ' + ' \n\t\t\t\t| '.join(bits) + ';\n')
				
            elif field['type'] in ('float'):
                c.write('\tc.Data['+str(dlc)+'] = ((can_float_union_t){.f='+field['name']+'}).b['+str(i)+'];\n')
            else:
                c.write('\tc.Data['+str(dlc)+'] = ' + field['name']+'>>'+str(i*8)+';\n')
            dlc += 1
    if dlc > 8:
        print "ERROR: DLC > 8 on packet " + packet['name']
        sys.exit(1)
    if int(packet['id']) > 0x7ff:
        print "ERROR: packet id > 0x7ff (extended not supported) on packet " + packet['name']
        sys.exit(1)
    c.write('\tc.DLC = '+str(dlc)+';\n')
    c.write('\td1k_CAN_SendPacket(CAN1, &c);\n')
    c.write('}\n');

h.write("#endif //GENERATED_CAN_HEADER\n\n")


