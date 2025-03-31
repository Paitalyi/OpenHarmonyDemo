#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>
#include "ohos_init.h"
#include "cmsis_os2.h"

#include <unistd.h>
#include "hi_wifi_api.h"
//#include "wifi_sta.h"
#include "lwip/ip_addr.h"
#include "lwip/netifapi.h"

#include "lwip/sockets.h"

#include "MQTTPacket.h"
#include "transport.h"

int toStop = 0;
int led_level = 0;

int mqtt_connect(void)
{
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	int rc = 0;
	int mysock = 0;
	int len = 0;
	int msgid = 1;
    int req_qos  = 0;

	// 定义缓冲区
	unsigned char buf[200];
	int buflen = sizeof(buf);
	memset(buf, 0, buflen);  // 清空缓冲区

    MQTTString topicString = MQTTString_initializer;

	char *host = "192.168.2.131"; // MQTT服务器地址
	int port = 1883;

	// 连接服务器
	mysock = transport_open(host, port);
	if (mysock < 0)
		return mysock;

	printf("Connecting to MQTT_Server: %s port %d\n", host, port);

	data.clientID.cstring = "hi3861_full_spectrum_led";
	data.keepAliveInterval = 20;
	data.cleansession = 1;
	//data.username.cstring = "testuser";
	//data.password.cstring = "testpassword";

	len = MQTTSerialize_connect(buf, buflen, &data);
	rc = transport_sendPacketBuffer(mysock, buf, len);

	/* wait for connack */
	if (MQTTPacket_read(buf, buflen, transport_getdata) == CONNACK)
	{
		unsigned char sessionPresent, connack_rc;

		if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
		{
			printf("Unable to connect, return code %d\n", connack_rc);
			goto exit;
		}else{
			printf("Connected to %s:%d\n", host, port);
		}
	}
	else{
		printf("CONNACK not received\n");
		goto exit;
	}
	// 订阅
	topicString.cstring = "led_1";
	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);
	rc = transport_sendPacketBuffer(mysock, buf, len);
	if (MQTTPacket_read(buf, buflen, transport_getdata) == SUBACK){ //等待SUBACK
		unsigned short submsgid;
		int subcount;
		int granted_qos;

		rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
		if(granted_qos != 0){
			printf("granted qos != 0, %d\n", granted_qos);
			goto exit;
		}
	}else{
		printf("SUBACK not received\n");
		goto exit;
	}

	// 循环获取被订阅主题的消息
	while(!toStop){
		if(MQTTPacket_read(buf, buflen, transport_getdata) == PUBLISH){ //transport_getdata默认延时1s
			unsigned char dup;
			int qos;
			unsigned char retained;
			unsigned short msgid;
			int payloadlen_in;
			unsigned char* payload_in;
			int rc;
			MQTTString receivedTopic;
			rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic, &payload_in, &payloadlen_in, buf, buflen);
			if (payloadlen_in > 0 && payloadlen_in < 30){
				payload_in[payloadlen_in] = '\0';
				printf("received topic: %s, payload: %s\n", receivedTopic.lenstring.data, payload_in);
				led_level = atoi((char *)payload_in); //字符串转整数
			}
			rc = rc;
		}
		usleep(500 * 1000); // 500ms
	}

	printf("disconnecting\n");
	len = MQTTSerialize_disconnect(buf, buflen);
	rc = transport_sendPacketBuffer(mysock, buf, len);
exit:
	transport_close(mysock);

	rc = rc;

	return 0;
}
