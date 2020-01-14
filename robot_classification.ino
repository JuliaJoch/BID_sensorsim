
// pins
int lightSensor = A0;

//constants
const int bucketcount = 50;
const int highestValue = 900;

// flags
bool foundMaxima = false;

//global variables
int measuredValueBuckets[bucketcount];
int classificationValues = 1000;

//Values that occured most often, therefore indicating a threshold
int maxWhite;
int maxGrey;
int maxBlack;

int threshWhiteGrey;
int threshGreyBlack;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(lightSensor, INPUT);
}

void loop() {


  //calibration
   if (classificationValues > 0){
    sample(analogRead(lightSensor));
    delay(10);
    classificationValues --;
  } else {
    if (!foundMaxima){
      findMaxima();
      computeThresholds();
      foundMaxima = true;
      for (int i = 0; i < bucketcount; i++){
        Serial.println(measuredValueBuckets[i]);
      }
      Serial.println(maxWhite);
    Serial.println(maxGrey);
    Serial.println(maxBlack);
    Serial.println(threshWhiteGrey);
    Serial.println(threshGreyBlack);
    

    int measuredValue = sample(analogRead(lightSensor));
    if (measuredValue < threshWhiteGrey){
      Serial.println("white");
    }else if (measuredValue < threshGreyBlack){
      Serial.println("grey");
    }else {
      Serial.println("black");
    }

    // code goes here
    // check both photodiodes and calculate the direction 
    // if one photodiode has a different value, 
    
    }
  }

   
  
  

}

int sample(int measuredValue) {
   int i = (int) ((measuredValue * bucketcount) / highestValue);
   measuredValueBuckets[i] = measuredValueBuckets[i] + 1;
   Serial.println(i);
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

  // associate three maxima with grey, white and black

  maxBlack = max(max1, max(max2, max3));
  maxWhite = min(max1, min(max2, max3));
  maxGrey = (max1+ max2+ max3)- maxWhite -  maxBlack;
}

void computeThresholds(){
  threshWhiteGrey = (int)((maxWhite + maxGrey) / 2);
  threshGreyBlack = (int)((maxBlack + maxGrey) / 2);
}
