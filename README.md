# Cleaner Robot
This is a final project for EEP 520 Winter 2024 intended to show the user two cleaner robot searching for the junks out of a room and take its to their junkyard in the two sides of room. The project is inspired by sweeping robots, which can spin around while sweeping up dust in their path. However, when it comes to large pieces of trash, it may cause clogging and thus cannot be recycled. So I extended the traditional sweeping robot so that it can autonomously recycle large pieces of trash to the landfill. In order to take full advantage of the rotating nature of the sweeping robot's work, it is possible to detect the location of the garbage under the space without any dead angle in 360 degrees by relying on only one sensor.

This system is built using [Enviro](https://github.com/klavinslab/enviro) with [Elma](https://github.com/klavinslab/elma). 

![](/images/Demo.gif)

## Overview
The cleaner will automatically search junks in room. Once detect junk, it will go its location and try to attach. Then take the junk to the junkyard in the two sides of room for receycling.

### Cleaner
The rotating robot fitted with a sensor to detects junk. and transports the junk to the corresponding junkyard. It is equipped with measures to avoid competing with another Cleaner for the same junk, and has a system to prevent stuck.

### Junk
The goal of Cleaners. And users can create a junk by mouse-clicking anywhere in the room. And press 's' in keyboard could change the shape of the added junk(Triangle or square). When a junk that is attached by a robot touches its corresponding junkyard, the junk will disappears (simulating the junk being recycled).

![](/images/add_demo.gif)

### JunkYard
Static object, are the blue and red board in the two sides of the room.

## Challenges

### Stuck
Cleaner often gets stuck when it touches the room board. 
Solution: Set up the robot's behavior logic so that it can automatically detect that itself is in a stuck state and make corresponding decisions to break out of the stuck. This method has achieved significant results in the experiments(the cleaner always get rid of stuck), but the drawback is that in some extreme cases, it takes a long time to break out of the stuck.

### Design Process
Designing the behavioral logic of the cleaner was the toughest part of this project. The pieces were tightly connected and had to go through a lot of testing and experimenting with parameter settings (like doing a physics experiment). In some cases, in order to regulate the behavior of the cleaner, physical formulas had to be used, such as trying to control the rotational behavior of the cleaner, which required the use of conservation of angular momentum to calculate the applied force (or even trying to solve some differential equations). Even if the final result is not perfect, but it works.

## Project Installation
### Pre Requirements
Please make sure that you have installed [Docker](https://docs.docker.com/get-docker/) and [Git](https://git-scm.com/) firstly.

### Installation Instruction
1. Open your CLI in your work dictionary.
2. Clone this git repo by using: 
```bash
git clone https://github.com/boschenus/Spin-Cleaner.git
```
3. Then change the path by:
```bash
cd Spin-Cleaner
```
4. Launch the docker in current path: $PWD by:
```bash
docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:v1.61 bash
```
5. The start the project by: 
```bash
esm start
enviro
```
6. Open your browser and go page [localhost](http://localhost/), you could see the client interface. Try to click to add more junks!

## [Elma](https://github.com/klavinslab/enviro) and [Enviro](https://github.com/klavinslab/elma)
This project is built on top of Elma and Enviro.

-  Elma is an event loop and process manager for embedded and reactive     systems. Elma is structured as a C++ library of classes on top of which you build your own C++ project. It keeps track of processes, finite state machines, events, and communication channels, executing them at specified frequencies.

- Enviro is a multi-agent, multi-user, multi-everything simulator for users to play around and have fun coding C++.

