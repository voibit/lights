# lights

## Dependencies
* Onebutton
* NeoPixelBus

## Inputs
Arduino nano

| Connetion                  |   Pin       |
| ---------           |-----------|
| Red button  | A0        |
| Green button| A1        |
| Blue button | A2        |
| White button| A3        |
| Pir in| A4   |
| Red button pwm out  | 11        |
| Green button pwm out| 10        |
| Blue button pwm out| 9        |
| Led data out | 6 |
| White button pwm out| 5        |


## Functions

| Button                 |   Function       |
| ---------           |-----------|
| Red Press  | change red brightness, disable PIR        |
| Green Press  | change green brightness, disable PIR        |
| Blue Press  | change Blue brightness, disable PIR        |
| White Press  | change White brightness, disable PIR        |
| Red Double Click  | Turn off red      |
| Green Double Click  | Turn off green      |
| Blue Double Click  | Turn off blue      |
| White Double Click  | Turn off All colors, enable PIR |
| White Longpress  | Strobe, disable PIR |
| Red Longpress  | Disco, disable PIR |



