#include "usb_hid_kvm.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "esphome/core/log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace esphome {
namespace usb_hid_kvm {

static const char *const TAG = "usb_hid_kvm";

// --- HID report descriptor: single boot-keyboard interface ---
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
    (const char[]){0x09, 0x04},  // English
    "DIY",
    "KVM Macro Keypad",
    "123456",
};

void UsbHidKvm::setup() {
  ESP_LOGI(TAG, "Installing raw TinyUSB HID keyboard stack");
  tinyusb_config_t tusb_cfg = {
      .device_descriptor = &hid_device_descriptor,
      .string_descriptor = string_desc_arr,
      .string_descriptor_count = sizeof(string_desc_arr) / sizeof(string_desc_arr[0]),
      .external_phy = false,
      .configuration_descriptor = hid_configuration_descriptor,
  };
  ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
}

void UsbHidKvm::send_report_(uint8_t modifier, uint8_t keycode) {
  uint8_t keycode_arr[6] = {keycode, 0, 0, 0, 0, 0};
  tud_hid_keyboard_report(0, modifier, keycode_arr);
}

void UsbHidKvm::release_() {
  uint8_t empty[6] = {0};
  tud_hid_keyboard_report(0, 0, empty);
}

void UsbHidKvm::send_kvm_switch(int port_num) {
  const uint8_t number_keycode = 0x1D + port_num;  // 1 -> 0x1E, 2 -> 0x1F

  // Tap Left Ctrl twice
  for (int i = 0; i < 2; i++) {
    send_report_(KEYBOARD_MODIFIER_LEFTCTRL, 0);
    delay(50);
    release_();
    delay(150);
  }

  // Tap the port number
  send_report_(0, number_keycode);
  delay(50);
  release_();
}

// Required TinyUSB HID callbacks (stubs — boot keyboard doesn't need real logic here)
extern "C" uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
  return hid_report_descriptor;
}
extern "C" uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                                           hid_report_type_t report_type, uint8_t *buffer,
                                           uint16_t reqlen) {
  return 0;
}
extern "C" void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                                       hid_report_type_t report_type, uint8_t const *buffer,
                                       uint16_t bufsize) {}

}  // namespace usb_hid_kvm
}  // namespace esphome
