# Wiring Diagram - Self-Balancing Robot

## Overview

This document provides detailed wiring instructions for the self-balancing robot.

---

## Pin Configuration

### Arduino UNO Pins Used

```
┌─────────────────────────────────────┐
│        ARDUINO UNO                  │
│  ┌──────────────────────────────┐   │
│  │  Digital Pins (PWM):         │   │
│  │  ├─ Pin 5  (PWM) → ENB       │   │
│  │  ├─ Pin 6  → IN3             │   │
│  │  ├─ Pin 7  → IN4             │   │
│  │  ├─ Pin 8  → IN1             │   │
│  │  ├─ Pin 9  → IN2             │   │
│  │  ├─ Pin 10 (PWM) → ENA       │   │
│  │  ├─ Pin 11 → ECHO            │   │
│  │  └─ Pin 12 → TRIG            │   │
│  │                              │   │
│  │  Analog Pins:                │   │
│  │  ├─ A4 → SDA (MPU6050)       │   │
│  │  └─ A5 → SCL (MPU6050)       │   │
│  │                              │   │
│  │  Power:                      │   │
│  │  ├─ 5V → Sensors             │   │
│  │  └─ GND → Common Ground      │   │
│  └──────────────────────────────┘   │
└─────────────────────────────────────┘
```

---

## Detailed Connections

### 1. MPU6050 (6-Axis IMU)

**Purpose:** Measures tilt angle (accelerometer + gyroscope)

| MPU6050 Pin | Arduino Pin | Color (Recommended) | Function |
|-------------|------------|-------------------|----------|
| VCC         | 5V         | Red | Power (5V) |
| GND         | GND        | Black | Ground |
| SDA         | A4         | Green | I2C Data |
| SCL         | A5         | Yellow | I2C Clock |
| INT         | N/A*       | - | Interrupt (not used) |
| AD0         | GND        | Black | I2C Address Select |

*INT pin is optional in our implementation

**I2C Address:** 0x68 (default, with AD0 connected to GND)

```
Raspberry Pi connection (example):
        Arduino
         ┌─────┐
    VCC ─┼─ 5V │
    GND ─┼─GND │
    SDA ─┼─ A4 │
    SCL ─┼─ A5 │
         └─────┘
         MPU6050
```

---

### 2. L298N Motor Driver

**Purpose:** Controls DC motors with H-bridge logic

| L298N Pin | Arduino Pin | Motor | Function |
|-----------|------------|-------|----------|
| ENA | Pin 10 (PWM) | Left | Speed control PWM |
| IN1 | Pin 8 | Left | Direction bit 1 |
| IN2 | Pin 9 | Left | Direction bit 2 |
| IN3 | Pin 6 | Right | Direction bit 1 |
| IN4 | Pin 7 | Direction bit 2 |
| ENB | Pin 5 (PWM) | Right | Speed control PWM |

**Motor Driver Outputs:**

| OUT1 | OUT2 | OUT3 | OUT4 | Direction |
|------|------|------|------|-----------|
| +    | -    | +    | -    | Forward |
| -    | +    | -    | +    | Backward |
| 0    | 0    | 0    | 0    | Stop |

**Power Connections:**

| L298N Pin | Connection | Voltage |
|-----------|-----------|---------|
| +12V | Battery + | 7.4V - 12V |
| GND | Battery - | |
| GND | Arduino GND | **MUST SHARE!** |
| 5V | (optional) | Can power Arduino |

⚠️ **CRITICAL:** L298N GND **MUST** be connected to Arduino GND for proper operation!

**Jumper Settings:**

```
L298N Module:
┌─────────────────────┐
│ ┌─────┐  ┌─────┐  │
│ │ ENA │  │ ENB │  │ ← REMOVE these jumpers!
│ │ [X] │  │ [X] │  │   for PWM control
│ └─────┘  └─────┘  │
└─────────────────────┘

Correct Setting:
┌─────────────────────┐
│ ┌─────┐  ┌─────┐  │
│ │ ENA │  │ ENB │  │ ← Jumpers REMOVED
│ │ [ ] │  │ [ ] │  │   for PWM control
│ └─────┘  └─────┘  │
└─────────────────────┘
```

---

### 3. DC Motors

**Specifications:**
- Type: DC Geared
- Voltage: 3-12V
- Gear Ratio: 1:30 (recommended)
- Speed: ~200-300 RPM at 6V

**Connections:**

| Motor | L298N Output | Connection |
|-------|-------------|-----------|
| Left | OUT1, OUT2 | Motor wires (polarity doesn't matter) |
| Right | OUT3, OUT4 | Motor wires (polarity doesn't matter) |

**Motor Polarity:** If motor spins backward, swap its two wires at the L298N.

```
L298N:              Motors:
  OUT1 ─────────── Motor Left +
  OUT2 ─────────── Motor Left -
  OUT3 ─────────── Motor Right +
  OUT4 ─────────── Motor Right -
```

---

### 4. Battery Power Supply

**Specifications:**
- Voltage: 7.4V - 12V
- Type: LiPo, Li-ion, or NiMH recommended
- Capacity: 1000mAh minimum
- Peak Current: 5A+ (for two motors)

**Connections:**

```
Battery (7.4V-12V)
    │
    ├─ POSITIVE (Red) ──→ L298N +12V Terminal
    │
    └─ NEGATIVE (Black) ──→ ┌─────────────────┐
                            │ L298N GND        │
                            │ Arduino GND      │
                            │ Sensors GND      │
                            └─────────────────┘
```

**Safety Features:**
- Add 1000µF capacitor across battery (+-  terminals)
- Add 100µF capacitor across Arduino power
- Add 100nF capacitor across each motor terminal
- Optional: Add 5A fuse in battery positive line

---

### 5. HC-SR04 Ultrasonic Sensor (Optional)

**Purpose:** Obstacle detection

| HC-SR04 Pin | Arduino Pin | Function |
|------------|------------|----------|
| VCC | 5V | Power |
| GND | GND | Ground |
| TRIG | Pin 12 | Trigger pulse |
| ECHO | Pin 11 | Echo response |

**Note:** Only connect if `ENABLE_ULTRASONIC true` in code.

```
HC-SR04:            Arduino:
  VCC ────────────── 5V
  GND ────────────── GND
  TRIG ───────────── Pin 12
  ECHO ───────────── Pin 11
```

---

## Complete Wiring Diagram

```
                    ┌──────────────────┐
                    │   BATTERY 7.4V   │
                    │    (LiPo 2S)     │
                    └────────┬─────────┘
                             │
                    ┌────────┴────────┐
                    │                 │
                    │                 │
          ┌─────────▼────┐    ┌──────▼────┐
          │ RED(+) →    │    │  BLACK(-) │
          │             │    │           │
          │   L298N     │    │   ┌───────┴─────────┐
          │  +12V  GND  │    │   │                 │
          │   │     │   │    │   │                 │
          └───┼─────┼───┘    │   │                 │
              │     │         │   │                 │
              │     └─────────┼───┴──┐              │
              │               │      │              │
          ┌───▼───────────────┴────────▼────────┐  │
          │        L298N Motor Driver            │  │
          │                                      │  │
          │  EN  IN  IN  IN  IN  EN             │  │
          │  A   1   2   3   4   B              │  │
          │  10  8   9   6   7   5              │  │
          │  │   │   │   │   │   │              │  │
          └──┼───┼───┼───┼───┼───┼──────────────┘  │
             │   │   │   │   │   │                 │
          ┌──┼───┼───┼───┼───┼───┼──────────┐     │
          │  │   │   │   │   │   │          │     │
          │  │   │   │   │   │   │          │     │
      ┌───────────────────┐  │   │     ┌────▼───┐ │
      │   ARDUINO UNO     │  │   │     │  5V    │ │
      │                   │  │   │     │  GND   │ │
      │  A5 A4 GND  5V   │  │   │     │        │ │
      │  │  │  │    │    │  │   │     └────┬───┘ │
      │  │  │  │    │    │  │   │          │     │
      │  └──────────┼────────┼───┼──────────┼─────┘
      │             │    │   │   │   │     │
      │  10 8 9 6 7 5   │   │   │   │     │
      │  │  │ │ │ │ │   │   │   │   │     │
      │  EN1IN2 IN3IN4EN2│   │   │   │     │
      │  │  │ │ │ │ │   │   │   │   │     │
      └──┼──┼─┼─┼─┼─┼───┼───┼───┼───┼─────┘
         │  │ │ │ │ │   │   │   │   │
         │  └─┘ │ └─┘   │   │   │   │
         │      │       │   │   │   │
         │   ┌──┘       │   │   │   │
         │   │          │   │   │   │
         │   ▼          │   │   │   │
      ┌─────────────┐   │   │   │   │
      │  MPU6050    │   │   │   │   │
      │   (SDA/SCL) │   │   │   │   │
      └─────────────┘   │   │   │   │
                        │   │   │   │
                    ┌───┘   │   │   └─ OUT3/OUT4 to Right Motor
                    │       │   │
                    │       └─ OUT1/OUT2 to Left Motor
                    │
         (Optional) HC-SR04 on pins 11/12
```

---

## Pin Usage Summary

| Arduino Pin | Component | Direction |
|------------|-----------|-----------|
| 5 | L298N ENB | Output (PWM) |
| 6 | L298N IN3 | Output |
| 7 | L298N IN4 | Output |
| 8 | L298N IN1 | Output |
| 9 | L298N IN2 | Output |
| 10 | L298N ENA | Output (PWM) |
| 11 | HC-SR04 ECHO | Input (optional) |
| 12 | HC-SR04 TRIG | Output (optional) |
| A4 | MPU6050 SDA | I2C Data |
| A5 | MPU6050 SCL | I2C Clock |
| 5V | Power for sensors | Output |
| GND | Common ground | - |

---

## Testing Connections

### Step 1: Visual Inspection
- [ ] All wires firmly connected
- [ ] No loose connections
- [ ] Correct pin numbers
- [ ] Color codes correct

### Step 2: Continuity Testing (Multimeter)
- [ ] Battery to L298N +12V
- [ ] GND connected everywhere (battery, L298N, Arduino)
- [ ] Motor wires connected to L298N outputs
- [ ] I2C wires SDA/SCL properly connected

### Step 3: Power-On Test
- [ ] Arduino powers on (LED blinks)
- [ ] No smoke or burning smell
- [ ] Motors don't immediately spin
- [ ] No unusual noises

### Step 4: Serial Monitor Test
- [ ] Upload test sketch
- [ ] Open Serial Monitor (115200 baud)
- [ ] Should see initialization messages
- [ ] No error messages

---

## Common Wiring Issues

### Problem: Arduino won't power on
- ❌ Check battery voltage (must be >7V)
- ❌ Check USB connection
- ❌ Verify power polarity

### Problem: MPU6050 not detected
- ❌ Check SDA/SCL connections (not swapped)
- ❌ Verify 5V to VCC
- ❌ Check for loose connections
- ❌ Try 4.7kΩ pull-up resistors on SDA/SCL

### Problem: Motors don't move
- ❌ Check battery voltage
- ❌ Verify L298N jumpers are REMOVED
- ❌ Check motor connections to L298N
- ❌ Test motors directly on battery

### Problem: One motor works, other doesn't
- ❌ Loose wire on non-working motor
- ❌ Motor might be defective
- ❌ L298N channel might be bad

### Problem: Motors spin wrong direction
- ❌ Swap OUT1↔OUT2 or OUT3↔OUT4
- ❌ Or change code logic in driveMotors()

---

## Advanced Additions (Optional)

### Battery Voltage Monitoring
```
Battery +12V ─┬─ 10kΩ ─┬─ A0 ─ Arduino
              │         │
              └─ 4.7kΩ ─┴─ GND
```

### Power Decoupling Capacitors
```
Battery +12V ─┬─ 1000µF ─┬─ GND
              └──────────┘

Arduino 5V ─┬─ 100µF ─┬─ GND
            └────────┘

Each Motor ─┬─ 100nF ─┬─ GND
            └────────┘
```

### Emergency Stop Switch
```
Battery + ─ [SWITCH] ─ L298N +12V
```

---

## Mechanical Assembly

1. Mount motors on chassis (secure with brackets)
2. Attach wheels to motor shafts
3. Mount MPU6050 flat on top (perpendicular to ground)
4. Mount L298N on chassis
5. Secure Arduino UNO (avoid vibration)
6. Mount battery low (center of gravity)
7. Route wires neatly, avoid pinching

---

## Final Checklist

Before uploading code:
- [ ] All connections verified
- [ ] No loose wires
- [ ] Battery charged and connected
- [ ] L298N jumpers removed (ENA, ENB)
- [ ] Ground connections shared
- [ ] MPU6050 powered and connected
- [ ] Motors can spin freely
- [ ] No mechanical binding

---

## Reference

- [Arduino UNO Pinout](https://www.arduino.cc/en/Hacking/PinMapping168)
- [MPU6050 Datasheet](https://invensense.tdk.com/products/motion-tracking/6-axis/)
- [L298N Datasheet](https://www.alldatasheet.com/datasheet-pdf/pdf/14223/STMICROELECTRONICS/L298N.html)
- [Arduino PWM Pins](https://www.arduino.cc/en/Tutorial/PWM)

---

*Last Updated: January 2026*
