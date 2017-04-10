## Project Proposal

### Summary
We will raytrace sounds in real space using the Microsoft HoloLens' wall detection in order to create physically accurate audio that is responsive to different room environments.

### Background
Currently, the Microsoft HoloLens utilizes spatial audio to create the illusion that sounds are coming from particular places in the augmented reality environment. However, the HoloLens does not take into account any properties of the room during audio processing. The size and shape of a room can significantly influence sound, and taking these parameters into consideration can improve imersion in an augmented reality environment. 
 
Our plan is to develop a parallel ray tracing algorithm on the HoloLens GPU, and use it to trace sounds in the augmented reality environment in real time. The HoloLens is already capable of detecting walls in the room by detecting planes, and we will use this information to model size and shape of the room the user is currently in.
 
Ray tracing clearly lends itself to parallelism because the calculations for each ray are independent. Each ray bounces off a surface in the room, and a new ray is created from this and so on, so the behavior of a ray is recursive and limited only by setting a maximum recursion depth. Because we would be doing the same calculation on each ray, we can exploit data parallelism. By processing each ray to some max recursive depth, we can compute which sound directions each ray contributes to, and process each direction in parallel by adding the contributing rays and scaling them accordingly for loudness. Since direction is really a continuous vector space aimed at the users head(shown below), we will split up the space into compartments and calculate which compartment each ray belongs to.

![sound directions](https://www.mattmontag.com/een502/project2/cipic.jpg "Logo Title Text 1")
<br>
*All possible directions affecting the user. Each ray contributes to a number of these directions. (Image from mattmontag.com)*

### The Challenge
Since the ray traces are independent of one another and all the instructions will remain constant, we can exploit data parallelism with coherent execution. However, we will need to maintain the 60 frames per second render time the HoloLens requires to maintain its usability. Since parallel ray tracing is still a computationally expensive task, we will need to make optimizations such as exploiting cache locality and data parallelism to make the program run in real time.

The HoloLens’ graphics card, the HPU (Holographic Processing Unit) is proprietary hardware and is not easy to interact with directly. The HoloLens is also a mobile computer, meaning its specifications are relatively low. It features the HPU for processing camera and sensory data, as well as an Intel Cherry Trail SoC with its own integrated GPU, but much of the system’s compute power is already being used to process and update the mesh of the room. We will also need to integrate our calculations into an already existing system, so other programs will be running. This means that at runtime, other factors we did not account for could affect our program. For example  another process could be using a lot of space in the cache, so we won't see the same speedup from cache locality that we were expecting.

### Resources
* We will be using a Microsoft HoloLens, which we already have access to
* The GPU/HPU("Holgraphic Processing Unit"). We found the specs for it on [The Register](https://www.theregister.co.uk/2016/08/22/microsoft_hololens_hpu/)
* We will be writing our code from scratch, but will be using Microsoft's Hololens API and programming in C++
* We think we can use DirectX or OpenGL to access the HoloLens HPU from our web searches, but we have not tested either so are a bit unsure on this aspect
* We were inspired by the [VRWorks Audio](https://developer.nvidia.com/vrworks) features in the Nvidia 1080 GPU
* We found a paper online on [Ray Tracing for a GPU](https://arxiv.org/pdf/1504.03151.pdf) and plan to use it as a reference

### Goals and Deliverables
Our main goal is to implement a ray tracing algorithm for Microsoft Hololens that renders audio in real time at 60fps using the HPU on the device. An extra goal is to expand our above algorithm to also consider the effect of different materials on the sound. For instance stone vs wood vs glass surfaces.

For our demo, we plan on having a live demo with the HoloLens including several Holographic sound sources. Additionally, because the full effect of our project will have more impact when comparing different rooms, we will also have a video showing the effects in rooms of different sizes and shapes. We will also include a graph comparing rendering times and frame rate to a sequential ray tracing algorithm

### Platform Choice
Microsoft HoloLens and C++/DirectX

### Schedule
* Week 1: Interface with the HoloLens HPU
  * Proof of concept for interfacing with the HPU
  * Write a simple pixel processing program that utilizes the HPU in order to gain familiarity with the hardware and how to access it
* Week 2: Test Harness
  * Implement a sequential ray tracing algorithm
  * Implement gathering mesh data from rooms using HoloLens API
  * Record framerate and rendering time data for sequential implementation
  * Make a reference implementation to test correctness of our future parallel code
* Week 3: Parallel Ray Tracing
  * Implement first iteration of parallel ray tracing algorithm that utilizes the HPU
  * Parallelize across rays, then across sound directions
  * Use a small recursion depth to reduce computation
  * Record frame rate and render time for this implementation
* Week 4: Improved Parallel Ray Tracing
  * Improve efficiency of parallel algorithm to run at near 60 fps at runtime
  * Optimize algorithm for data parallelism and cache locality
  * Try and maximize recursion depth to increase accuracy/detail of algorithm
* Week 5: Presentation
  * Prepare demo materials
    * make a video
    * make a poster
    * prepare graphs
  * Finish testing and debugging
