Holiday-Magic-Lights code

All recent developments are in ./HolidayLights/led_ir

Developed on Aquino IDE 1.05 and Arduino IDE 1.5.6r2

Todo:
    Add Basic Animations
        Scrolling
        Twinkling
        Rainbow
    Add more colors
	Add IR capability
	Fix even-number separation modes

NOTES:
Writing to strip of 144 length is 1.25ms
Writing to strip of 045 length is 1.35ms (what?)
Writing to strip of 189 length is 1.58ms
Writing to strip of 300 length is 1.85ms (specified maximum)

MODES:
0 - Off
1 - Separation Mode (Including Chaser Mode)
2 - Solid Mode
3 - Rainbow Mode
4 - Blink Mode
5 - Twinkle Mode

SPECIAL CONTROLS:
Special features activated through mode button (5th row, 2nd column):

1.
In Separation Mode, Switches through seperation modes (Starts at 1, goes to 2, then 3, then 0, then 1 again)
Mode number is now many times each color occurs.
!! Separation mode 0 is chaser mode. !!

2.
In Solid Mode, cycles through preset strip lengths. IN DEVELOPMENT FIRMWARE, THIS IS:
	45 long, 144 long, 189 long, then 45 long again.
THIS WILL BE CHANGED TO INTERVALS OF 60 FOR PRODUCTION?
