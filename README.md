# NCORE: Arduino Native Core

## What is a 'Native Core'?

The native core allows you to compile and run Arduino sketches on the PC, generally with no modification.  It provides native versions of standard Arduino functions, and a command-line interepreter to give inputs to your sketch that would normally come from the hardware itself.

## Why is that useful?

You can...

* Run full-fledged Arduino sketches without any hardware.  Perhaps you're away from your desk and didn't bring the hardware.  Maybe you're on a plane?  Or maybe the hardware isn't done yet.

* Get a trace of exactly what your sketch is doing to the hardware and when it happened, to help debug your logic.

* Use the debugger on Mac & Linux where there is no WinAVR.

* Run automated tests using normal frameworks, such as cxxtest.

* Run a script which pretends to be the hardware and see how your sketch reacts.

* Make sure your sketch behaves properly under hardware error conditions which are impossible to trigger normally.  (e.g. bad SD media)

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

## How do I build the examples?

The examples include all examples shipped with core Arduino 0021, plus many libraries which are handled by NCORE.  All are working without modification,
except to forward-declare functions and fix bugs.  Some bugs became much more obvious once they were running under NCORE.

Edit examples/Jamrules, and change the first line, SKETCH\_DIR to point to your Arduino sketch directory.

From the command line, cd to the 'examples' directory, and type:

$ jam Blink

This will build the Blink example.  You can then build all the examples using:

$ jam -j10

Just be sure to have built ONE example first, or it takes forever to compile.

Also, the -j10 is optional.  It goes faster that way.

## How do I launch it?

Again, from the examples directory, after building Blink, type:

$ out/Blink

## How do I run it?

After a few seconds, type 'list' to see a log of activity.

Type 'help' for a complete list of commands.

Type 'quit' when you're done.

## Do I have to modify my sketches?

Generally not.  To compile for the command line, you do need to forward declare any functions because the Arduino IDE magically does that for you, and there's no IDE to do it here.

There is a compiler define 'NATIVE' which you can use to add native-only chunks of code to your sketch.  Try to avoid that, but sometimes there's no way around it.

## How do I build my own sketches?

When setting up a sketch for NCORE, it's important to maintain interoperability with the Arduino IDE.  Here's an example of how I set it up.

My "Sketch Directory" in the Arduino IDE is ~/Source/Arduino.  Let's say we're setting up the "HelloWorld" sketch.

1. Create the HelloWorld sketch in the IDE the normal way.
2. This creates ~/Source/Arduino/HelloWorld/HelloWorld.pde.
3. Create a 'native' directory underneath, e.g. ~/Source/Arduino/HelloWorld/native
4. Copy Jamrules from ~/Source/Arduino/hardware/native/examples/Jamrules into ~/Source/Arduino/HelloWorld/native
5. Create Jamfile ...
5. From a command prompt, cd to ~/Source/Arduino/HelloWorld/native, and type 'jam' 
6. Run out/HelloWorld

In your sketch, use the ".PDE" extension for anything that should be included in both Arduino hardware builds and native builds.  Use ".CPP" for files
that should only be in the Arduino hardware build.  The native build will not pick those up.  In the 'native' directory off your sketch, include any files
that should be in the native build.  The Arduino hardware build will not pick those up.  This allows you to substitute native versions of any of your
functions if you need it.  This avoids lots of "#if NATIVE" everywhere in your sketch.
