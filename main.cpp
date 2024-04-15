    #include <iostream>
    #include <vector>
    #include <fstream>
    #include <string> 
    #include "brain.h"
    #include <random>

    using namespace std; //shortens syntax, instead of saying std::cout() i just say cout();

    int main() {
        try{
            const int size = 3;
            const int trainingSetNumber = 6;
            const int numberOfModels = 5;
            vector<Brain*> brainList = {};

            for(int i = 0; i < numberOfModels; ++i){
                Brain *currentBrain = new Brain(size,i);
                brainList.push_back(currentBrain);
            }

            for(int i = 0; i < numberOfModels; ++i){
                cout << "Brain number " << i << " has been created" << endl;
                brainList.at(i)->setWeights();
                cout << "Brain number " << i << " has it's weights set" << endl;
                for(int j = 0; j < trainingSetNumber; ++j){
                    cout << "Brain number " << i << " began reading from file: " << j << endl;
                    brainList.at(i)->setInput(j);
                    brainList.at(i)->runBrain();
                    brainList.at(i)->writeOutput(j);
                }
                cout << "Brain number " << i << " has a fitness of " << brainList.at(i)->getFitness(trainingSetNumber) <<"." << endl;
            }

            for(int i = 0; i < numberOfModels; ++i){
                cout << "Brain number " << i << " has a fitness of " << brainList.at(i)->getFitness(trainingSetNumber) <<"." << endl;
                brainList.at(i)->deconstructor();
            }

        } catch(const exception &e){
            //if any error occurs, this will tell us what
            cout << e.what() << endl;
            return 1; // default fail value;
        }
    }
