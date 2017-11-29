//The Sarting Line
//by Lauren Race and Terrick Gutierrez
//november 16th 2017

//there is one moderator that can initialize, update question, and finalize
//there are two players, each one with a mat, each mat with a yes/no state

//at first, the state is idle
//when the moderator button is pressed, the game starts, isStarted = true;

//when all the questions are answered, the game is over, isOver = false;

//has the game started
boolean isStarted = false;
//is the game over
boolean isOver = false;
int player1ans = 0; //1 is yes and 2 is no
int player2ans = 0; //1 is yes and 2 is no

// questions
int currentQuestion = 0;
int numberOfQuestions = 10;

//2 players, each one with yes and no
//first pair are yes and no buttons for player0
//second pair are yes and no buttons for player1
//at first no button is pressed, so everything is false
boolean answers[]  = {false, false, false, false};
int numberOfAnswers = 4;

//physical side, TODO: confirm that these are the actual pins
//pin2 connected to yes / player0
//pin3 connected to  no / player0
//pin4 connected to yes / player1
//pin5 connected to no / player1
//pin6 connected to moderatorButton
int pinReading[] = {2, 3, 4, 5, 6};
int numberOfPins = 5;

//it stores the current answers for the question
//it starts at 0, because at first no one has answered
//it goes to 1, when anyone answers
//it goes to 2 when both participants answer
//when it reaches 2, we send info to p5 and we reset the humber to 0
int answersPerQuestion = 0;

//button that the moderator has
//it is false if not pressed
//it is true if it is pressed
boolean moderatorButton = false;

//int for number of yes and scores
//for each player, player0 and player1
int yes[] = {0, 0};
int numberOfYes = 2;
int scores[] = {0, 0};
int scoreMultiplier = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  setupPins();

  //comment this out to actually run it plz
  //  simulate();

}

void loop() {
  // read the switch input
  //  int buttonState = digitalRead(2);
  //  if (buttonState == HIGH) {
  //    digitalWrite(2, HIGH);
  //    // Serial.println("b1");
  //
  //  } else {
  //    digitalWrite(2, LOW);
  //
  //    int buttonState = digitalRead(3);
  //    if (buttonState == HIGH) {
  //      digitalWrite(3, HIGH);
  //      // Serial.println("b2");
  //
  //    } else {
  //      digitalWrite(3, LOW);
  //
  //      int buttonState = digitalRead(4);
  //      if (buttonState == HIGH) {
  //        digitalWrite(4, HIGH);
  //        // Serial.println("b3");
  //
  //      } else {
  //        digitalWrite(4, LOW);
  //
  //        int buttonState = digitalRead(5);
  //        if (buttonState == HIGH) {
  //          digitalWrite(5, HIGH);
  //          // Serial.println("b4");
  //
  //        } else {
  //          digitalWrite(5, LOW);
  //
  //          int buttonState = digitalRead(6);
  //          if (buttonState == HIGH) {
  //            digitalWrite(6, HIGH);
  //            //Serial.println("b5");
  //
  //          } else {
  //            digitalWrite(6, LOW);
  //
  //          }
  //        }
  //      }
  //    }
  //  }
  //for debugging, comment this out to actually run it plz
  // logStatus();

  //check answers from players and button from moderator
  checkButtons();

  ///check if continue to next question
  updateQuestion();

  //send to p5 through serial port
  p5Send();
}

//setting up digital pins to be read
void setupPins() {
  for (int i = 0; i < numberOfPins; i++) {
    pinMode(pinReading[i], INPUT);
  }
}

void checkButtons() {

  //read all of the pins to detect the answers
  for (int i = 0; i < numberOfAnswers; i++) {
    //make the answer true if the corresponding button is pressed
    if (digitalRead(pinReading[i]) == 1) {
      Serial.print("PRESSED");
      Serial.println(i);
      //this button was pressed
      answers[i] = true;
    }
  }

  if (answers[0]) {
    player1ans = 1;
  }
  else if (answers[1]) {
    player1ans = 2;
  }
  if (answers[2]) {
    player2ans = 1;
  }
  else if (answers[2]) {
    player2ans = 2;
  }

  //  Serial.print("answers ");
  //  Serial.println(answers[0]);
  //  Serial.print("answers ");
  //  Serial.println(answers[1]);
  //  Serial.print("answers ");
  //  Serial.println(answers[2]);
  //  Serial.print("answers ");
  //  Serial.println(answers[3]);

  //read the moderator button
  //CAUTION: pinReading.length is 5, answers is 4
  //because we are storing the last pin reading in moderatorButton
  //so that answers from players are in an array and moderatorButton is a loner boolean
  if (digitalRead(pinReading[4]) == 1) {
    moderatorButton = true;
    if (player1ans == 0) {
      Serial.print("player 1! ANSWER ");
    }
    else if (player1ans == 1) {
      Serial.println("player 1 said yes ");
      //count the score
      //reset player1ans to 0
      //go to next question
    }
    else if (player1ans == 2) {
      Serial.println("player 1 said no ");
      //count the score
      //reset player1ans to 0
      //go to next question
    }

    
 
    if (player2ans == 0) {
      Serial.print("player 2! ANSWER ");
    }
    else if (player2ans == 1) {
      Serial.println("player 2 said yes ");
    }
    else if (player2ans == 2) {
      Serial.println("player 2 said no ");
    }
  }



  //reset answersPerQuestion
  answersPerQuestion = 0;

  //update answersPerQuestion
  for (int i = 0; i < numberOfAnswers; i++) {
    //for every true answer, add 1 to answersPerQuestion
    // Serial.println("counting answers");
    if (answers[i] == true) {
      //  Serial.print("updating answersPerQuestion ");
      answersPerQuestion = answersPerQuestion + 1;
    }
    //    Serial.print("0answersPerQuestion ");
    //    Serial.println(answersPerQuestion);
  }
  //Serial.print("1answersPerQuestion ");
  // Serial.println(answersPerQuestion);
}

//check if we need to move the next question
void updateQuestion() {
  //if both participants answered AND the moderator pressed the button, move to next question
  //  Serial.print("2answersPerQuestion ");
  //  Serial.println(answersPerQuestion);
  if (answersPerQuestion == 2 && moderatorButton) {
    currentQuestion = currentQuestion + 1;

    //update the current score according to the answers from the current question
    updateScores();

    //reset the answers
    for (int i = 0; i < numberOfAnswers; i++) {
      answers[i] = false;
    }
  }
}


//function for updating scores everytime we go to the next question
void updateScores() {

  //check yes for every player
  //check first player
  if (answers[0]) {
    //increment yes
    yes[0]++;
  }

  //skip answers[1]
  //because it is player0 no

  //check second player
  if (answers[2]) {
    //increment yes
    yes[1]++;
  }

  //skip answers[3]
  //because it is player1 no

  //actually update scores
  //according to yes[]
  for (int i = 0; i < numberOfYes; i++) {
    //calculate corresponding score
    scores[i] = yes[i] * scoreMultiplier;
    //    Serial.print("score ");
    //    Serial.print(i);
    //    Serial.print(" ");
    //    Serial.println(scores[i]);
  }
}


//function to send to p5 through serial port
void p5Send() {
  //TODO: figure out serial port stuff
  //send the scores here
  //Serial.print(scores[0]);
  //TODO: figure out if the separator is a string "" or character ''
  //  Serial.print(",");
  //  Serial.println(scores[1]);
}

//function for simulating states
//void simulate() {
//  isStarted = true;
//  isOver = false;
//  moderatorButton = true;
//  answers[0] = false;
//  answers[1] = true;
//  answers[2] = false;
//  answers[3] = true;
//}

//function for logging current status
//very useful for debugging
void logStatus() {
  //at first, idle state
  if (isStarted == false) {
    //Serial.println("waiting...");
  }
  else {
    if (isOver == false) {
      //if isStared and is not over yet
      //Serial.println("current question:" + String(currentQuestion) + ", player0: " + String(scores[0]) + ", player1: " + String(scores[1]));
    }
    else {
      //if isStarted and isOver
      //Serial.println("game over");
    }
  }
}

