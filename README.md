# MoaiSpriter
Full Spriter (http://www.brashmonkey.com/spriter.htm) implementation for the Moai SDK game engine (http://www.getmoai.com) .

## Features supported in v1.0.0:
- Bones based or boneless animation, with any level of nesting and complexity.
- Sprite position, scale animations and tweens
- Sprite angle animations and tweens (including > 360 degrees)
- Sprite image swapping within animation
- Sprite alpha tweens
- Custom image or sprite pivot points

## Upcoming features (not yet available):
- Sprite Z-Index changes within animation
- Variables
- Tags
- Event Triggers
- Timeline linked Sound Effects 
- Collision Rectangles
- Action Points
- Dynamic colour customisation (using shaders)

## Components
This project contains 3 parts. 

1. Exporter - /exporter - A command line tool to convert Spriter SCML files to a lua format understood by Moai. Written in C++, 
Mac binary and XCode project supplied, Windows or Linux has to be compiled separately. 

2. Raw spriter data and assets - /raw - For a complex scorpion animated character, with detailed multi-level bone structure, 
pivot points, rotatation angles > 360 degrees etc. for demonstration of features. Assets have been colored and modified to discourage re-use in games, they 
are for demonstration purposes only. 

3. Demo project and Moai integration Plugin - /plugin_sample - Sample Moai lua project which runs exported scorpion animation. 
The sample project also has a submodule called moai_spriter_plugin which will need to be cloned in any of your Moai games
that want to integrate with Spriter.

## Requirements:
1. Moai SDK installed with MOAI_BIN environment variable set up.
2. TexturePacker (https://www.codeandweb.com/texturepacker) to compress sprite parts into single sheet. 

For Windows and Linux -
2. C++ and build environment (Cmake or Visual Studio etc.) to generate executable binaries for the exporter
or 
For 64 bit Macs, binary is supplied, build environment optional.

## Setup:
1. Clone this repo using `git clone --recursive`, as you will need the plugin submodule to be cloned as well

2. If you have Moai set up, run the run.sh script to see the sample project.

3. To use your own Spriter animations, run the Spriter2Moai exporter under /exporter with your scml file as a parameter. 
Copy the resuling lua file into your moai game project and set up as shown in sample project.

4. Pack your sprite parts using TexturePacker and copy resulting lua and asset file to your game project.

5. Clone the submodule moai_spriter_plugin project (https://github.com/sshukul/moai_spriter_plugin) directly into your Moai 
game and integrate as shown in sample project. Keep as submodule and update from repo when newer features are released.
