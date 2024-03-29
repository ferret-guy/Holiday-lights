EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:Controller PTH-cache
EELAYER 27 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "LED Controller PTH"
Date "30 may 2014"
Rev "1"
Comp "Mars Rising"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ATMEGA328-P IC1
U 1 1 53875122
P 3900 4600
F 0 "IC1" H 3150 5850 40  0000 L BNN
F 1 "ATMEGA328-P" H 4300 3200 40  0000 L BNN
F 2 "DIL28" H 3900 4600 30  0000 C CIN
F 3 "" H 3900 4600 60  0000 C CNN
	1    3900 4600
	1    0    0    -1  
$EndComp
$Comp
L CONN_3 K1
U 1 1 53875169
P 8650 2100
F 0 "K1" V 8600 2100 50  0000 C CNN
F 1 "CONN_3" V 8700 2100 40  0000 C CNN
F 2 "" H 8650 2100 60  0000 C CNN
F 3 "" H 8650 2100 60  0000 C CNN
	1    8650 2100
	1    0    0    -1  
$EndComp
Text Notes 7300 2400 0    60   Italic 0
JST or direct solder to LED Conn wires
$Comp
L TSOP38238 Q1
U 1 1 538757A6
P 8350 3000
F 0 "Q1" H 8350 3300 60  0000 C CNN
F 1 "TSOP38238" H 8350 3400 60  0000 C CNN
F 2 "" H 8350 3000 60  0000 C CNN
F 3 "" H 8350 3000 60  0000 C CNN
	1    8350 3000
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 53875854
P 6950 2300
F 0 "C2" H 6950 2400 40  0000 L CNN
F 1 "0.1uF" H 6956 2215 40  0000 L CNN
F 2 "~" H 6988 2150 30  0000 C CNN
F 3 "~" H 6950 2300 60  0000 C CNN
	1    6950 2300
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 53875919
P 2600 3700
F 0 "C1" H 2600 3800 40  0000 L CNN
F 1 "0.1uF" H 2606 3615 40  0000 L CNN
F 2 "~" H 2638 3550 30  0000 C CNN
F 3 "~" H 2600 3700 60  0000 C CNN
	1    2600 3700
	1    0    0    -1  
$EndComp
$Comp
L ZTT-16.00MX Q2
U 1 1 5387776F
P 5850 3900
F 0 "Q2" H 5850 4200 60  0000 C CNN
F 1 "ZTT-16.00MX" H 5850 4300 60  0000 C CNN
F 2 "" H 5850 3900 60  0000 C CNN
F 3 "" H 5850 3900 60  0000 C CNN
	1    5850 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 2100 5500 3800
Wire Wire Line
	5500 3800 4900 3800
Wire Wire Line
	8250 3350 8250 3600
Wire Wire Line
	8250 3600 4900 3600
Wire Wire Line
	6750 2500 6950 2500
Connection ~ 6950 2100
Connection ~ 6950 2500
Wire Wire Line
	2400 3500 3000 3500
Wire Wire Line
	5750 4100 4900 4100
Wire Wire Line
	4900 4200 5950 4200
Wire Wire Line
	5950 4200 5950 4100
Wire Wire Line
	5850 4350 5850 4100
Wire Wire Line
	2400 3900 2600 3900
Connection ~ 2600 3500
$Comp
L LM7905CT U1
U 1 1 5387842B
P 3250 2100
F 0 "U1" H 3050 1900 40  0000 C CNN
F 1 "LM7905CT" H 3250 1900 40  0000 L CNN
F 2 "TO-220" H 3250 2000 30  0000 C CIN
F 3 "~" H 3250 2100 60  0000 C CNN
	1    3250 2100
	1    0    0    1   
$EndComp
Wire Wire Line
	2450 2050 2850 2050
Wire Wire Line
	3250 2700 3250 2350
$Comp
L CP1 C3
U 1 1 5387849C
P 2550 2400
F 0 "C3" H 2600 2500 50  0000 L CNN
F 1 "0.33uF" H 2600 2300 50  0000 L CNN
F 2 "~" H 2550 2400 60  0000 C CNN
F 3 "~" H 2550 2400 60  0000 C CNN
	1    2550 2400
	1    0    0    -1  
$EndComp
$Comp
L CP1 C4
U 1 1 538784AB
P 3950 2400
F 0 "C4" H 4000 2500 50  0000 L CNN
F 1 "1uF" H 4000 2300 50  0000 L CNN
F 2 "~" H 3950 2400 60  0000 C CNN
F 3 "~" H 3950 2400 60  0000 C CNN
	1    3950 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 2200 2550 2050
Connection ~ 2550 2050
Wire Wire Line
	2550 2600 2550 2700
Wire Wire Line
	2550 2700 4250 2700
Wire Wire Line
	3950 2700 3950 2600
Connection ~ 3250 2700
Wire Wire Line
	3950 2200 3950 2050
Connection ~ 3950 2050
$Comp
L CONN_6 P1
U 1 1 538785A9
P 5800 5350
F 0 "P1" V 5750 5350 60  0000 C CNN
F 1 "CONN_6" V 5850 5350 60  0000 C CNN
F 2 "" H 5800 5350 60  0000 C CNN
F 3 "" H 5800 5350 60  0000 C CNN
	1    5800 5350
	1    0    0    1   
$EndComp
Wire Wire Line
	5450 5400 5050 5400
Wire Wire Line
	5050 5400 5050 5100
Wire Wire Line
	5050 5100 4900 5100
Wire Wire Line
	4900 5200 4950 5200
Wire Wire Line
	4950 5200 4950 5500
Wire Wire Line
	4950 5500 5450 5500
$Comp
L R R1
U 1 1 53879813
P 6400 4300
F 0 "R1" V 6480 4300 40  0000 C CNN
F 1 "10K" V 6407 4301 40  0000 C CNN
F 2 "~" V 6330 4300 30  0000 C CNN
F 3 "~" H 6400 4300 30  0000 C CNN
	1    6400 4300
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 53879856
P 6400 5150
F 0 "C5" H 6400 5250 40  0000 L CNN
F 1 "0.1uF" H 6406 5065 40  0000 L CNN
F 2 "~" H 6438 5000 30  0000 C CNN
F 3 "~" H 6400 5150 60  0000 C CNN
	1    6400 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 4950 6400 4950
Wire Wire Line
	6400 4950 6400 4550
Wire Wire Line
	6400 5350 6400 5700
Wire Wire Line
	6400 5700 5450 5700
Wire Wire Line
	5450 5700 5450 5600
Wire Wire Line
	5450 5200 5450 5100
$Comp
L C C6
U 1 1 53879919
P 4250 2400
F 0 "C6" H 4250 2500 40  0000 L CNN
F 1 "0.1uF" H 4256 2315 40  0000 L CNN
F 2 "~" H 4288 2250 30  0000 C CNN
F 3 "~" H 4250 2400 60  0000 C CNN
	1    4250 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 2700 4250 2600
Connection ~ 3950 2700
Wire Wire Line
	4250 2200 4250 2050
Wire Wire Line
	3650 2050 4550 2050
Connection ~ 4250 2050
Wire Wire Line
	2400 3900 2400 5800
Wire Wire Line
	2400 5800 3000 5800
Wire Wire Line
	3000 5700 2400 5700
Connection ~ 2400 5700
NoConn ~ 4900 5800
NoConn ~ 4900 5700
NoConn ~ 4900 5600
NoConn ~ 4900 5500
NoConn ~ 4900 5400
NoConn ~ 4900 5300
NoConn ~ 4900 4850
NoConn ~ 4900 4750
NoConn ~ 4900 4650
NoConn ~ 4900 4550
NoConn ~ 4900 4450
NoConn ~ 4900 4350
NoConn ~ 4900 4000
NoConn ~ 4900 3900
NoConn ~ 4900 3700
NoConn ~ 4900 3500
NoConn ~ 3000 4100
$Comp
L VSS #PWR5
U 1 1 5387B701
P 2500 3900
F 0 "#PWR5" H 2500 3900 30  0001 C CNN
F 1 "VSS" H 2500 3830 30  0000 C CNN
F 2 "" H 2500 3900 60  0000 C CNN
F 3 "" H 2500 3900 60  0000 C CNN
	1    2500 3900
	1    0    0    -1  
$EndComp
$Comp
L VSS #PWR2
U 1 1 5387B710
P 3250 2700
F 0 "#PWR2" H 3250 2700 30  0001 C CNN
F 1 "VSS" H 3250 2630 30  0000 C CNN
F 2 "" H 3250 2700 60  0000 C CNN
F 3 "" H 3250 2700 60  0000 C CNN
	1    3250 2700
	1    0    0    -1  
$EndComp
$Comp
L VSS #PWR9
U 1 1 5387B71F
P 5850 4350
F 0 "#PWR9" H 5850 4350 30  0001 C CNN
F 1 "VSS" H 5850 4280 30  0000 C CNN
F 2 "" H 5850 4350 60  0000 C CNN
F 3 "" H 5850 4350 60  0000 C CNN
	1    5850 4350
	1    0    0    -1  
$EndComp
$Comp
L VSS #PWR7
U 1 1 5387B72E
P 5450 5100
F 0 "#PWR7" H 5450 5100 30  0001 C CNN
F 1 "VSS" H 5450 5030 30  0000 C CNN
F 2 "" H 5450 5100 60  0000 C CNN
F 3 "" H 5450 5100 60  0000 C CNN
	1    5450 5100
	0    1    -1   0   
$EndComp
$Comp
L VSS #PWR14
U 1 1 5387B73D
P 8350 3350
F 0 "#PWR14" H 8350 3350 30  0001 C CNN
F 1 "VSS" H 8350 3280 30  0000 C CNN
F 2 "" H 8350 3350 60  0000 C CNN
F 3 "" H 8350 3350 60  0000 C CNN
	1    8350 3350
	1    0    0    -1  
$EndComp
$Comp
L VSS #PWR11
U 1 1 5387B74C
P 6750 2500
F 0 "#PWR11" H 6750 2500 30  0001 C CNN
F 1 "VSS" H 6750 2430 30  0000 C CNN
F 2 "" H 6750 2500 60  0000 C CNN
F 3 "" H 6750 2500 60  0000 C CNN
	1    6750 2500
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR3
U 1 1 5387B75B
P 4550 2050
F 0 "#PWR3" H 4550 2150 30  0001 C CNN
F 1 "VCC" H 4550 2150 30  0000 C CNN
F 2 "" H 4550 2050 60  0000 C CNN
F 3 "" H 4550 2050 60  0000 C CNN
	1    4550 2050
	0    1    -1   0   
$EndComp
$Comp
L VCC #PWR4
U 1 1 5387B76A
P 2400 3500
F 0 "#PWR4" H 2400 3600 30  0001 C CNN
F 1 "VCC" H 2400 3600 30  0000 C CNN
F 2 "" H 2400 3500 60  0000 C CNN
F 3 "" H 2400 3500 60  0000 C CNN
	1    2400 3500
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR6
U 1 1 5387B779
P 3000 3800
F 0 "#PWR6" H 3000 3900 30  0001 C CNN
F 1 "VCC" H 3000 3900 30  0000 C CNN
F 2 "" H 3000 3800 60  0000 C CNN
F 3 "" H 3000 3800 60  0000 C CNN
	1    3000 3800
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR10
U 1 1 5387B788
P 6400 4050
F 0 "#PWR10" H 6400 4150 30  0001 C CNN
F 1 "VCC" H 6400 4150 30  0000 C CNN
F 2 "" H 6400 4050 60  0000 C CNN
F 3 "" H 6400 4050 60  0000 C CNN
	1    6400 4050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR8
U 1 1 5387B797
P 5450 5300
F 0 "#PWR8" H 5450 5400 30  0001 C CNN
F 1 "VCC" H 5450 5400 30  0000 C CNN
F 2 "" H 5450 5300 60  0000 C CNN
F 3 "" H 5450 5300 60  0000 C CNN
	1    5450 5300
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR15
U 1 1 5387B7A6
P 8450 3350
F 0 "#PWR15" H 8450 3450 30  0001 C CNN
F 1 "VCC" H 8450 3450 30  0000 C CNN
F 2 "" H 8450 3350 60  0000 C CNN
F 3 "" H 8450 3350 60  0000 C CNN
	1    8450 3350
	-1   0    0    1   
$EndComp
$Comp
L VPP #PWR1
U 1 1 5387B88E
P 2450 2050
F 0 "#PWR1" H 2450 2250 40  0001 C CNN
F 1 "VPP" H 2450 2200 40  0000 C CNN
F 2 "" H 2450 2050 60  0000 C CNN
F 3 "" H 2450 2050 60  0000 C CNN
	1    2450 2050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5500 2100 7250 2100
Wire Wire Line
	7250 2100 7250 2200
Wire Wire Line
	7250 2200 7450 2200
$Comp
L VSS #PWR13
U 1 1 5387C0A2
P 8300 2100
F 0 "#PWR13" H 8300 2100 30  0001 C CNN
F 1 "VSS" H 8300 2030 30  0000 C CNN
F 2 "" H 8300 2100 60  0000 C CNN
F 3 "" H 8300 2100 60  0000 C CNN
	1    8300 2100
	0    1    1    0   
$EndComp
$Comp
L VPP #PWR12
U 1 1 5387C0B1
P 8300 2000
F 0 "#PWR12" H 8300 2200 40  0001 C CNN
F 1 "VPP" H 8300 2150 40  0000 C CNN
F 2 "" H 8300 2000 60  0000 C CNN
F 3 "" H 8300 2000 60  0000 C CNN
	1    8300 2000
	0    -1   -1   0   
$EndComp
$Comp
L R R2
U 1 1 5387D252
P 7700 2200
F 0 "R2" V 7780 2200 40  0000 C CNN
F 1 "300" V 7707 2201 40  0000 C CNN
F 2 "~" V 7630 2200 30  0000 C CNN
F 3 "~" H 7700 2200 30  0000 C CNN
	1    7700 2200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7950 2200 8300 2200
$EndSCHEMATC
