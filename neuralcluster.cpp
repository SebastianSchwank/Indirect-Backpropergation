#include "neuralcluster.h"


NeuralCluster::NeuralCluster(int inputs, int outputs, int hidden, int attention){

    numInputs = inputs;
    numOutputs = outputs;
    numHiddens = hidden;
    numRekurrent = attention;

    vector<vector<float> > weightsCreation;
    for(int i = 0; i < inputs+outputs+hidden+attention+1; i++){
        fireCounter.push_back(1.0);
        counterActivation.push_back(1.0);
        lastCounter.push_back(1.0);
        polarityCounter.push_back(1.0);
        fireReal.push_back(1.0);
        realActivation.push_back(1.0);
        lastReal.push_back(1.0);
        beforelastReal.push_back(1.0);
        polarityReal.push_back(1.0);
        period.push_back(rand()%maxPeriod+1);
        counter.push_back(rand()%period[i]+1);
        slowness.push_back(1.0);
        momentumVector.push_back(0.0);
        //if(i > inputs+outputs) slowness[i] = 1.0*rand()/RAND_MAX;
        //slowness[i] = slowness[i]*slowness[i]*slowness[i];
        vector<float> weightColumnActive;
        vector<float> weightColumnInactive;

        vector<float> momentumColumn;
        vector<int> firingColumn;
        vector<float> deltaColumn;
        for(int j = 0; j < inputs+outputs+hidden+attention+1; j++){
            //if((j+i)%2 ==0) weightColumn.push_back(-0.001);
            //else  weightColumn.push_back(0.001);

            //weightColumn.push_back(0.01);

            weightColumnActive.push_back(2.0*((1.0*rand()/RAND_MAX-0.5)));
            weightColumnInactive.push_back(0.2*((1.0*rand()/RAND_MAX-0.5)));
            momentumColumn.push_back(0.2*((1.0*rand()/RAND_MAX-0.5)));
            firingColumn.push_back(rand()%2);
            deltaColumn.push_back(0.0);

        }
        firingMatrixCounter.push_back(firingColumn);
        weightsActive.push_back(weightColumnActive);
        weightsInactive.push_back(weightColumnInactive);
        deltaMatrix.push_back(deltaColumn);
        weightsNeurons.push_back(1.0);
        //weightsNeurons[i] = 10.0*((2.0*rand()/RAND_MAX)-1.0);
        momentum.push_back(momentumColumn);
        slope.push_back(2.0*((1.0*rand()/RAND_MAX-0.5)));
        error.push_back(0.02*((1.0*rand()/RAND_MAX-0.5)));
        lastError.push_back(0.01*((1.0*rand()/RAND_MAX-0.5)));
        beforeLasteError.push_back(0.01*((1.0*rand()/RAND_MAX-0.5)));
        derivedError.push_back(0.0);
        samplerReal.push_back(1.0);
        samplerCounter.push_back(1.0);
        OutputSamplerReal.push_back(1.0);
        OutputSamplerCounter.push_back(1.0);
        integratorReal.push_back(0.0);
        integratorCounter.push_back(0.0);
        OutputIntegratorReal.push_back(0.0);
        OutputIntegratorCounter.push_back(0.0);
        EnergyFlowReal.push_back(1.0);
        EnergyFlowCounter.push_back(1.0);
        ActivityReal.push_back(1.0);
        ActivityCounter.push_back(1.0);
        beforelastReal.push_back(0.0);
        beforelastCounter.push_back(0.0);
    }

    fireCounter[fireCounter.size()-1] = 1.0;
    fireReal[fireReal.size()-1] = 1.0;



}

vector<vector<float>> NeuralCluster::getWeights(){

    return weightsActive;
}

void NeuralCluster::resetSampler(bool randomize){
        samples = 0.0;

        for(int i = 0; i < weightsActive.size()-1; i++){

            integratorCounter[i] = 0.0;
            OutputIntegratorCounter[i] = 0.0;
            integratorReal[i] = 0.0;
            OutputIntegratorReal[i] = 0.0;

            ActivityReal[i] = 0.0;
            ActivityCounter[i] = 0.0;

            EnergyFlowReal[i] = 0.0;
            EnergyFlowCounter[i] = 0.0;

            fireCounter[i] = 0.0;
            samplerCounter[i] = 0.0;
            samplerReal[i] = 0.0;
            fireReal[i] = 0.0;

            lastReal[i] = 0.0;
            lastCounter[i] = 0.0;

            beforelastReal[i] = 0.0;
            beforelastCounter[i] = 0.0;
        }

        if(randomize){

            for(int i = 0; i < weightsActive.size()-1 ; i++){
                fireCounter[i] = 1.0*rand()/RAND_MAX;
                samplerCounter[i] = 1.0*rand()/RAND_MAX;
                EnergyFlowCounter[i] = 1.0*rand()/RAND_MAX;
                ActivityCounter[i] = 1.0*rand()/RAND_MAX;
                samplerCounter[i] = 1.0*rand()/RAND_MAX;
                EnergyFlowCounter[i] = 1.0*rand()/RAND_MAX;
                fireCounter[i] = 1.0*rand()/RAND_MAX;
                ActivityCounter[i] = 1.0*rand()/RAND_MAX;
            }
        }

}

void NeuralCluster::removeNonlin(float learningRate){

}

void NeuralCluster::train(float learningRate){
/*
    float maxVal = 0.0;
    vector<int> neuronsChain;
    int maxChainLength = 16;

    int length = rand()%maxChainLength;

    neuronsChain.push_back(rand()%weights.size());
    for(int i = 0; i < length; i++){
        int k = rand()%weights.size();
        while(weights[neuronsChain[i]][k] == 0.0){
            k = rand()%weights.size();
        }
        neuronsChain.push_back(k);
    }

    float ErrorSum = 0.0;
    for(int i = 0; i < neuronsChain.size()-1; i++){
        ErrorSum += (realNetActivation[neuronsChain[i]]-counterActivation[neuronsChain[i]]);
    }

    for(int i = 1; i < neuronsChain.size(); i++){
        weights[neuronsChain[i-1]][neuronsChain[i]] += counterActivation[neuronsChain[i]] * ErrorSum  * counterActivation[neuronsChain[i-1]] * (1.0-counterActivation[neuronsChain[i-1]]);
    }
*/

    beforeLasteError = lastError;
    lastError = error;
    float absMax = 0.0;
    for(int i = 0; i < weightsActive.size()-1; i++){
        //if((i >= numInputs) && (i < numInputs+numOutputs+numHiddens)) error[i] = (fireReal[i]-fireCounter[i]);
        //if(i < numInputs) error[numInputs+numOutputs+numHiddens+i] = (fireReal[i]-fireCounter[i]);

        error[i] = (EnergyFlowReal[i]-EnergyFlowCounter[i]);
        //if(i >= numInputs+numOutputs) error[i] = -error[i];
        //slope[i] += (slope[i]-(fireReal[i]-fireCounter[i])*(fireReal[i]-fireCounter[i])*fireCounter[i]*0.25)*0.001;
        //if(i > numInputs+numOutputs+numHiddens) error[i] = (fireReal[i+numInputs+numOutputs+numHiddens]-fireCounter[i+numInputs+numOutputs+numHiddens]);

        derivedError[i] = (error[i])/((error[i]*lastError[i]));
        if(derivedError[i] != derivedError[i]) derivedError[i] = 0.0;
        //cout << i << ":" << derivedError[i] << " ";

        float errorTerm = 0.0;


        for(int j = 0; j < weightsActive[i].size(); j++){
            //momentum[i][j] += lastCounter[j]*(realNetActivation[i]-counterActivation[i])*(counterActivation[i])*(1.0-counterActivation[i])*learningRate;
            //momentum[i][j] -= (1.0-lastCounter[j])*(realNetActivation[i]-counterActivation[i])*(counterActivation[i])*(1.0-counterActivation[i])*learningRate;


            //weights[j][i] += ((lastCounter[j])*(realNetActivation[i]-counterActivation[i]))*(1.0+counterActivation[i])*(1.0-counterActivation[i])*learningRate;
            //weights[i][j] -= ((2.0-lastCounter[j])*(realNetActivation[i]-counterActivation[i]))*(1.0+counterActivation[i])*(1.0-counterActivation[i])*learningRate;
          //weights[j][i] += ((counterActivation[j])*(realNetActivation[i]-counterActivation[i]))*(1.0+counterActivation[i])*(1.0-counterActivation[i])*learningRate;
            //weights[j][i] += ((counterActivation[j])*(realNetActivation[i]-counterActivation[i]))*counterActivation[i]*(1.0-counterActivation[i])*learningRate;
            //weights[j][i] += ((counterActivation[j])*(realNetActivation[i]-counterActivation[i]))*counterActivation[i]*(1.0-counterActivation[i])*learningRate;
            //weights[j][i] += (((counterActivation[j])*(realNetActivation[i]-counterActivation[i]))*counterActivation[i]*(1.0-counterActivation[i]))*learningRate;
            //weights[i][j] -= (1.0-(lastCounter[j]))*(realNetActivation[i]-counterActivation[i])*counterActivation[i]*(1.0-counterActivation[i])*learningRate;

            //deltaSynapse[i][j] = realNetActivation[j]*weights[i][j]*realNetActivation[i]-counterActivation[j]*weights[i][j]*counterActivation[i];

            bool skip = false;


            //if((i >= 0)&&(j >= 0) && (i < numInputs) && (j < numInputs)){ weightsActive[i][j] = 0.0; weightsInactive[i][j] = 0.0;skip = true;}

            if((i >= 0)&& (j >= 0) && (i < numInputs+numOutputs)&& (j < numInputs+numOutputs)){ weightsActive[i][j] = 0.0; weightsInactive[i][j] = 0.0; skip = true;}
            //if((i >= numInputs)&& (j >= numInputs) && (i < numInputs+numOutputs)&& (j < numInputs+numOutputs)){ weights[i][j] = 0.0; skip = true;}

            //if((i >= numInputs+numOutputs)&& (j >= numInputs+numOutputs) && (j < numInputs+numOutputs+numHiddens) && (i < numInputs+numOutputs+numHiddens)){ weightsActive[i][j] = 0.0;weightsInactive[i][j] = 0.0; skip = true;}
            if(i == j){ weightsActive[i][j] = 0.0;weightsInactive[i][j] = 0.0; skip = true;}

            //if((i >= numInputs+numOutputs)&& (j >= numInputs+numOutputs) && (j < numInputs+numOutputs+numHiddens) && (i < numInputs+numOutputs+numHiddens)){ weights[i][j] = 0.0; skip = true;}

            //if((i >= numInputs+numOutputs+numHiddens)&& (j >= numInputs+numOutputs+numHiddens) && (j <= numInputs+numOutputs+numHiddens+numRekurrent-1) && (i <= numInputs+numOutputs+numHiddens+numRekurrent)){ weights[i][j] = 0.0; skip = true;}

            //if((i >= numInputs+numOutputs+numHiddens+numRekurrent)&& (j >= numInputs+numOutputs+numHiddens+numRekurrent) && (j <= numInputs+numOutputs+numHiddens+numRekurrent*2-1) && (i <= numInputs+numOutputs+numHiddens+numRekurrent*2)){ weights[i][j] = 0.0; skip = true;}

            //if((i > numInputs+numOutputs+numHiddens/2)&& (j > numInputs+numOutputs+numHiddens/2) && (j <= numInputs+numOutputs+numHiddens-1) && (i <= numInputs+numOutputs+numHiddens)){ weights[i][j] = 0.0; skip = true;}
            //if((i > numInputs+numOutputs+numHiddens)&& (j > numInputs+numOutputs+numHiddens) && (j <= numInputs+numOutputs+numHiddens+numRekurrent-1) && (i <= numInputs+numOutputs+numHiddens+numRekurrent)){ weights[i][j] = 0.0; skip = true;}


            //if(i == j){ weights[i][j] = 0.0; skip = true;}

            //if(rand()%16 != 0) skip = true;





            if(!skip){

                //weights[i][j] *= counterActivation[j]*(1.0-abs(realNetActivation[i]-counterActivation[i]))*learningRate*0.001+1.0;
                //weights[i][j] *= 1.0+counterActivation[j]*(1.0-abs(realNetActivation[i]-counterActivation[i]));

                float currentError = ((1.0-abs((samplerCounter[j]*samplerCounter[j]*sqrt(0.25*error[i]*error[i])))));
                currentError *= ((1.0-abs((samplerCounter[i]*samplerCounter[i]*sqrt(0.25*error[j]*error[j])))));



                //momentum[i][j] *= currentError;
                //momentum[i][j] *= 0.9;
                errorTerm =   (error[i]);//+(lastError[j])*(1.0-abs(error[i]))*signum(-weightsActive[j][i]))*learningRate;

                weightsActive[i][j] = weightsActive[i][j]*(1.0-(lastReal[j]*lastCounter[j])*abs(error[j]*error[i])*learningRate);

                float signu = 1.0;

                if(i >= numInputs+numOutputs && j >= numInputs+numOutputs ){

                    float actI = (EnergyFlowCounter[i]+EnergyFlowReal[i])*0.5;
                    float actJ = (EnergyFlowCounter[j]+EnergyFlowReal[j])*0.5;
                    float errorDecLast = (1.0/sqrt(2))*sqrt(lastError[i]*lastError[i]+lastError[j]*lastError[j]);
                    float errorDec = (1.0/sqrt(2))*sqrt(error[i]*error[i]+error[j]*error[j]);
                    float errrorDev = errorDec-errorDecLast;

                    int k = rand()%weightsActive.size();
                    while(!(i != k && j != k)) k = rand()%weightsActive.size();
/*
                    weightsActive[i][j] -= (1.0-(error[j]))*(1.0+(error[j]))*error[j]*actI*actJ*(weightsActive[i][j])*(1.0-abs(error[i]))*abs(error[i]);
                    weightsActive[j][i] += (1.0-(error[j]))*(1.0+(error[j]))*error[j]*actI*actJ*(weightsActive[i][j])*(1.0-abs(error[i]))*abs(error[i]);
*/
                    /*
                    float errorOther = 0.0;
                    for(int k = 0; k < weightsActive.size()-1; k++){
                        float actK = (EnergyFlowCounter[k]*EnergyFlowReal[k]);
                        errorOther += ((actI*actK*(error[i])*weightsActive[i][k]*weightsActive[k][i])+(actK*(error[k])*weightsActive[i][k]));
                    }
                    */


                    //weightsActive[i][j] += abs(signum(actJ*weightsActive[i][j]))*(error[i])*errorDec*learningRate;


                    deltaMatrix[i][j] += actJ*(2.0*rand()/RAND_MAX-1.0)*abs(error[i])*learningRate;
                    deltaMatrix[i][j] -= actJ*abs(error[i])*abs(error[j])*signum(weightsActive[i][j])*learningRate;

                    //weightsActive[j][i] -= abs(error[i])*actJ*(error[i]+signum((actJ*(error[j])*weightsActive[i][j])))*errorDec*learningRate*0.1;
                    //weightsActive[j][i] -= actJ*(error[i]+signum((actJ*(error[j])*weightsActive[i][j])))*errorDec*learningRate;
                    //weightsActive[j][i] += actJ*signum(abs(actI*weightsActive[j][i]))*signum((error[i]*-weightsActive[i][j]))*errorDec*learningRate*signum(weightsActive[j][i]);

                    //if(((actI*sqrt(error[i]*error[i])*sqrt(weightsActive[j][i]*weightsActive[j][i])) > (actJ*sqrt(error[j]*error[j])*sqrt(weightsActive[i][j]*weightsActive[i][j]))))



/*
                        float errorBounceI = error[j];
                        float errorI = 0.0;


                        float errorBounceJ = error[i];
                        float errorJ = 0.0;


                        for(int i = 0; i < 2; i++){
                            errorI = signum(EnergyFlowReal[j]*errorBounceI*weightsActive[i][j]);
                            errorBounceI -= signum(EnergyFlowReal[i]*errorI*abs(error[i])*weightsActive[j][i]);

                            errorJ = signum(EnergyFlowReal[i]*errorBounceJ*weightsActive[j][i]);
                            errorBounceJ -= signum(EnergyFlowReal[j]*(errorJ*abs(error[j]))*weightsActive[i][j]);
                        }
*/

                        //float equilibrium = ((error[i])+(signum(EnergyFlowReal[j]*error[j]*weightsActive[i][j]))) + ((error[j])+(signum(EnergyFlowReal[i]*error[i]*weightsActive[j][i])));

                }else{


                    momentum[i][j] = (lastCounter[j]*lastReal[j])*error[i]*(1.0+error[j])*(1.0-error[j])*learningRate+0.9*momentum[i][j];

                    deltaMatrix[i][j] += lastCounter[j]*lastReal[j]*(error[i]*(1.0+error[j])*(1.0-error[j])+momentum[i][j]*0.1)*learningRate;

                }

                //weightsActive[j][i] += (lastReal[j]*lastCounter[j])*error[j]*error[i]*(lastReal[i]*lastCounter[i])*learningRate;
                //weightsActive[i][j] *= 0.9999;
                //weightsActive[j][i] += (lastReal[j]*lastCounter[j])*(error[i]+lastReal[i]*lastCounter[i]*error[j]*signum(weightsActive[j][i]))*learningRate;

                //weightsNeurons[j] += (lastReal[j]*lastCounter[j])*((error[i]+(lastReal[i]*lastCounter[i])*error[j]*signum(weightsNeurons[i]*weightsActive[j][i]))*learningRate);
                //weightsActive[j][i] += ((lastReal[j]*lastCounter[j]))*(error[i])*learningRate*signum(weightsNeurons[j]);


                //weightsNeurons[j] += lastCounter[j]*lastReal[j]*error[i]*learningRate*signum(weightsActive[i][j]);
                //weightsActive[j][i] += ((lastReal[j]*lastCounter[j]))*(error[i])*learningRate*signum(weightsNeurons[i]);


                //weightsNeurons[i] += lastCounter[j]*lastReal[j]*error[i]*learningRate;
                //weightsNeurons[i] += lastCounter[j]*lastReal[j]*error[i]*learningRate*signum(weightsNeurons[j]+weightsNeurons[i]);
                //weightsNeurons[i] += lastCounter[j]*error[i]*learningRate;
                //if(i >= numInputs) weightsNeurons[j] += EnergyFlowCounter[i]*error[j]*learningRate;
                //if(i >= numInputs) weightsNeurons[j] += EnergyFlowCounter[i]*error[j]*learningRate;

                //if(fireCounter[j] == 1.0 )weightsActive[j][i] += ((lastReal[j]*lastCounter[j]))*(error[i])*lastCounter[i]*learningRate;
                //else weightsInactive[j][i] += ((lastReal[j]*lastCounter[j]))*(error[i])*(1.0-lastCounter[i])*learningRate;

                //if(fireReal[j] == 1.0 )weightsActive[i][j] += ((lastReal[j]*lastCounter[j]))*(error[i])*lastReal[i]*learningRate;
                //else weightsInactive[i][j] += ((lastReal[j]*lastCounter[j]))*(error[i])*(1.0-lastReal[i])*learningRate;

                //if(fireReal[j] == fireReal[i])weightsActive[i][j] += (lastReal[j]*lastCounter[j]*(error[i]))*learningRate;
                //else weightsInactive[i][j] += (lastReal[j]*lastCounter[j]*(error[i]))*learningRate;

                //if(fireCounter[i] == 1.0)weightsActive[i][j] += (lastReal[j]*lastCounter[j]*(error[i]))*learningRate;
                //else weightsInactive[i][j] += (lastReal[j]*lastCounter[j]*(error[i]))*learningRate;

                //if(fireReal[i] == 1.0)weightsActive[i][j] += (lastReal[j]*(error[i]))*learningRate;
                //else weightsInactive[i][j] += (lastReal[j]*(error[i]))*learningRate;

                //weights[j][i] += (lastCounter[j]*error[i])*learningRate;

                //weights[j][i] += (2.0/(1.0+exp(-EnergyFlowCounter[j]*EnergyFlowReal[j]*(error[i])*weightsNeurons[j]*weightsNeurons[i]))-1.0)*learningRate;

                //weightsNeurons[j] += EnergyFlowCounter[j]*(error[i])*(2.0/(1.0-exp(-weightsNeurons[j]*weightsNeurons[i]))-1.0)*learningRate;
                //weights[j][i] += (EnergyFlowReal[j]*EnergyFlowCounter[j])*(error[i])*EnergyFlowCounter[i]*(1.0-EnergyFlowCounter[i])*learningRate;
                //weights[j][i] += (EnergyFlowReal[j]*EnergyFlowCounter[j])*(error[i])*learningRate;
                //weights[j][i] += (EnergyFlowReal[j]*EnergyFlowCounter[j])*(error[i])*(ActivityCounter[i])*(1.0-ActivityCounter[i])*learningRate;
                //weights[j][i] += (EnergyFlowReal[j]*EnergyFlowCounter[j])*(error[i])*(ActivityCounter[i])*(1.0-ActivityCounter[i])*learningRate;
                //weights[j][i] += (EnergyFlowReal[j])*(error[i])*learningRate+momentum[i][j]*0.0;
/*
                momentum[i][j] += (ActivityCounter[i])*error[j]*learningRate;
                weights[i][j] += (ActivityCounter[i])*error[j]*learningRate+momentum[i][j]*0.0;

                momentum[i][j] += (ActivityCounter[i])*error[j]*learningRate;
                weights[i][j] += (ActivityCounter[i])*error[j]*learningRate+momentum[i][j]*0.5;


                momentum[j][i] *= 0.9;

                momentum[j][i] += (ActivityCounter[j])*-error[i]*learningRate;
                weights[j][i] += (ActivityCounter[j])*-error[i]*learningRate+momentum[j][i]*0.5;
*/

                //weights[j][i] += (samplerCounter[j])*(error[i])*learningRate+momentum[i][j]*0.3;

                //weights[j][i] += (lastReal[j])*(realActivation[i]-lastReal[i])*0.01;

            }

        }

        /*
        float currentError = ((1.0-(((error[i]*0.5*error[i]*0.5)))));
        int j = weights.size()-1.0;

        momentum[i][j] *= currentError;
        momentum[i][j] *= 0.99;
        momentum[i][j] += 1.0*(error[i])*learningRate;
        weights[i][j] += 1.0*(error[i])*learningRate*5.0+momentum[i][j]*0.3;
        */


        //weights[i][i] += lastCounter[i]*(realNetActivation[i]-counterActivation[i])*learningRate;
    }

/*
    float absWeights = 0.0;

    for(int i = 0; i < weightsActive.size(); i++){
        for(int j = 0; j < weightsActive.size(); j++){
            absWeights += abs(weightsActive[i][j]);
        }
    }

    for(int i = 0; i < weightsActive.size(); i++){
        for(int j = 0; j < weightsActive.size(); j++){
            weightsActive[i][j] = (weightsActive[i][j])*weightsActive.size()*weightsActive.size()/absWeights;
        }
    }
*/


}
void NeuralCluster::applyLearning(){

    for(int i = 0; i < weightsActive.size(); i++){
        error[i] = (EnergyFlowReal[i]-EnergyFlowCounter[i]);
        for(int j = 0; j < weightsActive.size(); j++){
            float errorDecLast = (1.0/sqrt(2.0))*sqrt(lastError[i]*lastError[i]+lastError[j]*lastError[j]);
            float errorDec = (1.0/sqrt(2.0))*sqrt(error[i]*error[i]+error[j]*error[j]);
            float errorDev = errorDec-errorDecLast;


            float errorDecLastI = sqrt(lastError[i]*lastError[i]);
            float errorDecI = sqrt(error[i]*error[i]);
            float errorDevI = errorDec-errorDecLast;

            float signum = 1.0;
            if(errorDev < 0.0 && errorDevI < 0.0) signum = -1.0;

            weightsActive[i][j] += (lastCounter[j]*lastReal[j])*(deltaMatrix[i][j])*(errorDev*errorDevI);
/*
            float errorDecLastJ = (1.0/sqrt(2.0))*sqrt(lastError[j]*lastError[j]);
            float errorDecJ = (1.0/sqrt(2.0))*sqrt(error[j]*error[j]);
            float errorDevJ = errorDec-errorDecLast;

            weightsActive[j][i] += (lastCounter[j]*lastReal[j])*(deltaMatrix[j][i])*(errorDev*errorDevJ);
*/
        }
    }

}

void NeuralCluster::resetDeltaMatrix(){

    for(int i = 0; i < weightsActive.size(); i++){
        for(int j = 0; j < weightsActive.size(); j++){
            deltaMatrix[i][j] = 0.0;
        }
    }
}

void NeuralCluster::trainBP(vector<float> target,float learningRate,int iterations){

}

float NeuralCluster::signum(float x){
    return (2.0/(1.0+(exp(-x))))-1.0;
}

vector<float> NeuralCluster::getActivation(){
    return EnergyFlowCounter;
}

vector<float> NeuralCluster::getTarget(){
    return EnergyFlowReal;
}

float NeuralCluster::minMax(float x){

    //float signum = 1.0;
    //if(x < 0.0) x = 0.0;
    //if(x >= 0.0) signum = 1.0;

/*
    if( x > 0.0) x = (2.0/(1.0+(exp(-x))))-1.0;
    else x = 0.0;
*/

    return (1.0/(1.0+(exp(-x))));
}

void NeuralCluster::inputData(vector<float> input,vector<float> output,bool againstEmpty){

    if(!againstEmpty){
    for(int i = 0; i < input.size(); i++){
        samplerCounter[i] = input[i];
        fireCounter[i] = input[i];
        EnergyFlowCounter[i] = input[i];
        ActivityCounter[i] = input[i];

        fireReal[i] = input[i];
        EnergyFlowReal[i] = input[i];
        samplerReal[i] = input[i];
        ActivityReal[i] = input[i];
    }
    for(int i = input.size(); i < output.size()+input.size(); i++) {
        samplerReal[i] = output[i-input.size()];
        EnergyFlowReal[i] = output[i-input.size()];
        fireReal[i] = output[i-input.size()];
        ActivityReal[i] = output[i-input.size()];
    }
    }

    if(againstEmpty){
        for(int i = 0; i < input.size(); i++){

            fireReal[i] = input[i];
            EnergyFlowReal[i] = input[i];
            samplerReal[i] = input[i];
            ActivityReal[i] = input[i];

        }
        for(int i = numInputs; i < output.size()+numInputs; i++) {
            samplerReal[i] = output[i-numInputs];
            EnergyFlowReal[i] = output[i-numInputs];
            fireReal[i] = output[i-numInputs];
            ActivityReal[i] = output[i-numInputs];
        }


    }
}

void NeuralCluster::propergate(vector<float> input,vector<float> output, bool sleep, bool hiddenWrite,bool againstEmpty){

    beforelastCounter = lastCounter;
    beforelastReal = lastReal;

    lastCounter = EnergyFlowCounter;
    lastReal = EnergyFlowReal;

    /*
    if(inverted){
        for(int i = 0; i < input.size(); i++){ samplerReal[i] = input[i];}
        for(int i = numInputs; i < output.size()+numInputs; i++) { samplerCounter[i] = output[i-numInputs]; samplerReal[i] = output[i-numInputs]; }
    }
    */


    for(int i = numInputs; i < numInputs+numOutputs+numHiddens+numRekurrent; i++){

        //integratorCounter[i] *= 0.9;
        fireCounter[i] = EnergyFlowCounter[i];

        //fireCounter[i] = 0.0;
        //if(samplerCounter[i] > ((2.0*rand()/RAND_MAX)-1.0)) fireCounter[i] = 1.0;
        /*
        float probability = 1.0-(1.0/(1.0+(exp(-EnergyFlowCounter[i]))));
        float ActivationProbaility = 1.0*rand()/RAND_MAX;
        if(probability < ActivationProbaility) fireCounter[i] = 1.0;
        else fireCounter[i] = 0.0;
        */
    }


    for(int i = numInputs+numOutputs; i < numInputs+numOutputs+numHiddens+numRekurrent; i++){


        //integratorReal[i] *= 0.9;

        //fireReal[i] = 0.0;
        //if(samplerReal[i] > ((2.0*rand()/RAND_MAX)-1.0)) fireReal[i] = 1.0;

        fireReal[i] = EnergyFlowReal[i];

/*
        float probability = 1.0-(1.0/(1.0+(exp(-EnergyFlowReal[i]))));
        float ActivationProbaility = 1.0*rand()/RAND_MAX;
        if(probability < ActivationProbaility) fireReal[i] = 1.0;
        else fireReal[i] = 0.0;
*/
    }


    inputData(input,output,againstEmpty);

    vector<float> deltaEnergysReal;
    vector<float> deltaEnergysCounter;
    vector<float> deltaError;


    float absEnergyReal = 0.0;
    float absEnergyCounter = 0.0;

    for(int i = 0; i < weightsActive.size(); i++){
        for(int j = 0; j < weightsActive.size(); j++){
            absEnergyCounter += abs(weightsActive[i][j]*fireCounter[j]);
        }
    }

    for(int i = 0; i < weightsActive.size(); i++){
        for(int j = 0; j < weightsActive.size(); j++){
            absEnergyReal += abs(weightsActive[i][j]*fireReal[j]);
        }
    }

    float sumCounterClassyfier = 0;

        for(int i = 0; i < weightsActive.size()-1; i++){

            //cout << slope[i] << "\n";

            float EnergyReal = 0.0;
            float EnergyCounter = 0.0;

            float EnergyOutputReal = 0.0;
            float EnergyOutputCounter = 0.0;


            float weightsMean = 0.0;

            for(int j = 0; j < weightsActive[i].size(); j++){

                    EnergyCounter +=  (fireCounter[j])*(weightsActive[i][j]+deltaMatrix[i][j])*(1.0/absEnergyCounter)*weightsActive.size()*weightsActive.size();
                    EnergyReal += (fireReal[j])*(weightsActive[i][j]+deltaMatrix[i][j])*(1.0/absEnergyReal)*weightsActive.size()*weightsActive.size();

                    /*
                    if(fireCounter[j] == 1.0) EnergyCounter +=  (weightsActive[i][j])*(lastCounter[j]);
                    else EnergyCounter +=  (weightsInactive[i][j])*(lastCounter[j]);
                    if(fireReal[j] == 1.0)EnergyReal +=  (weightsActive[i][j])*(lastReal[j]);
                    else EnergyReal +=  (weightsInactive[i][j])*(lastReal[j]);
                    */

                     //integratorReal[j] += weights[i][j]*fireReal[i];
                     //integratorCounter[j] += weights[i][j]*fireCounter[i];


                     //EnergyReal +=  weights[i][j]*fireReal[j];
                     //EnergyCounter +=  weights[i][j]*fireCounter[j];

                     //integratorReal[j] -= weights[i][j]*(fireReal[j]);;
                     //integratorCounter[j] -= weights[i][j]*(fireCounter[j]);


                     //samplerReal[j] -= deltaEnergyReal;
                     //samplerCounter[j] -= deltaEnergyCounter;


                     //samplerReal[j] -= deltaEnergyReal;
                     //samplerCounter[j] -= deltaEnergyCounter;

                     //if((i > numInputs-1)&& (i < numInputs+numOutputs)) cout << (fireReal[j]-lastReal[j]) << ",";

                    /*
                     {samplerReal[j] -= deltaEnergyReal; samplerReal[i] += deltaEnergyReal;}
                     {samplerCounter[j] -= deltaEnergyCounter; samplerCounter[i] += deltaEnergyCounter;}
                    */
                     //sumWeights += minMax(weights[i][j]);
            }

            //derived[i] = cos(y);
            /*
            x = sin(x);
            y = sin(y);
            */

            integratorReal[i] = EnergyReal;
            integratorCounter[i] = EnergyCounter;


            //cout << "\n";


            //deltaError.push_back(deltaErr);

            //derived[i] = cos(y);

            //if(maxResultReal < abs(minMax(x))) maxResultReal = abs(minMax(x));
            //if(maxResultCounter < abs(minMax(y))) maxResultCounter = abs(minMax(y));

            //derived[i] = exp(-y);
            //if(deltaEnergys[i] == 0) derived[i] = 0.0;


            //deltaEnergysI.push_back(x/weightsSum);
            //deltaEnergys.push_back(y/weightsSum);

            //counterNetActivationDerivative[i] = cos(x);
            //slowness[i] = 1.0*rand()/RAND_MAX;
        }

        float energyAbs = 0.0;

        for(int i = 0; i < weightsActive.size(); i++){
            //samplerReal[i] = minMax(integratorReal[i]-OutputIntegratorReal[i]);//+samples*samplerReal[i])/(samples+1.0);
            //OutputSamplerReal[i] = (minMax(OutputIntegratorReal[i])+samples*OutputSamplerReal[i])/(samples+1.0);

            samplerReal[i] =  minMax(integratorReal[i]);
            energyAbs +=  samplerReal[i];
        }

        for(int i = numInputs+numOutputs; i < numInputs+numOutputs+numHiddens+numRekurrent; i++){
            //samplerReal[i] = minMax(integratorReal[i]-OutputIntegratorReal[i]);//+samples*samplerReal[i])/(samples+1.0);
            //OutputSamplerReal[i] = (minMax(OutputIntegratorReal[i])+samples*OutputSamplerReal[i])/(samples+1.0);

            samplerReal[i] =  minMax(integratorReal[i]);

            //ActivityReal[i] = (fireReal[i]+samples*ActivityReal[i])/(samples+1.0);
            //EnergyFlowReal[i] = 0.5*(weightsActive.size())*samplerReal[i]/energyAbs;
            EnergyFlowReal[i] = samplerReal[i];

        }

        energyAbs = 0.0;

        for(int i = 0; i < weightsActive.size(); i++){
            //samplerCounter[i] = minMax(integratorCounter[i]-OutputIntegratorCounter[i]);//+samples*samplerCounter[i])/(samples+1.0);
            //OutputSamplerCounter[i] = (minMax(OutputIntegratorCounter[i])+samples*OutputSamplerCounter[i])/(samples+1.0);

            samplerCounter[i] =  minMax(integratorCounter[i]);

            //ActivityCounter[i] = (fireCounter[i]+samples*ActivityCounter[i])/(samples+1.0);
            energyAbs +=  samplerCounter[i];
        }

        for(int i = numInputs; i < numInputs+numOutputs+numHiddens+numRekurrent; i++){
            //samplerCounter[i] = minMax(integratorCounter[i]-OutputIntegratorCounter[i]);//+samples*samplerCounter[i])/(samples+1.0);
            //OutputSamplerCounter[i] = (minMax(OutputIntegratorCounter[i])+samples*OutputSamplerCounter[i])/(samples+1.0);

            //ActivityCounter[i] = (fireCounter[i]+samples*ActivityCounter[i])/(samples+1.0);
            //EnergyFlowCounter[i] = 0.5*(weightsActive.size())*samplerCounter[i]/energyAbs;
            EnergyFlowCounter[i] =  samplerCounter[i];
        }

        inputData(input,output,againstEmpty);

        //if(hiddenWrite) for(int i = 0; i < input.size(); i++) samplerCounter[numInputs+numOutputs+numHiddens+i] = (samplerCounter[numInputs+numOutputs+numHiddens+i])*input[i];
        //if(hiddenWrite) for(int i = 0; i < input.size(); i++) samplerReal[numInputs+numOutputs+numHiddens+i] = (samplerReal[numInputs+numOutputs+numHiddens+i])*input[i];


        //samples++;
}
