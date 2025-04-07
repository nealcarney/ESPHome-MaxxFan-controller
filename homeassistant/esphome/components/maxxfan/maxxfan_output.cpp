#include "maxxfan_output.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/components/switch/switch.h"
#include "Arduino.h"  // Add this for pinMode, digitalWrite, HIGH, LOW, etc.

namespace esphome {
namespace maxxfan {

static const char *TAG = "maxxfan";

// Define global variables
int gpio_up_pin = 0;
int gpio_down_pin = 0;
int gpio_power_pin = 0;
int gpio_direction_pin = 0;
int gpio_auto_pin = 0;
int actual_fan_speed = 4;
bool actual_fan_power = false;
bool actual_fan_direction = true;
bool actual_fan_cover = false;
bool is_boot_finished = false;

// Define external access to the switch entity for publishing state
namespace {
  switch_::Switch *maxxfan_cover = nullptr;
}

void SpeedOutput::setup() {
  pinMode(gpio_up_pin, OUTPUT);
  pinMode(gpio_down_pin, OUTPUT);
  ESP_LOGD(TAG, "SpeedOutput setup complete");
}

void SpeedOutput::write_state(float state) {
  // state is a floating point number 0.0 to 1.0
  // convert it to an integer from 0 to 10
  int FanSet = state * 10;
  // Don't do anything if still booting, or if request already matches actual,
  // or if power is turned off, or if UI is setting 0 speed (which means off).
  if (FanSet == 0 || FanSet == actual_fan_speed || actual_fan_power == 0 || !is_boot_finished)
    return; // Do nothing
  else if (FanSet > actual_fan_speed) {
    while ((FanSet > actual_fan_speed) && (actual_fan_speed < 10)) {   // Set the fan higher
      digitalWrite(gpio_up_pin, HIGH);
      delay(100);
      digitalWrite(gpio_up_pin, LOW);
      delay(100);
      ++actual_fan_speed; // Increment tracker for actual speed
      ESP_LOGD(TAG, "Speed changed to: %d", actual_fan_speed);
    }
  } else if (FanSet < actual_fan_speed) {  
    while ((FanSet < actual_fan_speed) && (actual_fan_speed > 1)) {   // Set the fan lower
      digitalWrite(gpio_down_pin, HIGH);
      delay(100);
      digitalWrite(gpio_down_pin, LOW);
      delay(100);
      --actual_fan_speed; // Decrement tracker for actual speed
      ESP_LOGD(TAG, "Speed changed to: %d", actual_fan_speed);
    }
  }
}

void PowerOutput::setup() {
  pinMode(gpio_power_pin, OUTPUT);
  ESP_LOGD(TAG, "PowerOutput setup complete");
}

void PowerOutput::write_state(bool FanSet) {
  // Don't do anything if still booting or if request already matches actual 
  if (FanSet != actual_fan_power && is_boot_finished) {
    digitalWrite(gpio_power_pin, HIGH);
    delay(100);
    digitalWrite(gpio_power_pin, LOW);
    actual_fan_power = FanSet;
    actual_fan_cover = FanSet; // Maxxfan automatically changes cover on power action
    
    // Access the switch entity from the yaml configuration to publish state
    for (auto *sw : App.get_switches()) {
      if (sw->get_name() == "Cover") {
        maxxfan_cover = sw;
        break;
      }
    }
    
    if (maxxfan_cover != nullptr) {
      maxxfan_cover->publish_state(FanSet);  // Update front end
    }
    
    ESP_LOGD(TAG, "Power changed to: %s", actual_fan_power ? "On" : "Off");
    ESP_LOGD(TAG, "Cover changed to: %s", actual_fan_cover ? "Open" : "Close");
  }
}

void DirectionOutput::setup() {
  pinMode(gpio_direction_pin, OUTPUT);
  ESP_LOGD(TAG, "DirectionOutput setup complete");
}

void DirectionOutput::write_state(bool FanSet) {
  // Don't do anything if still booting, or if request already matches actual
  // or if power is turned off  
  if ((FanSet != actual_fan_direction) && (actual_fan_power != 0) && is_boot_finished) {
    digitalWrite(gpio_direction_pin, HIGH);
    delay(100);
    digitalWrite(gpio_direction_pin, LOW);
    actual_fan_direction = FanSet;
    ESP_LOGD(TAG, "Direction changed to: %s", actual_fan_direction ? "Reverse" : "Forward");
  }
}

void CoverOutput::setup() {
  pinMode(gpio_up_pin, OUTPUT);
  pinMode(gpio_down_pin, OUTPUT);
  
  // Find and store a reference to the maxxfan_cover switch entity
  for (auto *sw : App.get_switches()) {
    if (sw->get_name() == "Cover") {
      maxxfan_cover = sw;
      break;
    }
  }
  
  ESP_LOGD(TAG, "CoverOutput setup complete");
}

void CoverOutput::write_state(bool FanSet) {
  // Don't do anything if still booting or if request already matches actual 
  if (FanSet != actual_fan_cover && is_boot_finished) {
    // Need to simultaneously press the up and down buttons
    digitalWrite(gpio_up_pin, HIGH);
    digitalWrite(gpio_down_pin, HIGH);
    delay(200);
    digitalWrite(gpio_up_pin, LOW);
    digitalWrite(gpio_down_pin, LOW);
    actual_fan_cover = FanSet;
    ESP_LOGD(TAG, "Cover changed to: %s", actual_fan_cover ? "Open" : "Close");
  }
}

}  // namespace maxxfan
}  // namespace esphome