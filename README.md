# ESPHome Build USB HID KVM Impelemntation

> [!CAUTION]
> This code was written entirely by Claude, not me. It works as-is but I don't have the experience or expertise to know if this is the best way to do it, if I'm opening myself up to any additional security risks, etc. If it breaks _for me_ I will be fixing it but this is provided as-is with no support promised. **Use your own judgement.**

## How To Install:

In ESPHome Builder (verified on 2026.8.1), select your hardware and create a new project with this code:

```yaml
esphome:
  name: kvm-macro-keypad
  friendly_name: KVM Macro Keypad

esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: esp-idf

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

api:
  encryption:
    key: !secret api_encryption_key
  actions:
    - action: switch_kvm_port
      variables:
        port_num: int
      then:
        - lambda: "id(kvm_hid).send_kvm_switch(port_num);"

ota:
  - platform: esphome
    password: !secret ota_password

logger:
  hardware_uart: UART0

external_components:
  - source: github://doug-stewart/esphome-usb-hid-kvm
    components: [usb_hid_kvm]
    refresh: always

usb_hid_kvm:
  id: kvm_hid
```
