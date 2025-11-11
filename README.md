# ThreadStorm
MeltyBrain Combat Robot Controller

Based off of NothingLabs' Rich Olson's melty brain robot controller @ https://github.com/nothinglabs/openmelt2
Modified to fit the Teensy 4.0 as it has an ARM processor, which is not explicitly supported by OpenMelt2

# What is a Melty Brain?

A melty brain / translational drift / tornado drive robot is a remote controlled device that moves based on the principal that pulsing motors whilst a spinning body rotates will cause it to move in a single direction.
Given that they are pulsed at the same time in each rotation.
It is colloquially called a "Melty Brain" due to trying to implement it or drive it will melt your brain

# How can it be driven?

In normal combat robotics, drive is simple, use a transmitter to control 2+ wheels like an RC car or tank.
For this application, more logic is required.
As humans cannot feasibly change how fast the motors spin to make a straight line at this RPM, a microcontroller on board is needed to simplify this.

To see in which direction the robot is "facing" at any given time, an LED is placed near the edge of the robot and flashes for a short time per rotation.
This is to imitate a line of LEDs pointing in 1 general direction.

Using both of these principles, we can drive a spinning mass somewhat controllably.

# Hardware
