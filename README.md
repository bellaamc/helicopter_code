# ENCE361 Helicopter Project

Git repository for Monday morning lab, group 86. This repository contains the source files for the helicopter control project. Note: This helicopter was tested on an emulator rather than real rig, due to test rigs breaking.

## Getting Started

### Before Starting
- Download TI Code Composer via TI website
- Download the tivaware api for TM4C123G

### Installing repository
- Clone repo onto a local directory of choice using 'git clone https://eng-git.canterbury.ac.nz/ence361-2023/group_86.git' 
- Create the code composer project as an empty project in the new project wizard.
- Set the target as the Tiva TM4C123GH6PM
- Set the debug target as the Stellaris In-Circuit Debug Interface.
- Import the source files from repo and add the api library.

### Building For Devboard
- Click the build project "hammer" icon to build files
- Click the debug "bug" icon to build and run code in debug mode. Then press the play button, set breakpoints etc.
- To flash the code, without running in debug mode, click the flash code icon

## Authors
- Arabella Cryer <acr151@uclive.ac.nz>
- Amber Waymouth <awa155@uclive.ac.nz>

## Acknowledgements
- Phil Bones: Author of Buttons and Circle buffer files
- mdp46: Code for OrbitOLED display
