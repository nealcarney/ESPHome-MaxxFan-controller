"""MaxxFan controller component for ESPHome."""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

# Define the component namespace
maxxfan_ns = cg.esphome_ns.namespace("maxxfan")

# Define component variables
CONF_GPIO_UP_PIN = "gpio_up_pin"
CONF_GPIO_DOWN_PIN = "gpio_down_pin"
CONF_GPIO_POWER_PIN = "gpio_power_pin"
CONF_GPIO_DIRECTION_PIN = "gpio_direction_pin"
CONF_GPIO_AUTO_PIN = "gpio_auto_pin"
CONF_ACTUAL_FAN_SPEED = "actual_fan_speed"
CONF_ACTUAL_FAN_POWER = "actual_fan_power"
CONF_ACTUAL_FAN_DIRECTION = "actual_fan_direction"
CONF_ACTUAL_FAN_COVER = "actual_fan_cover"
CONF_IS_BOOT_FINISHED = "is_boot_finished"

# Define component configuration schema
CONFIG_SCHEMA = cv.Schema({})

# Setup global variables for the component
gpio_up_pin = maxxfan_ns.declare_variable(cg.int_, "gpio_up_pin")
gpio_down_pin = maxxfan_ns.declare_variable(cg.int_, "gpio_down_pin")
gpio_power_pin = maxxfan_ns.declare_variable(cg.int_, "gpio_power_pin")
gpio_direction_pin = maxxfan_ns.declare_variable(cg.int_, "gpio_direction_pin")
gpio_auto_pin = maxxfan_ns.declare_variable(cg.int_, "gpio_auto_pin")
actual_fan_speed = maxxfan_ns.declare_variable(cg.int_, "actual_fan_speed")
actual_fan_power = maxxfan_ns.declare_variable(cg.bool_, "actual_fan_power")
actual_fan_direction = maxxfan_ns.declare_variable(cg.bool_, "actual_fan_direction")
actual_fan_cover = maxxfan_ns.declare_variable(cg.bool_, "actual_fan_cover")
is_boot_finished = maxxfan_ns.declare_variable(cg.bool_, "is_boot_finished")

# Define async_setup function (will be called by ESPHome)
async def async_setup(config):
    # Get references to global variables from YAML
    var1 = await cg.get_variable(cg.global_ns.namespace("gpio_up_pin"))
    var2 = await cg.get_variable(cg.global_ns.namespace("gpio_down_pin"))
    var3 = await cg.get_variable(cg.global_ns.namespace("gpio_power_pin"))
    var4 = await cg.get_variable(cg.global_ns.namespace("gpio_direction_pin"))
    var5 = await cg.get_variable(cg.global_ns.namespace("gpio_auto_pin"))
    var6 = await cg.get_variable(cg.global_ns.namespace("actual_fan_speed"))
    var7 = await cg.get_variable(cg.global_ns.namespace("actual_fan_power"))
    var8 = await cg.get_variable(cg.global_ns.namespace("actual_fan_direction"))
    var9 = await cg.get_variable(cg.global_ns.namespace("actual_fan_cover"))
    var10 = await cg.get_variable(cg.global_ns.namespace("is_boot_finished"))
    
    # Link YAML globals to component globals
    cg.add(gpio_up_pin.set(var1))
    cg.add(gpio_down_pin.set(var2))
    cg.add(gpio_power_pin.set(var3))
    cg.add(gpio_direction_pin.set(var4))
    cg.add(gpio_auto_pin.set(var5))
    cg.add(actual_fan_speed.set(var6))
    cg.add(actual_fan_power.set(var7))
    cg.add(actual_fan_direction.set(var8))
    cg.add(actual_fan_cover.set(var9))
    cg.add(is_boot_finished.set(var10))
    
    return True