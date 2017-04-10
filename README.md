## Project Proposal

### Summary

### Background
Currently, the Microsoft HoloLens utilizes spatial audio to create the illusion that sounds are coming from particular places in the augmented reality environment. However, the HoloLens does not take into account any properties of the room during audio processing. The size and shape of a room can significantly influence sound, and taking these parameters into consideration can improve imersion in an augmented reality environment. 

Our plan is to develop a parallel ray tracing algorithm on the HoloLens GPU, and use it to trace sounds in the augmented reality environment in real time. The HoloLens is already capable of detecting walls in the room by detecting planes, and we will use this information to model size and shape of the room the user is currently in.

***insert paragraph on how to parallelize ray tracing for a GPU here***

### The Challenge

### Resources
* We will be using a Microsoft HoloLens, which we already have access to
* The GPU/HPU("Holgraphic Processing Unit"). We found the specs for it on [The Register](https://www.theregister.co.uk/2016/08/22/microsoft_hololens_hpu/)
* We will be writing our code from scratch, but will be using Microsoft's Hololens API and programming in C++
* We think we can use DirectX or OpenGL to access the HoloLens HPU from our web searches, but we have not tested either so are a bit unsure on this aspect
* We were inspired by the [VRWorks Audio](https://developer.nvidia.com/vrworks) features in the Nvidia 1080 GPU
* We found a paper online on [Ray Tracing for a GPU](https://arxiv.org/pdf/1504.03151.pdf) and plan to use it as a reference

### Goals and Deliverables

### Platform Choice

### Schedule
