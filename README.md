Net
===

A simple, fully-connected, C++ neural net API created to help foster a deeper understanding of neural net functionality. The basic structure of the neural net is based on this [tutorial](https://vimeo.com/19569529).


Build via CMake
---------------

```bash
# from command line in
# project root directory
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

This should create three executables in the build directory:
* runXOR
* runAddition
* runIntersection


Executables
-----------

Three example programs were created to test the learning capabilities of the neural net. These examples are fairly trivial but still give insight into the advantages of more complex networks.

All of the example programs start by training the neural net with random input values. The input values are fed into the net followed by the correctly computed target values (for back propagation). The average error between the network's computed values and the expected target values is displayed until the error is below a reasonable threshold and training ceases:

```bash
Error: 0.906008
Error: 0.00535156
Error: 0.00161872
Error: 0.00151562
Error: 0.00072726
Error: 0.000775959
Error: 0.000856428
Error: 0.000856574
Error: 0.000607436
Error: 0.000804196
Error: 0.000461414
Error: 0.000628718

Done training (Error: 9.1413e-05)

Results:
```

Once the program ceases training it shows what the neural net has learned by selecting more random input and displaying the neural net's computed results.


### XOR

For the XOR program the input is just two random binary integers. If both inputs are the same, the output should be zero. Otherwise the output should be one. Below is sample output from the program after training.

```bash
Input:
0 1
Output: 1

'Enter' : new random input, 'q' : quit

Input:
0 0
Output: 0

'Enter' : new random input, 'q' : quit

Input:
1 1
Output: 0

'Enter' : new random input, 'q' : quit

Input:
1 0
Output: 1

'Enter' : new random input, 'q' : quit
```


### Addition

For the Addition program the input is four random binary integers. The output from the neural net should be the sum of all four numbers (base-10). Below is sample output from the program after training.

```bash
Input:
0 0 1 0
Output: 1

'Enter' : new random input, 'q' : quit

Input:
1 1 1 1
Output: 4

'Enter' : new random input, 'q' : quit

Input:
0 0 0 1
Output: 1

'Enter' : new random input, 'q' : quit

Input:
0 0 0 0
Output: 0

'Enter' : new random input, 'q' : quit

Input:
1 0 1 1
Output: 3

'Enter' : new random input, 'q' : quit
```


### Ray-Sphere Intersection

The Intersection program teaches the neural net to calculate intersections between a 3D ray and sphere. The input to the neural net is the x, y, and z component of the 3D ray. The output is simply a positive or negative number indicating a hit or miss respectively.

For the sake of simplicity, the sphere is being *rendered* in the terminal via an 8x8 image of characters where '0' represents part of the sphere and '  ' represents background. The value of each "pixel" is computed by determining if a ray shot from the view point through the pixel would intersection the sphere in the scene. The apparent size of the sphere can then be adjusted by changing the zoom factor used to shoot rays into the scene. Below is sample output from the program after training.

```bash
Zoom factor: 1.5
Expected Output:


        0
      0 0 0
    0 0 0 0 0
      0 0 0
        0


Neural Net Output:


        0
      0 0 0
    0 0 0 0 0
      0 0 0
        0


'w' : zoom in, 's' : zoom out, 'q' : quit

...

Zoom factor: 0.5
Expected Output:




        0




Neural Net Output:




        0



'w' : zoom in, 's' : zoom out, 'q' : quit

...

Zoom factor: 2.5
Expected Output:

      0 0 0
    0 0 0 0 0
  0 0 0 0 0 0 0
  0 0 0 0 0 0 0
  0 0 0 0 0 0 0
    0 0 0 0 0
      0 0 0

Neural Net Output:

      0 0 0
    0 0 0 0 0
  0 0 0 0 0 0 0
  0 0 0 0 0 0 0
  0 0 0 0 0 0 0
    0 0 0 0 0
      0 0 0

'w' : zoom in, 's' : zoom out, 'q' : quit
```

Only the extreme limits of the camera zoom are shown above. There are a few zoom levels where the limits of the neural network begin to show through slight inaccuracies between the expected and computed output:

```bash
Zoom factor: 2
Expected Output:


    0 0 0 0 0
    0 0 0 0 0
    0 0 0 0 0
    0 0 0 0 0
    0 0 0 0 0


Neural Net Output:


      0 0 0
    0 0 0 0 0
    0 0 0 0 0
    0 0 0 0 0
      0 0 0


'w' : zoom in, 's' : zoom out, 'q' : quit

...

Zoom factor: 1
Expected Output:



      0 0 0
      0 0 0
      0 0 0



Neural Net Output:



        0
      0 0 0
        0


'w' : zoom in, 's' : zoom out, 'q' : quit
```


Future Work
-----------
[CNN](https://en.wikipedia.org/wiki/Convolutional_neural_network)s!

