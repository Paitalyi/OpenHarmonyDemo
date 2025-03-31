#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifi_device.h"
#include "lwip/netifapi.h"
#include "lwip/api_shell.h"

void connect_wifi(void)
{

    WifiErrorCode errCode;
    WifiDeviceConfig apConfig = {};
    int netId = -1;

    // // setup your AP params
    // strcpy(apConfig.ssid, "TP-LINK_888");
    // strcpy(apConfig.preSharedKey, "Cto888.com");

    // // setup your AP params
    // strcpy(apConfig.ssid, "1211-A");
    // strcpy(apConfig.preSharedKey, "12345678");
    // strcpy(apConfig.ssid, "M8");
    // strcpy(apConfig.preSharedKey, "12345678");
    strcpy(apConfig.ssid, "TP-LINK_A673");  //apConfig.ssid是一个字符数组 因为数组名在 C 中是不可赋值的左值
                                            //所以apConfig.ssid = "TP-LINK_A673";这种赋值方式是非法的
    strcpy(apConfig.preSharedKey, "aaaaaaaaaa");
    apConfig.securityType = WIFI_SEC_TYPE_PSK;

    errCode = EnableWifi();
    errCode = AddDeviceConfig(&apConfig, &netId);

    errCode = ConnectTo(netId);
    printf("ConnectTo(%d): %d\r\n", netId, errCode);
    usleep(1000 * 1000);
    // 联网业务开始
    // 这里是网络业务代码...
    struct netif *iface = netifapi_netif_find("wlan0");
    if (iface)
    {
        err_t ret = netifapi_dhcp_start(iface);
        printf("netifapi_dhcp_start: %d\r\n", ret);

        usleep(5000 * 1000);
        ; // wait DHCP server give me IP
        ret = netifapi_netif_common(iface, dhcp_clients_info_show, NULL);
        printf("netifapi_netif_common: %d\r\n", ret);
    }
}
