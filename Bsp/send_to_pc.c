#include "send_to_pc.h"
#include "tdma_c.h"
#include "usart.h"

#define STX 0x02
#define ETX 0x03

char send_pc_buf[64];

#define NTC_SEN_ID                0x01
#define PHASE_3_VOL_ID            0x02
#define PHASE_2_VOL_ID            0x03
#define PREASURE_ID               0x04
#define WATER_LEAK_ID             0x05
#define VIB_ID                    0x06
#define UL_SONIC_ID               0x07
#define PHASE_3_CUR_ID            0x08
#define PHASE_2_CUR_ID            0x09  


extern uint16_t REG_INPUT_BUF[];
extern uint16_t REG_HOLD_BUF[];

#define NTC_IDX 				0

#define MODBUS_ID0_IDX				0
#define MODBUS_ID0_TYPE_IDX			1
#define MODBUS_ID0_DATA				2



#define TWO_PAHSE_VOL_IDX 		1
#define THREE_PHASE_VOL_R_IDX 	2
#define THREE_PHASE_VOL_S_IDX  	3
#define THREE_PHASE_VOL_T_IDX 	4
#define PRESURE_IDX 			5
#define WATER_LEAKAGE_IDX 		6
#define GYRO_X_IDX 				7
#define GYRO_Y_IDX 				8
#define GYRO_Z_IDX 				9
#define ULTRA_SONIC_IDX			10
#define THREE_PHASE_CUR_R_IDX	11
#define THREE_PHASE_CUR_S_IDX 	12
#define THREE_PHASE_CUR_T_IDX 	13
#define TWO_PHASE_CUR_IDX		14



#if 1
void send_to_pc()
{
	uint16_t nodeIdIdx;

	nodeIdIdx = (data_msg.hdr.fctl & 0x0F) - 1;
	nodeIdIdx = nodeIdIdx << 4;

	switch (data_msg.payload[0])
	{
		case NTC_SEN_ID: //type
		{
			uint8_t ntc_type = data_msg.payload[0];
			uint8_t ntc_value_100;
			uint8_t ntc_value_10;
			uint8_t ntc_value_1;
			uint8_t ntc_sign;
			uint8_t bat_h =data_msg.payload[7];
			uint8_t bat_l =data_msg.payload[8];
			uint16_t tmpValue;
			

			if( data_msg.payload[1] == 1) // +
			{
				ntc_sign = 0x2b;
			}
			else   // - 
			{
				ntc_sign = 0x2d;
			}
			ntc_value_100 = data_msg.payload[2]%1000/100;
			ntc_value_10 = data_msg.payload[2]%1000%100/10;
			ntc_value_1 = data_msg.payload[2]%1000%100%10/1;

			tmpValue = (ntc_value_100 *100)  + (ntc_value_10 *10) + ntc_value_1 ;

			REG_INPUT_BUF[nodeIdIdx++] = ntc_type; // TPYE
			REG_INPUT_BUF[nodeIdIdx++] = 1+2; // DATA LEN

			if ( ntc_sign == 0x2b ) // +
			{
				REG_INPUT_BUF[nodeIdIdx++] = tmpValue ;
			}
			else
			{
				REG_INPUT_BUF[nodeIdIdx++] = -tmpValue ;
			}

			REG_INPUT_BUF[nodeIdIdx++] = bat_h*10 + bat_l ;
			REG_INPUT_BUF[nodeIdIdx++] = -footer.rssi;

			sprintf(send_pc_buf, "%c%d%d%d%c%d%d%d%d%d%d%c", 					STX,	// stx

																				data_msg.hdr.length-2,  // ?
																				src,
																				ntc_type,  //
																				ntc_sign,  // NTC 부호
																				ntc_value_100,   // NTC VALUE
																				ntc_value_10,   // NTC VALUE
																				ntc_value_1,   // NTC VALUE
																				bat_h,  // bat H
																				bat_l,  // bat L
																				footer.rssi,
																				ETX
			);
			HAL_UART_Transmit(&huart1, send_pc_buf, 14, 10);
		}
		break;
		case PHASE_3_VOL_ID:
		{
			uint8_t phase_3_vol = data_msg.payload[0];
			
			uint8_t phase_R_vol_100 = data_msg.payload[1]/10;
			uint8_t phase_R_vol_10 = data_msg.payload[1]%10;
			uint8_t phase_R_vol_1 = data_msg.payload[2]/10;
			uint8_t phase_R_vol_0 = data_msg.payload[2]%10;

			uint8_t phase_S_vol_100 = data_msg.payload[3]/10;
			uint8_t phase_S_vol_10 = data_msg.payload[3]%10;
			uint8_t phase_S_vol_1 = data_msg.payload[4]/10;
			uint8_t phase_S_vol_0 = data_msg.payload[4]%10;

			uint8_t phase_T_vol_100 = data_msg.payload[5]/10;
			uint8_t phase_T_vol_10 = data_msg.payload[5]%10;
			uint8_t phase_T_vol_1 = data_msg.payload[6]/10;
			uint8_t phase_T_vol_0 = data_msg.payload[6]%10;

			uint8_t bat_h =data_msg.payload[7];
			uint8_t bat_l =data_msg.payload[8];


			REG_INPUT_BUF[nodeIdIdx++] = phase_3_vol; // TPYE
			REG_INPUT_BUF[nodeIdIdx++] = 3+2; // DATA LEN  3PHASE   R S T



			REG_INPUT_BUF[nodeIdIdx++] = (phase_R_vol_100 *1000)  + (phase_R_vol_10 *100) + (phase_R_vol_1 *10)+ phase_R_vol_0 ;
			REG_INPUT_BUF[nodeIdIdx++] = (phase_S_vol_100 *1000)  + (phase_S_vol_10 *100) + (phase_S_vol_1 *10)+ phase_S_vol_0 ;
			REG_INPUT_BUF[nodeIdIdx++] = (phase_T_vol_100 *1000)  + (phase_T_vol_10 *100) + (phase_T_vol_1 *10)+ phase_T_vol_0 ;
			REG_INPUT_BUF[nodeIdIdx++] = bat_h*10 + bat_l ;
			REG_INPUT_BUF[nodeIdIdx++] = -footer.rssi;

			sprintf(send_pc_buf, "%c%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%c", 											STX,	// stx
																										data_msg.hdr.length+6,  // ?
																										src,  // id
																										phase_3_vol,  // sen_id 
																										phase_R_vol_100,
																										phase_R_vol_10,
																										phase_R_vol_1,
																										phase_R_vol_0,
																										phase_S_vol_100,
																										phase_S_vol_10,
																										phase_S_vol_1,
																										phase_S_vol_0,
																										phase_T_vol_100,
																										phase_T_vol_10,
																										phase_T_vol_1,
																										phase_T_vol_0,
																										bat_h,  // bat H
																										bat_l,  // bat L
																										footer.rssi,
																										ETX
			);
			HAL_UART_Transmit(&huart1, send_pc_buf,22, 10);
		}
		break;
		case PHASE_2_VOL_ID:
		{
			uint8_t phase_2_vol = data_msg.payload[0];
			
			uint8_t phase_2_vol_100 = data_msg.payload[1]/10;
			uint8_t phase_2_vol_10 = data_msg.payload[1]%10;
			uint8_t phase_2_vol_1 = data_msg.payload[2]/10;
			uint8_t phase_2_vol_0 = data_msg.payload[2]%10;
			uint8_t bat_h =data_msg.payload[7];
			uint8_t bat_l =data_msg.payload[8];

			REG_INPUT_BUF[nodeIdIdx++] = phase_2_vol; // TPYE
			REG_INPUT_BUF[nodeIdIdx++] = 1+2; // DATA LEN

			REG_INPUT_BUF[nodeIdIdx++] = (phase_2_vol_100 *1000)  + (phase_2_vol_10 *100) + (phase_2_vol_1 *10) + phase_2_vol_0;
			REG_INPUT_BUF[nodeIdIdx++] = bat_h*10 + bat_l ;
			REG_INPUT_BUF[nodeIdIdx++] = -footer.rssi;

			sprintf(send_pc_buf, "%c%d%d%d%d%d%d%d%d%d%d%c", 											STX,	// stx
																										data_msg.hdr.length-2,  // ?
																										src,  // id
																										phase_2_vol,
																										phase_2_vol_100,
																										phase_2_vol_10,
																										phase_2_vol_1,
																										phase_2_vol_0,
																										bat_h,  // bat H
																										bat_l,  // bat L
																										footer.rssi,
																										ETX
			);
			HAL_UART_Transmit(&huart1, send_pc_buf,14, 10);
		}
		break;
		case PREASURE_ID:
		{
			uint8_t preasure_type=data_msg.payload[0];

			uint8_t preasure_val_1 		= data_msg.payload[1]/10;
			uint8_t preasure_val_2 		= data_msg.payload[1]%10;
			uint8_t preasure_val_3 		= data_msg.payload[2]/10;
			uint8_t preasure_val_4      = data_msg.payload[2]%10;
			uint8_t bat_h =data_msg.payload[7];
			uint8_t bat_l =data_msg.payload[8];


			REG_INPUT_BUF[nodeIdIdx++] = preasure_type; // TPYE
			REG_INPUT_BUF[nodeIdIdx++] = 1+2; // DATA LEN

			REG_INPUT_BUF[nodeIdIdx++] = (preasure_val_1 *1000)  + (preasure_val_2 *100) + (preasure_val_3 *10) + preasure_val_4;
			REG_INPUT_BUF[nodeIdIdx++] = bat_h*10 + bat_l ;
			REG_INPUT_BUF[nodeIdIdx++] = -footer.rssi;
#if 1
			sprintf(send_pc_buf, "%c%d%d%d%d%d%d%d%d%d%d%c", 											STX,	// stx
																										data_msg.hdr.length-2,  // ?
																										src,
																										preasure_type,
																										preasure_val_1,
																										preasure_val_2,
																										preasure_val_3,
																										preasure_val_4,
																										bat_h,  // bat H
																										bat_l,  // bat L
																										footer.rssi,
																										ETX
			);
			HAL_UART_Transmit(&huart1, send_pc_buf,14, 10);
#endif
		}
		break;
		case WATER_LEAK_ID:
		{
			char sen_value;
			uint8_t water_type = data_msg.payload[0];
			uint8_t dont_care = 0;
			uint8_t bat_h =data_msg.payload[7];
			uint8_t bat_l =data_msg.payload[8];
			if( data_msg.payload[2] )
			{
				sen_value = 'D';
			}
			else
			{
				sen_value = 'N';
			}


			REG_INPUT_BUF[nodeIdIdx++] = water_type; // TPYE
			REG_INPUT_BUF[nodeIdIdx++] = 1+2; // DATA LEN

			if ( sen_value == 'D')
				REG_INPUT_BUF[nodeIdIdx++] = 0x0001;
			else
				REG_INPUT_BUF[nodeIdIdx++] = 0x0000;

			REG_INPUT_BUF[nodeIdIdx++] = bat_h*10 + bat_l ;
			REG_INPUT_BUF[nodeIdIdx++] = -footer.rssi;

			sprintf(send_pc_buf, "%c%d%d%d%c%d%d%d%d%d%d%c", 											STX,	// stx
																										data_msg.hdr.length-2,  // ?
																										src,
																										water_type,
																										sen_value,
																										dont_care,
																										dont_care,
																										dont_care,
																										bat_h,  // bat H
																										bat_l,  // bat L
																										footer.rssi,
																										ETX
			);
			HAL_UART_Transmit(&huart1, send_pc_buf,14, 10);
		}
		break;
		case VIB_ID:
		{
			uint8_t vib_type = data_msg.payload[0];
			uint8_t x_data_100;
			uint8_t x_data_10;
			uint8_t x_data_1;
			uint8_t x_data_sign;
			uint8_t y_data_100;
			uint8_t y_data_10;
			uint8_t y_data_1;
			uint8_t y_data_sign;
			uint8_t z_data_100;
			uint8_t z_data_10;
			uint8_t z_data_1;
			uint8_t z_data_sign;
			uint8_t bat_h =data_msg.payload[7];
			uint8_t bat_l =data_msg.payload[8];
			uint16_t tmpValue[3];

			if( data_msg.payload[1] == 0) // +
			{
				x_data_sign = 0x2b;
			}
			else
			{
				x_data_sign = 0x2d;
			}
			if( data_msg.payload[3] == 0) // +
			{
				y_data_sign = 0x2b;
			}
			else
			{
				y_data_sign = 0x2d;
			}

			if( data_msg.payload[5] == 0) // +
			{
				z_data_sign = 0x2b;
			}
			else
			{
				z_data_sign = 0x2d;
			}
			x_data_100 = data_msg.payload[2]%1000/100;
			x_data_10  = data_msg.payload[2]%1000%100/10;
			x_data_1   = data_msg.payload[2]%1000%100%10/1;
			y_data_100 = data_msg.payload[4]%1000/100;
			y_data_10  = data_msg.payload[4]%1000%100/10;
			y_data_1   = data_msg.payload[4]%1000%100%10/1;
			z_data_100 = data_msg.payload[6]%1000/100;
			z_data_10  = data_msg.payload[6]%1000%100/10;
			z_data_1   = data_msg.payload[6]%1000%100%10/1;

			tmpValue[0] = (x_data_100 *100)  + (x_data_10 *10) + (x_data_1 );
			tmpValue[1] = (y_data_100 *100)  + (y_data_10 *10) + (y_data_1 );
			tmpValue[2] = (z_data_100 *100)  + (z_data_10 *10) + (z_data_1 );


			REG_INPUT_BUF[nodeIdIdx++] = vib_type; // TPYE
			REG_INPUT_BUF[nodeIdIdx++] = 3+2; // DATA LEN  3PHASE   R S T


			if ( x_data_sign == '+')
			{
				REG_INPUT_BUF[nodeIdIdx++]=tmpValue[0];
			}
			else
			{
				REG_INPUT_BUF[nodeIdIdx++]=-tmpValue[0];
			}
			if ( y_data_sign == '+')
			{
				REG_INPUT_BUF[nodeIdIdx++]=tmpValue[1];
			}
			else
			{
				REG_INPUT_BUF[nodeIdIdx++]=-tmpValue[1];
			}
			if ( z_data_sign == '+')
			{
				REG_INPUT_BUF[nodeIdIdx++]=tmpValue[2];
			}
			else
			{
				REG_INPUT_BUF[nodeIdIdx++]= -tmpValue[2];
			}
			REG_INPUT_BUF[nodeIdIdx++] = bat_h*10 + bat_l ;
			REG_INPUT_BUF[nodeIdIdx++] = -footer.rssi;

			sprintf(send_pc_buf, "%c%d%d%d%c%d%d%d%c%d%d%d%c%d%d%d%d%d%d%c", 							STX,	// stx
																										data_msg.hdr.length+6,  // ?
																										src,
																										vib_type,  //
																										x_data_sign, 
																										x_data_100,  
																										x_data_10,   
																										x_data_1,   
																										y_data_sign, 
																										y_data_100,  
																										y_data_10,   
																										y_data_1,   
																										z_data_sign, 
																										z_data_100,  
																										z_data_10,   
																										z_data_1,   
																										bat_h,  // bat H
																										bat_l,  // bat L
																										footer.rssi,
																										ETX
					);
			HAL_UART_Transmit(&huart1, send_pc_buf,22, 10);
		}
		break;
		case UL_SONIC_ID:
		{
			uint8_t ul_sonic = data_msg.payload[0];
			
//			uint8_t ul_sonic_100 = data_msg.payload[1]/10;
//			uint8_t ul_sonic_10 = data_msg.payload[2]%10;
//			uint8_t ul_sonic_1 = data_msg.payload[3]/10;
//			uint8_t ul_sonic_0 = data_msg.payload[4]%10;


			//uint8_t ul_sonic_100 = data_msg.payload[1]/10;
			//uint8_t ul_sonic_10 = data_msg.payload[2]%10;
			//uint8_t ul_sonic_1 = data_msg.payload[3]/10;
			//uint8_t ul_sonic_0 = data_msg.payload[4]%10;
			
			uint8_t ul_sonic_100 = data_msg.payload[1];
			uint8_t ul_sonic_10 = data_msg.payload[2];
			uint8_t ul_sonic_1 = data_msg.payload[3];
			uint8_t ul_sonic_0 = data_msg.payload[4];

			uint8_t bat_h =data_msg.payload[7];
			uint8_t bat_l =data_msg.payload[8];

			REG_INPUT_BUF[nodeIdIdx++] = ul_sonic; // TPYE
			REG_INPUT_BUF[nodeIdIdx++] = 1+2; // DATA LEN  ul_sonic + bat + rssi

			REG_INPUT_BUF[nodeIdIdx] = ( ul_sonic_100 *1000 ) + (ul_sonic_10 *100 ) + (ul_sonic_1 *10) + ul_sonic_0 ;
			REG_INPUT_BUF[nodeIdIdx++] = bat_h*10 + bat_l ;
			REG_INPUT_BUF[nodeIdIdx++] = -footer.rssi;

			sprintf(send_pc_buf, "%c%d%d%d%d%d%d%d%d%d%d%c", 											STX,	// stx
																										data_msg.hdr.length-2,  // ?
																										src,  // id
																										ul_sonic,
																										ul_sonic_100,
																										ul_sonic_10,
																										ul_sonic_1,
																										ul_sonic_0,
																										bat_h,  // bat H
																										bat_l,  // bat L
																										footer.rssi,
																										ETX
			);
			HAL_UART_Transmit(&huart1, send_pc_buf,14, 10);

		}
		break;
		case PHASE_3_CUR_ID:
		{
			uint8_t phase_3_cur = data_msg.payload[0];
			
			uint8_t phase_R_cur_100 = data_msg.payload[1]/10;
			uint8_t phase_R_cur_10 = data_msg.payload[1]%10;
			uint8_t phase_R_cur_1 = data_msg.payload[2]/10;
			uint8_t phase_R_cur_0 = data_msg.payload[2]%10;

			uint8_t phase_S_cur_100 = data_msg.payload[3]/10;
			uint8_t phase_S_cur_10 = data_msg.payload[3]%10;
			uint8_t phase_S_cur_1 = data_msg.payload[4]/10;
			uint8_t phase_S_cur_0 = data_msg.payload[4]%10;

			uint8_t phase_T_cur_100 = data_msg.payload[5]/10;
			uint8_t phase_T_cur_10 = data_msg.payload[5]%10;
			uint8_t phase_T_cur_1 = data_msg.payload[6]/10;
			uint8_t phase_T_cur_0 = data_msg.payload[6]%10;

			uint8_t bat_h =data_msg.payload[7];
			uint8_t bat_l =data_msg.payload[8];


			REG_INPUT_BUF[nodeIdIdx++] = phase_3_cur; // TPYE
			REG_INPUT_BUF[nodeIdIdx++] = 3+2; // DATA LEN  3PHASE   R S T

			REG_INPUT_BUF[nodeIdIdx++] = (phase_R_cur_100 * 1000) + (phase_R_cur_10 *100) + (phase_R_cur_1 *10) + phase_R_cur_0;
			REG_INPUT_BUF[nodeIdIdx++] = (phase_S_cur_100 * 1000) + (phase_S_cur_10 *100) + (phase_S_cur_1 *10) + phase_S_cur_0;
			REG_INPUT_BUF[nodeIdIdx++] = (phase_T_cur_100 * 1000) + (phase_T_cur_10 *100) + (phase_T_cur_1 *10) + phase_T_cur_0;
			REG_INPUT_BUF[nodeIdIdx++] = bat_h*10 + bat_l ;
			REG_INPUT_BUF[nodeIdIdx++] = -footer.rssi;

			sprintf(send_pc_buf, "%c%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%c", 											STX,	// stx
																										data_msg.hdr.length+6,  // ?
																										src,  // id
																										phase_3_cur,  // sen_id 
																										phase_R_cur_100,
																										phase_R_cur_10,
																										phase_R_cur_1,
																										phase_R_cur_0,
																										phase_S_cur_100,
																										phase_S_cur_10,
																										phase_S_cur_1,
																										phase_S_cur_0,
																										phase_T_cur_100,
																										phase_T_cur_10,
																										phase_T_cur_1,
																										phase_T_cur_0,
																										bat_h,  // bat H
																										bat_l,  // bat L
																										footer.rssi,
																										ETX
			);
			HAL_UART_Transmit(&huart1, send_pc_buf,22, 10);
		}
		break;
		case PHASE_2_CUR_ID:
		{
			uint8_t phase_2_cur = data_msg.payload[0];
			uint8_t phase_2_cur_100 = data_msg.payload[1]/10;
			uint8_t phase_2_cur_10 = data_msg.payload[1]%10;
			uint8_t phase_2_cur_1 = data_msg.payload[2]/10;
			uint8_t phase_2_cur_0 = data_msg.payload[2]%10;
			uint8_t bat_h =data_msg.payload[7];
			uint8_t bat_l =data_msg.payload[8];

			REG_INPUT_BUF[nodeIdIdx++] = phase_2_cur; // TPYE
			REG_INPUT_BUF[nodeIdIdx++] = 1 + 2; // DATA LEN   2phase + bat + rssi
			REG_INPUT_BUF[nodeIdIdx++] = (phase_2_cur_100 *1000 ) + (phase_2_cur_10 *100) + (phase_2_cur_1 *10) + phase_2_cur_0;
			REG_INPUT_BUF[nodeIdIdx++] = bat_h*10 + bat_l ;
			REG_INPUT_BUF[nodeIdIdx++] = -footer.rssi;

			sprintf(send_pc_buf, "%c%d%d%d%d%d%d%d%d%d%d%c", 											STX,	// stx
																										data_msg.hdr.length-2,  // ?
																										src,  // id
																										phase_2_cur,
																										phase_2_cur_100,
																										phase_2_cur_10,
																										phase_2_cur_1,
																										phase_2_cur_0,
																										bat_h,  // bat H
																										bat_l,  // bat L
																										footer.rssi,
																										ETX
			);
			HAL_UART_Transmit(&huart1, send_pc_buf,14, 10);
		}
		break;

	}

}

#endif

