KD Tree
Project Description:
This project is intended to act a functional KD tree.
The program will take in a certain amount of points (n) and probes (p) that have a certain number of dimensions(k).
The program will use the points and recursively put them into a KD tree by finding the best dimension and the median of that dimension.
Once the KD tree has been made from the points the probes are then used as test to see where they would be placed in the KD tree.
The program uses a random number generator to create all of the points and probes.

Expected Input:
The project has 3 command line parameters that it expects: n (the number of points), k (the number of dimensions), p (the number of probes).
The program also uses the randGen.pl with the piping operator (|) to pipe random numbers into the simulation.

Expected Output:
The program should print out each probe and the respective bucket that they should be placed in.

How to compile with Unix:
type: make run

Example:
This example is used with the dimension element set to 3, number of probes set to 10, and the number of points set to 64.

![Screenshot (10) 117](https://user-images.githubusercontent.com/84202362/178803050-7ce74d11-c5e9-4ffd-a94a-277fc2f6d92e.png)
