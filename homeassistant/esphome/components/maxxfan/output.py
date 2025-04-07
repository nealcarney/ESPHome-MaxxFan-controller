"""MaxxFan output component for ESPHome."""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID, CONF_TYPE

from . import maxxfan_ns

# Define the output classes
SpeedOutput = maxxfan_ns.class_("SpeedOutput", cg.Component, output.FloatOutput)
PowerOutput = maxxfan_ns.class_("PowerOutput", cg.Component, output.BinaryOutput)
DirectionOutput = maxxfan_ns.class_("DirectionOutput", cg.Component, output.BinaryOutput)
CoverOutput = maxxfan_ns.class_("CoverOutput", cg.Component, output.BinaryOutput)

# Configuration schema for each output type
SPEED_OUTPUT_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(SpeedOutput),
}).extend(cv.COMPONENT_SCHEMA)

POWER_OUTPUT_SCHEMA = output.BINARY_OUTPUT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(PowerOutput),
}).extend(cv.COMPONENT_SCHEMA)

DIRECTION_OUTPUT_SCHEMA = output.BINARY_OUTPUT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(DirectionOutput),
}).extend(cv.COMPONENT_SCHEMA)

COVER_OUTPUT_SCHEMA = output.BINARY_OUTPUT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(CoverOutput),
}).extend(cv.COMPONENT_SCHEMA)

# Register the output platforms
CONFIG_SCHEMA = cv.Schema({
    cv.Optional("speed"): SPEED_OUTPUT_SCHEMA,
    cv.Optional("power"): POWER_OUTPUT_SCHEMA,
    cv.Optional("direction"): DIRECTION_OUTPUT_SCHEMA,
    cv.Optional("cover"): COVER_OUTPUT_SCHEMA,
})

async def to_code(config):
    if "speed" in config:
        var = cg.new_Pvariable(config["speed"][CONF_ID])
        await cg.register_component(var, config["speed"])
        await output.register_output(var, config["speed"])

    if "power" in config:
        var = cg.new_Pvariable(config["power"][CONF_ID])
        await cg.register_component(var, config["power"])
        await output.register_output(var, config["power"])

    if "direction" in config:
        var = cg.new_Pvariable(config["direction"][CONF_ID])
        await cg.register_component(var, config["direction"])
        await output.register_output(var, config["direction"])

    if "cover" in config:
        var = cg.new_Pvariable(config["cover"][CONF_ID])
        await cg.register_component(var, config["cover"])
        await output.register_output(var, config["cover"])