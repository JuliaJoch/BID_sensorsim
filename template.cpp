#include<stdio.h>
#include<algorithm>

//constants
const int bucketcount = 120;
const int highestValue = 900;


//global variables
int measuredValueBuckets[bucketcount];

//Values that occured most often, therefore indicating a threshold
int maxWhite;
int maxGrey;
int maxBlack;

int threshWhiteGrey;
int threshGreyBlack;


int sample(int measuredValue) {
   int i = (int) ((measuredValue * bucketcount) / highestValue);
   measuredValueBuckets[i] = measuredValueBuckets[i] + 1;
   return i;
}

void findMaxima(){
  int localMax[(int) bucketcount/2] = {0};
  int pointerToCurrentLocalMax = 0;
  
  //save all local maxima in localMax[]
  for (int i = 1; i< bucketcount; i++){
    if (measuredValueBuckets[i-1]< measuredValueBuckets[i] && measuredValueBuckets[i]>measuredValueBuckets[i+1]){
        localMax[pointerToCurrentLocalMax] = i;
        pointerToCurrentLocalMax ++;
    }
  }

  //find the three highest local maxima
  int max1 = localMax[0];
  int maxIndex = 0;
  for (int i = 1; i< (int) (bucketcount/2); i++){
    if (measuredValueBuckets[localMax[i]]>measuredValueBuckets[max1]){
      max1 = localMax[i];
      maxIndex = i;
    }
  }
  localMax[maxIndex] = 0;
  maxIndex = 0;

  int max2 = localMax[0];
  for (int i = 1; i< (int) (bucketcount/2); i++){
    if (measuredValueBuckets[localMax[i]]>measuredValueBuckets[max2]){
      max2 = localMax[i];
      maxIndex = i;
    }
  }
 localMax[maxIndex] = 0;
  maxIndex = 0;

  int max3 = localMax[0];
  for (int i = 1; i< (int) (bucketcount/2); i++){
    if (measuredValueBuckets[localMax[i]]>measuredValueBuckets[max3]){
      max3 = localMax[i];
      maxIndex = i;
    }
  }
  localMax[maxIndex] = 0;
  //idea was to make sure local Maxima won't be be to close to each other but doesn't work
  /*localMax[maxIndex-1]=0;
  localMax[maxIndex+1]=0;*/
  
  
  // associate three maxima with grey, white and black

  maxWhite = std::max(max1, std::max(max2, max3));
  maxBlack = std::min(max1, std::min(max2, max3));
  maxGrey = (max1+ max2+ max3)- maxWhite -  maxBlack;
}

void computeThresholds(){
  threshWhiteGrey = (int)((maxWhite + maxGrey) / 2); 
  threshGreyBlack = (int)((maxBlack + maxGrey) / 2);
}



//YOUR CODE
//input:
// int[] data : array of data including previous data so far, with length of num_prev_data.
// num_prev_data : number of data above
int predict(int* data, int num_prev_data){
    //your new data should be data[num_prev_data-1];
    if (num_prev_data == 1)
    {
    	for (int i = 0; i < bucketcount; ++i)
    	{
    		measuredValueBuckets[i] = 0;
    	}
    	printf("initialize array again");
    }
    int value = data[num_prev_data-1];
   	int measuredValue = sample(value);
    findMaxima();
    computeThresholds();
    
    if (measuredValue < threshGreyBlack){
      return 0;
    }else if (measuredValue < threshWhiteGrey){
      return 1;
    }else {
      return 2;
    }


    //output: 0 for black, 1 for gray and 2 for white.
    /*if(value > 300)return 2;
    if(value > 200)return 1;
    else return 0;*/
}



//////////////////////////////////////////////////////////
//////////////////DO NOT MODIFY BELOW/////////////////////
//////////////////////////////////////////////////////////
int main(void){
    float average_score = 0;
    for(int f=1; f <= 4; f++){
        char filename[10];
        sprintf(filename, "data%d.txt", f);
        FILE* fp = fopen(filename, "r");
        
        int N;
        fscanf(fp, "%d", &N);
        int data[N];
        int score = 0;
        for(int i=0; i < N; i++){
            int d, label;
            fscanf(fp, "%d %d", &d, &label);
            data[i] = d;
            int pred = predict(data, i+1);
            if(pred == label)score++;
        }
        printf("round %d: your score: %f\n",f, score / float(N));
        average_score += score/float(N);
        //score the prediction
        fclose(fp);
    }
    average_score /= 4;
    printf("average score: %f\n", average_score);
}