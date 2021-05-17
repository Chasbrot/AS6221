#include "Arduino.h"
// Register Map
#define AS6221_REG_TVAL 0x0
#define AS6221_REG_CONFIG 0x1
#define AS6221_REG_TLOW 0x2
#define AS6221_REG_THIGH 0x3

// Configuration constants
#define AS6221_DEFAULT_CONFIG 0x40A0
#define AS6221_DEFAULT_ADDRESS 0x48
#define AS6221_T_ERR 0.1
#define AS6221_SINGLE_SHOT 0x8000

typedef enum
{
  AS6221_CONV_RATE025 = 0x0000,
  AS6221_CONV_RATE1   = 0x0040,
  AS6221_CONV_RATE4   = 0x0080,
  AS6221_CONV_RATE8   = 0x00C0,
} as6221_conv_rate_t;

typedef enum
{
  AS6221_STATE_SLEEP = 0x0100,
  AS6221_STATE_ACTIVE = 0x0000,
} as6221_state_t;

typedef enum
{
  AS6221_CONSEC_FAULTS1 = 0x0000,
  AS6221_CONSEC_FAULTS2 = 0x1000,
  AS6221_CONSEC_FAULTS3 = 0x2000,
  AS6221_CONSEC_FAULTS4 = 0x3000,
} as6221_consec_faults_t;

typedef enum
{
  AS6221_ALERT_INTERRUPT = 0x0200,
  AS6221_ALERT_COMPARATOR = 0x0000,
} as6221_alert_mode_t;

typedef enum
{
  AS6221_ALERT_ACTIVE_LOW = 0x0000,
  AS6221_ALERT_ACTIVE_HIGH = 0x0400,
} as6221_alert_polarity_t;

struct as6221_config_t
{
  as6221_conv_rate_t cr;
  as6221_state_t state;
  as6221_alert_mode_t alert_mode;
  as6221_alert_polarity_t alert_polarity;
  as6221_consec_faults_t cf;
  bool singleShot;
};

class AS6221 {
  public:
    double getTemp();
    void setAlertLimits(uint16_t low, uint16_t high);
    void sleepMode();
    void ccMode();
    void triggerSingleShot();
    void init(uint8_t address, as6221_config_t conf);
    void updateDevice(as6221_config_t conf);
  private:
    void writeData(uint8_t* data, uint8_t len);
    void readData(uint8_t* rec, uint8_t len);
};
