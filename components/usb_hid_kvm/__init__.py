import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

usb_hid_kvm_ns = cg.esphome_ns.namespace("usb_hid_kvm")
UsbHidKvm = usb_hid_kvm_ns.class_("UsbHidKvm", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(UsbHidKvm),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    # pulls in the raw TinyUSB library (not esp_tinyusb) as an IDF managed component
    cg.add_platformio_option("board_build.embed_txtfiles", [])
