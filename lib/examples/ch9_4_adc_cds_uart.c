
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        //AVR 입출력에 대한 헤더 파일
#include <util/delay.h>    // delay 함수사용을 위한 헤더파일

volatile unsigned int ADC_result = 0;

void putch(unsigned char data)
{
    while ((UCSR0A & 0x20) == 0)
        ;        // 전송준비가 될때까지 대기
    UDR0 = data; // 데이터를 UDR0에 쓰면 전송된다
    UCSR0A |= 0x20;
}

// 문자열을 출력하는 함수
void putch_Str(char *str)
{
    unsigned char i = 0;
    while (str[i] != '\0')
        putch(str[i++]); // 문자열을 출력
}

int main(void)
{
    unsigned int AdData = 0; // 10bit ADC값 저장 변수

    DDRF = 0x00; // PF0 입력으로 설정
    DDRE = 0x02; // Rx(입력 0), Tx(출력, 1), SW0입력

    UCSR0A = 0x00;
    UCSR0B = 0x18; // Rx, Tx enable
    UCSR0C = 0x06; // 비동기 방식, No Parity bit, 1 Stop bit

    UBRR0H = 0x00;
    UBRR0L = 0x07; // 115200 bps

    ADMUX = 0x40;   // ADC0선택 //single mode, 0번 채널, 3.3V 외부 기준전압(AREF)
    ADCSRA = 0xAF;  // 10101111 //ADC 허가, free running Mode, Interrupt 허가, 128분주
    ADCSRA |= 0x40; // ADSC AD 개시(Start)

    sei();

    while (1)
    {
        AdData = ADC_result; // ADC 값을 변수 AdData에 저장
        // ADC 값 출력
        putch_Str("\n\r CDS ADC_data : ");
        putch((AdData / 1000) % 10 + '0'); // 1000의 자리 출력
        putch((AdData / 100) % 10 + '0');  // 100의 자리 출력
        putch((AdData / 10) % 10 + '0');   // 10의 자리 출력
        putch((AdData) % 10 + '0');        // 1의 자리 출력
        _delay_ms(500);
    }
}

SIGNAL(ADC_vect)
{
    cli();

    ADC_result = ADC; // 전압이 디지털로 변환된 값 읽어오기.

    sei();
}