# 🤖 Two-Wheeled Self-Balancing Robot

![Arduino](https://img.shields.io/badge/Arduino-UNO-00979D?style=for-the-badge&logo=arduino)
![License](https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active%20Development-brightgreen?style=for-the-badge)
![Version](https://img.shields.io/badge/Version-2.0-blue?style=for-the-badge)

A **production-grade self-balancing robot** using cascaded PID control, sensor fusion, and advanced motor compensation. Inspired by Segway and hoverboard technology, this project demonstrates industry-standard control systems on Arduino.

![Balancing Robot](docs/images/robot-overview.png)

---

## 🌟 Key Features

### Control System
- ✅ **Cascaded PID Architecture** - Inner loop (angle control) + Outer loop (velocity control)
- ✅ **Fixed 100Hz Control Loop** - Using Timer1 hardware interrupt for consistent timing
- ✅ **Complementary Sensor Fusion** - Combines MPU6050 accelerometer + gyroscope
- ✅ **Advanced Anti-Windup** - Conditional integration + back-calculation
- ✅ **Derivative Filtering** - Reduces sensor noise amplification
- ✅ **Motor Dead-Zone Compensation** - Smooth control across full range

### Performance
- ⏱️ **Balance Duration**: 60+ seconds continuous
- 📍 **Recovery Time**: <1 second from gentle push
- 🎯 **Angle Accuracy**: ±0.5° resolution
- 🔋 **Battery Efficiency**: 70%+ capacity usable

### Hardware Support
- 🎮 **Arduino UNO** - Main microcontroller
- 📡 **MPU6050 IMU** - 6-axis (accel + gyro)
- ⚡ **L298N Motor Driver** - H-bridge control
- 🔊 **HC-SR04 Ultrasonic** - Optional obstacle detection
- 🔋 **7.4V-12V Battery** - LiPo or Li-ion recommended

### Robot Specifications
- **Dimensions**: 17×14cm base, 9cm tower (customizable)
- **Weight**: ~300-500g (target)
- **Motor Type**: DC geared motors (1:30 ratio recommended)
- **Operating Voltage**: 7.4V - 12V

---

## 🚀 Quick Start

### Prerequisites
- Arduino IDE 1.8.0+ or 2.x
- Arduino UNO board
- Required libraries:
  - Wire.h (pre-installed)
  - TimerOne.h (must install)

### Installation

#### 1. Install TimerOne Library
```
Arduino IDE → Sketch → Include Library → Manage Libraries
Search: "TimerOne"
Install: "TimerOne by Paul Stoffregen"
```

#### 2. Hardware Setup
See [wiring diagram](docs/WIRING_DIAGRAM.md) for detailed connections:

```
MPU6050:     SDA→A4, SCL→A5, VCC→5V, GND→GND
L298N:       ENA→Pin10, IN1→Pin8, IN2→Pin9, IN3→Pin6, IN4→Pin7, ENB→Pin5
Motors:      OUT1/OUT2→Right, OUT3/OUT4→Left
Battery:     +12V→L298N, GND→L298N + Arduino (SHARED GROUND)
HC-SR04:     TRIG→Pin12, ECHO→Pin11 (optional)
```

#### 3. Upload Code
```bash
1. Open src/Self_Balancing_Robot.ino
2. Select: Tools → Board → Arduino UNO
3. Select: Tools → Port → (your Arduino)
4. Click Upload
```

#### 4. First Run
```
1. Hold robot UPRIGHT and STILL
2. Monitor will show: "Calibrating......"
3. Wait for: "Calibration complete!"
4. Place robot on floor
5. Let go after "Starting in 3 seconds..."
6. Watch it balance! 🎉
```

---

## 📊 Configuration

### PID Tuning

Default values optimized for **17×14cm base, 9cm tall robots**:

```cpp
float Kp = 50.0;    // Proportional gain
float Ki = 200.0;   // Integral gain
float Kd = 3.2;     // Derivative gain
```

**For different robots**, see [TUNING_GUIDE.md](docs/TUNING_GUIDE.md):

| Robot Type | Kp | Ki | Kd |
|-----------|----|----|-----|
| Lightweight (<500g) | 45 | 180 | 2.0 |
| Medium (500g-1kg) | 50 | 200 | 3.2 |
| Heavy (>1kg) | 35 | 140 | 3.5 |

### Motor Settings

```cpp
#define MOTOR_DEAD_ZONE 35      // Minimum PWM to overcome friction
#define MOTOR_MIN_SPEED 20      // Active dead-band threshold
#define MOTOR_MAX_SPEED 255     // Maximum PWM value
```

### Safety Limits

```cpp
#define MAX_ANGLE 40.0          // Fall detection threshold (degrees)
#define FALL_TIMEOUT 2000       // Recovery attempt duration (ms)
#define OBSTACLE_DISTANCE 15.0  // Ultrasonic threshold (cm)
```

---

## 📁 Project Structure

```
self-balancing-robot/
├── README.md                          # This file
├── LICENSE                            # MIT License
├── .gitignore                         # Git ignore rules
├── CHANGELOG.md                       # Version history
│
├── src/
│   ├── Self_Balancing_Robot.ino      # Main code
│   ├── config.h                       # Configuration header
│   └── functions.ino                  # Function definitions
│
├── docs/
│   ├── WIRING_DIAGRAM.md             # Hardware connections
│   ├── TUNING_GUIDE.md               # PID tuning instructions
│   ├── TROUBLESHOOTING.md            # Problem solving
│   ├── ARCHITECTURE.md               # System design
│   ├── API_REFERENCE.md              # Code reference
│   └── images/
│       ├── robot-overview.png
│       ├── wiring-diagram.png
│       └── pid-response.png
│
├── examples/
│   ├── basic_balance.ino             # Simple balancing
│   ├── with_obstacle_detection.ino   # Ultrasonic enabled
│   ├── serial_commands.ino           # Remote control
│   └── kalman_filter.ino             # Advanced sensor fusion
│
├── hardware/
│   ├── BOM.md                        # Bill of materials
│   ├── PCB_LAYOUT.md                 # Circuit design
│   ├── MECHANICAL.md                 # 3D model info
│   └── schematics/
│       └── l298n_wiring.png
│
├── tests/
│   ├── sensor_test.ino               # MPU6050 test
│   ├── motor_test.ino                # L298N test
│   └── calibration_test.ino          # Calibration test
│
├── scripts/
│   ├── serial_monitor.py             # Python data logger
│   └── pid_tuner.py                  # Interactive tuner
│
└── CONTRIBUTING.md                    # Contribution guidelines
```

---

## 🎮 Usage Examples

### Basic Balancing
```cpp
// Just upload the code - robot balances automatically
// Serial Monitor shows: Angle | PID | PWM values
```

### Serial Commands (Optional)
```cpp
// Send commands via Serial Monitor:
// 'f' - Forward (if autonomous mode enabled)
// 'b' - Backward
// 's' - Stop
// 'o' - Toggle obstacle avoidance
```

### Python Data Logger
```bash
# Monitor robot performance via serial
python scripts/serial_monitor.py /dev/ttyUSB0 115200
```

---

## 🔧 Troubleshooting

### Robot Oscillates
→ Reduce `Kp` by 5, increase `Kd` by 0.3

### Robot Falls Immediately
→ Check motor direction, increase `Kp`, verify calibration

### One Motor Doesn't Work
→ Check L298N jumpers, verify wiring, see [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)

### Serial Monitor Shows Garbage
→ Set baud rate to **115200**

See [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) for complete guide.

---

## 📈 Performance Metrics

### Expected Results (After Tuning)

| Metric | Target | Typical |
|--------|--------|---------|
| Balance Duration | 60+ sec | 120+ sec |
| Recovery Time | <1 sec | 0.3-0.5 sec |
| Max Tilt Angle | ±40° | ±35° |
| Position Drift | <1m/min | <0.5m/min |
| Oscillation | Minimal | Damped |

### Control Loop Performance

```
Sample Rate: 100Hz (10ms)
Response Time: <50ms
Compute Time: <5ms
Memory Usage: ~1.2KB
```

---

## 🏗️ Architecture

### Control System Design

```
┌─────────────────────────────────────┐
│    CASCADED CONTROL SYSTEM          │
└─────────────────────────────────────┘
            ↓
┌─────────────────────────────────────┐
│  OUTER LOOP (Velocity Control)      │
│  • Estimates velocity                │
│  • Generates angle setpoint          │
└──────────────┬──────────────────────┘
               ↓ (Angle setpoint)
┌─────────────────────────────────────┐
│  INNER LOOP (Angle Control - PID)   │
│  • P: Proportional response          │
│  • I: Eliminate steady error         │
│  • D: Damping & noise filtering      │
└──────────────┬──────────────────────┘
               ↓ (Motor PWM)
┌─────────────────────────────────────┐
│  MOTOR COMPENSATION                 │
│  • Dead-zone offset                  │
│  • Voltage compensation (optional)   │
│  • Active dead-band                  │
└──────────────┬──────────────────────┘
               ↓
           [MOTORS]
```

### Sensor Fusion (Complementary Filter)

```
Accelerometer (noisy, no drift)
        ↓
    [98% Gyro] + [2% Accel]  → Complementary Filter
        ↑
    Gyroscope (drifts, no noise)

Result: Accurate angle with 100Hz update rate
```

---

## 📚 Documentation

- [Wiring Diagram](docs/WIRING_DIAGRAM.md) - Complete hardware connections
- [Tuning Guide](docs/TUNING_GUIDE.md) - Step-by-step PID optimization
- [Architecture](docs/ARCHITECTURE.md) - System design details
- [Troubleshooting](docs/TROUBLESHOOTING.md) - Problem solving guide
- [API Reference](docs/API_REFERENCE.md) - Code documentation
- [Bill of Materials](hardware/BOM.md) - Parts list with links

---

## 🧪 Testing

### Basic Tests
```bash
# Test 1: Sensor reading
Upload: tests/sensor_test.ino
Expected: Angle values change when tilting

# Test 2: Motor control
Upload: tests/motor_test.ino
Expected: Motors spin forward/backward/stop

# Test 3: Calibration
Upload: src/Self_Balancing_Robot.ino
Expected: Calibration completes, angle ≈ 0°
```

### Advanced Diagnostics
```bash
# Run Python serial monitor
python scripts/serial_monitor.py /dev/ttyUSB0 115200

# Interactive PID tuner
python scripts/pid_tuner.py
```

---

## 🚀 Advanced Features (Optional)

### 1. Wireless Control
Add HC-05 Bluetooth module → Remote control via phone

### 2. Wheel Encoders
Add shaft encoders → Precise position tracking

### 3. Kalman Filter
Replace complementary filter → Better sensor fusion

### 4. LQR Control
Replace PID → Optimal control theory

See [examples/](examples/) for implementation.

---

## 📊 Comparison with Original Code

| Feature | Original | This Version |
|---------|----------|--------------|
| Control Architecture | Single loop | **Cascaded dual-loop** |
| Timing | Variable | **Fixed 100Hz** |
| Sensor Fusion | DMP (black box) | **Complementary filter** |
| Motor Compensation | None | **Dead-zone + voltage** |
| Anti-Windup | Basic | **Advanced** |
| Derivative Filter | None | **Low-pass filter** |
| Code Understanding | Hard | **Fully commented** |
| Performance | Oscillates | **Stable** |

---

## 🤝 Contributing

Contributions welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) first.

### Areas for Contribution
- [ ] Kalman filter implementation
- [ ] Wireless control module
- [ ] Web dashboard for tuning
- [ ] Machine learning optimization
- [ ] Documentation improvements
- [ ] Test suite expansion

---

## 📝 License

This project is licensed under the **MIT License** - see [LICENSE](LICENSE) file.

### You are free to:
- ✅ Use commercially
- ✅ Modify and distribute
- ✅ Use privately

### You must:
- ⚠️ Include license and copyright notice

---

## 🙏 Acknowledgments

- Arduino community for excellent tools
- Jeff Rowberg for MPU6050 libraries
- Paul Stoffregen for TimerOne
- Segway for inspiration in self-balancing technology

---

## 📞 Support

### Getting Help
1. Check [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)
2. Read [TUNING_GUIDE.md](docs/TUNING_GUIDE.md)
3. Review [Architecture](docs/ARCHITECTURE.md)
4. Open an issue on GitHub

### Report Bugs
- Include: Hardware setup, PID gains, Serial output
- Use: [Bug report template](.github/ISSUE_TEMPLATE/bug_report.md)

### Request Features
- Use: [Feature request template](.github/ISSUE_TEMPLATE/feature_request.md)

---

## 📖 Educational Value

This project teaches:
- ✓ PID control theory and tuning
- ✓ Sensor fusion techniques
- ✓ Real-time embedded systems
- ✓ Motor control and compensation
- ✓ Arduino programming
- ✓ Control system stability analysis
- ✓ Cascaded control architecture

**Perfect for:**
- 🎓 Students learning control theory
- 🤖 Robotics enthusiasts
- 💼 Embedded systems engineers
- 🏫 Educational demonstrations

---

## 🔗 Quick Links

- **GitHub Repository**: https://github.com/yourusername/self-balancing-robot
- **Issues**: https://github.com/yourusername/self-balancing-robot/issues
- **Discussions**: https://github.com/yourusername/self-balancing-robot/discussions

---

## 📅 Version History

### v2.0 (Current)
- ✅ Cascaded control architecture
- ✅ Fixed 100Hz timing
- ✅ Motor dead-zone compensation
- ✅ Advanced anti-windup
- ✅ Derivative filtering

### v1.5
- Added HC-SR04 support
- Improved calibration

### v1.0
- Initial release
- Basic PID control

See [CHANGELOG.md](CHANGELOG.md) for full history.

---

## ⭐ Star History

![Star History](https://api.star-history.com/svg?repos=Rubenjoe/self-balancing-robot&type=Date)

---

## 🎯 Next Steps

1. **Hardware Assembly** → Follow [WIRING_DIAGRAM.md](docs/WIRING_DIAGRAM.md)
2. **Library Installation** → Install TimerOne
3. **Upload Code** → Use Arduino IDE
4. **Calibrate Robot** → Run startup sequence
5. **Tune PID** → Follow [TUNING_GUIDE.md](docs/TUNING_GUIDE.md)
6. **Enjoy!** → Watch it balance 🎉

---

**Happy Building!** 🤖⚡

*Last Updated: January 2026 | Version: 2.0 | Status: Active Development*
