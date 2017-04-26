## Project Checkpoint

### Revised Schedule
* Week 1: Interface with the HoloLens HPU
  * <s>Proof of concept for interfacing with the HPU</s>
  * <s>Write a simple pixel processing program that utilizes the HPU in order to gain familiarity with the hardware and how to access it </s>

<i>We have been unsuccessful at accessing the HPU, as it seems to be proprietary to Microsoft and not available for 3rd party programs. The HoloLens contains a 1.04Ghz Intel Atom x5-Z8100 64-bit CPU running Intel HD Graphics, so we will be using this chip to parallelize over instead of the HPU(Holgraphic Processing Unit)</i>
  
* Week 2: Test Harness
  * Implement a sequential ray tracing algorithm
  * Implement gathering mesh data from rooms using HoloLens API
  * <b>Record framerate and rendering time data for sequential implementation</b> (Incomplete)
  * <b>Make a reference implementation to test correctness of our future parallel code</b> (Incomplete)
  
<i>We are using a working implementation of the ray tracing code from the Computer Graphics class as a baseline sequential reference from our code. This code will be modified to apply to sound(i.e, how much a ray of sound contributes to what the user hears as opposed to how much a ray of light contributes to the luminence of a pixel). We are using Unity and Visual Studio to program with the HoloLens API and have written code to gather real time mesh data from the room.</i>
  
<b>Modified Remaining Timeline</b>

* Week 3: April 24-April 29
  * Use DirectX Spatial Audio API to convert sound ray vectors to spatialized sound (Sara)
  * Design data structure for storing audio ray results (Sara)
  * Create base app for placing holograms and storing their sound output (Jonathan)
  * Record framerate and rendering time data for sequential implementation (Jonathan)
* Week 4: April 30 - May 6
  * Parallelize across raytracer using barneshut-esque algorithm (Sara & Jonathan)
  * Optimize for SIMD (Sara)
  * Optimize for cahce locality (Jonathan)
  * Try and maximize recursion depth to increase accuracy/detail of algorithm (Jonathan)
  * Record framerate and rendering time data for graphs (Sara)
  * Celebrate Sara's Birthday (EVERYONE)
* Week 5: May 7-May 9
  * Prepare demo materials
    * make a video
    * make a poster
    * prepare graphs
  * Finish testing and debugging

### Goals and Deliverables
Our original goal was to use the HoloLens Holographic Processing Unit to implement a paralellized ray tracer for audio. Due to complications with accessing the HPU, we have decided to settle for using the CPU/GPU that is on the HoloLens(1.04Ghz Intel Atom x5-Z8100 running Intel HD Graphics) which is much less powerful. Our goal remains to be able to raytrace the audio in real time while maintaining 60fps but we have shifted our focus to CPU programming. Originally, we wanted our code to run as a background app on the HoloLens, but for the sake of time have decided to have our raytracer run as a full screen app where the user can place several holograms in the room and experience the effect of the ray tracer on their sound outputs.

We will have a demo video of our app to present at the HoloLens competition. We also hope to have a live version, but the effects of our raytracer will not be as apparent unless the user has a good amount of space to roam around in. We will also present the graphs of our speedup compared to the sequential raytracer and our average framerates, as well as the maximum recursive depth of the rays and the maximum number of rays we are able to process while maintaining 60fps.

## Project Proposal

### Summary
We will raytrace sounds in real space using the Microsoft HoloLens' wall detection in order to create physically accurate audio that is responsive to different room environments.

### Background
Currently, the Microsoft HoloLens utilizes spatial audio to create the illusion that sounds are coming from particular places in the augmented reality environment. However, the HoloLens does not take into account any properties of the room during audio processing. The size and shape of a room can significantly influence sound, and taking these parameters into consideration can improve imersion in an augmented reality environment. 
 
Our plan is to develop a parallel ray tracing algorithm on the HoloLens GPU, and use it to trace sounds in the augmented reality environment in real time. The HoloLens is already capable of detecting walls in the room by detecting planes, and we will use this information to model size and shape of the room the user is currently in.
 
Ray tracing clearly lends itself to parallelism because the calculations for each ray are independent. Each ray bounces off a surface in the room, and a new ray is created from this and so on, so the behavior of a ray is recursive and limited only by setting a maximum recursion depth. Because we would be doing the same calculation on each ray, we can exploit data parallelism. By processing each ray to some max recursive depth, we can compute which direction each ray points to relative to the users head, and group rays with similar directions together, scaling each ray for volume. We can then calculate the contribution to the overall sound from each ray grouping in parallel.

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
