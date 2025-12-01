Artefacts Tumbller on Webots
============================

Tumbller is a nice small self-balancing two wheelers. This repo contains a 3D model to run Tumbller is simulations, here applied to the [Webots simulator](https://cyberbotics.com), so one can develop its software controller further, first in simulations for fast iterations, and ultimately on hardware.

Simulation runs and tests can be streamlined and scaled by using Artefacts. At this point in the repository, Artefacts requires a container image not included here yet.

The Elegoo Tumbller front image from [their website](https://www.elegoo.com/blogs/arduino-projects/elegoo-tumbller-self-balancing-robot-car-tutorial):

<p align="center">
<img width="500" alt="official image at Elegoo's" src="https://github.com/user-attachments/assets/efc3b28a-10f9-40bd-b246-2eeb0b53da91" />
</p>

Requirements
------------

This repository only requires a recent **Webots simulator** to display Tumbller. A recent **GCC compiler** is needed to compile its controller to run in the simulation (warning: code compiles but does not run well at time of writing). The GCC compiler must be accessible by Webots (basically in the `PATH`-like environment).

Resources
---------

* `protos/TumbllerRobot.proto` is the Webots 3D model.
* `controllers/tumbller_elegoo` contains the current WIP controller code to run in simulations.

Gallery
-------

<p align="center">
<img width="500" alt="Screenshot from 2025-12-01 12-10-29" src="https://github.com/user-attachments/assets/73c975c6-0fc6-4596-852e-ba949c63d5be" />
</p>

<p align="center">
<img width="500" alt="Screenshot from 2025-12-01 12-11-28" src="https://github.com/user-attachments/assets/b79f5d6a-66a5-4172-ae28-5bef2bbf2f3f" />
</p>

This close up of the wheel shows a white dot to help showing actual rotation (the wheel model is a simple cylinder, whereas the actual ones are nicer and harder to 3D model):
<p align="center">
<img width="410" height="232" alt="Screenshot from 2025-12-01 12-12-34" src="https://github.com/user-attachments/assets/5a73eded-4c3e-4b14-b9fe-4e54cbe4a2ca" />
</p>

Thanks and licensing
--------------------

This work derives code from the [Elegoo Tumbller source](https://github.com/elegooofficial/ELEGOO-TumbllerV1.1-Self-Balancing-Car-Tutorial) (unclear license, so linking to their code), which is a fun bot to work with---recommanded to all.
