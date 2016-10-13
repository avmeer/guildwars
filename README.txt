CSCI441, Computer Graphics, Fall 2016
Name: Stephen Tracy
Email: stracy@mines.edu
Midterm Project: Guild Wars

Brief Description:

	Program contains 3 hero characters which can all be followed with either an arcball cam or a first person cam (which appears as a picture in picture viewport).
	One hero appears as the wanderer which can move along a bezier surface (and follow the surface orientation), while the other two heros follow a roller coaster closed
	loop track. One of the track heros simply moves at constant step sizes while the other hero moves along the curve utilizing arc length parameterization to go along the 
	track at equal distance intervals.

Usage: 
    Pressing 'w' moves the car forward in its heading, 's' backwards
	Pressing 'a' rotates the cars heading to the left, 'd' to the right

	Pressing 'i' moves the free cam forward in its heading, 'k' backwards

    Left click and dragging the mouse moves the camera around (arcball centered around object, freecam changes heading)
	Ctrl Click and dragging the mouse zooms the camera in and out while in arcball mode

    The user can also press the 'q' key to quit the program, or escape etc.

    Right clicking yields a menu for toggling camera modes/hero to follow etc.

	
	IMPORTANT: the intended file to be passed to the .exe in the console call is worldFile.txt

Compilation Instructions:

    Simply navigate to the directory and type 'make' <filename.txt> or 'gmake' <filename.txt>
	Note there are many .o objects that must be included in the makefile

Notes: 
We did the extra credit by adding in sound to the Oghad Urr car and the hero3 (Schmitty).
We also added in lighting and did some material manipulation on the teapot.

Description of world file format:

<16 bezier surface points x,y,z>
<Number of control points for track bezier curve>
<x,y,z for control points>
<Number of world objects>
<type of object as character>,<x,z>

e.g.
0.000000,0.006256,0.000000
 ...(more points)
13
75.000000,35.000000,0.000000
 ...(more points)
2
t,25,25
c,-25,-25

Description of student contributions: 

We pretty much pair programmed the whole thing. Each student was responsible for making their own hero class work with the camera etc but the hard stuff such as the bezier surface calculations (drawing, getting tangents) and arc length parametrization was all done together on one computer.

Time taken on assignment: OVER 9000 hours!!!! Roughly 30 hours.

Lab Effectiveness 8/10: More practice with bezier surfaces beforehand would have been nice, the rest was pretty similar to previous labs.

Fun: 10/10, but hard.