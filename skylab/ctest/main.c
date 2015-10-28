#include <stdio.h>
#include <stdint.h>


#include "gencan.h"

void printCanPacket(CANPacket * cp)
{
  int i;
  printf("id: 0x%02hhX\n", cp->id);
  printf("length: 0x%02hhX\n", cp->length);
  for(i = 0; i < cp->length; i++)
  {
    printf("data %d: 0x%02hhX\n", i, cp->data[i]);
  }
}

int main(int argc, char ** argv)
{
  CANPacket c = CAN_INIT_KILL;
  CANPacket c1= CAN_INIT_DRIVE;
  printf("size of canpacketid: %d\n", sizeof(CANPacketId));
  printf("size of canpacket: %d\n", sizeof(CANPacket));
  c.kill.board_id = 0xAB;
  c.kill.error_code = 0xCD;
  printCanPacket(&c);

  c1.drive.tracker_disable = 1;
  c1.drive.direction=1;
  c1.drive.accelerator = 0xAA;
  c1.drive.regen = 0xBB;
  printCanPacket(&c1);
  return 0;
}
