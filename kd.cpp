#include<iomanip>
#include<iostream>
#include<vector>
#include<cmath>
using namespace std;

double b = 10; //Size of the buckets

//Creates a struct called node that contains a data value and two children nodes
struct node {
	node *leftChild;
	node *rightChild;
	int median; //Holds the nodes median value
	int dimension; //Holds the nodes best dimension
	vector<vector<int> > bucket; //Holds a vector of points
	bool isLeaf; //Determines if a node is a leaf or internal node
	static node* makeInternalNode(int medianVal, int aDimension) { //Creates a new internal node
		node* myNode = new node; 
		myNode->median = medianVal; //Sets the median value
		myNode->dimension = aDimension; //Sets the dimension value
		myNode->leftChild = NULL; //Sets the left child to NULL
		myNode->rightChild = NULL; //Sets the right child to NULL
		myNode->isLeaf = 0; //0 if the node is an internal node
		return myNode;
	}
	static node* makeLeafNode(vector<vector<int> > aBucket) { //Creates a new leaf node
		node* myNode = new node;
		myNode->bucket = aBucket; //Sets the nodes bucket
		myNode->leftChild = NULL; //Sets the left child to NULL
		myNode->rightChild = NULL; //Sets the right child to NULL
		myNode->isLeaf = 1; //! if the node is a leaf node
		return myNode;
	}
};

//Function for finding the best dimension
int bestDimension(vector<vector<int> > aVector) {
	int bestDimension; //Variable that holds the best dimension
	int bestDimensionRange = 0; //Variable that holds the best dimensions range
	
	for (double i = 0; i < aVector[0].size(); i++) { //Goes through each dimension
		int range = 0; //Variable for holding the dimensions range
		int lowestVal = aVector[0][i]; //Variable that holds the lowest value in the dimension
		int highestVal = aVector[0][i]; //Variable that holds the highest value in the dimension
		for (double j = 0; j < aVector.size(); j++) { //Goes through each element in the dimension
			if (aVector[j][i] < lowestVal) //Checks to see if the element is the smallest element
				lowestVal = aVector[j][i];
			if (aVector[j][i] > highestVal) //Checks to see if the element is the largest element
				highestVal = aVector[j][i];
		}
		range = highestVal - lowestVal; //Calculates the range of the dimension
		if (range > bestDimensionRange) { //If the dimensions range is higher than the best dimensions range
			bestDimensionRange = range; //Make the dimensions range the best dimensions range
			bestDimension = i; //Make the dimension the best dimension
		}
	}
	return bestDimension; //Return the best dimension
}

//Function for partitioning a vector
int partitionStep(vector<vector<int> > &aVector, int low, int high, int dimension) {
	int pivot = aVector[high][dimension]; //Sets the pivot point to the last element 
	int p = (low - 1); 
	for (int j = low; j <= high - 1; j++) { //Goes through each element in the vector with the best dimension
		if (aVector[j][dimension] <= pivot) { //Checks to see if the jth element is les than or equal to the pivot element
			p++;
			swap(aVector[p][dimension], aVector[j][dimension]); //Swaps the pth element and the jth element
		}
	}
	swap (aVector[p + 1][dimension], aVector[high][dimension]); //Swaps the last element with the pivot
	return (p + 1);
}

//Function for using quick select to sort through a vector and find the median
int quickSelect(vector<vector<int> > aVector, int left, int right, int position, int dimension) {
	vector<vector<int>> temp = aVector; //Creates a temporary vector
	while (left <= right) { 
		int pivotNum = partitionStep(temp, left, right, dimension); //Uses the partitionStep function to find a pivot point
		if (pivotNum == position - 1) {
			return temp[pivotNum][dimension]; //Returns the pivot element if the pivot number equals the correct position
		}
		else if (pivotNum > position - 1) //If the pivot number is greater than the correct position it moves the right element to the left
			right = pivotNum - 1;
		else 
			left = pivotNum + 1; //If the pivot number is less than the correct partition it moves the left element to the right
	}
	return -1; 
}

//Function that creates a KD tree based on a node and a double vector
void makeTree(node** aNode, vector<vector<int> > aVector) {
	int realMedian; //Variable for the median of the vector
	vector<vector<int> > subRight; //Creates a new double vector for the values less than or equal to the median
	vector<vector<int> > subLeft; //Creates a new double vector for the values greater than the median
	if(aVector.size() > b) { //Checks to see if the vector size is greater than the bucket size limit
		int dimension = bestDimension(aVector); //Finds the best dimension of the double vector
		if (aVector.size() % 2 == 0) { //Calculates the median if the number of points is even
			int medVal1 = quickSelect(aVector, 0, aVector.size() - 1, aVector.size()/2, dimension);
			int medVal2 = quickSelect(aVector, 0, aVector.size() - 1, aVector.size()/2 + 1, dimension);
			realMedian = (medVal1 + medVal2)/2;
		}
		else { //Calculates the median if the number of points is odd
			realMedian = quickSelect(aVector, 0, aVector.size() - 1, ceil(aVector.size()/2), dimension);
		}
		for (double i = 0; i < aVector.size(); i++) { //Goes through each point of the initial vector
			vector<int>  temp1; //Creates two temporary vectors
			vector<int>  temp2;
			for (double j = 0; j < aVector[0].size(); j++) { //Goes through each dimension of a point
				if (aVector[i][dimension] <= realMedian) //Checks to see if the value is less than or equal to the median
					temp1.push_back(aVector[i][j]); //If it is then put that value into the first temporary vector
				else 
					temp2.push_back(aVector[i][j]); //If it is not then put that value into the second temporary vector
			}
			if (aVector[i][dimension] <= realMedian) 
				subLeft.push_back(temp1); //Puts the first temporary vector in the left vector
			else 
				subRight.push_back(temp2); //Puts the second temporary vector in the right vector
		}
		if (*aNode == NULL) { //Checks to see if the node is NULL
			node* newNode = node::makeInternalNode(realMedian, dimension); //Creates a new node
			*aNode = newNode;
		}
		makeTree(&( (*aNode)->leftChild), subLeft); //Recursively calls makeTree on the left sub vector
		makeTree(&( (*aNode)->rightChild), subRight); //Recursively calls makeTree on the right sub tree
		
	}

	else { //if the size of the vector is less than the bucket size
		node* newNode = node::makeLeafNode(aVector); //Create a node to store the bucket
		*aNode = newNode;
	}
}

//Function that prints a double vector
void printKD(vector<vector<int> > aVector) {
	for (double i = 0; i < aVector.size(); i++) { //Goes through each point
		for (double j = 0; j < aVector[i].size(); j++) {//Goes through each dimension
			cout << aVector[i][j] << " ";
		}
		cout << endl;
	}
}


//Function that test a probe point to see where it would go in the KD tree
void testProbe(vector<int> probe, node** aNode) {
	if ((*aNode)->isLeaf == 1) { //Checks to see if the node is a leaf node
		cout << "probe "; // If it is then print the probe and the bucket it would do in
		for (double i = 0; i < probe.size(); i++)
			cout << probe[i] << " ";
		cout << "reach bucket: " << (*aNode)->bucket.size() << " elements" << endl;
		printKD((*aNode)->bucket); //Uses printKD to print the bucket
	}
	else { //If it is not a leaf node
		int bestDimension = (*aNode)->dimension; //find the best dimension
		if (probe[bestDimension] <= (*aNode)->median) //Compare the probe to the median
			testProbe(probe, &( (*aNode)->leftChild)); //Recursively calls testProbe on the left child
		else 
			testProbe(probe, &( (*aNode)->rightChild)); //Recursively calls testProbe on the right child
	
	}
}

int main(int argc, char *argv[]) {

	int k; //Variable for number of dimensions
	int n; //Variable for number of data points
	int p; //Variable for the number of probes
	if (argc == 4) {
		k = stoi(argv[1]);
		n = stoi(argv[2]);
		p = stoi(argv[3]);
	}

	if ((k > 0) && (n > 0) && (p > 0)) { //Checks to see if the command line parameters are all greater than 0

		//int data[n*k];
		vector<vector<int> > myVector; //Creates a double vector
		int x; //Variable to hold data being inputted
		node* myNode = NULL; //Creates a node and sets it to NULL

		for (int i = 0; i < n; i++) { //Goes through and puts each point into the double vector
			vector<int> temp;
			for (int j = 0; j < k; j++) {
				cin >> x;
				temp.push_back(x);
			}
			myVector.push_back(temp);
		}

		makeTree(&myNode, myVector); //Makes a KD tree based on the double vector and node that were created
	
		vector<int> probeVector; //Creates a vector to hold a probe data point
	
		for (int i = 0; i < p; i++) { //Creates and test p amount of probes
			for (int j = 0; j < k; j++) { //Inputs the probe data point
				cin >> x;
				probeVector.push_back(x);
			}
			testProbe(probeVector, &myNode); //Tests the probe to see where it would be placed
			probeVector.clear(); //Clears the probe vector
		}
	}

	else { //prints an error message if the command line parameters are not correct
		cout << "Invalid command line parameters" << endl;
	}

	return 0;
}
