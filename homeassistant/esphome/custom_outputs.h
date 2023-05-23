#include "esphome.h"
using namespace esphome;

// constants used to set gpio pin numbers:
const int gpio_down = 26;
const int gpio_up = 23;
const int gpio_power = 33;
const int gpio_direction = 27;
const int gpio_auto = 25; //Not used

class SpeedOutput : public Component, public FloatOutput {
public:
    void setup() override {
    // This will be called by App.setup()
        pinMode(gpio_down, OUTPUT);
        pinMode(gpio_up, OUTPUT);
    }
    void write_state(float state) override {
        // state is a floating point number 0.0 to 1.0
        // convert it to an integer from 0 to 10
        int FanSet = state * 10;
        int& FanSpeed = actual_fan_speed->value();
        // Don't do anything if still booting, or if request already matches actual,
        // or if power is turned off, or if UI is setting 0 speed (which means off).
        if (FanSet == 0 || FanSet == FanSpeed || actual_fan_power->value() == 0 || !is_boot_finished->value());
        //Do nothing
        else if (FanSet > FanSpeed){
            while ((FanSet > FanSpeed) && (FanSpeed < 10)){   // Set the fan higher
                digitalWrite(gpio_up, HIGH); //Close the relay
                delay(100);
                digitalWrite(gpio_up, LOW); //Release the relay
                delay(100);
                ++FanSpeed; //Increment tracker for actual speed
                ESP_LOGD("custom_outputs", "Speed changed to: %d", FanSpeed);};
        }
        else if (FanSet < FanSpeed){  
            while ((FanSet < FanSpeed) && (FanSpeed > 1)){   // Set the fan lower
                digitalWrite(gpio_down, HIGH); //Close the relay
                delay(100);
                digitalWrite(gpio_down, LOW); //Release the relay
                delay(100);
                --FanSpeed; //Decrement tracker for actual speed
                ESP_LOGD("custom_outputs", "Speed changed to: %d", FanSpeed);};
        }
    }
};

class PowerOutput : public Component, public BinaryOutput {
public:
    void setup() override {
    // This will be called by App.setup()
        pinMode(gpio_power, OUTPUT);
    }
    void write_state(bool FanSet) override {
        bool& FanPower = actual_fan_power->value();
        // Don't do anything if still booting or if request already matches actual 
        if (FanSet != FanPower && is_boot_finished->value()){
            digitalWrite(gpio_power, HIGH); //Close the relay
            delay(100);
            digitalWrite(gpio_power, LOW); //Release the relay
            FanPower = FanSet;
            actual_fan_cover->value() = FanPower; //Maxxfan automatically changes cover on power action
            maxxfan_cover->publish_state(FanPower);  //Update front end
            ESP_LOGD("custom_outputs", ("Power changed to: " + std::string((FanPower ? "On" : "Off"))).c_str());
            ESP_LOGD("custom_outputs", ("Cover changed to: " + std::string((actual_fan_cover->value() ? "Open" : "Close"))).c_str());
        };
    }
};

class DirectionOutput : public Component, public BinaryOutput {
public:
    void setup() override {
    // This will be called by App.setup()
        pinMode(gpio_direction, OUTPUT);
    }
    void write_state(bool FanSet) override {
        bool& FanDirection = actual_fan_direction->value();
        // Don't do anything if still booting, or if request already matches actual
        // or if power is turned off  
        if ((FanSet != FanDirection) && (actual_fan_power->value() != 0) && is_boot_finished->value()){
            digitalWrite(gpio_direction, HIGH); //Close the relay
            delay(100);
            digitalWrite(gpio_direction, LOW); //Release the relay
            FanDirection = FanSet;
            ESP_LOGD("custom_outputs", ("Direction changed to: " + std::string((FanDirection ? "Reverse" : "Forward"))).c_str());
        };
    }
};

class CoverOutput : public Component, public BinaryOutput {
public:
    void setup() override {
    // This will be called by App.setup()
        pinMode(gpio_up, OUTPUT);
        pinMode(gpio_down, OUTPUT);
    }
    void write_state(bool FanSet) override {
        bool& FanCover = actual_fan_cover->value();
        // Don't do anything if still booting or if request already matches actual 
        if (FanSet != FanCover && is_boot_finished->value()){
            //Need to simultaneously press the up and down buttons
            digitalWrite(gpio_up, HIGH); //Close the up relay
            digitalWrite(gpio_down, HIGH); //Close the down relay
            delay(200);
            digitalWrite(gpio_up, LOW); //Release the up relay
            digitalWrite(gpio_down, LOW); //Release the down relay
            FanCover = FanSet;
            ESP_LOGD("custom_outputs", ("Cover changed to: " + std::string((FanCover ? "Open" : "Close"))).c_str());
        };
    }
};
