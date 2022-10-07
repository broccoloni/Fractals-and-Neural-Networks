# Fractals And Neural Networks

This project was part of my MSc. Math.

The problem is: Given an image of a fractal created from an Iterated Function System (IFS) consisting
of affine functions, can we predict the parameters of the functions that created it. More generally,
can we find a set of parameters corresponding to affine functions that when used as an IFS and run
through the chaos game are able to mimic even non-fractal sets. Essentially this would be a specific 
image compression technique where we find the closest fractal approximation to the image.

To run the code, one must first compile using make, and then create data using ./generatedata. It will
prompt you to input specification to create a fractal database with your specifications (see 
rungeneratedata.txt for an example). One can then run trainmodel.py to train a neural network on 
the dataset. I trained and tested the neural networks with fractal datasets of size 250,000, and for 
IFSs that consist of 2,4,6, and 8 functions. The networks produced better results the lower the 
number of functions in the IFS. I then tested the fractal trained networks on images of non-fractal
sets and found that they were akin to fractals with large amounts of functions. 

Below is an example of input and output images for:

2-function IFS fractal and it's network-predicted reconstruction:

<img src = "https://user-images.githubusercontent.com/38572823/194652848-7b738dd3-f818-487b-b2bd-6c587881a8de.png" width = "500" height = "250">

4-function IFS fractal and it's network-predicted reconstruction:

<img src = "https://user-images.githubusercontent.com/38572823/194652826-34cc25de-d968-44ac-89f8-20aaf6e2fb30.png" width = "500" height = "250">

6-function IFS fractal and it's network-predicted reconstruction:

<img src = "https://user-images.githubusercontent.com/38572823/194652811-088b4dd5-0247-436c-a3ef-21414044a60f.png" width = "500" height = "250">

8-function IFS fractaland it's network-predicted reconstruction:

<img src = "https://user-images.githubusercontent.com/38572823/194652790-e1becbea-05d3-44ef-8414-3e38bcbf7235.png" width = "500" height = "250">

The results were published in:  
L. Graham and M. Demers. Applying Neural Networks to a Fractal Inverse Problem. In AMMCS 2019 conference proceedings.
