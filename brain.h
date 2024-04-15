#ifndef BRAIN_H
#define BRAIN_H
#include "brain.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string> 
using namespace std;

void filePutContents(const std::string& name, const std::string& content, bool append = false) {
        std::ofstream outfile;
        if (append)
            outfile.open(name, std::ios_base::app);
        else
            outfile.open(name);
        outfile << content;
    }

void clearAllFiles(int numFiles){
    for(int i = 0; i < numFiles; ++i){
        string filestring = "files/input/weights";
        filestring.append(to_string(i));
        filestring.append(".txt");
        remove(filestring.c_str());
        filestring = "files/output/results";
        filestring.append(to_string(i));
        filestring.append(".txt");
        remove(filestring.c_str());
    }
}

class Neuron { // defines a new type of object, like "int" but it has attributes (value) and methods (basically functions)
    public:
        Neuron(); // defines the constructure, which is below all these class declarations
        double getValue(); // it returns the double value; i can't ust "neuronName.value" because value is a private attribute. It's just good form, prevents mistakes.
        void setValue(double newValue); // allows you to modify value
        //double applyActivator(int type); // gonna implement this later
    private:
        double value;
};

    
class Dendrite { // compare to neuron
        public:
            Dendrite();
            double getWeight();
            void setWeight(double weight);
        private:
            double weight;
    };

class Brain {
        public:
            Brain(int size, int brainNum); // notice the constructor has a parameter. This allows you to decide the size of the brain
            void setWeights(); // sets dendrite weights from file
            void setInput(int trainingSetNum); // sets input neurons from file
            void runBrain();
            void saveWeights();
            void writeOutput(int trainingSetNum); // calculates final output, calculates the error, saves it to file. Eventually, it will contain hundreds of errors from hundreds of input files
            void deconstructor();
            int getBrainNum();
            double getFitness(int trainingSetSize);
            Neuron*** brainArray; // Strange syntax, but the * means its a pointer This means it's a 2D array of Neurons. The extra star is because you can't actually store a neuron
                                // in an array, you can only store a pointer to it
            Dendrite*** dendriteArray;
            Dendrite** finalDendriteArray; // this one is seperate because each neuron only has 1 pointing to the final output
        private:
            int brainNum; //Sets which individual number the brain is. Because I'll be working with hundreds, i don't want hundreds of variable names.
            int size; // size of array, in length of neuron square. It is always a square
    };



Dendrite::Dendrite() : weight(0) {
    this->weight = weight;
};

double Dendrite::getWeight() {
    return this->weight;
};

void Dendrite::setWeight(double weight){
    this->weight = weight;
}


Neuron::Neuron() : value(0) {// constructor, sets value to zero as a default;
};

double Neuron::getValue() {
    return this->value;
};

void Neuron::setValue(double value) {
    this->value = value;
}

    Brain::Brain(int size, int brainNum) : brainArray(), dendriteArray(), finalDendriteArray(), brainNum(brainNum), size(size){
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

    int Brain::getBrainNum() {
        return this->brainNum;
    }

    void Brain::setWeights() {
        ifstream inFS;
        string filestring = "files/input/weights";
        filestring.append(to_string((int)(this->getBrainNum())));
        filestring.append(".txt");
        inFS.open(filestring);
        if(!inFS.is_open()){
            string errorstring = filestring;
            errorstring.append(" could  not be opened.");
            throw invalid_argument(errorstring);
        }
        string current;
        double currentDouble;
        for(int i = 0; i < size-1; ++i){
            for(int j = 0; j < size*size; ++j){
                inFS >> current;
                currentDouble = atof(current.c_str());
                if(inFS.fail() || inFS.bad()){
                    string errorString = "inFs failure. ";
                    errorString.append(current);
                    throw runtime_error(errorString);
                }
                dendriteArray[i][j]->setWeight(currentDouble);
            }
        }

        for(int i = 0; i < size; ++i){
            inFS >> current;
            currentDouble = atof(current.c_str());
            if(inFS.fail() || inFS.bad()){
                string errorString = "inFs failure. ";
                errorString.append(current);
                throw runtime_error(errorString);
            }
            finalDendriteArray[i]->setWeight(currentDouble);
        }
        inFS.close();
    };

    void Brain::setInput(int trainingSetNum) {
        ifstream inFS;
        string filestring = "files/input/trainingdata";
        filestring.append(to_string(trainingSetNum));
        filestring.append(".txt");
        inFS.open(filestring);
        if(!inFS.is_open()){
            throw runtime_error(filestring);
        }
        string garbage;
        inFS>>garbage;
        string current;
        double currentDouble;
        for(int i = 0; i < size; ++i){
            inFS >> current;
            currentDouble = atof(current.c_str());
            if(inFS.fail() || inFS.bad()){
                string errorString = "inFs failures. ";
                errorString.append(current);
                errorString.append(garbage);
                throw runtime_error(errorString);
            }
            this->brainArray[0][i]->setValue(currentDouble);
        }
        inFS.close();
    }

    void Brain::runBrain() {
        for(int i = 1; i < size; ++i){
            for(int j = 0; j < size; ++j){
                for(int k = 0; k < size; ++k){
                    double valueToSet = brainArray[i][j]->getValue();
                    valueToSet += brainArray[i-1][k]->getValue() * dendriteArray[i-1][k*size+j]->getWeight(); //add the neurons previous * the weight of the dendrites
                    brainArray[i][j]->setValue(valueToSet);
                }

                if(brainArray[i][j]->getValue() < 0){ //Sigmoid?
                    brainArray[i][j]->setValue(0);
                }
            }
        }
    };

    void Brain::saveWeights() {
        ofstream oFS;
        string filestring = "files/output/results";
        filestring.append(to_string((int)(this->getBrainNum())));
        filestring.append(".txt");

        string current;
        for(int i = 0; i < size-1; ++i){
            for(int j = 0; j < size*size; ++j){
                current = dendriteArray[i][j]->getWeight();
                current.append(" ");
                oFS << current;
            }
        }
        oFS.close();
    }

    void Brain::writeOutput(int trainingSetNum) {
        ifstream inFS;
        string filestring = "files/input/trainingdata";
        filestring.append(to_string(trainingSetNum));
        filestring.append(".txt");
        string garbage;
        double doubleGarbage;
        doubleGarbage = atof(garbage.c_str());
        inFS>>garbage; //These last 6 lines give us the correct value from the training sets

        filestring = "files/output/results";
        filestring.append(to_string((int)(this->getBrainNum())));
        filestring.append(".txt"); // these set the filestring for the filePutContents button

        double finalValue = 0;
        for(int i = 0; i < size; ++i){
            finalValue += brainArray[size-1][i]->getValue() * finalDendriteArray[i]->getWeight();
        } //sets final value from input
        finalValue = finalValue-doubleGarbage;
        finalValue = finalValue*finalValue; //subtract correct value and square
        string finalStrValue = to_string(finalValue);
        finalStrValue.append("\n");
        filePutContents(filestring, finalStrValue, true);
        inFS.close();
    }

    double Brain::getFitness(int trainingSetSize) {
        double fitness = 0;
        string filestring;
        filestring = "files/output/results";
        filestring.append(to_string((int)(this->getBrainNum())));
        filestring.append(".txt"); // these set the filestring for the filePutContents button
        ifstream inFS;
        inFS.open(filestring);
        string current;
        double currentDouble;
        for(int i = 0; i < trainingSetSize; ++i){
            inFS >> current;
            currentDouble = atof(current.c_str());
            fitness += currentDouble;
        }
        inFS.close();
        return fitness;
    }


    void Brain::deconstructor(){
        for(int i = 0; i < size; ++i){
            for(int j = 0; j < size; ++j){
                delete brainArray[i][j];
            }
            delete[] brainArray[i];
            delete finalDendriteArray[i];
        }
        //All this does is initialize the arrays. You don't need to do this in most other languages, but because I'm interacting
        //Directly with the RAM, you need to.
        for(int i = 0; i < size-1; ++i){
            for(int j = 0; j < size*size; ++j){
                delete dendriteArray[i][j];
            }
            delete[] dendriteArray[i];
        }
        delete[] brainArray;
        delete[] dendriteArray;
        delete[] finalDendriteArray;
        delete this;
    };

#endif