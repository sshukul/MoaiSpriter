# MoaiSpriter
Full Spriter (http://www.brashmonkey.com/spriter.htm) implementation for the Moai SDK game engine (http://www.getmoai.com) .
Current version - 1.0.4

## Features supported in v1.0.4:
- Bones based or boneless animation, with any level of nesting and complexity. (Since 1.0.0)
- Sprite position, scale animations and tweens (Since 1.0.0)
- Sprite angle animations and tweens (including > 360 degrees) (Since 1.0.0)
- Sprite image swapping within animation (Since 1.0.0)
- Sprite alpha tweens (Since 1.0.0)
- Custom image or sprite pivot points (Since 1.0.0)
- Tags (Added in 1.0.1)
- Action / Spawn Points (Added in 1.0.2)
- Sprite Z-Index changes within animation (Added in 1.0.3)
- Timeline linked Sound Effects (Added in 1.0.3)
- Character maps (Added in 1.0.4)

## Upcoming features (not yet available):
- Variables
- Event Triggers
- Collision Rectangles
- Sprite runtime colour customisation (using shaders)
- Windows and Linux binaries and Cmake build projects.

## Components
This project contains 3 parts. 

1. Exporter - /exporter - A command line tool to convert Spriter SCML files to a lua format understood by Moai. Written in C++, 
Mac binary and XCode project supplied, Windows or Linux has to be compiled separately. 

2. Raw spriter data and assets - /raw - For a complex scorpion animated character, with detailed multi-level bone structure, 
pivot points, rotatation angles > 360 degrees etc. for demonstration of features. Assets have been silhouetted to discourage re-use 
in games, they are for demonstration purposes only. 

3. Demo project and Moai integration Plugin - /plugin_sample - Sample Moai lua project which runs exported scorpion animation. 
The sample project also has a submodule called moai_spriter_plugin (https://github.com/sshukul/moai_spriter_plugin) which will need to be cloned in any of your Moai games that want to integrate with Spriter.

## Requirements:
1. Moai SDK installed with MOAI_BIN environment variable set up.
2. TexturePacker (https://www.codeandweb.com/texturepacker) to compress sprite parts into single sheet. 

For Windows and Linux -
C++ and build environment (Cmake or Visual Studio etc.) to generate executable binaries for the exporter

or 

For 64 bit Macs, binary is supplied, build environment optional.

## Setup:
1. Clone this repo using `git clone --recursive`, as you will need the plugin submodule to be cloned as well

2. If you have Moai set up, run the run.sh script (on Mac and Linux) or run.bat (on Windows) to run the sample project and see it working.

3. To use your own Spriter animations, run the Spriter2Moai exporter under /exporter with your scml file as a parameter. 
Copy the resulting lua file into your moai game project and set up as shown in sample project.

4. Pack your sprite parts using TexturePacker, export for Moai, and copy resulting lua and packed asset files to your game project.

5. Clone the submodule moai_spriter_plugin project (https://github.com/sshukul/moai_spriter_plugin) directly into your Moai 
game and integrate as shown in sample project. Keep as submodule and update from repo when newer features are released.

## Troubleshooting
1. Sprite sounds or z-index changes within animation aren't working - If you are adding a listener for EVENT_TIMER_KEYFRAME on an  animation, don't forget to add a call to player.keyFrameFunc() at the end if you want sounds and z-index hanges to work. This is 
because the plugin also makes use of keyframe listeners to activate sounds and z-index changes.

## Credits:
Thanks to github users miletbaker (https://github.com/miletbaker/spriter2moai)
and samuchan (https://github.com/samuchan/NekoPunch)
for providing the previous versions of the Moai plugin and exporter upon which this was built.

##Licence
These files unless specified in the source header are distributed under the MPL-2.0 licence (http://opensource.org/licenses/MPL-2.0)
