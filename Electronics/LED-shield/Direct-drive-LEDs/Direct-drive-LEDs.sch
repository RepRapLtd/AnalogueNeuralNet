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
L Device:R R1
U 1 1 5FD8EFBA
P 1350 1400
F 0 "R1" V 1143 1400 50  0000 C CNN
F 1 "33K" V 1234 1400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1280 1400 50  0001 C CNN
F 3 "~" H 1350 1400 50  0001 C CNN
	1    1350 1400
	0    1    1    0   
$EndComp
$Comp
L Device:C C1
U 1 1 5FD8F543
P 1650 1625
F 0 "C1" H 1765 1671 50  0000 L CNN
F 1 "220nF" H 1765 1580 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1688 1475 50  0001 C CNN
F 3 "~" H 1650 1625 50  0001 C CNN
	1    1650 1625
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 1400 1650 1400
Wire Wire Line
	1650 1400 1650 1475
Wire Wire Line
	2925 1500 3075 1500
Wire Wire Line
	3375 1700 3375 1775
Wire Wire Line
	3375 1775 2250 1775
Wire Wire Line
	2250 1775 2250 1600
Wire Wire Line
	2250 1600 2325 1600
Wire Wire Line
	3375 1825 3375 1775
Connection ~ 3375 1775
Wire Wire Line
	3375 2125 3375 2200
Wire Wire Line
	3375 2500 3375 2600
Wire Wire Line
	3025 2800 3075 2800
$Comp
L power:GND #PWR01
U 1 1 5FD9D426
P 2125 3075
F 0 "#PWR01" H 2125 2825 50  0001 C CNN
F 1 "GND" H 2130 2902 50  0000 C CNN
F 2 "" H 2125 3075 50  0001 C CNN
F 3 "" H 2125 3075 50  0001 C CNN
	1    2125 3075
	1    0    0    -1  
$EndComp
Text GLabel 1125 2800 0    50   Input ~ 0
LEDon0
Text GLabel 1125 1400 0    50   Input ~ 0
PWMin0
Wire Wire Line
	1200 1400 1125 1400
$Comp
L Device:R R6
U 1 1 5FDE88F2
P 3375 1975
F 0 "R6" H 3305 1929 50  0000 R CNN
F 1 "100R" H 3305 2020 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3305 1975 50  0001 C CNN
F 3 "~" H 3375 1975 50  0001 C CNN
	1    3375 1975
	-1   0    0    1   
$EndComp
$Comp
L Transistor_FET:BS170F Q2
U 1 1 5FDED525
P 3275 2800
F 0 "Q2" H 3480 2846 50  0000 L CNN
F 1 "BS170F" H 3480 2755 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3475 2725 50  0001 L CIN
F 3 "http://www.diodes.com/assets/Datasheets/BS170F.pdf" H 3275 2800 50  0001 L CNN
	1    3275 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5FDF6358
P 2875 2800
F 0 "R4" V 3082 2800 50  0000 C CNN
F 1 "10K" V 2991 2800 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2805 2800 50  0001 C CNN
F 3 "~" H 2875 2800 50  0001 C CNN
	1    2875 2800
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_BJT:MMBT3904 Q1
U 1 1 5FDF7F6A
P 3275 1500
F 0 "Q1" H 3466 1546 50  0000 L CNN
F 1 "MMBT3904" H 3466 1455 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3475 1425 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 3275 1500 50  0001 L CNN
	1    3275 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3375 3000 3375 3075
$Comp
L Device:R R2
U 1 1 5FDA4DE9
P 1925 1400
F 0 "R2" V 2132 1400 50  0000 C CNN
F 1 "120K" V 2041 1400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1855 1400 50  0001 C CNN
F 3 "~" H 1925 1400 50  0001 C CNN
	1    1925 1400
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R3
U 1 1 5FDA55DC
P 2200 1150
F 0 "R3" H 2130 1104 50  0000 R CNN
F 1 "330K" H 2130 1195 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2130 1150 50  0001 C CNN
F 3 "~" H 2200 1150 50  0001 C CNN
	1    2200 1150
	-1   0    0    1   
$EndComp
Wire Wire Line
	2075 1400 2200 1400
Wire Wire Line
	1650 1400 1775 1400
Connection ~ 1650 1400
Wire Wire Line
	2200 1300 2200 1400
Connection ~ 2200 1400
Wire Wire Line
	2200 1400 2325 1400
$Comp
L power:+5V #PWR04
U 1 1 5FDA7E5E
P 2625 925
F 0 "#PWR04" H 2625 775 50  0001 C CNN
F 1 "+5V" H 2640 1098 50  0000 C CNN
F 2 "" H 2625 925 50  0001 C CNN
F 3 "" H 2625 925 50  0001 C CNN
	1    2625 925 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 925  2200 1000
Wire Wire Line
	2200 925  2625 925 
Wire Wire Line
	2625 925  3375 925 
Wire Wire Line
	3375 925  3375 1300
Connection ~ 2625 925 
Wire Wire Line
	3375 3075 2125 3075
Wire Wire Line
	1650 3075 2125 3075
Wire Wire Line
	1650 1775 1650 3075
Connection ~ 2125 3075
Wire Wire Line
	1125 2800 2725 2800
$Comp
L Device:R R7
U 1 1 5FF07AC1
P 4950 1400
F 0 "R7" V 4743 1400 50  0000 C CNN
F 1 "33K" V 4834 1400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4880 1400 50  0001 C CNN
F 3 "~" H 4950 1400 50  0001 C CNN
	1    4950 1400
	0    1    1    0   
$EndComp
$Comp
L Device:C C3
U 1 1 5FF07AC7
P 5250 1625
F 0 "C3" H 5365 1671 50  0000 L CNN
F 1 "220nF" H 5365 1580 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5288 1475 50  0001 C CNN
F 3 "~" H 5250 1625 50  0001 C CNN
	1    5250 1625
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 1400 5250 1400
Wire Wire Line
	5250 1400 5250 1475
Wire Wire Line
	6525 1500 6675 1500
Wire Wire Line
	6975 1700 6975 1775
Wire Wire Line
	6975 1775 5850 1775
Wire Wire Line
	5850 1775 5850 1600
Wire Wire Line
	5850 1600 5925 1600
Wire Wire Line
	6975 1825 6975 1775
Connection ~ 6975 1775
Wire Wire Line
	6975 2125 6975 2200
Wire Wire Line
	6975 2500 6975 2600
Wire Wire Line
	6625 2800 6675 2800
$Comp
L power:GND #PWR05
U 1 1 5FF07AE5
P 5725 3075
F 0 "#PWR05" H 5725 2825 50  0001 C CNN
F 1 "GND" H 5730 2902 50  0000 C CNN
F 2 "" H 5725 3075 50  0001 C CNN
F 3 "" H 5725 3075 50  0001 C CNN
	1    5725 3075
	1    0    0    -1  
$EndComp
Text GLabel 4725 2800 0    50   Input ~ 0
LEDon1
Text GLabel 4725 1400 0    50   Input ~ 0
PWMin1
Wire Wire Line
	4800 1400 4725 1400
$Comp
L Device:R R11
U 1 1 5FF07AEE
P 6975 1975
F 0 "R11" H 6905 1929 50  0000 R CNN
F 1 "100R" H 6905 2020 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6905 1975 50  0001 C CNN
F 3 "~" H 6975 1975 50  0001 C CNN
	1    6975 1975
	-1   0    0    1   
$EndComp
$Comp
L Transistor_FET:BS170F Q4
U 1 1 5FF07AF4
P 6875 2800
F 0 "Q4" H 7080 2846 50  0000 L CNN
F 1 "BS170F" H 7080 2755 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7075 2725 50  0001 L CIN
F 3 "http://www.diodes.com/assets/Datasheets/BS170F.pdf" H 6875 2800 50  0001 L CNN
	1    6875 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 5FF07AFA
P 6475 2800
F 0 "R10" V 6682 2800 50  0000 C CNN
F 1 "10K" V 6591 2800 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6405 2800 50  0001 C CNN
F 3 "~" H 6475 2800 50  0001 C CNN
	1    6475 2800
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_BJT:MMBT3904 Q3
U 1 1 5FF07B00
P 6875 1500
F 0 "Q3" H 7066 1546 50  0000 L CNN
F 1 "MMBT3904" H 7066 1455 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7075 1425 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 6875 1500 50  0001 L CNN
	1    6875 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6975 3000 6975 3075
$Comp
L Device:R R8
U 1 1 5FF07B07
P 5525 1400
F 0 "R8" V 5732 1400 50  0000 C CNN
F 1 "120K" V 5641 1400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5455 1400 50  0001 C CNN
F 3 "~" H 5525 1400 50  0001 C CNN
	1    5525 1400
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R9
U 1 1 5FF07B0D
P 5800 1150
F 0 "R9" H 5730 1104 50  0000 R CNN
F 1 "330K" H 5730 1195 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5730 1150 50  0001 C CNN
F 3 "~" H 5800 1150 50  0001 C CNN
	1    5800 1150
	-1   0    0    1   
$EndComp
Wire Wire Line
	5675 1400 5800 1400
Wire Wire Line
	5250 1400 5375 1400
Connection ~ 5250 1400
Wire Wire Line
	5800 1300 5800 1400
Connection ~ 5800 1400
Wire Wire Line
	5800 1400 5925 1400
$Comp
L power:+5V #PWR06
U 1 1 5FF07B19
P 6225 925
F 0 "#PWR06" H 6225 775 50  0001 C CNN
F 1 "+5V" H 6240 1098 50  0000 C CNN
F 2 "" H 6225 925 50  0001 C CNN
F 3 "" H 6225 925 50  0001 C CNN
	1    6225 925 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 925  5800 1000
Wire Wire Line
	5800 925  6225 925 
Wire Wire Line
	6225 925  6975 925 
Wire Wire Line
	6975 925  6975 1300
Connection ~ 6225 925 
Wire Wire Line
	6975 3075 5725 3075
Wire Wire Line
	5250 3075 5725 3075
Wire Wire Line
	5250 1775 5250 3075
Connection ~ 5725 3075
Wire Wire Line
	4725 2800 6325 2800
$Comp
L Device:R R13
U 1 1 5FF0E315
P 8275 1500
F 0 "R13" V 8068 1500 50  0000 C CNN
F 1 "33K" V 8159 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8205 1500 50  0001 C CNN
F 3 "~" H 8275 1500 50  0001 C CNN
	1    8275 1500
	0    1    1    0   
$EndComp
$Comp
L Device:C C5
U 1 1 5FF0E31B
P 8575 1725
F 0 "C5" H 8690 1771 50  0000 L CNN
F 1 "220nF" H 8690 1680 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 8613 1575 50  0001 C CNN
F 3 "~" H 8575 1725 50  0001 C CNN
	1    8575 1725
	1    0    0    -1  
$EndComp
Wire Wire Line
	8425 1500 8575 1500
Wire Wire Line
	8575 1500 8575 1575
Wire Wire Line
	9850 1600 10000 1600
Wire Wire Line
	10300 1800 10300 1875
Wire Wire Line
	10300 1875 9175 1875
Wire Wire Line
	9175 1875 9175 1700
Wire Wire Line
	9175 1700 9250 1700
Wire Wire Line
	10300 1925 10300 1875
Connection ~ 10300 1875
Wire Wire Line
	10300 2225 10300 2300
Wire Wire Line
	10300 2600 10300 2700
Wire Wire Line
	9950 2900 10000 2900
$Comp
L power:GND #PWR08
U 1 1 5FF0E339
P 9050 3175
F 0 "#PWR08" H 9050 2925 50  0001 C CNN
F 1 "GND" H 9055 3002 50  0000 C CNN
F 2 "" H 9050 3175 50  0001 C CNN
F 3 "" H 9050 3175 50  0001 C CNN
	1    9050 3175
	1    0    0    -1  
$EndComp
Text GLabel 8050 2900 0    50   Input ~ 0
LEDon2
Text GLabel 8050 1500 0    50   Input ~ 0
PWMin2
Wire Wire Line
	8125 1500 8050 1500
$Comp
L Device:R R21
U 1 1 5FF0E342
P 10300 2075
F 0 "R21" H 10230 2029 50  0000 R CNN
F 1 "100R" H 10230 2120 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10230 2075 50  0001 C CNN
F 3 "~" H 10300 2075 50  0001 C CNN
	1    10300 2075
	-1   0    0    1   
$EndComp
$Comp
L Transistor_FET:BS170F Q8
U 1 1 5FF0E348
P 10200 2900
F 0 "Q8" H 10405 2946 50  0000 L CNN
F 1 "BS170F" H 10405 2855 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 10400 2825 50  0001 L CIN
F 3 "http://www.diodes.com/assets/Datasheets/BS170F.pdf" H 10200 2900 50  0001 L CNN
	1    10200 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R19
U 1 1 5FF0E34E
P 9800 2900
F 0 "R19" V 10007 2900 50  0000 C CNN
F 1 "10K" V 9916 2900 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 9730 2900 50  0001 C CNN
F 3 "~" H 9800 2900 50  0001 C CNN
	1    9800 2900
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_BJT:MMBT3904 Q7
U 1 1 5FF0E354
P 10200 1600
F 0 "Q7" H 10391 1646 50  0000 L CNN
F 1 "MMBT3904" H 10391 1555 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 10400 1525 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 10200 1600 50  0001 L CNN
	1    10200 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 3100 10300 3175
$Comp
L Device:R R15
U 1 1 5FF0E35B
P 8850 1500
F 0 "R15" V 9057 1500 50  0000 C CNN
F 1 "120K" V 8966 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8780 1500 50  0001 C CNN
F 3 "~" H 8850 1500 50  0001 C CNN
	1    8850 1500
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R17
U 1 1 5FF0E361
P 9125 1250
F 0 "R17" H 9055 1204 50  0000 R CNN
F 1 "330K" H 9055 1295 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 9055 1250 50  0001 C CNN
F 3 "~" H 9125 1250 50  0001 C CNN
	1    9125 1250
	-1   0    0    1   
$EndComp
Wire Wire Line
	9000 1500 9125 1500
Wire Wire Line
	8575 1500 8700 1500
Connection ~ 8575 1500
Wire Wire Line
	9125 1400 9125 1500
Connection ~ 9125 1500
Wire Wire Line
	9125 1500 9250 1500
$Comp
L power:+5V #PWR010
U 1 1 5FF0E36D
P 9550 1025
F 0 "#PWR010" H 9550 875 50  0001 C CNN
F 1 "+5V" H 9565 1198 50  0000 C CNN
F 2 "" H 9550 1025 50  0001 C CNN
F 3 "" H 9550 1025 50  0001 C CNN
	1    9550 1025
	1    0    0    -1  
$EndComp
Wire Wire Line
	9125 1025 9125 1100
Wire Wire Line
	9125 1025 9550 1025
Wire Wire Line
	9550 1025 10300 1025
Wire Wire Line
	10300 1025 10300 1400
Connection ~ 9550 1025
Wire Wire Line
	10300 3175 9050 3175
Wire Wire Line
	8575 3175 9050 3175
Wire Wire Line
	8575 1875 8575 3175
Connection ~ 9050 3175
Wire Wire Line
	8050 2900 9650 2900
$Comp
L Device:R R12
U 1 1 5FF151DF
P 8250 4250
F 0 "R12" V 8043 4250 50  0000 C CNN
F 1 "33K" V 8134 4250 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8180 4250 50  0001 C CNN
F 3 "~" H 8250 4250 50  0001 C CNN
	1    8250 4250
	0    1    1    0   
$EndComp
$Comp
L Device:C C4
U 1 1 5FF151E5
P 8550 4475
F 0 "C4" H 8665 4521 50  0000 L CNN
F 1 "220nF" H 8665 4430 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 8588 4325 50  0001 C CNN
F 3 "~" H 8550 4475 50  0001 C CNN
	1    8550 4475
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 4250 8550 4250
Wire Wire Line
	8550 4250 8550 4325
Wire Wire Line
	9825 4350 9975 4350
Wire Wire Line
	10275 4550 10275 4625
Wire Wire Line
	10275 4625 9150 4625
Wire Wire Line
	9150 4625 9150 4450
Wire Wire Line
	9150 4450 9225 4450
Wire Wire Line
	10275 4675 10275 4625
Connection ~ 10275 4625
Wire Wire Line
	10275 4975 10275 5050
Wire Wire Line
	10275 5350 10275 5450
Wire Wire Line
	9925 5650 9975 5650
$Comp
L power:GND #PWR07
U 1 1 5FF15203
P 9025 5925
F 0 "#PWR07" H 9025 5675 50  0001 C CNN
F 1 "GND" H 9030 5752 50  0000 C CNN
F 2 "" H 9025 5925 50  0001 C CNN
F 3 "" H 9025 5925 50  0001 C CNN
	1    9025 5925
	1    0    0    -1  
$EndComp
Text GLabel 8025 5650 0    50   Input ~ 0
LEDon3
Text GLabel 8025 4250 0    50   Input ~ 0
PWMin3
Wire Wire Line
	8100 4250 8025 4250
$Comp
L Device:R R20
U 1 1 5FF1520C
P 10275 4825
F 0 "R20" H 10205 4779 50  0000 R CNN
F 1 "100R" H 10205 4870 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10205 4825 50  0001 C CNN
F 3 "~" H 10275 4825 50  0001 C CNN
	1    10275 4825
	-1   0    0    1   
$EndComp
$Comp
L Transistor_FET:BS170F Q6
U 1 1 5FF15212
P 10175 5650
F 0 "Q6" H 10380 5696 50  0000 L CNN
F 1 "BS170F" H 10380 5605 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 10375 5575 50  0001 L CIN
F 3 "http://www.diodes.com/assets/Datasheets/BS170F.pdf" H 10175 5650 50  0001 L CNN
	1    10175 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R18
U 1 1 5FF15218
P 9775 5650
F 0 "R18" V 9982 5650 50  0000 C CNN
F 1 "10K" V 9891 5650 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 9705 5650 50  0001 C CNN
F 3 "~" H 9775 5650 50  0001 C CNN
	1    9775 5650
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_BJT:MMBT3904 Q5
U 1 1 5FF1521E
P 10175 4350
F 0 "Q5" H 10366 4396 50  0000 L CNN
F 1 "MMBT3904" H 10366 4305 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 10375 4275 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 10175 4350 50  0001 L CNN
	1    10175 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	10275 5850 10275 5925
$Comp
L Device:R R14
U 1 1 5FF15225
P 8825 4250
F 0 "R14" V 9032 4250 50  0000 C CNN
F 1 "120K" V 8941 4250 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8755 4250 50  0001 C CNN
F 3 "~" H 8825 4250 50  0001 C CNN
	1    8825 4250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R16
U 1 1 5FF1522B
P 9100 4000
F 0 "R16" H 9030 3954 50  0000 R CNN
F 1 "330K" H 9030 4045 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 9030 4000 50  0001 C CNN
F 3 "~" H 9100 4000 50  0001 C CNN
	1    9100 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	8975 4250 9100 4250
Wire Wire Line
	8550 4250 8675 4250
Connection ~ 8550 4250
Wire Wire Line
	9100 4150 9100 4250
Connection ~ 9100 4250
Wire Wire Line
	9100 4250 9225 4250
$Comp
L power:+5V #PWR09
U 1 1 5FF15237
P 9525 3775
F 0 "#PWR09" H 9525 3625 50  0001 C CNN
F 1 "+5V" H 9540 3948 50  0000 C CNN
F 2 "" H 9525 3775 50  0001 C CNN
F 3 "" H 9525 3775 50  0001 C CNN
	1    9525 3775
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 3775 9100 3850
Wire Wire Line
	9100 3775 9525 3775
Wire Wire Line
	9525 3775 10275 3775
Wire Wire Line
	10275 3775 10275 4150
Connection ~ 9525 3775
Wire Wire Line
	10275 5925 9025 5925
Wire Wire Line
	8550 5925 9025 5925
Wire Wire Line
	8550 4625 8550 5925
Connection ~ 9025 5925
Wire Wire Line
	8025 5650 9625 5650
$Comp
L Device:C C2
U 1 1 5FF1F3F6
P 1275 6775
F 0 "C2" H 1390 6821 50  0000 L CNN
F 1 "100nF" H 1390 6730 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1313 6625 50  0001 C CNN
F 3 "~" H 1275 6775 50  0001 C CNN
	1    1275 6775
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5FF294B9
P 1100 7375
F 0 "#PWR02" H 1100 7125 50  0001 C CNN
F 1 "GND" H 1105 7202 50  0000 C CNN
F 2 "" H 1100 7375 50  0001 C CNN
F 3 "" H 1100 7375 50  0001 C CNN
	1    1100 7375
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR03
U 1 1 5FF29B2E
P 1100 6225
F 0 "#PWR03" H 1100 6075 50  0001 C CNN
F 1 "+5V" H 1115 6398 50  0000 C CNN
F 2 "" H 1100 6225 50  0001 C CNN
F 3 "" H 1100 6225 50  0001 C CNN
	1    1100 6225
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5FF3EC21
P 1625 6475
F 0 "R5" H 1555 6429 50  0000 R CNN
F 1 "3K3" H 1555 6520 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1555 6475 50  0001 C CNN
F 3 "~" H 1625 6475 50  0001 C CNN
	1    1625 6475
	-1   0    0    1   
$EndComp
$Comp
L Amplifier_Operational:OP275 U1
U 1 1 5FF96DDF
P 2625 1500
F 0 "U1" H 2625 1867 50  0000 C CNN
F 1 "OP275" H 2625 1776 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 2625 1500 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/OP275.pdf" H 2625 1500 50  0001 C CNN
	1    2625 1500
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:OP275 U1
U 2 1 5FFA949E
P 6225 1500
F 0 "U1" H 6225 1867 50  0000 C CNN
F 1 "OP275" H 6225 1776 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 6225 1500 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/OP275.pdf" H 6225 1500 50  0001 C CNN
	2    6225 1500
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:OP275 U2
U 1 1 5FFAB683
P 9550 1600
F 0 "U2" H 9550 1967 50  0000 C CNN
F 1 "OP275" H 9550 1876 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 9550 1600 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/OP275.pdf" H 9550 1600 50  0001 C CNN
	1    9550 1600
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:OP275 U2
U 2 1 5FFADA40
P 9525 4350
F 0 "U2" H 9525 4717 50  0000 C CNN
F 1 "OP275" H 9525 4626 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 9525 4350 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/OP275.pdf" H 9525 4350 50  0001 C CNN
	2    9525 4350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 5FFBC0A6
P 900 6775
F 0 "C6" H 1015 6821 50  0000 L CNN
F 1 "100nF" H 1015 6730 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 938 6625 50  0001 C CNN
F 3 "~" H 900 6775 50  0001 C CNN
	1    900  6775
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5FEFE37C
P 3375 2350
F 0 "D1" V 3414 2232 50  0000 R CNN
F 1 "LED" V 3323 2232 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 3375 2350 50  0001 C CNN
F 3 "~" H 3375 2350 50  0001 C CNN
	1    3375 2350
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D2
U 1 1 5FF0B296
P 6975 2350
F 0 "D2" V 7014 2232 50  0000 R CNN
F 1 "LED" V 6923 2232 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 6975 2350 50  0001 C CNN
F 3 "~" H 6975 2350 50  0001 C CNN
	1    6975 2350
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D4
U 1 1 5FF0BFF0
P 10300 2450
F 0 "D4" V 10339 2332 50  0000 R CNN
F 1 "LED" V 10248 2332 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 10300 2450 50  0001 C CNN
F 3 "~" H 10300 2450 50  0001 C CNN
	1    10300 2450
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D3
U 1 1 5FF0CD3C
P 10275 5200
F 0 "D3" V 10314 5082 50  0000 R CNN
F 1 "LED" V 10223 5082 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 10275 5200 50  0001 C CNN
F 3 "~" H 10275 5200 50  0001 C CNN
	1    10275 5200
	0    -1   -1   0   
$EndComp
Text GLabel 1750 6775 2    50   Input ~ 0
photoC
Text GLabel 1750 7050 2    50   Input ~ 0
PhotoE
Wire Wire Line
	1750 6775 1625 6775
Wire Wire Line
	1750 7050 1625 7050
Wire Wire Line
	1625 7050 1625 7325
Wire Wire Line
	1625 6625 1625 6775
Wire Wire Line
	1100 6225 1100 6325
Connection ~ 1100 6325
Wire Wire Line
	1100 6325 1275 6325
Wire Wire Line
	900  6625 900  6325
Wire Wire Line
	900  6325 1100 6325
Wire Wire Line
	1275 6625 1275 6325
Connection ~ 1275 6325
Wire Wire Line
	1275 6325 1625 6325
Wire Wire Line
	1275 6925 1275 7325
Connection ~ 1275 7325
Wire Wire Line
	1275 7325 1625 7325
Wire Wire Line
	900  6925 900  7325
Wire Wire Line
	900  7325 1100 7325
$Comp
L arduino:Arduino_Mega2560_Shield XA1
U 1 1 5FFCB39A
P 4500 5025
F 0 "XA1" V 4447 2697 60  0000 R CNN
F 1 "Arduino_Mega2560_Shield" V 4553 2697 60  0000 R CNN
F 2 "arduino:Arduino_Mega2560_Shield" H 5200 7775 60  0001 C CNN
F 3 "https://store.arduino.cc/arduino-mega-2560-rev3" H 5200 7775 60  0001 C CNN
	1    4500 5025
	0    1    1    0   
$EndComp
Wire Wire Line
	2450 3725 2550 3725
Wire Wire Line
	2550 3725 2600 3725
Connection ~ 2550 3725
$Comp
L power:+5V #PWR0101
U 1 1 5FFDBD59
P 2600 3625
F 0 "#PWR0101" H 2600 3475 50  0001 C CNN
F 1 "+5V" H 2615 3798 50  0000 C CNN
F 2 "" H 2600 3625 50  0001 C CNN
F 3 "" H 2600 3625 50  0001 C CNN
	1    2600 3625
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3625 2600 3725
Connection ~ 2600 3725
Wire Wire Line
	2600 3725 2650 3725
Wire Wire Line
	2850 3725 2950 3725
Wire Wire Line
	2950 3725 3000 3725
Connection ~ 2950 3725
Wire Wire Line
	3050 3725 3150 3725
Connection ~ 3050 3725
Wire Wire Line
	3150 3725 3250 3725
Connection ~ 3150 3725
Wire Wire Line
	1100 7375 1100 7325
Connection ~ 1100 7325
Wire Wire Line
	1100 7325 1275 7325
$Comp
L power:GND #PWR0102
U 1 1 5FFF80E0
P 3000 3600
F 0 "#PWR0102" H 3000 3350 50  0001 C CNN
F 1 "GND" H 3005 3427 50  0000 C CNN
F 2 "" H 3000 3600 50  0001 C CNN
F 3 "" H 3000 3600 50  0001 C CNN
	1    3000 3600
	-1   0    0    1   
$EndComp
Wire Wire Line
	3000 3600 3000 3725
Connection ~ 3000 3725
Wire Wire Line
	3000 3725 3050 3725
Text GLabel 3050 6525 3    50   Input ~ 0
PWMin0
Text GLabel 3150 6525 3    50   Input ~ 0
PWMin1
Text GLabel 3250 6525 3    50   Input ~ 0
PWMin2
Text GLabel 5550 6525 3    50   Input ~ 0
PWMin3
Text GLabel 5450 6525 3    50   Input ~ 0
LEDon0
Text GLabel 5350 6525 3    50   Input ~ 0
LEDon1
Text GLabel 5250 6525 3    50   Input ~ 0
LEDon2
Text GLabel 5150 6525 3    50   Input ~ 0
LEDon3
Text GLabel 5350 3625 1    50   Input ~ 0
photoC
Wire Wire Line
	5350 3725 5350 3625
Wire Wire Line
	3050 6525 3050 6325
Wire Wire Line
	3150 6525 3150 6325
Wire Wire Line
	3250 6525 3250 6325
Wire Wire Line
	5150 6525 5150 6325
Wire Wire Line
	5250 6525 5250 6325
Wire Wire Line
	5350 6525 5350 6325
Wire Wire Line
	5450 6525 5450 6325
Wire Wire Line
	5550 6525 5550 6325
$EndSCHEMATC
