import os
import yaml

c_lengths = {
    'uint8_t' : 1,
    'int8_t' : 1,
    'uint16_t' : 2,
    'int16_t' : 2,
    'uint32_t' : 4,
    'int32_t' : 4,
    'float' : 4,
    'bool' : 1,
}


def loadByName():
    allpackets = {}
    for file in os.listdir('packets/'):
        f = open('packets/' + file, 'r')
        yaml_file = yaml.load(f)
        for packet in yaml_file['packets']:
            allpackets[packet['name']] = packet
    return allpackets


def loadAsList(expandRepeatPackets=False):
    allpackets = {}
    allpackets['packets'] = []
    for file in os.listdir('packets/'):
        f = open('packets/' + file, 'r')
        yaml_file = yaml.load(f)
        for packet in yaml_file['packets']:
            if expandRepeatPackets and 'repeat' in packet:
                baseId = packet['id']
                baseName = packet['name']
                if 'offset' in packet:
                    offset = packet['offset']
                else:
                    offset = 1
                for i in range(packet['repeat']):
                    newPacket = packet.copy()
                    newPacket['id'] = baseId+i*offset
                    newPacket['name'] = baseName + "__" + str(i)
                    allpackets['packets'].append(newPacket)
                    del newPacket['repeat']
            else: 
                allpackets['packets'].append(packet)
    return allpackets

	
def getPrototypeArgs(packet):
    arguments = []
    if 'repeat' in packet:
        arguments.append('int _n')
    for field in packet['data']:
        if field['type'] == 'bitfield':
            for subfield in field['bits']:
                arguments.append('uint8_t ' + field['name'] + '_' + subfield['name'])
        else:
            arguments.append(field['type'] + ' ' + field['name'])
    return ', '.join(arguments)

if __name__ == "__main__":
    