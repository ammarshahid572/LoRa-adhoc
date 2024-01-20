#define tableLength 6
#define maxTimeout 3

/*
   By default destinations array will contain 0.
   Therefore destination number will be more than 0.
*/


int destinations[tableLength];
int nextHops[tableLength];
int costs[tableLength];
int timeouts[tableLength]; //if timeouts reaches zero the link is down

int indexOfDestination(int destination) {
  for (int i=0; i < tableLength; i++) {
    if (destinations[i] == 0 ) break; // this helps in keeping routing table arranged.
    if (destinations[i] == destination) return i;
  }
  return -1;
}


// this function updates the table
void updateTable(int neighbour, int destination, int s2nCost) {
  //n2dCost: neighbour to destination cost
  //s2nCost: self to neighbour cost
  int totalCost = s2nCost;

  //first check if destination already exist in the table
  int i = indexOfDestination(destination);
  if (i > -1) { //destination exists
    if (neighbour == nextHops[i]) //if neighbour is already set as next hop for this destination
    { costs[i] = totalCost; //update total cost for this destination
      timeouts[i] = maxTimeout;
    }
    else {
      //otherwise check if totalCost is less than previous cost
      if (costs[i] > totalCost) {
        //update the next hop to this neighbour and its cost
        nextHops[i] = neighbour;
        timeouts[i] = maxTimeout;
        costs[i] = totalCost;
      }
    }
    return;
  }

  //destination does not exist
  //find an empty spot for it.
  for (i = 0; i < tableLength; i++) {
    if (destinations[i] == 0) {
      //empty space found at i
      destinations[i] = destination;
      nextHops[i] = neighbour;
      costs[i] = totalCost;
      timeouts[i] = maxTimeout;
      return;
    }
  }
}

void updateTimeouts() {
  for (int i = 0; i < tableLength; i++) {
    if (timeouts[i]>-1)
      timeouts[i]--;
    if (timeouts[i] ==-1) {
      nextHops[i]=0;
      destinations[i]=0;
      }
    if (timeouts[i] <= 0) {
      costs[i] = 255;
    }
  }
}



void printTable() {
  Serial.println("Routing Table:");
  for (int i = 0; i < tableLength; i++) {

    Serial.print(destinations[i]);
    Serial.print('\t');
    Serial.print(nextHops[i]);
    Serial.print('\t');
    Serial.print(costs[i]);
    Serial.print('\t');
    Serial.print(timeouts[i]);
    Serial.println ('\t');

  }
}
