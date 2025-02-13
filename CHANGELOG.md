# 개발 로그

## [2025-02-11] 최초 생성

- 개발로그 파일 생성
- 예제 파일 추가 3 장

## [2025-02-12] LCD 라이브러리 추가 및 다수의 AVR 예제 파일 추가

- LCD 라이브러리 추가 (lcd.c, lcd.h, lcdconf.h 및 관련 JSON 파일)
- AVR 예제 파일 추가 (ch5_1_fndapp.c, ch6_1_timer0led.c, ch7_lcd.c 등)
- platformio.ini 파일 수정 (빌드 설정 변경, 기존 main.cpp 제거, 새로운 main.c 적용)
  - 맹글링 오류가 있었음.

## [2025-02-13] PWM 기반의 서보 및 DC 모터 예제 추가, LCD 디스플레이 및 DC 모터 제어 예제 추가

- lib/examples 폴더에 LCD 초기화, 센서 인터페이스, DC 모터 동작(전진, 후진, 정지, PWM 제어) 예제 추가
- 외부 EEPROM, SHT2x 센서, I2C 통신 지원을 위한 드라이버 및 라이브러리 파일(at25160.c/h, SHT2x.c/h, TWI_driver.c/h) 추가
- SPI, TWI 작업, 센서 측정, UART 기반 데이터 출력 구조화 함수 구현
- 실시간 LCD 출력, 모터 방향 제어 및 주기적 센서 데이터 취득 기능 포함
- 서보 예제 (ch12_servo_pwm.c) 추가: 인터럽트 기반 및 지연 기반 PWM 구현, LCD 상태 업데이트 지원
- DC 모터 예제 (ch13_dc_motor_pwm.c) 추가: 인터럽트 기반 및 지연 기반 PWM 구현, LCD 피드백 기능 포함
- src/main.c 수정: LCD 업데이트와 PWM 제어를 통한 서보 동작 관리
- 문제 사항
  - at 라이브러리 함수 없음 ___array 두 함수 추가.
  - pwm 제어 중 servo 및 stepper 모터 보드 이상 현상 -> servo 모터 분리 사용
  - dc 모터 pwm 수치가 70 이하가 되면 모터 동작하지 않음 -> 동작 범위 수정
  - ch10 timer_temp 에서 오버플로우 체크가 안되고 스위치 인터럽트 코드가 빠져 있음.
  - ch7 pwm buzzer switch 에서 버저 동작이 안됨 -> 버저 동작 코드 추가

