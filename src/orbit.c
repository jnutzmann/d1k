/// @file 
/// File generated from can2C.py
/// Simply run the python script everytime the yaml CAN specification is changed
/// This is located at: centaurus3/Utility Programs/cangen/can2C.py

#include "orbit.h"

void orbit_fullcurrent_drive(uint8_t drivemode, float motor_current){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 1024;
	c.Data[0] = drivemode>>0;
	c.Data[1] = ((can_float_union_t){.f=motor_current}).b[0];
	c.Data[2] = ((can_float_union_t){.f=motor_current}).b[1];
	c.Data[3] = ((can_float_union_t){.f=motor_current}).b[2];
	c.Data[4] = ((can_float_union_t){.f=motor_current}).b[3];
	c.DLC = 5;
	d1k_CAN_SendPacket(CAN1, &c);
}

void orbit_tritium_motor_drive(float motor_velocity, float motor_current){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 289;
	c.Data[0] = ((can_float_union_t){.f=motor_velocity}).b[0];
	c.Data[1] = ((can_float_union_t){.f=motor_velocity}).b[1];
	c.Data[2] = ((can_float_union_t){.f=motor_velocity}).b[2];
	c.Data[3] = ((can_float_union_t){.f=motor_velocity}).b[3];
	c.Data[4] = ((can_float_union_t){.f=motor_current}).b[0];
	c.Data[5] = ((can_float_union_t){.f=motor_current}).b[1];
	c.Data[6] = ((can_float_union_t){.f=motor_current}).b[2];
	c.Data[7] = ((can_float_union_t){.f=motor_current}).b[3];
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_motor_power(float reserved, float bus_current){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 290;
	c.Data[0] = ((can_float_union_t){.f=reserved}).b[0];
	c.Data[1] = ((can_float_union_t){.f=reserved}).b[1];
	c.Data[2] = ((can_float_union_t){.f=reserved}).b[2];
	c.Data[3] = ((can_float_union_t){.f=reserved}).b[3];
	c.Data[4] = ((can_float_union_t){.f=bus_current}).b[0];
	c.Data[5] = ((can_float_union_t){.f=bus_current}).b[1];
	c.Data[6] = ((can_float_union_t){.f=bus_current}).b[2];
	c.Data[7] = ((can_float_union_t){.f=bus_current}).b[3];
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_reset(float unused1, float unused2){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 291;
	c.Data[0] = ((can_float_union_t){.f=unused1}).b[0];
	c.Data[1] = ((can_float_union_t){.f=unused1}).b[1];
	c.Data[2] = ((can_float_union_t){.f=unused1}).b[2];
	c.Data[3] = ((can_float_union_t){.f=unused1}).b[3];
	c.Data[4] = ((can_float_union_t){.f=unused2}).b[0];
	c.Data[5] = ((can_float_union_t){.f=unused2}).b[1];
	c.Data[6] = ((can_float_union_t){.f=unused2}).b[2];
	c.Data[7] = ((can_float_union_t){.f=unused2}).b[3];
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_identification(uint32_t tritium_id, uint32_t serial_number){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 256;
	c.Data[0] = tritium_id>>0;
	c.Data[1] = tritium_id>>8;
	c.Data[2] = tritium_id>>16;
	c.Data[3] = tritium_id>>24;
	c.Data[4] = serial_number>>0;
	c.Data[5] = serial_number>>8;
	c.Data[6] = serial_number>>16;
	c.Data[7] = serial_number>>24;
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_status(uint16_t limit_flags, uint16_t error_flags, uint16_t active_motor, uint16_t reserved){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 257;
	c.Data[0] = limit_flags>>0;
	c.Data[1] = limit_flags>>8;
	c.Data[2] = error_flags>>0;
	c.Data[3] = error_flags>>8;
	c.Data[4] = active_motor>>0;
	c.Data[5] = active_motor>>8;
	c.Data[6] = reserved>>0;
	c.Data[7] = reserved>>8;
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_bus(float bus_voltage, float bus_current){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 258;
	c.Data[0] = ((can_float_union_t){.f=bus_voltage}).b[0];
	c.Data[1] = ((can_float_union_t){.f=bus_voltage}).b[1];
	c.Data[2] = ((can_float_union_t){.f=bus_voltage}).b[2];
	c.Data[3] = ((can_float_union_t){.f=bus_voltage}).b[3];
	c.Data[4] = ((can_float_union_t){.f=bus_current}).b[0];
	c.Data[5] = ((can_float_union_t){.f=bus_current}).b[1];
	c.Data[6] = ((can_float_union_t){.f=bus_current}).b[2];
	c.Data[7] = ((can_float_union_t){.f=bus_current}).b[3];
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_velocity(float motor_velocity, float vehicle_velocity){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 259;
	c.Data[0] = ((can_float_union_t){.f=motor_velocity}).b[0];
	c.Data[1] = ((can_float_union_t){.f=motor_velocity}).b[1];
	c.Data[2] = ((can_float_union_t){.f=motor_velocity}).b[2];
	c.Data[3] = ((can_float_union_t){.f=motor_velocity}).b[3];
	c.Data[4] = ((can_float_union_t){.f=vehicle_velocity}).b[0];
	c.Data[5] = ((can_float_union_t){.f=vehicle_velocity}).b[1];
	c.Data[6] = ((can_float_union_t){.f=vehicle_velocity}).b[2];
	c.Data[7] = ((can_float_union_t){.f=vehicle_velocity}).b[3];
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_phase_current(float phase_b_current, float phase_a_current){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 260;
	c.Data[0] = ((can_float_union_t){.f=phase_b_current}).b[0];
	c.Data[1] = ((can_float_union_t){.f=phase_b_current}).b[1];
	c.Data[2] = ((can_float_union_t){.f=phase_b_current}).b[2];
	c.Data[3] = ((can_float_union_t){.f=phase_b_current}).b[3];
	c.Data[4] = ((can_float_union_t){.f=phase_a_current}).b[0];
	c.Data[5] = ((can_float_union_t){.f=phase_a_current}).b[1];
	c.Data[6] = ((can_float_union_t){.f=phase_a_current}).b[2];
	c.Data[7] = ((can_float_union_t){.f=phase_a_current}).b[3];
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_motor_voltage_vector(float vq, float vd){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 261;
	c.Data[0] = ((can_float_union_t){.f=vq}).b[0];
	c.Data[1] = ((can_float_union_t){.f=vq}).b[1];
	c.Data[2] = ((can_float_union_t){.f=vq}).b[2];
	c.Data[3] = ((can_float_union_t){.f=vq}).b[3];
	c.Data[4] = ((can_float_union_t){.f=vd}).b[0];
	c.Data[5] = ((can_float_union_t){.f=vd}).b[1];
	c.Data[6] = ((can_float_union_t){.f=vd}).b[2];
	c.Data[7] = ((can_float_union_t){.f=vd}).b[3];
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_motor_current_vector(float iq, float id){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 262;
	c.Data[0] = ((can_float_union_t){.f=iq}).b[0];
	c.Data[1] = ((can_float_union_t){.f=iq}).b[1];
	c.Data[2] = ((can_float_union_t){.f=iq}).b[2];
	c.Data[3] = ((can_float_union_t){.f=iq}).b[3];
	c.Data[4] = ((can_float_union_t){.f=id}).b[0];
	c.Data[5] = ((can_float_union_t){.f=id}).b[1];
	c.Data[6] = ((can_float_union_t){.f=id}).b[2];
	c.Data[7] = ((can_float_union_t){.f=id}).b[3];
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_motor_backemf(float bemfq, float bemfd){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 263;
	c.Data[0] = ((can_float_union_t){.f=bemfq}).b[0];
	c.Data[1] = ((can_float_union_t){.f=bemfq}).b[1];
	c.Data[2] = ((can_float_union_t){.f=bemfq}).b[2];
	c.Data[3] = ((can_float_union_t){.f=bemfq}).b[3];
	c.Data[4] = ((can_float_union_t){.f=bemfd}).b[0];
	c.Data[5] = ((can_float_union_t){.f=bemfd}).b[1];
	c.Data[6] = ((can_float_union_t){.f=bemfd}).b[2];
	c.Data[7] = ((can_float_union_t){.f=bemfd}).b[3];
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_15_165_voltage_rail(float reference_165v, float supply_15v){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 264;
	c.Data[0] = ((can_float_union_t){.f=reference_165v}).b[0];
	c.Data[1] = ((can_float_union_t){.f=reference_165v}).b[1];
	c.Data[2] = ((can_float_union_t){.f=reference_165v}).b[2];
	c.Data[3] = ((can_float_union_t){.f=reference_165v}).b[3];
	c.Data[4] = ((can_float_union_t){.f=supply_15v}).b[0];
	c.Data[5] = ((can_float_union_t){.f=supply_15v}).b[1];
	c.Data[6] = ((can_float_union_t){.f=supply_15v}).b[2];
	c.Data[7] = ((can_float_union_t){.f=supply_15v}).b[3];
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
void orbit_tritium_25_12_voltage_rail(float supply_12v, float supply_25v){
	CanTxMsg c;
	c.IDE = 0;
	c.RTR = 0;
	c.StdId = 265;
	c.Data[0] = ((can_float_union_t){.f=supply_12v}).b[0];
	c.Data[1] = ((can_float_union_t){.f=supply_12v}).b[1];
	c.Data[2] = ((can_float_union_t){.f=supply_12v}).b[2];
	c.Data[3] = ((can_float_union_t){.f=supply_12v}).b[3];
	c.Data[4] = ((can_float_union_t){.f=supply_25v}).b[0];
	c.Data[5] = ((can_float_union_t){.f=supply_25v}).b[1];
	c.Data[6] = ((can_float_union_t){.f=supply_25v}).b[2];
	c.Data[7] = ((can_float_union_t){.f=supply_25v}).b[3];
	c.DLC = 8;
	d1k_CAN_SendPacket(CAN1, &c);
}
