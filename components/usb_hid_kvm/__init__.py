import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.esp32 import add_idf_sdkconfig_option, only_on_variant
from esphome.const import CONF_ID

usb_hid_kvm_ns = cg.esphome_ns.namespace("usb_hid_kvm")
UsbHidKvm = usb_hid_kvm_ns.class_("UsbHidKvm", cg.Component)

CONFIG_SCHEMA = cv.All(
    cv.Schema({
        cv.GenerateID(): cv.declare_id(UsbHidKvm),
    }).extend(cv.COMPONENT_SCHEMA),
    only_on_variant(supported=["ESP32S3"]),
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    add_idf_sdkconfig_option("CONFIG_TINYUSB_HID_COUNT", 1)
