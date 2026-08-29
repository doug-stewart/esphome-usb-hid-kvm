#include "usb_hid_kvm.h"
#include "tinyusb.h"
#include "tinyusb_default_config.h"
#include "class/hid/hid_device.h"
#include "esphome/core/log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace esphome {
namespace usb_hid_kvm {

static const char *const TAG = "usb_hid_kvm";

static const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD()
};

#define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

static const uint8_t hid_configuration_descriptor[] = {
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, 0, 100),
    TUD_HID_DESCRIPTOR(0, 0, HID_ITF_PROTOCOL_KEYBOARD,
                        sizeof(hid_report_descriptor), 0x81, 16, 10),
};

static const tusb_desc_device_t hid_device_descriptor = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x303A,
    .idProduct = 0x4002,
    .bcdDevice = 0x0100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01,
};

static const char *string_desc_arr[] = {
    (const char[]){0x09, 0x04},  // English langID
    "DIY",
    "KVM Macro Keypad",
    "123456",
};

void UsbHidKvm::setup() {
  ESP_LOGI(TAG, "Installing TinyUSB HID keyboard stack");
  tinyusb_config_t tusb_cfg = TINYUSB_DEFAULT_CONFIG();
  tusb_cfg.descriptor.device = &hid_device_descriptor;
  tusb_cfg.descriptor.string = string_desc_arr;
  tusb_cfg.descriptor.string_count = sizeof(string_desc_arr) / sizeof(string_desc_arr[0]);
  tusb_cfg.descriptor.full_speed_config = hid_configuration_descriptor;
  ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
}

// ... rest of the file (send_report_, release_, send_kvm_switch, callbacks) unchanged
