# Contributing to Self-Balancing Robot

Thank you for your interest in contributing! This document provides guidelines and instructions for contributing.

## Code of Conduct

This project adheres to the [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md).
By participating, you are expected to uphold this code.

---

## How to Contribute

### 1. Report Bugs

**Before reporting a bug, please check:**
- [ ] The issue doesn't already exist in [Issues](https://github.com/yourusername/self-balancing-robot/issues)
- [ ] You're using the latest version
- [ ] You've read the [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)

**When reporting, include:**
- Hardware setup (components, wiring)
- Current PID gains and configuration
- Serial Monitor output (screenshot or text)
- Steps to reproduce
- Expected vs actual behavior

**Example:**
```
Title: Robot oscillates with default PID gains

Hardware:
- Arduino UNO
- MPU6050 (GY-521)
- L298N motor driver
- 2x DC geared motors
- 7.4V LiPo battery

Configuration:
- Kp: 50, Ki: 200, Kd: 3.2
- Dimensions: 17×14cm base, 9cm tower

Problem:
Robot oscillates rapidly back and forth instead of balancing smoothly.

Serial Output:
```
Angle: 5.34 | PID: 267 | PWM: 302
Angle: -4.21 | PID: -211 | PWM: -246
```

Steps to Reproduce:
1. Uploaded Self_Balancing_Robot.ino
2. Calibrated with robot upright
3. Placed on floor
4. Robot immediately starts oscillating

Expected:
Smooth balancing with minimal corrections
```

### 2. Suggest Features

**Before suggesting, check:**
- [ ] Feature doesn't already exist
- [ ] It's relevant to the project
- [ ] It aligns with project goals

**Suggestion Template:**
```
Title: Add Kalman Filter for sensor fusion

Description:
Implement Kalman filter to replace complementary filter for improved 
sensor fusion accuracy and better performance.

Motivation:
Current complementary filter works well but could be optimized further
using optimal estimation theory.

Implementation Ideas:
- Create kalman_filter.h library
- Add example in examples/with_kalman.ino
- Include tuning guide for Q and R matrices

Additional Context:
Research papers on Kalman filters for IMU fusion
```

### 3. Submit Code Changes

#### Fork and Clone
```bash
# Fork the repository on GitHub
# Clone your fork
git clone https://github.com/YOUR_USERNAME/self-balancing-robot.git
cd self-balancing-robot
git remote add upstream https://github.com/original/self-balancing-robot.git
```

#### Create Feature Branch
```bash
# Update master
git fetch upstream
git checkout master
git merge upstream/master

# Create feature branch
git checkout -b feature/your-feature-name
```

#### Make Changes
- Follow the [Coding Style](#coding-style) guide
- Write clear commit messages
- Test thoroughly
- Update documentation

#### Commit and Push
```bash
git add .
git commit -m "Add feature: description of changes"
git push origin feature/your-feature-name
```

#### Create Pull Request
1. Go to GitHub and create a pull request
2. Describe your changes clearly
3. Reference any related issues
4. Wait for review and feedback

---

## Coding Style

### Arduino/C++ Code

**Naming Conventions:**
```cpp
// Constants: UPPER_CASE_WITH_UNDERSCORES
#define MAX_ANGLE 40.0
#define MOTOR_DEAD_ZONE 35

// Variables: camelCase for local, snake_case for globals
float targetAngle = 0;
float current_time = millis();

// Functions: camelCase starting with verb
void calculatePID();
void readMPU6050();
bool checkSafety();
```

**Code Style:**
```cpp
// Braces on same line
if (condition) {
    // Code here
}

// Use spaces around operators
pidOutput = Kp * error + Ki * integral + Kd * derivative;

// One statement per line
float value1 = 0;
float value2 = 0;
// NOT: float value1 = 0, value2 = 0;

// Comment complex logic
// Complementary filter: 98% gyro, 2% accel for stability
currentAngle = 0.98 * gyroAngle + 0.02 * accelAngle;
```

**Function Documentation:**
```cpp
// ============================================================================
// FUNCTION_NAME
// ============================================================================
// Purpose: Clear description of what function does
// Parameters:
//   - param1: Description
//   - param2: Description
// Returns: Description
// Example: How to use
// ============================================================================

void calculatePID() {
    // Function implementation
}
```

### Documentation

**Markdown Style:**
```markdown
# Main Heading
## Sub Heading
### Sub-sub Heading

- Bullet points
- Use meaningful names
- Clear and concise

**Bold** for emphasis
`inline code` for variables
```{code block}` for longer code
```

### Comments

**Good Comments:**
```cpp
// ✓ Explains WHY, not WHAT
// Kd is high because tall robots need damping
float Kd = 3.2;

// ✓ Explains complex algorithm
// Complementary filter: gyro is fast but drifts,
// accel is slow but accurate. Combine 98:2 ratio.
currentAngle = 0.98 * gyroAngle + 0.02 * accelAngle;
```

**Bad Comments:**
```cpp
// ✗ Obvious - just restates code
float Kd = 3.2;  // Set Kd to 3.2

// ✗ Too vague
// Do the thing
someFunction();
```

---

## Testing

### Before Submitting PR

**Hardware Tests:**
- [ ] Code compiles without errors
- [ ] Robot balances (even if not perfectly)
- [ ] No compilation warnings
- [ ] Serial output is clean

**Code Tests:**
```bash
# Test sensor reading
Upload: tests/sensor_test.ino
Verify: Angle values change when tilting

# Test motor control
Upload: tests/motor_test.ino
Verify: Motors spin in correct directions

# Test full system
Upload: src/Self_Balancing_Robot.ino
Verify: Robot balances and recovers from pushes
```

### Regression Testing

If your change affects core functionality:
- [ ] Verify default tuning still works
- [ ] Test with different PID gains
- [ ] Check with different robot dimensions
- [ ] Verify motor behavior

---

## Pull Request Process

1. **Update your branch with latest master:**
   ```bash
   git fetch upstream
   git rebase upstream/master
   ```

2. **Write clear PR description:**
   - What problem does it solve?
   - How does it work?
   - Any breaking changes?
   - Testing performed

3. **PR Template:**
   ```markdown
   ## Description
   Brief summary of changes
   
   Fixes #[issue_number]
   
   ## Type of Change
   - [ ] Bug fix
   - [ ] New feature
   - [ ] Documentation
   - [ ] Code cleanup
   
   ## Testing
   - [ ] Tested on Arduino UNO
   - [ ] Verified robot balances
   - [ ] No new warnings/errors
   
   ## Checklist
   - [ ] Code follows style guide
   - [ ] Documentation updated
   - [ ] No breaking changes
   - [ ] Tests pass
   ```

4. **Review Process:**
   - Maintainer will review
   - May request changes
   - Once approved, will merge

---

## Development Setup

### Required Tools
- Arduino IDE 1.8.0+ or 2.x
- Git
- Python 3.7+ (for helper scripts)

### Optional Tools
- VS Code with Arduino extension
- PlatformIO IDE
- Serial monitor visualization tools

### Libraries Needed
```
Wire.h (pre-installed)
TimerOne.h (must install via Library Manager)
```

---

## Project Structure

```
src/              # Main Arduino code
├── Self_Balancing_Robot.ino
└── config.h      # Configuration constants

docs/             # Documentation
├── README.md
├── TUNING_GUIDE.md
├── ARCHITECTURE.md
└── ...

tests/            # Test sketches
├── sensor_test.ino
├── motor_test.ino
└── ...

examples/         # Example implementations
└── ...

hardware/         # Hardware files
├── BOM.md
└── schematics/
```

---

## Areas Needing Help

### High Priority
- [ ] Kalman filter implementation
- [ ] Wheel encoder support
- [ ] Better documentation with diagrams
- [ ] Python control interface

### Medium Priority
- [ ] Wireless Bluetooth control
- [ ] Web dashboard
- [ ] More test cases
- [ ] Performance benchmarks

### Low Priority
- [ ] LQR control
- [ ] Machine learning tuning
- [ ] 3D printed designs
- [ ] Community examples

---

## Questions?

- **Documentation**: Check [docs/](docs/) folder
- **Issues**: Check existing [GitHub Issues](https://github.com/yourusername/self-balancing-robot/issues)
- **Discussions**: Use GitHub Discussions for general questions

---

## Acknowledgments

Thank you for contributing to make this project better! 🎉

---

**Last Updated: January 2026**
