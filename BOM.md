# Bill of Materials (BOM) - Self-Balancing Robot

## Overview

This is a complete list of all components needed to build a two-wheeled self-balancing robot.

**Total Estimated Cost:** ₹2,000 - ₹3,500 USD (depending on where you source)

---

## Core Components

### 1. Microcontroller

| Item | Quantity | Part Number | Estimated Cost | Source |
|------|----------|------------|----------------|--------|
| Arduino UNO R3 | 1 | A000066 | ₹400-600 | Amazon, AliExpress, Local Electronics Store |
| USB Cable (Type B) | 1 | - | ₹100-200 | Any electronics store |

**Alternative:** Arduino Nano (smaller, similar functionality)

---

### 2. Sensors

| Item | Quantity | Part Number | Estimated Cost | Source |
|------|----------|------------|----------------|--------|
| MPU6050 (GY-521) | 1 | GY-521 | ₹300-500 | Amazon, AliExpress |
| HC-SR04 Ultrasonic | 1 | HC-SR04 | ₹200-400 | Amazon, AliExpress *(optional)* |

**Notes:**
- MPU6050 includes accelerometer + gyroscope in one module
- HC-SR04 is optional (for obstacle detection)

---

### 3. Motor Control

| Item | Quantity | Part Number | Estimated Cost | Source |
|------|----------|------------|----------------|--------|
| L298N Motor Driver | 1 | L298N | ₹200-400 | Amazon, AliExpress |
| DC Geared Motor | 2 | 3-6V, 1:30 ratio | ₹300-600 | Robotics stores, AliExpress |

**Motor Specifications:**
- Voltage: 3-12V DC
- Gear Ratio: 1:30 (1:15 to 1:50 also work)
- Speed: ~200-300 RPM at 6V
- Torque: Sufficient for chassis weight
- Shaft: 3-5mm diameter (for wheel attachment)

**Alternatives:**
- Different gear ratio (higher ratio = more torque, slower speed)
- Motors with encoders (for position tracking)

---

### 4. Power Supply

| Item | Quantity | Part Number | Estimated Cost | Source |
|------|----------|------------|----------------|--------|
| 2S LiPo Battery | 1 | 7.4V, 2000mAh+ | ₹600-1000 | Amazon, Hobby shops |
| XT60 Connector | 1 | - | ₹50 | Hobby shops |
| LiPo Charger | 1 | iMAX B6 compatible | ₹300-600 | Amazon, Hobby shops |
| Power Switch | 1 | Toggle or Slide | ₹50-100 | Electronics store |

**Battery Specifications:**
- Voltage: 7.4V (2S LiPo) - 11.1V (3S LiPo)
- Capacity: 1000mAh minimum (2000+ recommended)
- Discharge Rate: 20C+ (for peak current of 2A+)
- Type: LiPo, Li-ion, or NiMH

**Battery Alternatives:**
- 2S LiPo: 7.4V, lighter, better performance
- Li-ion: 7.4V, safer than LiPo
- NiMH: 7.2V, affordable, older technology
- 4x AA Batteries: 6V, simple, lower performance

⚠️ **Safety Note:** LiPo batteries require proper charging and storage!

---

### 5. Wiring & Connectors

| Item | Quantity | Specification | Estimated Cost | Source |
|------|----------|--------------|----------------|--------|
| Jumper Wires (Male-Male) | 20 | 20cm length | ₹100-200 | Electronics store |
| Jumper Wires (Male-Female) | 10 | 20cm length | ₹100-200 | Electronics store |
| Breadboard (optional) | 1 | 400-pin | ₹100-200 | Electronics store |
| Soldering Wire | 1 | Sn60Pb40, 25g | ₹100-150 | Electronics store |

---

### 6. Capacitors (For Noise Filtering)

| Item | Quantity | Spec | Estimated Cost | Source |
|------|----------|------|----------------|--------|
| Electrolytic Capacitor | 2 | 1000µF, 16V | ₹50-100 | Electronics store |
| Electrolytic Capacitor | 1 | 100µF, 16V | ₹25-50 | Electronics store |
| Ceramic Capacitor | 4 | 100nF, 16V | ₹50-100 | Electronics store |

**Purpose:** Reduce electrical noise and voltage spikes

---

## Mechanical Components

### 7. Chassis & Structure

| Item | Quantity | Specification | Estimated Cost | Source |
|------|----------|--------------|----------------|--------|
| Acrylic Sheet (Base) | 1 | 17×14cm, 3mm | ₹100-200 | Craft stores, Online |
| Aluminum Tower | 1 | 6cm × 3cm profile | ₹200-400 | Hardware store |
| Wheels | 2 | 60-100mm diameter | ₹200-400 | Hobby shops |
| Wheel Hubs | 2 | 3-5mm shaft | ₹100-200 | Hobby shops |
| Bearings (optional) | 4 | 608 or 625 | ₹100-200 | Hardware store |
| Servo Motor Brackets | 2 | Metal L-brackets | ₹100-200 | Hobby shops |
| Fasteners | - | Screws, nuts, bolts | ₹200-400 | Hardware store |

**Mechanical Alternatives:**
- 3D-printed chassis (if printer available)
- MDF wood construction
- PVC pipe framework

---

### 8. Assembly Supplies

| Item | Quantity | Purpose | Estimated Cost | Source |
|------|----------|---------|----------------|--------|
| Hot Glue Gun | 1 | Assembly | ₹200-400 | Online, Craft stores |
| Double-Sided Tape | 1 | Component mounting | ₹50-100 | Stationery store |
| Zip Ties | 20 | Cable management | ₹50 | Electronics store |
| Thermal Paste (optional) | 1 | Heat dissipation | ₹100-200 | Electronics store |

---

## Tools (One-Time Investment)

| Item | Cost | Purpose |
|------|------|---------|
| Multimeter | ₹300-800 | Testing circuits, troubleshooting |
| Soldering Iron | ₹400-1000 | Soldering connections |
| Hot Glue Gun | ₹200-400 | Assembly |
| Screwdrivers Set | ₹200-500 | Assembly |
| Wire Cutters | ₹100-300 | Wiring |
| Wire Strippers | ₹100-300 | Wiring |

**Total Tools Cost:** ₹1,400-3,300 (optional, depends on current tools)

---

## Cost Breakdown

### Mandatory Components
```
Arduino UNO:          ₹400-600
MPU6050:              ₹300-500
L298N:                ₹200-400
DC Motors (2x):       ₹300-600
Battery + Charger:    ₹900-1600
Wiring & Connectors:  ₹300-400
Capacitors:           ₹125-250
────────────────────────────
SUBTOTAL:             ₹2,425-4,350
```

### Optional Components
```
HC-SR04 Sensor:       ₹200-400
Breadboard:           ₹100-200
Mechanical parts:     ₹500-1000
Extra fasteners:      ₹200-400
```

### Total Estimated Cost
**₹2,400-4,500** (without optional components)
**₹3,200-6,500** (with optional components)

---

## Where to Buy

### Online (India)
- **Amazon.in** - Good selection, fast delivery
- **AliExpress** - Cheapest, slow delivery (1-3 months)
- **eBay.in** - Variety of components
- **Hobbyist shops** - Robotics-specific components

### Local Electronics Stores
- Check for local electronics shops
- Better for immediate delivery
- Usually more expensive
- Can inspect items before buying

### Specialty Robotics Stores
- Better quality components
- Expert advice available
- Premium pricing
- Online: RoboIndia, xploreRobotics, etc.

---

## Shopping Tips

1. **Buy from verified sellers** - Check ratings/reviews
2. **Compare prices** - Same component, different prices
3. **Check compatibility** - Pin configurations, voltage ratings
4. **Watch for fakes** - Especially on AliExpress
5. **Buy extras** - You'll definitely burn out something!
6. **Bulk discounts** - Sometimes cheaper to buy bundles
7. **Quality > Price** - Save money, get working robot

---

## Component Specifications

### Arduino UNO Required Specs
- Microcontroller: ATmega328P
- Clock Speed: 16 MHz
- RAM: 2KB
- Flash: 32KB
- PWM Pins: 6 (we use pins 5, 6, 9, 10)
- I2C Support: Yes (A4, A5)

### MPU6050 Required Specs
- 6-axis IMU (accelerometer + gyroscope)
- I2C Interface (0x68 address)
- Voltage: 3-5V
- Accuracy: ±0.5° (typical)
- Update Rate: 100-1000 Hz

### L298N Required Specs
- Dual H-bridge motor driver
- Max Current: 2A per channel
- Logic Voltage: 5V (Arduino compatible)
- Motor Voltage: 7-46V (our use: 7-12V)
- PWM Speed Control: Yes

### Motor Required Specs
- Type: DC brushed motor
- Voltage: 3-12V (we use 7.4-11.1V)
- Gear Ratio: 1:15 to 1:50
- Output Shaft: 3-5mm diameter
- Current Draw: ~0.5A per motor (max)

---

## Optional Sensor Upgrades

### Wheel Encoders
- Incremental encoders
- Track actual position
- Enable closed-loop navigation
- Cost: ₹300-500

### IMU with Compass
- Add magnetometer (compass)
- Determine orientation
- Cost: ₹400-700

### Wireless Modules
- HC-05 (Bluetooth Classic)
- HC-12 (Long range RF)
- Enable remote control
- Cost: ₹200-500 each

---

## Consumables to Stock

| Item | Recommended Quantity | Purpose |
|------|---------------------|---------|
| Solder | 25-50g | Re-soldering bad joints |
| Wire (22 AWG) | 10m | Re-wiring connections |
| Screws | Assorted set | Replacements |
| Hot glue sticks | 10 | Re-assembly |
| Zip ties | 50+ | Wire management |
| Thermal paste | 1 tube | Heat dissipation |

---

## Important Notes

1. **Voltage Safety:** Always double-check power connections
2. **LiPo Safety:** Keep fire extinguisher nearby during testing
3. **Component Verification:** Test components before soldering
4. **Quality Matters:** Cheap motors might not work well
5. **Spare Parts:** Buy extras of small components
6. **Compatibility:** Verify all parts work together
7. **Storage:** Keep components in dry place

---

## Sourcing Strategy

### Budget Build (₹2,500-3,000)
- Standard Arduino
- Basic MPU6050
- Simple DC motors
- 2S LiPo battery
- Skip extras

### Standard Build (₹3,500-4,500)
- Arduino UNO
- Quality MPU6050
- Good DC motors with encoders
- 2S LiPo + charger
- Capacitors for filtering

### Premium Build (₹5,000-7,000)
- Arduino UNO or Arduino Due
- Quality 6-axis IMU
- High-quality motors
- 3S LiPo battery
- All optional sensors
- Quality mechanical parts

---

## Final Assembly Shopping List

**Print this checklist:**

- [ ] Arduino UNO
- [ ] MPU6050 (GY-521)
- [ ] L298N Motor Driver
- [ ] DC Motors (2x)
- [ ] Wheels (2x)
- [ ] 2S LiPo Battery (7.4V)
- [ ] LiPo Charger
- [ ] Jumper Wires (assorted)
- [ ] Capacitors (assorted)
- [ ] Acrylic/Chassis Material
- [ ] Fasteners & Brackets
- [ ] USB Cable
- [ ] Power Switch

---

**Total Estimated Budget:** ₹2,400-4,500
**Preparation Time:** 2-4 weeks (delivery)
**Assembly Time:** 3-4 hours

---

*Last Updated: January 2026*

For current prices, check Amazon.in and AliExpress directly.
