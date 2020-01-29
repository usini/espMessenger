EESchema Schematic File Version 4
LIBS:esp8266_8x8MatrixLed-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "ESP8266 8x8 Matrix Led"
Date "2019-08-08"
Rev "1.3"
Comp "Rémi Sarrailh - µsini.eu"
Comment1 "TESTED - NEED MORE TEST"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x05_Male J2
U 1 1 5D46BB85
P 7050 3200
F 0 "J2" H 6950 3600 50  0000 R CNN
F 1 "Matrix Connector" H 7200 3500 50  0000 R CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x05_P2.54mm_Horizontal" H 7050 3200 50  0001 C CNN
F 3 "~" H 7050 3200 50  0001 C CNN
	1    7050 3200
	-1   0    0    1   
$EndComp
Text Notes 7100 3000 0    50   ~ 0
VCC
Text Notes 7100 3100 0    50   ~ 0
GND
Text Notes 7100 3200 0    50   ~ 0
DIN
Text Notes 7100 3300 0    50   ~ 0
CS
Text Notes 7100 3400 0    50   ~ 0
CLK
$Comp
L power:+3.3V #PWR0101
U 1 1 5D4721E1
P 6850 3000
F 0 "#PWR0101" H 6850 2850 50  0001 C CNN
F 1 "+3.3V" V 6865 3128 50  0000 L CNN
F 2 "" H 6850 3000 50  0001 C CNN
F 3 "" H 6850 3000 50  0001 C CNN
	1    6850 3000
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5D472C37
P 6850 3100
F 0 "#PWR0102" H 6850 2850 50  0001 C CNN
F 1 "GND" V 6855 2972 50  0000 R CNN
F 2 "" H 6850 3100 50  0001 C CNN
F 3 "" H 6850 3100 50  0001 C CNN
	1    6850 3100
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR0103
U 1 1 5D4730C8
P 5850 2300
F 0 "#PWR0103" H 5850 2150 50  0001 C CNN
F 1 "+3.3V" H 5865 2473 50  0000 C CNN
F 2 "" H 5850 2300 50  0001 C CNN
F 3 "" H 5850 2300 50  0001 C CNN
	1    5850 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5D473711
P 5750 3900
F 0 "#PWR0104" H 5750 3650 50  0001 C CNN
F 1 "GND" H 5755 3727 50  0000 C CNN
F 2 "" H 5750 3900 50  0001 C CNN
F 3 "" H 5750 3900 50  0001 C CNN
	1    5750 3900
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 5D478799
P 4900 2600
F 0 "J1" H 4900 2850 50  0000 R CNN
F 1 "RST Button" H 5100 2750 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4900 2600 50  0001 C CNN
F 3 "~" H 4900 2600 50  0001 C CNN
	1    4900 2600
	1    0    0    -1  
$EndComp
Text Notes 4850 2750 2    50   ~ 0
RST
Text Notes 4850 2650 2    50   ~ 0
GND
$Comp
L MCU_Module:WeMos_D1_mini U1
U 1 1 5D46B188
P 5750 3100
F 0 "U1" H 5750 4200 50  0000 C CNN
F 1 "WeMos_D1_mini" H 5750 4300 50  0000 C CNN
F 2 "wemos_D1_mini:WEMOS_D1_mini_light_no_female_connectors" H 5750 1950 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H 3900 1950 50  0001 C CNN
	1    5750 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5D47B8D5
P 5100 2600
F 0 "#PWR0105" H 5100 2350 50  0001 C CNN
F 1 "GND" V 5105 2472 50  0000 R CNN
F 2 "" H 5100 2600 50  0001 C CNN
F 3 "" H 5100 2600 50  0001 C CNN
	1    5100 2600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5100 2700 5350 2700
NoConn ~ 5650 2300
NoConn ~ 6150 2600
NoConn ~ 6150 2700
NoConn ~ 6150 3100
NoConn ~ 5350 3000
NoConn ~ 5350 3100
$Comp
L Connector:Conn_01x02_Male J3
U 1 1 5D49FA38
P 7100 2600
F 0 "J3" H 7250 2350 50  0000 C CNN
F 1 "USER Button" H 7200 2250 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7100 2600 50  0001 C CNN
F 3 "~" H 7100 2600 50  0001 C CNN
	1    7100 2600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5D4A2833
P 6900 2600
F 0 "#PWR01" H 6900 2350 50  0001 C CNN
F 1 "GND" V 6905 2472 50  0000 R CNN
F 2 "" H 6900 2600 50  0001 C CNN
F 3 "" H 6900 2600 50  0001 C CNN
	1    6900 2600
	0    1    1    0   
$EndComp
NoConn ~ 6150 3500
NoConn ~ 6150 2900
NoConn ~ 6150 2800
Wire Wire Line
	6150 3200 6850 3200
Wire Wire Line
	6150 3300 6850 3300
Wire Wire Line
	6150 3400 6850 3400
Wire Wire Line
	6350 3000 6350 2500
Wire Wire Line
	6350 2500 6900 2500
Wire Wire Line
	6150 3000 6350 3000
$EndSCHEMATC
