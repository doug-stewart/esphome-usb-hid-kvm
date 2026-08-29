#pragma once
#include "esphome/core/component.h"

namespace esphome {
namespace usb_hid_kvm {

class UsbHidKvm : public Component {
 public:
  void setup() override;
  void send_kvm_switch(int port_num);

 protected:
  bool wait_ready_(uint32_t timeout_ms);
  bool send_report_(uint8_t modifier, uint8_t keycode);
  bool release_();
};

}  // namespace usb_hid_kvm
}  // namespace esphome
