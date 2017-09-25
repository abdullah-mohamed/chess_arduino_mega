/*   This is the code of arduino Mega in Chess Project -HV team
 *   Black Rock is at row 0 and column 0
 *   written by : Abdullah abunar , Sama abdel naser and Ahmed hany
 *   Not DONE YET : 
 *   // define pins for lcd 
 *   // define pins for lcd1
 *   // IT must now read from Uno to know if a promotion (read =0 ) happened or a check ( read =1 )
 */
#include <LiquidCrystal.h>                                               // NOT DONE YET
LiquidCrystal lcdwhite(12, 11, 5, 4, 3, 2);                                   // define pins for lcd 
LiquidCrystal lcdblack(12, 11, 5, 4, 3, 2);                                  // define pins for lcd1

#define TX1    18                 // Transmission pin in Mega
#define RX1    19                 // Recieving pin in Mega
#define King   1                  // Until now giving each piece its own code isn't necessary but
#define Queen  2                  // for clarity of the code for a human reader.
#define Bishop 3
#define Knight 4
#define Rock   5
#define Pawn   6
#define Free   7                   // This represent a free spot on the board
#define Black  1                   
#define White  0 

int buzzer_p;                                               // BUZZER anode and cathode
int buzzer_n;
int changes = 0, place0 = -1, place1 = -1;                  // number of changes captured by the sensors,places of the changes on the board
int ones ;                                                  // counts number of ones on the board, i.e. : number of pieces (according to the circuit, a reading of 1 is a piece on the block and a reading of 0 is a free spot). 
int killed_pieces = 0;                                      // number of killed_pieces

int board_inti[64] =    { 5, 4, 3, 2, 1, 3, 4, 5,          // This represent the board at the beginning of the game with a black rock in row 0, col 0
                          6, 6, 6, 6, 6, 6, 6, 6,           
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          6, 6, 6, 6, 6, 6, 6, 6,
                          5, 4, 3, 2, 1, 3, 4, 5  };
                                        
int board[64]       =   { 5, 4, 3, 2, 1, 3, 4, 5,          // This represent the board while the game is on. Changes when a move happens
                          6, 6, 6, 6, 6, 6, 6, 6,           
                          7, 7, 7, 7, 7, 7, 7, 7,               
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          6, 6, 6, 6, 6, 6, 6, 6,
                          5, 4, 3, 2, 1, 3, 4, 5  };

int color_inti[64]  =   { 1, 1, 1, 1, 1, 1, 1, 1,          // This represent the colors of the pieces at the beginning of the game
                          1, 1, 1, 1, 1, 1, 1, 1,
                          7, 7, 7, 7, 7, 7, 7, 7,               
                          7, 7, 7, 7, 7, 7, 7, 7,             
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0  };

int color[64]       =   { 1, 1, 1, 1, 1, 1, 1, 1,          // This represent the colors of the pieces while the game is on. Changes when a move happens
                          1, 1, 1, 1, 1, 1, 1, 1,
                          7, 7, 7, 7, 7, 7, 7, 7, 
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0  };

int detect_inti[64] =   { 1, 1, 1, 1, 1, 1, 1, 1,         // This represent the reading of the blocks at the beginning of the game
                          1, 1, 1, 1, 1, 1, 1, 1,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1  };

int detect_0[64]    =   { 1, 1, 1, 1, 1, 1, 1, 1,           // This represent the reading of the blocks while the game is on. Changes when a move happens
                          1, 1, 1, 1, 1, 1, 1, 1,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1  };

int detect_1[64]    =   { 1, 1, 1, 1, 1, 1, 1, 1,           // This represent the reading of the blocks while the game is on. Changes when a move happens
                          1, 1, 1, 1, 1, 1, 1, 1,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1  };


//functions
void Intialize_board(void);                                 // This function intializes all arrays at the start of the game to the default values.
void Detect_me(void);                                       // This function is resposible for detection of movement on board
int  Compare_me(int *place0,int *place1);                   // Compares detect_0 and detect_1, calculates differences betwwen the 2 arrays.
                                                            // returns 1 if only a move is detected and 2 if a piece is eaten.
int  Count_my_ones(void);                                   // counts ones inside detect_1[], i.e. : number of pieces
int Get_row(int led);                                       // takes the led number from 0 to 63 and returns the row where it is located from 0 to 7
void Send_uno(int,int,int);                                 // this sends 3 pieces of data to the arduino (case of detection,row,col),
                                                            // if case = 0 , this is when only one change is detected (no eaten pieces, no moved pieces yet), a piece is lifted.
                                                            // if case = 1 , this is when only one change is detected (no eaten pieces, no moved pieces yet), a piece is put back.
                                                            // if case = 2 , this means a piece is moved to a free place,
                                                            // if case = 3 , this means a piece moved to eat another.
                                                            // in case 2 and 3 , we will summon the function twice, the first time will be (2,from_row,from_col),
                                                            //                                                      the second time will be (2,to_row,to_col),
                                                            // case 2 and 3 will require changes in board array,color array
void Notify_uno(int changes, int ones, int *killed_pieces, int place0,int place1); 
                                                            // tells the uno what happened
void Rearrange_me(int,int,int,int);                        // Changes the suitable values inside board array,color array. takes (killer,killer_color,from_place,to_place)
void Swap_0_and_1(void);                                    // swaps detect_0 and detect_1
void Read_serial(void);                                     // Reads from Uno to know if there is any thing wrong with the move
void Buzzer(void);

                                                            

void setup() {
  // intialize all arrays at the start
  Intialize_board();
  // Configuring Output and Input pins for detection
  for(int i = 2 ;i < 10 ; i++){
    pinMode(i,OUTPUT); 
  }
  for(int i = 10;i<18;i++){
    pinMode(i,INPUT);
  }
  // Buzzer Configuration and music at the start
  pinMode(buzzer_p,OUTPUT);
  pinMode(buzzer_n,INPUT);
  digitalWrite(buzzer_p,HIGH);  digitalWrite(buzzer_n,LOW);   delay(400);                          
  digitalWrite(buzzer_p,LOW);   delay(100);
  digitalWrite(buzzer_p,HIGH);  delay(250);  
  digitalWrite(buzzer_p,LOW);   delay(100);
  digitalWrite(buzzer_p,HIGH);  delay(100);  
  digitalWrite(buzzer_p,LOW);   delay(150);
  digitalWrite(buzzer_p,HIGH);  delay(200);
  digitalWrite(buzzer_p,LOW);   delay(100);
  digitalWrite(buzzer_p,HIGH);  delay(400);  
  digitalWrite(buzzer_p,LOW);
  
  // Serial and Lcd configuration
  Serial1.begin(9600);                                                        // Serial1 not Serial because we are using TX1 and RX1
  lcdwhite.begin(16,2);
  lcdblack.begin(16,2);
  lcdwhite.print("Hello, White!");
  lcdblack.print("Hello, Black!");
}

void loop() {
  // This block is just calling the functions Detect_me and Compare_me
  // if only one changes is captured then no change in pieces happened
  // most likely the player would be just testing the board's creditiblity
  // and we don't need to change any arrays
  // but if 2 changes are detected then either a piece has moved or 
  // a piece has eaten another one
  Detect_me();
  changes = Compare_me(&place0,&place1);
  ones = Count_my_ones();
  Notify_uno(changes,ones,&killed_pieces,place0,place1);
  Swap_0_and_1();
  Read_serial();
}

void Intialize_board(void){
  int i = 0;                                                                // loop counter
  for ( i = 0 ; i <64; i++){
    board[i]    = board_inti[i] ;
    color[i]    = color_inti[i] ;
    detect_0[i] = detect_inti[i];
  }
}

void Detect_me(void){
  // Detection function depends on  scrolling on blocks one by one,
  // while reading the results are stored inside an array then(using
  // Compare_me() fn, we start comparing the reading with
  // the previous one until a change is noticed and also
  // we are calculating the number of pieces on the board in order to 
  // know if a piece is eaten . The Mega sends 3 numbers to the Uno,
  // one is if a piece is eaten, the other 2 is the row and the col of the
  // movement that  happened
  int i = 0, j = 0;                                                        // Counters for loops
  int first_read = -1, second_read = -1, third_read = -1;                 // to make sure the reading is write we read it three times
  int count_out = 0;                                                      // to control where to write in the detect1 
  for( i = 2; i < 10; i++){                                               // for all the rows
        for ( j =2 ; j < 10 ; j++){                                        // for making only one row HIGH and the rest are LOW
          if ( i == j ){
            digitalWrite(j,HIGH);
          }else{
            digitalWrite(j,LOW);
          }
        }
        delay(10);
        for( j = 10; j < 18 ;j++){                                           // This reads all the columns and the reading belongs to the only row which is HIGH
                                                                             // signal interference is prevented by putting a diode after every single key in the matrix
          reread :
          first_read = (digitalRead(j));
          delay(10);
          second_read = (digitalRead(j));
          delay(10);
          third_read = (digitalRead(j));
          delay(10);
          if ( (first_read == second_read) && (first_read == third_read) ){  // I read the same input pin 3 times with a tiny delay just to make sure the signal is correct
              detect_1[count_out++] = first_read;                            // detect1[ ((i-2)*8) + (j-10)] but I have free memory :)
          }else {
            goto reread;
          }
        }
  }
}


int Compare_me(int *place0,int *place1){
  int i = 0 ;                                                               // loop counter
  int changes = 0;                                                          // Number of differences between detect_0[] and detect_1[], can't exceed 2 changes which is the case
                                                                            // a piece eating another.
  int index[4];                                                             // storage for the indicies where a change is noticed, [4] for avoiding unexpected behavior if something happened
  int index_counter = 0;
  for ( i = 0 ; i < 64; i++){
          if (detect_0[i] != detect_1[i]){                                  // count differences between the 2 arrays
              changes++;
              index[index_counter++] = i;
          }
      }
      if (changes == 1){
        *place0 = index[0];        
      }else if (changes == 2){
        // we need to know from where it came and to where it moved
        // the place where it moved will hold a one but remember that
        if(detect_0[index[0]] == 1 && detect_1[index[0]] == 0){
            *place0 = index[0];                                             //moved from index[0] to index[1]
            *place1 = index[1];
        }else{
          *(place0) = index[1];                                             //moved from index[1] to index[0]
          *(place1) = index[0];
        }
      }else{
        // There shouldn't be more than 2 changes between the 2 arrays and an error message must appear if so
        // We will disregard the last move 
        for(int i = 0 ; i< 64;i++){
          detect_1[i] = detect_0[i];
        }
        for ( i =10 ; i >0 ; i--){
        lcdwhite.clear(); lcdblack.setCursor(0,1);
        lcdblack.clear(); lcdwhite.setCursor(0,1);
        lcdwhite.print("Error!  "); lcdwhite.print(i);
        lcdblack.print("Error!  "); lcdblack.print(i);
        delay(1000);
        }
      }
  return changes;
}

int Count_my_ones(void){
  int ones =0;
  for(int i = 0; i <64 ; i++){
    if(detect_1[i] == 1){
      ones++;
    }
  }
  return ones;
}


int Get_row(int led){
  int row =0;
 if (led <= 7){
  row = 2;  //leds are connected in a matrix where rows start from pin2 to pin 9 or less and columns start from pin10 to pin 15 or less
  goto aa; 
 }else if(led <=15){
  row = 3;
  goto aa;
 }else if(led <= 23){
 row =  4;
 goto aa;
}else if (led <= 31){
  row = 5;
  goto aa;
}else if ( led <= 39){
  row = 6;
  goto aa;
}else if ( led <= 47){
  row = 7;
  goto aa;
}else if ( led <= 55){
  row = 8;
  goto aa;
}else if ( led <= 63){
  row = 9;
  goto aa;
}
aa:
return row;
}

void Notify_uno(int changes, int ones, int *killed_pieces, int place0,int place1){
  if (changes == 1){
    // a piece is lifted but not placed --> case 1 (look documentation beside Send_uno() function).
    // DONE but not sure :::: we should send something to let the board know whether the piece is lifted or put.
    // this will be done by through the code variable which is sent first to the UNO.
    // if it's 0 then the place is empty and it should light the leds.
    // if it's 1 then the place isn't empty and it should turn off the leds.
      if (detect_1[place0] == 0){
          Send_uno(0,Get_row(place0),(place0%8));                                  // rows start counting from 2 but columns start counting from 0
      }else{
          Send_uno(1,Get_row(place0),(place0%8));
      }
  }else if (changes == 2){
    // either a piece is moved or killed
  if ( ones < (32 - (*killed_pieces))){
      // this means that a piece is eaten --> case 2 ( look documentation beside Send_uno() function )
      // we will change board,color arrays with the suitable values Rearrange_me() does this
      (*killed_pieces)++;                                                  // because we are certain that a piece is eaten
      Send_uno(2,Get_row(place0),(place0%8));                              // we now send 6 values ( case,from_row,from_col0,case,to_row1,to_col1)
      Send_uno(2,Get_row(place1),(place1%8));                              // we are sure that from_row is held in place0 because of the checks done in the function
      int killer = -1 ;
      int killer_color = -1;
      killer = board[place0];
      killer_color = color[place0];
      // we may need to check legallity before the next line   (Uno must send a feedback on Legality)  (Lcd notification if not legal)
      Rearrange_me(killer,killer_color,place0,place1);
    }
    if ( ones == (32 - (*killed_pieces))){
      // this means that a piece moved form a place to another but no killing --> case 3 ( look documentation beside Send_uno() function )
      // we will change board,color arrays with the suitable values
      Send_uno(3,Get_row(place0),(place0%8));                              // we now send 6 values ( case,from_row,from_col0,case,to_row1,to_col1)
      Send_uno(3,Get_row(place1),(place1%8));                              // we are sure that from_row is held in place0 because of the checks done in the function
      int move_from = -1;
      int move_from_color = -1;
      move_from = board[place0];
      move_from_color = color[place0];  
      Rearrange_me(move_from,move_from_color,place0,place1);
    }
  }
}

void Rearrange_me(int killer, int killer_color, int from_place, int to_place){
    board[from_place] = 7;
    board[to_place]   = killer;
    color[from_place] = 7;
    board[to_place]   = killer_color;
}

void Send_uno(int code,int row,int col){
  char x[3];
  itoa(code,x,10);
  Serial1.write(x);
  itoa(row,x,10);
  Serial1.write(x);
  itoa(col,x,10);
  Serial1.write(x);
}

void Swap_0_and_1(void){
  for(int i = 0 ; i < 64 ;i++){
    detect_0[i] = detect_1[i];
  }
}



void Read_serial(void){
  if ( Serial1.available()){
    // IT must now read from Uno to know if a promotion (code = 0) happened or a check ( code =1 )or a move wasn't legal(either wrong turn(code = 2) or wrong place(code = 3))
    // also the uno must determine at which side this happened for the mega to determine on which lcd it should print
    int code = Serial1.read();
    int color = Serial1.read();
    if (code == 0){                                                          // Promotion
        if (color == 0){
          lcdwhite.setCursor(0, 1);
          lcdwhite.print("Promote pawn!");
          Buzzer();
        }else{
          lcdblack.setCursor(0,1);
          lcdblack.print("Promote pawn!");
          Buzzer();
        }
    }else if (code == 1){                                                 //check
        if (color == 0){
          lcdwhite.setCursor(0, 1);
          lcdwhite.print("Check!");
          Buzzer();
        }else{
          lcdblack.setCursor(0,1);
          lcdblack.print("Check!");
          Buzzer();
        }
    }else if (code == 2){
        if (color == 0){
          lcdwhite.setCursor(0, 1);
          lcdwhite.print("WRONG TURN!");
          lcdblack.setCursor(0,1);
          lcdblack.print("ALERT! YOUR TURN");
          Buzzer();
        }else{
          lcdwhite.setCursor(0, 1);
          lcdwhite.print("ALERT! YOUR TURN");
          lcdblack.setCursor(0,1);
          lcdblack.print("WRONG TURN!");
          Buzzer();
        }
    }else if(code == 3){
        if (color == 0){
          lcdwhite.setCursor(0, 1);
          lcdwhite.print("ALERT! ILLEGAL");
          lcdblack.setCursor(0,1);
          lcdblack.print("ALERT! ILLEGAL");
          Buzzer();
        }else{
          lcdwhite.setCursor(0, 1);
          lcdwhite.print("ALERT! ILLEGAL");
          lcdblack.setCursor(0,1);
          lcdblack.print("ALERT! ILLEGAL");
          Buzzer();
        }
    }
  }
}

void Buzzer(void){
          digitalWrite(buzzer_p,HIGH);
          digitalWrite(buzzer_n,LOW);
          delay(100);
          digitalWrite(buzzer_p,HIGH);
          digitalWrite(buzzer_n,LOW);
          delay(100);
}

