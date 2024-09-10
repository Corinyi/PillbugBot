# Pill Bug Robot Project

![pillbug_intro-2](https://github.com/user-attachments/assets/b4da6d1f-5558-4abf-93e7-9c3327559ae8)

## Project Overview
The Pill Bug Robot Project aims to develop a next-generation mobile robot inspired by the natural movements of pill bugs. This robot can detect its surroundings using various sensors and adapt its form by contracting into a ball or expanding to move. The project combines multiple technologies to create a versatile and engaging robot with potential applications in reconnaissance and other fields.

## Necessity and Purpose
- **Aligned Interests**: Team members share a common interest in robots that can alter their form and interact with users based on environmental changes. Inspired by childhood memories of playing with pill bugs, the idea to develop a pill bug robot emerged.
- **Usability and Potential**:
  - Research indicates that pill bug robots can function as small-scale reconnaissance robots:
    - Capable of being tossed and navigating rough terrain.
    - Practical applications beyond mere novelty, addressing real-world needs.
  - There are few existing robotic implementations that mimic the contraction and expansion behavior seen in pill bugs.
  - Due to its unique blend of technologies and intriguing nature, the project is expected to generate significant interest if released as open-source.

## Features and Characteristics
- **Adaptive Mobility**: The robot contracts into a ball for rapid movement on slopes and expands to navigate flat terrain.
- **Durability**: Designed to withstand impacts and collisions.
- **Terrain Adaptability**: Overcomes uneven surfaces.
- **Segmented Shell**: Composed of 10 TPU segments that enable smooth contraction and expansion movements.
- **Dynamic Form**: Contracts into a spherical shape and expands into a form suitable for driving and moving.
- **Responsive Movement**: Reacts to impacts and approaching objects with appropriate contraction actions.
- **User Control**:
  - Controlled via a mobile device connected to the robot’s Wi-Fi signal.
  - Real-time monitoring of status (tilt, acceleration, temperature).
  - 30-minute battery life.

## Structure
- The robot's shell consists of 10 TPU segments that can contract and expand without interference.
- 2 footWheels for mobility.

## Technology
- Built on an ESP32-based Platform IO.
- Incorporates C++ and web development for app control.
- Utilizes Wi-Fi AP mode for a mobile UI that allows control and monitoring.
- Introduces a new tandem-based shell mechanism.

## Future Improvements
- Enhancing the automatic contraction and expansion functions.
- Upgrading vision recognition and terrain awareness.
- Adding a posture correction mechanism during contraction and expansion.
- Modifying to 4 footWheels to improve driving performance.

## User Manual

1. Platform IO Platform > Erase Flash > Build Filesystem Image > Upload Filesystem
2. Platform IO General > Build > Upload
3. Wifi connect via "SSID: OMG_pillbug", "Password: aaaa1111"
4. Contorl Panel "http://192.168.4.1"
5. D-controller: Robot Movement, O-controller: Tendom Action

<img width="400" alt="control panel" src="https://github.com/user-attachments/assets/2a33bf09-c475-4e6e-94f4-229f262f99c7">


## BOM

<details>
<summary>Click to view the required components</summary>

| Component             | Description                                      | Link                                                                                                                                                                                                                                                                                                              |
|-----------------------|--------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Wormgear Motor        | Singleshaft, B 7-27RPM                           | [AliExpress Link](https://ko.aliexpress.com/item/1005004226888906.html?spm=a2g0o.productlist.main.1.7118leYaleYaJZ&algo_pvid=28bd5bf2-91db-481d-a27c-530f68f8aae4&algo_exp_id=28bd5bf2-91db-481d-a27c-530f68f8aae4-0&pdp_npi=4%40dis%21KRW%2111000%2111000%21%21%217.97%217.97%21%402140e7df17259336444694219e558f%2112000033444336011%21sea%21KR%21846118265%21X&curPageLogUid=bJ8A75AHJS4U&utparam-url=scene%3Asearch%7Cquery_from%3A) |
| MPU6050               | GY-521 [SZH-EK007]                               | [DeviceMart Link](https://www.devicemart.co.kr/goods/view?no=1247052)                                                                                                                                                                                                                                             |
| High Discharge Battery| GiantPower 50C, 11.1V                            | [RCBank Link](https://www.rcbank.co.kr/shop/goods/goods_view.php?goodsno=89112&category=066001021)                                                                                                                                                                                                               |
| Photo Sensor          | BS5-Y2M                                          | [DeviceMart Link](https://www.devicemart.co.kr/goods/view?no=29436)                                                                                                                                                                                                                                               |
| ESP32                 | ESP32-WROOM                                      | [Amazon Link](https://www.amazon.com/-/ko/dp/B08D5ZD528/ref=sr_1_3?crid=10QZUQ3TDQ92W&dib=eyJ2IjoiMSJ9.C2zjybnvjkLUB8ybNqfUPbWLZs77EvsCLWGQngJqEvkGLv_o48kwC8Ijeis6JInJYV29VdFNlGSuwccUtYOahjRJZFOW_orOSH7qUgNVms3S8t3DU5cPAia8ZIfm9QXAcjqZs2v1X-aN8NYo8gOShIUd3G9fT2ejRui_p5zcwY61inU-9c8njjRGILt7F6DUs3HYvG6o2UYMvwrt14sgvu4Moyjb95mC89epeD4xb-s.tg52QPy9_i1l_SolbtDWiLZ42QImO6pen4gR__GrhOE&dib_tag=se&keywords=esp32%2Bwroom&qid=1725934235&sprefix=esp32%2Bwroom%2Caps%2C309&sr=8-3&th=1)                     |
| Motor Driver          | L9110S                                           | [AliExpress Link](https://ko.aliexpress.com/item/32916608264.html?spm=a2g0o.productlist.main.53.5adda2c7lRn6nq&algo_pvid=01731282-d0a7-4a7f-b58f-853a078476fe&algo_exp_id=01731282-d0a7-4a7f-b58f-853a078476fe-26&pdp_npi=4%40dis%21KRW%21676%21579%21%21%210.49%210.42%21%402141005d17259341786538693e9e04%2165980918324%21sea%21KR%21846118265%21X&curPageLogUid=6fs4WSxVCf44&utparam-url=scene%3Asearch%7Cquery_from%3A)  |
| Charging and Step-Down| SZH-PWSD-045                                     | [DeviceMart Link](https://www.devicemart.co.kr/goods/view?no=1321982)                                                                                                                                                                                                                                             |

</details>

---

## Conclusion
The Pill Bug Robot Project combines various technologies to bring a new approach to mobile robotics by mimicking the unique movements of pill bugs. By open-sourcing this project, we aim to foster innovation and inspire the robotics community with this engaging and practical design.

### Made by.
OpensourceMakerGroup SungkyunkwanUniv

contact via leejimmy1@g.skku.edu