#include <stdbool.h>
#include "nrf_gpio.h"
#include "nrf_drv_twi.h"
#include "bsp.h"
#include "app_error.h"
#include "nrf_drv_config.h"
#include "app_util_platform.h"
#include "nrf_delay.h"

// TWI event handler. (content is visible in GDB under evt structure p_data)
static void twi_event_handler(nrf_drv_twi_evt_t *evt){
     
    // Light up LED 1 if transmit succeeded 
    if(evt->type == NRF_DRV_TWI_TX_DONE)
    {
        nrf_gpio_pin_clear(LED_1);
    }
    // Light up LED 2 if receive succeeded 
    if(evt->type == NRF_DRV_TWI_RX_DONE)
    {
        nrf_gpio_pin_clear(LED_2);
    }
   
}

static void gpio_init()
{
    nrf_gpio_cfg_output(LED_1);
    nrf_gpio_pin_set(LED_1);
    nrf_gpio_cfg_output(LED_2);
    nrf_gpio_pin_set(LED_2);
}


int main(void)
{
    uint8_t tx_data[] = {0x0,0x0,0xFF,0x02,0xFE,0xD4,0x02,0x2A,0x00};  // I believe this is the i2c command sequence for firmware version
    uint8_t rx_data[32] ; 
    gpio_init() ;
    
    ret_code_t ret_code; 

    const nrf_drv_twi_t           p_twi_instance = NRF_DRV_TWI_INSTANCE(1); // Set up TWI instance 1 with default values
    
//    nrf_drv_twi_config_t    p_twi_config = NRF_DRV_TWI_DEFAULT_CONFIG(1);// Set up TWI configuration default values (this is SDA Pin 1, SCL Pin 0)

    nrf_drv_twi_config_t    p_twi_config;
    p_twi_config.scl = 19;
    p_twi_config.sda = 20;
    p_twi_config.frequency = NRF_TWI_FREQ_400K;
    p_twi_config.interrupt_priority = APP_IRQ_PRIORITY_LOW;
    
    ret_code = nrf_drv_twi_init(&p_twi_instance, &p_twi_config, twi_event_handler); // Initiate twi driver with instance and configuration values
    APP_ERROR_CHECK(ret_code); // Check for errors in return value
    
    nrf_drv_twi_enable(&p_twi_instance); // Enable the TWI instance
   
    // transmit firmware version command 0x24 is the address for my PN532 
    ret_code = nrf_drv_twi_tx(&p_twi_instance, 0x24, tx_data, sizeof(tx_data), false); 
    APP_ERROR_CHECK(ret_code); // Check for errors in return value

    nrf_delay_ms(5);  

    // receive firmware version results handler should have the data
    ret_code = nrf_drv_twi_rx(&p_twi_instance, 0x24, rx_data, 12, false); 
    APP_ERROR_CHECK(ret_code); 
     
    nrf_delay_ms(5);  
    
    ret_code = nrf_drv_twi_rx(&p_twi_instance, 0x24, rx_data, 12, false); 
    APP_ERROR_CHECK(ret_code); 
      
    while (true)
    {
        // Do nothing
    }
}
