#include "mbed.h"
#include <stdio.h>
#include <string.h>

AnalogIn analog_X(PC_2);
AnalogIn analog_Y(PC_3);
Ticker ticker;
BufferedSerial pc(USBTX, USBRX);
BufferedSerial mp(PA_15, PB_7);

PwmOut left(PA_0);     // leftClk
DigitalOut leftDir(PA_1);
PwmOut right(PC_8);    // rightClk
DigitalOut rightDir(PC_6);
DigitalOut en(PA_4);

void run1() {
    char c = mp.getc();

    // 방향 정보에 따른 모터 제어
    switch(c) {
        case 'G': // 전진
            pc.putc(c);
            en=1;
            leftDir = true;
            rightDir = false;
            left.period_ms(1);
            right.period_ms(1);
            left.write(0.5);
            right.write(0.5);
            break;
        case 'B': // 후진
            pc.putc(c);
            en=1;
            leftDir = false;
            rightDir = true;
            left.period_ms(1);
            right.period_ms(1);
            left.write(0.5);
            right.write(0.5);
            break;
        case 'R': // 오른쪽
            pc.putc(c);
            en=1;
            leftDir = true;
            rightDir = true;
            left.period_ms(3);
            right.period_ms(3);
            left.write(0.5);
            right.write(0.5);
            break;
        case 'L': // 왼쪽
            pc.putc(c);
            en=1;
            leftDir = false;
            rightDir = false;
            left.period_ms(3);
            right.period_ms(3);
            left.write(0.5);
            right.write(0.5);
            break;
        case 'S': // 정지
            pc.putc(c);
            en=0;
            break;
    }
}

// pc입력 mp로 송신
void pc_mp(){
    if (pc.getc() == 'A') mp.sprintf("AT\r\n");       // AT 연결

    else if(pc.getc() == 'D') mp.sprintf("AT+ADDR?\r\n");  // 어드레스 확인

    else if(pc.getc() == 'N') mp.sprintf("AT+NAME=WG-06\r\n");  // 이름 설정

    else if(pc.getc() == 'C') mp.sprintf("AT+CMODE=0\r\n");  //CMODE 설정 (통신 수준 설정)

    else if(pc.getc() == 'R') mp.sprintf("AT+ROLE=0\r\n");// 마스터 슬레이브 설정

    else if(pc.getc() == 'B') mp.sprintf("AT+BIND=\r\n");  // 연결

    else mp.putc(pc.getc());
}

int main() {
    pc.baud(9600);
    mp.baud(9600);
    //pc.printf("Hello2");
    mp.attach(&run1);
    mp.attach(&pc_mp);
    while(1) {
        
    }
}
