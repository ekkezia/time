# LED Matrix — Pin Connections

2-layer LED matrix using common cathode multiplexing.

## Wiring

| Pin | Role | Notes |
|-----|------|-------|
| 11  | Cathode — Layer 0 (GND) | Layer enable: OUTPUT = grounded/on, INPUT = floating/off |
| 12  | Cathode — Layer 1 (GND) | Same as above |
| 4   | Anode — Row 0 | Digital HIGH/LOW |
| 5   | Anode — Row 1 | PWM-capable (used in fade sketch) |
| 6   | Anode — Row 2 | PWM-capable (used in fade sketch) |
| 7   | Anode — Row 3 | Digital HIGH/LOW |
| 9   | Anode (fade only) | PWM |
| 10  | Anode (fade only) | PWM |
| 2   | Interrupt input | INPUT_PULLUP — connect button between pin 2 and GND |

## Sketches

- **led_matrix_interrupt** — button on pin 2 triggers an interrupt that toggles between Layer 0 and Layer 1
- **led_matrix_fade** — fades all LEDs in/out over 10 seconds using PWM (ping-pong effect)
- **interrupt_test** — bare interrupt test wired to the same pin 2 setup
