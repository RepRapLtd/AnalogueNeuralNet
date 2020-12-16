EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:+5V #PWR01
U 1 1 5FD8E700
P 3000 5450
F 0 "#PWR01" H 3000 5300 50  0001 C CNN
F 1 "+5V" H 3015 5623 50  0000 C CNN
F 2 "" H 3000 5450 50  0001 C CNN
F 3 "" H 3000 5450 50  0001 C CNN
	1    3000 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2525 5600 3000 5600
Wire Wire Line
	3000 5600 3000 5450
$Comp
L power:GND #PWR02
U 1 1 5FD8EC28
P 3000 6300
F 0 "#PWR02" H 3000 6050 50  0001 C CNN
F 1 "GND" H 3005 6127 50  0000 C CNN
F 2 "" H 3000 6300 50  0001 C CNN
F 3 "" H 3000 6300 50  0001 C CNN
	1    3000 6300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5FD8EFBA
P 3300 3050
F 0 "R1" V 3093 3050 50  0000 C CNN
F 1 "33K" V 3184 3050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3230 3050 50  0001 C CNN
F 3 "~" H 3300 3050 50  0001 C CNN
	1    3300 3050
	0    1    1    0   
$EndComp
$Comp
L Device:C C2
U 1 1 5FD8F543
P 3600 3275
F 0 "C2" H 3715 3321 50  0000 L CNN
F 1 "220nF" H 3715 3230 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3638 3125 50  0001 C CNN
F 3 "~" H 3600 3275 50  0001 C CNN
	1    3600 3275
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 3050 3600 3050
Wire Wire Line
	3600 3050 3600 3125
$Comp
L Amplifier_Operational:OPA2277 U1
U 1 1 5FD911C5
P 4575 3150
F 0 "U1" H 4675 3350 50  0000 C CNN
F 1 "TLC2272" H 4775 3275 50  0000 C CNN
F 2 "Package_SO:SOIC-8-N7_3.9x4.9mm_P1.27mm" H 4575 3150 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/opa2277.pdf" H 4575 3150 50  0001 C CNN
	1    4575 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4875 3150 5025 3150
Wire Wire Line
	5325 3350 5325 3425
Wire Wire Line
	5325 3425 4200 3425
Wire Wire Line
	4200 3425 4200 3250
Wire Wire Line
	4200 3250 4275 3250
Wire Wire Line
	5325 3475 5325 3425
Connection ~ 5325 3425
$Comp
L adafruit-new:LED3MM LED1
U 1 1 5FD9907B
P 5325 3950
F 0 "LED1" H 5402 3942 45  0000 L CNN
F 1 "LED3MM" H 5402 3858 45  0000 L CNN
F 2 "adafruit-new_LED3MM" H 5355 4100 20  0001 C CNN
F 3 "" H 5325 3950 50  0001 C CNN
	1    5325 3950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5325 3775 5325 3850
Wire Wire Line
	5325 4150 5325 4250
Wire Wire Line
	4975 4450 5025 4450
$Comp
L power:GND #PWR03
U 1 1 5FD9D426
P 4075 4725
F 0 "#PWR03" H 4075 4475 50  0001 C CNN
F 1 "GND" H 4080 4552 50  0000 C CNN
F 2 "" H 4075 4725 50  0001 C CNN
F 3 "" H 4075 4725 50  0001 C CNN
	1    4075 4725
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5FD9E562
P 3350 5775
F 0 "C1" H 3465 5821 50  0000 L CNN
F 1 "100nF" H 3465 5730 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3388 5625 50  0001 C CNN
F 3 "~" H 3350 5775 50  0001 C CNN
	1    3350 5775
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_Photo_NPN Q3
U 1 1 5FDABAC1
P 5325 6325
F 0 "Q3" H 5515 6371 50  0000 L CNN
F 1 "Q_Photo_NPN" H 5515 6280 50  0000 L CNN
F 2 "" H 5525 6425 50  0001 C CNN
F 3 "~" H 5325 6325 50  0001 C CNN
	1    5325 6325
	-1   0    0    -1  
$EndComp
Text GLabel 4225 5925 2    50   Input ~ 0
Vout
Wire Wire Line
	4225 5925 4025 5925
Connection ~ 4025 5925
Wire Wire Line
	4025 5925 4025 5900
Text GLabel 2775 5900 2    50   Input ~ 0
Vout
Wire Wire Line
	2525 5900 2775 5900
Text GLabel 4550 4450 0    50   Input ~ 0
LEDon
Wire Wire Line
	4550 4450 4675 4450
Text GLabel 2775 5800 2    50   Input ~ 0
LEDon
Wire Wire Line
	2525 5800 2775 5800
Text GLabel 3075 3050 0    50   Input ~ 0
PWMin
Wire Wire Line
	3150 3050 3075 3050
Text GLabel 2775 5700 2    50   Input ~ 0
PWMin
Wire Wire Line
	2775 5700 2525 5700
Wire Wire Line
	3000 5600 3350 5600
Connection ~ 3000 5600
Wire Wire Line
	3350 5600 3350 5625
$Comp
L Connector:Conn_01x02_Female J2
U 1 1 5FDD5DC1
P 4225 6125
F 0 "J2" H 4253 6101 50  0000 L CNN
F 1 "Conn_01x02_Female" H 4253 6010 50  0000 L CNN
F 2 "" H 4225 6125 50  0001 C CNN
F 3 "~" H 4225 6125 50  0001 C CNN
	1    4225 6125
	1    0    0    -1  
$EndComp
Wire Wire Line
	4025 5925 4025 6125
Wire Wire Line
	4025 6225 4025 6450
$Comp
L Connector:Conn_01x05_Female J1
U 1 1 5FDD826E
P 2325 5800
F 0 "J1" H 2217 5375 50  0000 C CNN
F 1 "Conn_01x05_Female" H 2217 5466 50  0000 C CNN
F 2 "" H 2325 5800 50  0001 C CNN
F 3 "~" H 2325 5800 50  0001 C CNN
	1    2325 5800
	-1   0    0    1   
$EndComp
Wire Wire Line
	2525 6000 3000 6000
Wire Wire Line
	3350 6000 3350 5925
Connection ~ 3000 6000
Wire Wire Line
	3000 6000 3350 6000
Wire Wire Line
	3000 6000 3000 6300
$Comp
L Connector:Conn_01x02_Male J3
U 1 1 5FDDFFF1
P 4750 6225
F 0 "J3" H 4722 6107 50  0000 R CNN
F 1 "Conn_01x02_Male" H 4722 6198 50  0000 R CNN
F 2 "" H 4750 6225 50  0001 C CNN
F 3 "~" H 4750 6225 50  0001 C CNN
	1    4750 6225
	1    0    0    1   
$EndComp
Wire Wire Line
	4950 6125 5225 6125
Wire Wire Line
	4950 6225 4950 6525
Wire Wire Line
	4950 6525 5225 6525
$Comp
L Device:R R6
U 1 1 5FDE88F2
P 5325 3625
F 0 "R6" H 5255 3579 50  0000 R CNN
F 1 "100R" H 5255 3670 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5255 3625 50  0001 C CNN
F 3 "~" H 5325 3625 50  0001 C CNN
	1    5325 3625
	-1   0    0    1   
$EndComp
$Comp
L Device:R R3
U 1 1 5FDE904A
P 4025 5750
F 0 "R3" H 3955 5704 50  0000 R CNN
F 1 "3K3" H 3955 5795 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3955 5750 50  0001 C CNN
F 3 "~" H 4025 5750 50  0001 C CNN
	1    4025 5750
	-1   0    0    1   
$EndComp
$Comp
L Transistor_FET:BS170F Q2
U 1 1 5FDED525
P 5225 4450
F 0 "Q2" H 5430 4496 50  0000 L CNN
F 1 "BS170F" H 5430 4405 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5425 4375 50  0001 L CIN
F 3 "http://www.diodes.com/assets/Datasheets/BS170F.pdf" H 5225 4450 50  0001 L CNN
	1    5225 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5FDF6358
P 4825 4450
F 0 "R5" V 5032 4450 50  0000 C CNN
F 1 "10K" V 4941 4450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4755 4450 50  0001 C CNN
F 3 "~" H 4825 4450 50  0001 C CNN
	1    4825 4450
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_BJT:MMBT3904 Q1
U 1 1 5FDF7F6A
P 5225 3150
F 0 "Q1" H 5416 3196 50  0000 L CNN
F 1 "MMBT3904" H 5416 3105 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5425 3075 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 5225 3150 50  0001 L CNN
	1    5225 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5325 4650 5325 4725
Wire Wire Line
	3350 5600 3350 5400
Wire Wire Line
	3350 5400 4025 5400
Connection ~ 3350 5600
Wire Wire Line
	4025 5400 4025 5600
Wire Wire Line
	3350 6000 3350 6450
Wire Wire Line
	3350 6450 4025 6450
Connection ~ 3350 6000
$Comp
L Device:R R2
U 1 1 5FDA4DE9
P 3875 3050
F 0 "R2" V 4082 3050 50  0000 C CNN
F 1 "120K" V 3991 3050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3805 3050 50  0001 C CNN
F 3 "~" H 3875 3050 50  0001 C CNN
	1    3875 3050
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R4
U 1 1 5FDA55DC
P 4150 2800
F 0 "R4" H 4080 2754 50  0000 R CNN
F 1 "330K" H 4080 2845 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4080 2800 50  0001 C CNN
F 3 "~" H 4150 2800 50  0001 C CNN
	1    4150 2800
	-1   0    0    1   
$EndComp
Wire Wire Line
	4025 3050 4150 3050
Wire Wire Line
	3600 3050 3725 3050
Connection ~ 3600 3050
Wire Wire Line
	4150 2950 4150 3050
Connection ~ 4150 3050
Wire Wire Line
	4150 3050 4275 3050
$Comp
L power:+5V #PWR04
U 1 1 5FDA7E5E
P 4575 2575
F 0 "#PWR04" H 4575 2425 50  0001 C CNN
F 1 "+5V" H 4590 2748 50  0000 C CNN
F 2 "" H 4575 2575 50  0001 C CNN
F 3 "" H 4575 2575 50  0001 C CNN
	1    4575 2575
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 2575 4150 2650
Wire Wire Line
	4150 2575 4575 2575
Wire Wire Line
	4575 2575 5325 2575
Wire Wire Line
	5325 2575 5325 2950
Connection ~ 4575 2575
Wire Wire Line
	5325 4725 4075 4725
Wire Wire Line
	3600 4725 4075 4725
Wire Wire Line
	3600 3425 3600 4725
Connection ~ 4075 4725
$EndSCHEMATC
