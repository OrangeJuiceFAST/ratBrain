    #include <iostream>
    #include <vector>

    using namespace std; //shortens syntax, instead of saying std::cout() i just say cout();

    class Neuron { // defines a new type of object, like "int" but it has attributes (value) and methods (basically functions)
        public:
            Neuron(); // defines the constructure, which is below all these class declarations
            double getValue(); // it returns the double value; i can't ust "neuronName.value" because value is a private attribute. It's just good form, prevents mistakes.
            void setValue(); // allows you to modify value
            //double applyActivator(int type); // gonna implement this later
        private:
            double value;
    };

    class Dendrite { // compare to neuron
        public:
            double getWeight();
            void setWeight();
        private:
            double weight;
    };

    class Brain {
        public:
            Brain(int size, int brainNum); // notice the constructor has a parameter. This allows you to decide the size of the brain
            void setWeights(); // sets dendrite weights from file
            void setInput(); // sets input neurons from file
            void writeOutput(); // calculates final output, calculates the error, saves it to file. Eventually, it will contain hundreds of errors from hundreds of input files
            void deconstructor();
        private:
            int brainNum; //Sets which individual number the brain is. Because I'll be working with hundreds, i don't want hundreds of variable names.
            int size; // size of array, in length of neuron square. It is always a square
            Neuron*** brainArray; // Strange syntax, but the * means its a pointer This means it's a 2D array of Neurons. The extra star is because you can't actually store a neuron
                                // in an array, you can only store a pointer to it
            Dendrite*** dendriteArray;
            Dendrite** finalDendriteArray; // this one is seperate because each neuron only has 1 pointing to the final output
    };

    Neuron::Neuron() : value(0) {// constructor, sets value to zero as a default;
    }

    Brain::Brain(int size, int brainNum) : brainNum(brainNum), size(size), brainArray(), dendriteArray(), finalDendriteArray(){
        this->brainArray = new Neuron**[size];
        this->dendriteArray = new Dendrite**[size-1];
        this->finalDendriteArray = new Dendrite*[size];
        for(int i = 0; i < size; ++i){
            brainArray[i] = new Neuron*[size];
            finalDendriteArray[i] = new Dendrite();
            for(int j = 0; j < size; ++j){
                brainArray[i][j] = new Neuron();
            }
        }
        //All this does is initialize the arrays. You don't need to do this in most other languages, but because I'm interacting
        //Directly with the RAM, you need to.
        for(int i = 0; i < size-1; ++i){
            dendriteArray[i] = new Dendrite*[size*size];
            for(int j = 0; j < size*size; ++j){
                dendriteArray[i][j] = new Dendrite();
            }
        }
    };

    void Brain::setWeights() {
        //Read from file, loop through dendrite array, set proper values. 

        // if no file/ failure, set to random values.

        // dont forget the finalArray
    };

    void Brain::writeOutput(){
        //using finalDendriteArray, get output
    };

    void Brain::deconstructor(){
        //deconstruct
    };

    int main() {
        try{
            //real program here
        } catch(const exception &e){
            //if any error occurs, this will tell us what
            cout << e.what() << endl;
            return 1; // default fail value;
        }
    }
