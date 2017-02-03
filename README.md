Net
===

A simple, fully-connected, C++ neural net API created to help foster a deeper understanding of neural net functionality. The basic structure of the neural net is based on this [tutorial](https://vimeo.com/19569529).


Examples
--------

Three example programs were created to test the learning capabilities of the neural net. These examples are fairly trivial but still give insight into the advantages of more complex networks.

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


### XOR

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


Zoom factor: 0.5
Expected Output:




        0




Actual Output:




        0



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


Zoom factor: 1
Expected Output:



      0 0 0
      0 0 0
      0 0 0



Actual Output:



        0
      0 0 0
        0



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
