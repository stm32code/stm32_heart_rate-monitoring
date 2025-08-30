#include "git.h"

Data_TypeDef Data_init;						  // �豸���ݽṹ��
Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
Device_Satte_Typedef device_state_init;		  // �豸״̬
extern int32_t n_sp02;		 // SPO2 value
extern int8_t ch_spo2_valid; // indicator to show if the SP02 calculation is valid
extern int32_t n_heart_rate; // heart rate value
extern int8_t ch_hr_valid;	 // indicator to show if the heart rate calculation is valid

// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data)
{

	return MY_SUCCESSFUL;
}
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state)
{

	// ��
	R_Test();


	return MY_SUCCESSFUL;
}
// ����OLED��ʾ��������
mySta Update_oled_massage()
{
#if OLED // �Ƿ��
	char str[50];
	// ����Ѫ��
	if(0<n_heart_rate&&n_heart_rate<150 && ch_spo2_valid){
	sprintf(str, "Heart : %03d",n_heart_rate);
	OLED_ShowCH(0, 0, (unsigned char *)str);
	sprintf(str, "Blood : %03d", n_sp02);
	OLED_ShowCH(0, 2, (unsigned char *)str);


	}else{
		sprintf(str, "Heart : %03d",0);
		OLED_ShowCH(0, 0, (unsigned char *)str);
		sprintf(str, "Blood : %03d", 0);
		OLED_ShowCH(0, 2, (unsigned char *)str);

	}
	sprintf(str, "Waning: ");
	OLED_ShowCH(0, 4, (unsigned char *)str);
	if(device_state_init.Waning){
		sprintf(str, " Open ");
	}else{
		sprintf(str, "Close ");
	}
	OLED_ShowCH(60, 4, (unsigned char *)str);

#endif

	return MY_SUCCESSFUL;
}

// �����豸״̬
mySta Update_device_massage()
{
	// �Զ�ģʽ

	if (120<n_heart_rate&&n_heart_rate<150)
	{
		BEEP = 1;
		delay_ms(50);
		BEEP = 0;
		device_state_init.Waning = 1;
	}
	else
	{
		BEEP = 0;
		device_state_init.Waning = 0;
	}

	return MY_SUCCESSFUL;
}

// ��ʱ��
void Automation_Close(void)
{

	
	if (Data_init.App)
	{
		switch (Data_init.App)
		{
		case 1:
			SendMqtt(1); // �������ݵ�APP
			break;
		case 2:
			SendData(); // �������ݵ���ƽ̨
			break;
		}
		Data_init.App = 0;
	}

}
// ��ⰴ���Ƿ���
static U8 num_on = 0;
static U8 key_old = 0;
void Check_Key_ON_OFF()
{
	U8 key;
	key = KEY_Scan(1);
	// ����һ�εļ�ֵ�Ƚ� �������ȣ������м�ֵ�ı仯����ʼ��ʱ
	if (key != 0 && num_on == 0)
	{
		key_old = key;
		num_on = 1;
	}
	if (key != 0 && num_on >= 1 && num_on <= Key_Scan_Time) // 25*10ms
	{
		num_on++; // ʱ���¼��
	}
	if (key == 0 && num_on > 0 && num_on < Key_Scan_Time) // �̰�
	{
		switch (key_old)
		{
		case KEY1_PRES:
			printf("Key1_Short\n");

			break;
		case KEY2_PRES:
			printf("Key2_Short\n");

			break;
		case KEY3_PRES:
			printf("Key3_Short\n");

			break;

		default:
			break;
		}
		num_on = 0;
	}
	else if (key == 0 && num_on >= Key_Scan_Time) // ����
	{
		switch (key_old)
		{
		case KEY1_PRES:
			printf("Key1_Long\n");

			Data_init.App = 1;

			break;
		case KEY2_PRES:
			printf("Key2_Long\n");

			break;
		case KEY3_PRES:
			printf("Key3_Long\n");

			break;
		default:
			break;
		}
		num_on = 0;
	}
}
// ����json����
mySta massage_parse_json(char *message)
{

	cJSON *cjson_test = NULL; // ���json��ʽ
	cJSON *cjson_data = NULL; // ����
	const char *massage;
	// ������������
	u8 cjson_cmd; // ָ��,����

	/* ��������JSO���� */
	cjson_test = cJSON_Parse(message);
	if (cjson_test == NULL)
	{
		// ����ʧ��
		printf("parse fail.\n");
		return MY_FAIL;
	}

	/* ���θ���������ȡJSON���ݣ���ֵ�ԣ� */
	cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
	/* ����Ƕ��json���� */
	cjson_data = cJSON_GetObjectItem(cjson_test, "data");

	switch (cjson_cmd)
	{
	case 0x01: // ��Ϣ��

		device_state_init.Key_State= cJSON_GetObjectItem(cjson_data, "led")->valueint;

		break;
	case 0x02: // ��Ϣ��

		break;
	case 0x03: // ���ݰ�

		break;
	case 0x04: // ���ݰ�
		Data_init.App = cjson_cmd + 1;

		break;
	default:
		break;
	}

	/* ���JSON����(��������)���������� */
	cJSON_Delete(cjson_test);

	return MY_SUCCESSFUL;
}
