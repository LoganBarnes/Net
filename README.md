Net
===

A simple, fully-connected, C++ neural net API created to help foster a deeper understanding of neural net functionality. The basic structure of the neural net is based on this [tutorial](https://vimeo.com/19569529).


Examples
--------

Three example programs were created to test the learning capabilities of the neural net. These examples are fairly trivial but still give insight into the advantages of more complex networks.

All of the example programs start by training the net with random input values and correctly computed target values (for back propagation). The average error between the network's values and the expected target values is computed and displayed until the error is below a reasonable threshold:

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

For the XOR program the input is just two random binary integers. If both inputs are the same, the output should be zero. Otherwise the output should be one. Below is sample output from the program.

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

For the Addition program the input is now four random binary integers. The output from the neural net should be the sum of all four numbers. Below is sample output from the program.

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

```bash
Zoom factor: 1.5
Expected Output:


        0
      0 0 0
    0 0 0 0 0
      0 0 0
        0


Actual Output:


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




Actual Output:




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

Actual Output:

      0 0 0
    0 0 0 0 0
  0 0 0 0 0 0 0
  0 0 0 0 0 0 0
  0 0 0 0 0 0 0
    0 0 0 0 0
      0 0 0

'w' : zoom in, 's' : zoom out, 'q' : quit
```

```bash
Zoom factor: 2
Expected Output:


    0 0 0 0 0
    0 0 0 0 0
    0 0 0 0 0
    0 0 0 0 0
    0 0 0 0 0


Actual Output:


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



Actual Output:



        0
      0 0 0
        0


'w' : zoom in, 's' : zoom out, 'q' : quit
```




Build via CMake
---------------

```bash
# from command line in
# project root directory
mkdir build
cd build
cmake ..
cmake --build .
```
