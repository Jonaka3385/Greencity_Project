/**
   @file lorawan.cpp
   @author Julian Schaa
   @brief LoRaWan node example with OTAA registration
          RAK4631
   @version 1.1
   @date 2023-06-09
**/

#include "main.h"

#define LORAWAN_DATARATE DR_0                                       /** LoRaMac datarates definition, from DR_0 to DR_5 **/
#define LORAWAN_TX_POWER TX_POWER_5                                 /** LoRaMac tx power definition, from TX_POWER_0 to TX_POWER_15 **/
#define LORAWAN_ADR LORAWAN_ADR_ON                                  /** LoRAMac enable ADR */
#define LORAWAN_DO_OTAA true                                       /** Enable OTAA */
#define JOINREQ_NBTRIALS 3                                          /** < Number of trials for the join request. **/

DeviceClass_t g_CurrentClass    = CLASS_A;                          /** class definition **/
LoRaMacRegion_t g_CurrentRegion = LORAMAC_REGION_EU868;             /** Region:EU868 **/
lmh_confirm g_CurrentConfirm    = LMH_UNCONFIRMED_MSG;              /** confirm/unconfirm packet definition **/
uint8_t gAppPort                = LORAWAN_APP_PORT;                 /** data port **/
bool lorawanJoined              = false;

// OTAA keys !!!! KEYS ARE MSB !!!!
uint8_t nodeDeviceEUI[8]    = {0xAC, 0x1F, 0x09, 0xFF, 0xFE, 0x08, 0xFB, 0xA1};
uint8_t nodeJoinEUI[8]      = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t nodeAppKey[16]      = {0x1A,0xE1,0x9D,0x5C,0x88,0xA6,0xD3,0xA6,0x3B,0xCE,0xEB,0x08,0xFE,0x66,0xDB,0x10};

// Private definitions
#define LORAWAN_APP_DATA_BUFF_SIZE 32                                         /** buffer size in bytes of the data that can be transmitted. */
static uint8_t m_lora_app_data_buffer[LORAWAN_APP_DATA_BUFF_SIZE];            /** Lora user application data buffer. */
static lmh_app_data_t m_lora_app_data = {m_lora_app_data_buffer, 0, 0, 0, 0}; /** Lora user application data structure. */
static uint32_t count = 0;                                                    /** Message counter (not fCnt!)*/
static uint32_t count_fail = 0;                                               /** Fail Message counter (not fCnt!)*/

// Forward declarations
static void lorawan_has_joined_handler(void);
static void lorawan_join_failed_handler(void);
static void lorawan_rx_handler(lmh_app_data_t *app_data);
static void lorawan_confirm_class_handler(DeviceClass_t Class);

/**
 * @brief Structure containing LoRaWan parameters, needed for lmh_init()
 */
static lmh_param_t g_lora_param_init = {LORAWAN_ADR, LORAWAN_DATARATE, LORAWAN_PUBLIC_NETWORK, JOINREQ_NBTRIALS, LORAWAN_TX_POWER, LORAWAN_DUTYCYCLE_OFF};

/**
 * @brief Structure containing LoRaWan callback functions, needed for lmh_init()
 */
static lmh_callback_t g_lora_callbacks = {  BoardGetBatteryLevel, BoardGetUniqueId, BoardGetRandomSeed,
                                            lorawan_rx_handler, lorawan_has_joined_handler, lorawan_confirm_class_handler, lorawan_join_failed_handler
                                         };

/**
 * @brief LoRaWAN setup function
 *
 */
void initLoRaWAN()
{
    uint32_t err_code;
    // Initialize LoRa chip.
    lora_rak4630_init();
    Serial.println("=====================================");
    Serial.println("Welcome to RAK4630 LoRaWan!!!");
    Serial.println("Type: OTAA");
    Serial.println("Region: EU868");
    Serial.println("=====================================");

    lmh_setDevEui(nodeDeviceEUI);
    lmh_setAppEui(nodeJoinEUI);
    lmh_setAppKey(nodeAppKey);

    // Initialize LoRaWan
    err_code = lmh_init(&g_lora_callbacks, g_lora_param_init, LORAWAN_DO_OTAA, g_CurrentClass, g_CurrentRegion);
    if (err_code != 0)
    {
        Serial.printf("lmh_init failed - %d\n", err_code);
        return;
    }

    // Start Join procedure
    lmh_join();
}

/**
 * @brief LoRa function for handling HasJoined event.
 */
void lorawan_has_joined_handler(void)
{
    Serial.println("OTAA Mode, Network Joined!");
    lorawanJoined = true;
}

/**
 * @brief LoRa function for handling OTAA join failed
 */
static void lorawan_join_failed_handler(void)
{
    Serial.println("OTAA join failed!");
    Serial.println("Check your EUI's and Keys's!");
    Serial.println("Check if a Gateway is in range!");
}

/**
 * @brief Function for handling LoRaWan received data from Gateway
 * @param[in] app_data  Pointer to rx data
 */
void lorawan_rx_handler(lmh_app_data_t *app_data)
{
  Serial.printf("LoRa Packet received on port %d, size:%d, rssi:%d, snr:%d, data:%s\n",
          app_data->port, app_data->buffsize, app_data->rssi, app_data->snr, app_data->buffer);
}

/**
 * @brief Function for handling a LoRaWAN Class switch
 * sends an empty packet to inform the gateway
 * @param Class
 */
void lorawan_confirm_class_handler(DeviceClass_t Class)
{
  Serial.printf("switch to class %c done\n", "ABC"[Class]);
  // Informs the server that switch has occurred ASAP
  m_lora_app_data.buffsize = 0;
  m_lora_app_data.port = gAppPort;
  lmh_send(&m_lora_app_data, g_CurrentConfirm);
}

void send_lora_frame()
{

    /** are we joined? */
    if (lmh_join_status_get() != LMH_SET)
    {
        //Not joined, try again later
        return;
    }
    //Aufruf der Methode Bodenfeutigkeit
    uint16_t data = bodenFeutigikeit();

    uint32_t i = 0;
    // reserve some space in memory
    memset(m_lora_app_data.buffer, 0, LORAWAN_APP_DATA_BUFF_SIZE);
    m_lora_app_data.port = gAppPort;

    // SET PAYLOAD HERE
    m_lora_app_data.buffer[i++] = data;
    m_lora_app_data.buffsize = i;

    //fehlerbehandlung 
    lmh_error_status error = lmh_send(&m_lora_app_data, g_CurrentConfirm);
    if (error == LMH_SUCCESS)
    {
        count++;
        Serial.printf("lmh_send ok count %d\n", count);
    }
    else
    {
        count_fail++;
        Serial.printf("lmh_send fail count %d\n", count_fail);
    }
}

