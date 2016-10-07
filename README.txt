CSCI441, Computer Graphics, Fall 2016
Name: Stephen Tracy
Email: stracy@mines.edu
Assignment 4 - How to Train Your Pegasus

Contains code for a vehicle for my character, with passive animation, animation based on keyboard input_iterator
And utilizes an arcball style camera centered on the vehicle. User can change car heading and position with WSAD
And the camera can be moved by clicking and dragging. Zooming is adjusted by ctrl clicking and dragging

Additionally, there is now a bezier curve and control grid displayed around the vehicle, with a sprite continuously moving
along this curve. The sprite is animated itself and always moves regardlesss of car movement, but the grid,curves and sprite
move with the car. Viewing the grid and the bezier curve can be toggled on and off via right clicking.

Usage: 
    Pressing 'w' moves the car forward in its heading, s backwards
	Pressing 'a' rotates the cars heading to the left, 'd' to the right
    Left click and dragging the mouse moves the camera around (always looking at car)
	Ctrl Click and dragging the mouse zooms the camera in and out
    The user can also press the 'q' key to quit the program, or escape etc.
	Right clicking gives options to quit, toggle the control cage of the bezier curve, and toggle the bezier curve being displayed
	
	IMPORTANT: the intended file to be passed to the .exe in the console call is spriteControlPoints2.csv

Compilation Instructions:
    Simply navigate to the directory and type 'make' <filename.csv> or 'gmake' <filename.csv>
	Note there are many .o objects that must be included in the makefile

Notes: None

Time taken on assignment: roughly 3 or 4 hours

Lab Effectiveness 10/10: the lab was super helpful and solidifed the concepts of bezier curves. I was able to reuse alot of the code from the lab too.

Fun: 10/10, I really wish I had more time to make it even better! 