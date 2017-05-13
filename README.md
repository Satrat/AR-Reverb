# Project Writeup

## Summary
 We implemented an approximation of realistic spatial sound on the HoloLens, taking into account the topology of the space when calculating reverberation. We used Unity and C# to interface with the HoloLens, broke down the space into subsections that could be calculated independently of each other, and Mono’s SIMD library for parallelism. The HoloLens has an Intel Cherry Trail processor, which supports SIMD instructions, but not direct interactions with the GPU.

**Video:** https://youtu.be/HtJZRteUPh0

## Background
 We parallelized raycasting soundwaves to create realistic reverberation and spatial sound so we could run it in real time on the Microsoft HoloLens while maintaining 60 frames per second. Exceeding 60 fps would cause rendering the holograms to lag, breaking immersion and defeating the purpose of the application. The HoloLens calculates a spatial map of the space it is in as well as the position of the user relative to the space.

 We modelled how sound travels in a room similarly to how light reflects off of objects, which is a close approximation to how sound will actually travel in the room (despite light being rays and sound being waves). We have sound sources spawn many rays travelling in random directions around the room which ricochet off of the walls, and the sound dampens every time the sound ray bounces off the wall. When a ray passes through an acceptable distance from the user’s head, the sound plays.

 The most compute-intensive operation is going through all the rays and checking which enter the user’s earshot. Each ray is independent of each other ray, and so the raycasts can be computed entirely in parallel. However, there are not enough threads available on a HoloLens to make computing the large number of rays efficient enough to run at 60fps. In addition to this, we must also account for the fact that the user’s head is constantly changing position, and that the HoloLens updates the mesh of the space roughly once per second, which affects how sound should be reverberating in the room.

 Parallelizing tasks on a HoloLens in Unity/C# is complicated on its own because Unity is not threadsafe, so we could not use any Unity data structures of Library functions outside of the main thread, which meant less access to the HoloLens APIs. The HoloLens does support parallelism, but only using the Microsoft Universal Windows Platform (UWP) threading library, which was introduced in the .NET 4.0 framework, but Unity only supports .NET 3.5 and earlier, making the build from Unity incompatible with the HoloLens.

## Approach
 Our project was specifically geared toward the Microsoft HoloLens. We used Unity and C# so we could take advantage of Microsoft’s low level APIs for interacting with the spatial map of the room and using the HoloLens’ ASICs. For parallelism, we used Unity’s Mono.Simd dll and C#’s native System.Threading.Tasks library.
 
 To approach the problem, we divided the space into a custom data structure we called Quadrants, each responsible for keeping track of the spatial mapping data relevant to that section of the room and the rays that intersect it. When the HoloLens grabs more spatial mapping data, which can be represented as Add, Remove, or Change; we asynchronously request the new information regarding the mesh and send it to the relevant Quadrant. That way, we keep most Quadrants free and ready to do work if they are needed. The mesh data is stored in a dictionary with unique identifiers for each of the mesh objects, so if a mesh object is deleted or updated, we can easily remove the old piece of the mesh and replace it with the updated mapping.
 
 When a new sound source is spawned, we use Unity’s Physics.Raycast to spawn 75 rays originating from the source and getting back where the sound rays collide with the walls of the room. On every collision, we spawn a new sound ray reflecting off the object it collided with and dampen the associated volume by a factor of 0.8. We allow this to continue up to our maximum recursion depth, which is set to 30 reflections. We then cache the results, since these will stay the same unless the mesh gets updated in a location that is relevant to the ray. This means that at steady state (once all the sound sources are placed and the room has been scanned), we won’t need to do any additional computation surrounding how the sound rays are created.
 
 Whenever a ray crosses through a Quadrant, we add it to the Quadrant’s list of active rays, represented as an instance of the rayData class (defined below). On every frame, we check what Quadrant the user’s position is in and loop through its active rays to compute which rays come close enough to the user to be heard, eliminating the majority of the rays from the loop. When a sound has completed playing, it is no longer active, so we can remove it from the Quadrant and destroy the ray.
 
 Since Unity itself is not thread-safe, we could not use any Unity data structures or library functions anywhere but the main thread. To get around this, we created a rayData class that contains the relevant information about each ray that we calculated when we spawned the sound sources. We then implemented our own collision detection method taking into account the user’s current position and the rayData which could be safely run on multiple threads. We used this to calculate which rays should be heard, then sequentially went through and played the sounds (since beginning a sound was a Unity function, we could not do it on the thread. However, triggering a sound is not a computationally expensive task, so this was not a significant slowdown).
 
 To get around the problem of incompatible .NET frameworks, we had to put in blocks of #ifdef !UNITY_EDITOR and #ifdef UNITY_EDITOR to micromanage what parts of the code should be compiled by Unity and what parts should be ignored until it is compiled in Microsoft Visual Studio.


## Results
 The computation done by our raytracing algorithm occurred in two places: on the creation of a new sound object, and in the user position update function. Therefore when measuring performance we put timing markers at the beginning and end of these sections to calculate the time spent in each. Since our goal was to maintain a framerate of 60fps, our main goal was to limit the latency of user update function, which is called once per frame, to about 1ms. For testing our latency, we implemented a gesture recognizer in Unity that allowed the user to place sound objects in their environment with a tap. We performed tests with about 7 sound sources running at once, each emitting 75 rays with a maximum depth of 30. We did separate tests for both long(25 second) and short(5 second) sound sources. The hardware for all  tests was the 4 core Intel Atom processor on the HoloLens.
 
![source creation](https://github.com/Satrat/Raytracing-Sound-in-3D-Space-Using-the-Microsoft-HoloLens/blob/master/Images/objectCreation.png)
 
 The above graph represents the latency of initializing and raytracing a single sound source at various stages of our implementation, using the testing conditions described above. Note that we did not split up results between short and long clips, as the actual length of the clip does not influence this section of the algorithm. The baseline implementation was a sequential raytracer made using Unity’s built in Physics.Raycast() function. The quadrants implementation shows the speed up gained from sending rays only to the quadrant’s they intersect, calculated using Unity’s built in Bounds.Intersect() function. The final bar shows the latency obtained from replacing the built in intersect function with our own implementation that utilizes the Mono.Simd library to perform 4 wide vector calculations in order to determine the intersections for each ray. We ultimately were not able to achieve the desired 1ms latency for this function, however since it is only called when a new audio source is created it, it does not significantly affect the overall framerate.
 
![user update](https://github.com/Satrat/Raytracing-Sound-in-3D-Space-Using-the-Microsoft-HoloLens/blob/master/Images/userUpdate.png)

 This second graph represents the amount of time spent processing the user moving through the sound field each frame. Since this function is being consistently called each frame, this is where the majority of execution is spent. The baseline implementation loops through all the rays and detects collisions with the user using the built in Physics.Raycast() function. The second set of bars represents the improvement from processing only the rays intersecting the user’s quadrant, and from cleverly deleting rays that are no longer audible. Finally, the multithreading bars represent the speedup obtained from replacing Unity’s Physics.Raycast() function with our own thread-safe implementation that made multithreading across 4 threads possible. Using the multithreaded approach, we were able to achieve a latency of about 1.1ms per frame, getting us just under our goal of 60 frames per second.
 
 In the user update graph, there is a large difference between the performance on long and shorter sources. When using longer sound clips, there is a lot more overlap of ray sounds, which takes up a larger amount of the CPU’s resources for audio processing. The multithreaded implementation allows for collision detection calculations to be performed off of the main thread, so they are not affected by the strain of audio processing. For shorter clips, there is less overlap in the rays so the benefits of multithreading is minimal.

The main limit to our speedup was our reliance on the Unity API. In order to access the mesh data on the HoloLens, we needed to use Unity built in functions, which are not thread safe. Because of this we were unable to parallelize our code by simply treating each raytrace as an independent event, since the rays are interacting with the mesh which can only be accessed sequentially. Starting audio could also only be done on the main thread, forcing us to preprocess collisions in parallel and then sequentially start sound each update frame. Furthermore, the Mono.Simd library, which is the only SIMD library that Unity supports, utilizes at most 4 byte wide vector instructions, while the Intel Atom on the Hololens has 12 execution contexts. We could have benefited from being able to use all of these execution contexts when calculating the intersections of rays with quadrants, as the data parallelism of the intersection calculations could have benefited from up to 16 execution contexts, one for each quadrant.
 

## References
[GSound](http://gamma.cs.unc.edu/GSOUND/gsound_aes41st.pdf)

[iSound](http://gamma.cs.unc.edu/Sound/iSound/isound-tech_report.pdf)

[Intel Geometric Sound Propogation](https://software.intel.com/sites/default/files/m/d/4/1/d/8/InteractiveGeometricSoundPropagationandRendering.pdf)

## Work Division
Equal work was done by both students

***

## Project Checkpoint

### Progress
We struggled to find the right approach for this project. There is not a lot of literature on best practices for Hololens development since the developer kits were only released last year, and a lot of the documentation is still in the process of being written. Additionally, most existing work is in Unity and C#.

Our initial approach was to make the application entirely in C++. We have a basic Hololens project that builds and can read in spatial mapping data, including building the mesh of the current surroundings. We also have a working raytracer provided by a student who completed Computer Graphics (15-462). Our hope was to adapt the raytracer to work with the Windows Holographic environment, then parallelize it to simulate how sound interacts with the environment. We do not have the two working together at the moment, and putting them together is proving difficult.

Our next idea was to work in Unity and C# for the high level interaction with the Hololens and spatial mapping data, then write our sound processing as a C++ plugin for Unity. Unity compiles C++ plugins to machine code, so we should not see a significant decrease in throughput. We can currently read in the spatial mapping mesh and plan on using Unity's Physics.Raycast for our soundwaves.

We also have begun writing the sound processing code. We implemented an output audio buffer using the PortAudio api, which allows us to process the spatial sound on its own thread.

### Revised Schedule
* Week 1: Interface with the HoloLens HPU
  * <s>Proof of concept for interfacing with the HPU</s>
  * <s>Write a simple pixel processing program that utilizes the HPU in order to gain familiarity with the hardware and how to access it </s>

<i>We have been unsuccessful at accessing the HPU (Holgraphic Processing Unit), as it seems to be proprietary to Microsoft and not available for 3rd party programs. The HoloLens contains a 1.04Ghz Intel Atom x5-Z8100 64-bit CPU running Intel HD Graphics, so we will be using this chip to parallelize over instead of the HPU</i>
  
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

***

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
