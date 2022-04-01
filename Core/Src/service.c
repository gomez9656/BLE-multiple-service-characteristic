/*
 * service.c
 *
 *  Created on: 26 Mar 2022
 *      Author: Juan Gomez
 */
#include <stdio.h>
#include <stdlib.h>
#include "bluenrg1_gap.h"
#include "bluenrg1_gatt_aci.h"
#include "service.h"

//device has 2 services with 2 characteristic each
//health service with  BPM and weight characteristic
//weather service with temperature and humidity characteristic

uint8_t HEALTH_SERVICE_UUID[16] = {0x66, 0x9a, 0x0c, 0x20, 0x00, 0x08, 0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xe0, 0xf2, 0x73, 0xd9};
uint8_t BPM_CHAR_UUID[16] = 			{0x66, 0x9a, 0x0c, 0x20, 0x00, 0x08, 0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xe1, 0xf2, 0x73, 0xd9};
uint8_t WEIGHT_CHAR_UUID[16] = 	{0x66, 0x9a, 0x0c, 0x20, 0x00, 0x08, 0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xe2, 0xf2, 0x73, 0xd9};

uint8_t WEATHER_SERVICE_UUID[16] = 	{0x92, 0x25, 0xc1, 0xfa, 0x86, 0x69, 0x46, 0xa1, 0x85, 0xb8, 0x76, 0x60, 0x70, 0x85, 0xa7,  0x83};
uint8_t TEMP_CHAR_UUID[16] = 		{0x92, 0x25, 0xc1, 0xfa, 0x86, 0x69, 0x46, 0xa1, 0x85, 0xb8, 0x76, 0x60, 0x71, 0x85, 0xa7,  0x83};
uint8_t HUM_CHAR_UUID[16] =			{0x92, 0x25, 0xc1, 0xfa, 0x86, 0x69, 0x46, 0xa1, 0x85, 0xb8, 0x76, 0x60, 0x72, 0x85, 0xa7,  0x83};

uint16_t health_service_handle, bpm_char_handle, weight_char_handle;
uint16_t weather_service_handle, temp_char_handle, hum_char_handle;

uint8_t connected = 0;
uint8_t set_connectable = 1;
uint16_t connection_handle = 0;
uint8_t notification_enabled = 0;

uint8_t bpm = 85;
uint8_t weight = 90;
uint8_t temp = 20;
uint8_t hum = 80;

tBleStatus add_services(void){

	tBleStatus ret;
	Service_UUID_t health_service_uuid, weather_service_uuid;
	Char_UUID_t bpm_char_uuid, weight_char_uuid, temp_char_uuid, hum_char_uuid;

	BLUENRG_memcpy(health_service_uuid.Service_UUID_128, HEALTH_SERVICE_UUID, 16);
	BLUENRG_memcpy(weather_service_uuid.Service_UUID_128, WEATHER_SERVICE_UUID, 16);


	BLUENRG_memcpy(bpm_char_uuid.Char_UUID_128, BPM_CHAR_UUID, 16);
	BLUENRG_memcpy(weight_char_uuid.Char_UUID_128, WEIGHT_CHAR_UUID, 16);
	BLUENRG_memcpy(temp_char_uuid.Char_UUID_128, TEMP_CHAR_UUID, 16);
	BLUENRG_memcpy(hum_char_uuid.Char_UUID_128, HUM_CHAR_UUID, 16);
	//add health service service
	ret = aci_gatt_add_service(UUID_TYPE_128, &health_service_uuid, PRIMARY_SERVICE, 7, &health_service_handle);

	//add health weather service
	ret = aci_gatt_add_service(UUID_TYPE_128, &weather_service_uuid, PRIMARY_SERVICE, 7, &weather_service_handle);

	// add bpm characteristic
	ret = aci_gatt_add_char(health_service_handle, UUID_TYPE_128, &bpm_char_uuid, 2, CHAR_PROP_READ, ATTR_PERMISSION_NONE,
											GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 0, 0, &bpm_char_handle);

	//add weight char
	ret = aci_gatt_add_char(health_service_handle, UUID_TYPE_128, &weight_char_uuid, 2, CHAR_PROP_READ, ATTR_PERMISSION_NONE,
											GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 0, 0, &weight_char_handle);

	// add temperature char
	ret = aci_gatt_add_char(weather_service_handle, UUID_TYPE_128, &temp_char_uuid, 2, CHAR_PROP_READ, ATTR_PERMISSION_NONE,
											GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 0, 0, &temp_char_handle);

	//add humidity char
	ret = aci_gatt_add_char(weather_service_handle, UUID_TYPE_128, &hum_char_uuid, 2, CHAR_PROP_READ, ATTR_PERMISSION_NONE,
											GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 0, 0, &hum_char_handle);

	return ret;
}

void update_bpm_data(int16_t new_data){

	tBleStatus ret;

	//update characteristic value
	ret = aci_gatt_update_char_value(health_service_handle, bpm_char_handle, 0, 2, (uint8_t*)&new_data);
	if(ret != BLE_STATUS_SUCCESS){
		printf("BPM aci_gatt_update_char_value : FAILED \n\r");
	}
}

void update_weight_data(int16_t new_data){

	tBleStatus ret;

	//update characteristic value
	ret = aci_gatt_update_char_value(health_service_handle, weight_char_handle, 0, 2, (uint8_t*)&new_data);
	if(ret != BLE_STATUS_SUCCESS){
		printf("weight aci_gatt_update_char_value : FAILED \n\r");
	}
}

void update_temp_data(int16_t new_data){

	tBleStatus ret;

	//update characteristic value
	ret = aci_gatt_update_char_value(weather_service_handle, temp_char_handle, 0, 2, (uint8_t*)&new_data);
	if(ret != BLE_STATUS_SUCCESS){
		printf("temperature aci_gatt_update_char_value : FAILED \n\r");
	}
}

void update_hum_data(int16_t new_data){

	tBleStatus ret;

	//update characteristic value
	ret = aci_gatt_update_char_value(weather_service_handle, hum_char_handle, 0, 2, (uint8_t*)&new_data);
	if(ret != BLE_STATUS_SUCCESS){
		printf("humidity aci_gatt_update_char_value : FAILED \n\r");
	}
}

/* These functions are depreceated
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle){

	connected = 1;
	connection_handle = handle;

	prinf("Connection complete \n\r");
}

void GAP_DisconnectionComplete_CB(void){

	prinf("Disconnection complete \n\r");
}
*/

void Read_Request_CB(uint16_t handle){

	if(handle == bpm_char_handle + 1){

		bpm = bpm + 2;

		update_bpm_data(bpm);
	}
	else 	if(handle == weight_char_handle + 1){

		weight = weight + 2;

		update_weight_data(weight);
	}
	else 	if(handle == temp_char_handle + 1){

		temp = temp + 2;

		update_temp_data(temp);
	}
	else 	if(handle == hum_char_handle + 1){

		hum = hum + 2;

		update_hum_data(hum);
	}

	if(connection_handle != 0){

		aci_gatt_allow_read(connection_handle);
	}
}

void aci_gatt_read_permit_req_event(uint16_t Connection_Handle, uint16_t Attribute_Handle, uint16_t Offset){

	Read_Request_CB(Attribute_Handle);
}

void hci_le_connection_complete_event(uint8_t Status, uint16_t Connection_Handle, uint8_t Role, uint8_t Peer_Address_Type,
																	uint8_t Peer_Address[6], uint16_t Conn_Interval, uint16_t Conn_Latency,
																	uint16_t Supervision_Timeout, uint8_t Master_Clock_Accuracy)
{
	connected = 1;
	connection_handle = Connection_Handle;
}

void hci_disconnection_complete_event(uint8_t Status, uint16_t Connection_Handle, uint8_t Reason){

	connected = 1;
	set_connectable = 1;
	connection_handle = 0;

	printf("Disconnected \n\r");
}

void APP_UserEvRx(void *pData){

	uint32_t i;

	hci_spi_pckt *hci_pckt = (hci_spi_pckt*)pData;

	//process event packet
	if(hci_pckt->type == HCI_EVENT_PKT){

		//Get data from packet
		hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;

		//process meta data
		if(event_pckt->evt == EVT_LE_META_EVENT){

			//get meta data
			evt_le_meta_event *evt = (void*)event_pckt->data;

			//process each meta data event
			for(i = 0; i < (sizeof(hci_le_meta_events_table) / sizeof(hci_le_meta_events_table_type)); i++){

				if(evt->subevent == hci_le_meta_events_table[i].evt_code){

					hci_le_meta_events_table[i].process((void*)evt->data);
				}
			}
		}

		//process vendor event
		else if(event_pckt->evt == EVT_VENDOR){

			//get meta data
			evt_blue_aci *blue_evt = (void*)event_pckt->data;

			//process each vendor event
			for(i = 0; i < (sizeof(hci_vendor_specific_events_table) / sizeof(hci_vendor_specific_events_table_type)); i++){

				if(blue_evt->ecode == hci_vendor_specific_events_table[i].evt_code){

					hci_vendor_specific_events_table[i].process((void*)blue_evt->data);
				}
			}
		}

		//process other events
		else{

			for(i = 0; i < (sizeof(hci_events_table) / sizeof(hci_events_table_type)); i++){

				if(event_pckt->evt == hci_events_table[i].evt_code){

					hci_events_table[i].process((void*)event_pckt->data);
				}
			}
		}
	}
}












