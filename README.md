# NCORE: Arduino Native Core

## What is a 'Native Core'?

The native core allows you to compile and run Arduino sketches on the PC, generally with no modification.  It provides native versions of standard Arduino functions, and a command-line interepreter to give inputs to your sketch that would normally come from the hardware itself.

## Why is that useful?

You can full-fledged Arduino sketches without any hardware.  Perhaps you're away from your desk and didn't bring the hardware.  Maybe you're on a plane?  Or maybe the hardware isn't done yet.

You get a trace of exactly what your sketch is doing to the hardware and when it happened, to help debug your logic.

You can use the debugger on Mac & Linux where there is no WinAVR.

You can run automated tests using normal frameworks, such as cxxtest.

You can run a script which pretends to be the hardware and see how your sketch reacts.

## What do I need to use it?

NCORE runs on Linux and Mac.  I use it on Ubuntu 11.04 and Mac OSX 10.6 (Snow Leopard).

NCORE runs from the command-line.  It requires an understanding of build scripts and running build tools from the command-line.

Beyond that, the requirements are pretty simple.  It just needs gcc and the 'ftjam' build tool.

If you want to build the tests, you'll need cxxtest from http://cxxtest.tigris.org.  NCORE has been tested with cxxtest 3.10.1.

## How do I get it?

Clone it from github.  Create a 'hardware' directory off your sketches directory, and clone into 'native' under that.  For example...

$ cd ~/Source/Arduino/
$ mkdir hardware
$ cd hardware
$ git clone ... native

## How do I set it up?

Edit examples/Jamrules, and change the first line, SKETCH\_DIR to point to your Arduino sketch directory.

## How do I build it?

From the command line, cd to the 'examples' directory, and type:

$ jam Blink

This will build the Blink example.  You can build all the examples using:

$ jam -j10

Just be sure to have built ONE example first, or it takes forever to compile.

Also, the -j10 is optional.  It goes faster that way.

## How do I launch it?

Again, from the examples directory, after building Blink, type:

$ out/Blink

## How do I run it?

After a few seconds, type 'list' to see a log of activity.

Type 'help' for a complete list of commands.

## Do I have to modify my sketches?

Generally not.  To compile for the command line, you do need to forward declare any functions because the Arduino IDE magically does that for you, but there's no IDE to do it here.

Also there is a compiler define 'NATIVE' that you can use to add native-only chunks of code to your sketch.  Try to avoid that, but sometimes there's no way around it.
