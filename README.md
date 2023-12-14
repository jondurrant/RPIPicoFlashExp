# RPIPicoFlashExp
Experiments in Writing to Flash on Raspberry Pi Pico. This  repo goes
along with a Video on my [YouTube channel](https://youtube.com/@drjonea).

The experiments show approaches to writing to Flash as non volatile storage on the Raspberry Pi Pico and Pico-W.

## 1-BareMetal
This shows the basic write and read approach to using Flash. Calculating a position at the top of the Flash away from the code.

NOTE this is not a safe example as does not protect the code from interrupts.

## 2-BareMetalInt
This example extends exampe  1 to also protect from interrupts. It sets up a simple timer interrupt to give the code some challenge to defend against. 

## 3-BareMetal2Core
This example extends 2 by also adding code to run on the secon core (core 1). The second core must be also locked out so it stops reading its code form flash during writing to the flash.


### Build Process
Each project is a folder. Each requires a build folder created within in it and the normal build process:
```
mkdir build
cd build
cmake ..
make
```

