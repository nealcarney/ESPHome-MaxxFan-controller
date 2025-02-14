/*
 * Custom output components for controlling a Maxxfan through ESPHome
 * 
 * This code simulates button presses on the Maxxfan's control panel by briefly pulsing GPIO pins
 * that are connected to relays. The relays are wired in parallel with the physical buttons.
 * 
 * Pin Logic:
 * - All pins are active HIGH (HIGH = button pressed, LOW = button released)
 * - Each button press is simulated by a brief HIGH pulse (100-200ms) followed by LOW
 * - For cover operation, UP and DOWN pins are pulsed simultaneously
 * 
 * Components:
 * - SpeedOutput: Controls fan speed (1-10) using UP/DOWN pins
 * - PowerOutput: Controls fan power using POWER pin
 * - DirectionOutput: Controls air flow direction using DIRECTION pin
 * - CoverOutput: Controls lid position using simultaneous UP/DOWN pins
 * 
 * State Management:
 * - Each component maintains its state in global variables (actual_fan_*)
 * - Components only act when requested state differs from actual state
 * - All operations are blocked until boot is finished (is_boot_finished)
 */

#include "esphome.h"
using namespace esphome;

class SpeedOutput : public Component, public FloatOutput {
public:
    void setup() override {
    // This will be called by App.setup()
        pinMode(gpio_up_pin->value(), OUTPUT);
        pinMode(gpio_down_pin->value(), OUTPUT);
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
                digitalWrite(gpio_up_pin->value(), HIGH);
                delay(100);
                digitalWrite(gpio_up_pin->value(), LOW);
                delay(100);
                ++FanSpeed; //Increment tracker for actual speed
                ESP_LOGD("custom_outputs", "Speed changed to: %d", FanSpeed);};
        }
        else if (FanSet < FanSpeed){  
            while ((FanSet < FanSpeed) && (FanSpeed > 1)){   // Set the fan lower
                digitalWrite(gpio_down_pin->value(), HIGH);
                delay(100);
                digitalWrite(gpio_down_pin->value(), LOW);
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
        pinMode(gpio_power_pin->value(), OUTPUT);
    }
    void write_state(bool FanSet) override {
        bool& FanPower = actual_fan_power->value();
        // Don't do anything if still booting or if request already matches actual 
        if (FanSet != FanPower && is_boot_finished->value()){
            digitalWrite(gpio_power_pin->value(), HIGH);
            delay(100);
            digitalWrite(gpio_power_pin->value(), LOW);
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
        pinMode(gpio_direction_pin->value(), OUTPUT);
    }
    void write_state(bool FanSet) override {
        bool& FanDirection = actual_fan_direction->value();
        // Don't do anything if still booting, or if request already matches actual
        // or if power is turned off  
        if ((FanSet != FanDirection) && (actual_fan_power->value() != 0) && is_boot_finished->value()){
            digitalWrite(gpio_direction_pin->value(), HIGH);
            delay(100);
            digitalWrite(gpio_direction_pin->value(), LOW);
            FanDirection = FanSet;
            ESP_LOGD("custom_outputs", ("Direction changed to: " + std::string((FanDirection ? "Reverse" : "Forward"))).c_str());
        };
    }
};

class CoverOutput : public Component, public BinaryOutput {
public:
    void setup() override {
    // This will be called by App.setup()
        pinMode(gpio_up_pin->value(), OUTPUT);
        pinMode(gpio_down_pin->value(), OUTPUT);
    }
    void write_state(bool FanSet) override {
        bool& FanCover = actual_fan_cover->value();
        // Don't do anything if still booting or if request already matches actual 
        if (FanSet != FanCover && is_boot_finished->value()){
            //Need to simultaneously press the up and down buttons
            digitalWrite(gpio_up_pin->value(), HIGH);
            digitalWrite(gpio_down_pin->value(), HIGH);
            delay(200);
            digitalWrite(gpio_up_pin->value(), LOW);
            digitalWrite(gpio_down_pin->value(), LOW);
            FanCover = FanSet;
            ESP_LOGD("custom_outputs", ("Cover changed to: " + std::string((FanCover ? "Open" : "Close"))).c_str());
        };
    }
};
