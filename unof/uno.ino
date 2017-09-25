/* The code for UNO arduino.
 *  Written by : Abdullah mohamed, Sama abed nasr and Ahmed hany
 *  
 *  NOT DONE YET :
 *     // Check if turn is right
 *     // white and black_king_loc must be modified 
 *      // if a king moves , king_loc must be changed
*/

#define TX0    0               // transmitting pin
#define RX0    1               // recieving pin
#define King   1                  // Until now giving each piece its own code isn't necessary but
#define Queen  2                  // for clarity of the code for a human reader.
#define Bishop 3
#define Knight 4
#define Rock   5
#define Pawn   6
#define Free   7                   // This represent a free spot on the board
#define White  0                   
#define Black  1 

int i;                                                     // loop counter (better make it local to each)
int y = 0;                                                 // loop counter also
int led_array[64];                                         // contains the leds you want to light from 0 to 63  // note : isn't important now since max is deleted
int led_indicies[128];                                     // contains the indicies of, the leds you want to illuminate according to their pin connections
int turn = 0;                                              // White always start
int white_king_loc = 60;                                   // we will change this as the game goes on
int black_king_loc = 4;


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

int color_inti[64]  =   { 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1,             // This represent the colors of the pieces at the beginning of the game
                          7, 7, 7, 7, 7, 7, 7, 7,               
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0  };

int color[64]       =   { 1, 1, 1, 1, 1, 1, 1, 1,            // This represent the colors of the pieces while the game is on. Changes when a move happens
                          1, 1, 1, 1, 1, 1, 1, 1,
                          7, 7, 7, 7, 7, 7, 7, 7, 
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          7, 7, 7, 7, 7, 7, 7, 7,
                          0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0  };



// functions
void Intialize_board(void);                                 // This function intializes all arrays at the start of the game to the default values.
int Get_row_col(int, int *);                                //takes led number from 0 to 63 and returns the 
                                                            // pin number where the row of the led is connected and the pin number
                                                            // where the col of the led is connected
void Intialize_led_indicies_led_array(void);                // intialize the matrix to values of -1 to know when to stop because we
                                                            // don't need to illuminate all leds each time
void Light_leds(void);                                      // Lights the leds inside Led_indicies
int  Length_array(void);                                    // gets the length of led_matricies to stop using max (didn't try strlen YET)
void Fill_led_indicies(int length1);                        // fills led_indicies as we deal with leds as its row and col not its index(0~63)
void Pawn_moves(int row,int col);                           // get moves for black or white pawns
void Rock_moves(int row,int col);                           // get moves for rocks
void Knight_moves(int row,int col);                         // get moves for knights
void Bishop_moves(int row ,int col);                        // get moves for bishops
void Queen_moves(int row ,int col);                         // get moves for queen basically its (bishop moves+rock moves)
void King_moves(int row, int col);                         // get moves for king
int Is_King_in_check(int row,int col);                      // checks if the king was in check
int Is_allowed_to_king(int from_row, int from_col, int to_row, int to_col);                  
                                                            // filters the movements of the king
int Is_allowed_relative_to_king(int from_row, int from_col, int to_row, int to_col);          
                                                            // checks if the movement will leave the king in check or not
void Send_mega(int code);                                   // Sends a code to mega to tell if if there is a promotion (code =0), check(code = 1)
                                                            // so that the mega can show it on the LCD
void Start_lights(void);                                    // Some lights at the beginning of the game

void setup() {
  // Configuring pins as Output (16 pins) starting from pin 2 as pin 0 and 1 are reserved for serial communication
  for( i = 2; i < 18 ; i++){
      pinMode(i,OUTPUT);
  }

  // Configuring pins for Serial communication
  Serial.begin(9600);

  Intialize_board();
  Start_lights();
}


void loop() {
  // The function would go as:
  // we wait for serial input from the Mega controller
  // according to the number of sent data, we start to check to know which case of detection happened
  // (look at the Mega code) for reference if you need to know the 4 cases
     if(Serial.available()){
        int code = Serial.read();
        int row_loop = Serial.read();                                      // loop is just for its local identification from other global variables
        int col_loop = Serial.read();
        int piece_loop = board[row_loop*8+col_loop];
        if (color[row_loop*8+col_loop] == turn){                           // first move is always White
          if(code == 0){
                // This code means a piece is lifted and it is still in the air. We will find which piece exists in such place and light its leds. Note : When the Mega sends a row, it starts from 0. while the Uno starts from 2 on sending
                if ( piece_loop == 1){                                     // if a king moves , king_loc must be changed
                  King_moves(row_loop,col_loop);                                        
                }else if ( piece_loop == 2){
                  Queen_moves(row_loop,col_loop);
                }else if ( piece_loop == 3){
                  Bishop_moves(row_loop,col_loop);
                }else if ( piece_loop == 4){
                  Knight_moves(row_loop,col_loop);
                }else if ( piece_loop == 5){
                  Rock_moves(row_loop,col_loop);
                }else if ( piece_loop == 6){
                  Pawn_moves(row_loop,col_loop);
                }
                Intialize_led_indicies_led_array();
                int len = Length_array();
                Fill_led_indicies(len);
                while(!Serial.available()){                       // Lights must be left ON until new data is recieved
                Light_leds();
                }
        }else if (code == 1){
                // This means a Piece is placed down and we need to do nothing
                while (!Serial.available()){};
        }else if (code == 2){
                // This means a piece is moved but no eating happened and we need to update our arrays
                int useless_code;                                                            // this is useless and will be equal to 2
                int to_row = Serial.read();
                int to_col = Serial.read();
                board[to_row*8+to_col] = piece_loop;                                         // The new place will hold the new piece
                board[row_loop*8+col_loop] = 7;                                              // The old place will hold a space
                turn ^=1;                                                                    // a move is made so the turn must be switched to the other color
        }else if (code == 3){
                // This means a piece is moved and also a piece is eaten
                int useless_code;                                                            // this is useless and will be equal to 3
                int to_row = Serial.read();
                int to_col = Serial.read();
                board[to_row*8+to_col] = piece_loop;                                         // The new place will hold the new piece
                board[row_loop*8+col_loop] = 7;                                              // The old place will hold a space
                turn ^=1;
        }
     }else{
        Send_mega(2);                                                                        // Wrong turn warning , This will give the players a 10-sec period to retreat the move 
     }
   }
}

void Intialize_board(void){
  int i = 0;                                                                // loop counter
  for ( i = 0 ; i <64; i++){
    board[i]    = board_inti[i] ;
    color[i]    = color_inti[i] ;
  }
}

void Intialize_led_indicies_led_array(void){
  for (i = 0 ; i < 128; i++){
    led_indicies[i] = -1 ;                                   // make all members hold -1
    }
  for ( int counter = 0 ; counter < 64 ; counter++){
    led_array[counter] = -1;                          
  }
  }

int Get_row_col(int led, int *col){
  int row;
 if (led <= 7){
  row = 2;  //leds are connected in a matrix where rows start from pin2 to pin 9 or less and columns start from pin10 to pin 15 or less
  goto aa; 
 }else if(led <=15){
  row = 3;
  goto aa;
 }else if(led <= 23){
 row = 4;
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
}
aa:
if ( led-(8*(row-2)) == 0){                      // led % 8
  *col = 17;
  goto bb;
}else if ( led-(8*(row-2)) == 1){
  *col = 16;
  goto bb;
}else if ( led-(8*(row-2)) == 2){
  *col = 15;
  goto bb;
}else if ( led-(8*(row-2)) == 3){
  *col = 14;
  goto bb;
}else if ( led-(8*(row-2)) == 4){
  *col = 13;
  goto bb;
}else if ( led-(8*(row-2)) == 5){
  *col = 12;
  goto bb;
}else if ( led-(8*(row-2)) == 6){
  *col = 11;
  goto bb;
}else if ( led-(8*(row-2)) == 7){
  *col = 10;
}
bb:
return row;
}

void Start_lights(void){
  for ( int i = 0 ; i < 2 ;i++){
  // Light row by row from left to the right
  for(int row =2; row <10;row++){
    for(int col = 2 ; col < 10; col++){
      if(row == col){
        digitalWrite(col,HIGH);
      }else{
        digitalWrite(col,LOW);
      }
    }
    delay(10);
    for(int col = 10 ; col < 18; col++){
      for(int j = 10;j< 18;j++){
        if(j == col){
          digitalWrite(j,LOW);
        }else{
          digitalWrite(j,HIGH);
        }
      }
    }
  }
  // Light row by row from right to left
  for(int row =10; row >2;row--){
    for(int col = 10 ; col >2; col--){
      if(row == col){
        digitalWrite(col,HIGH);
      }else{
        digitalWrite(col,LOW);
      }
    }
    delay(10);
    for(int col = 18 ; col > 10; col--){
      for(int j = 18;j> 10;j--){
        if(j == col){
          digitalWrite(j,LOW);
        }else{
          digitalWrite(j,HIGH);
        }
      }
    }
  }
}
}


int Length_array(void){
  //int length = sizeof(led_array)/sizeof(int);     // gets you the length of the whole array (rows*rows) not the number of the elements you entered. 
                                                    // NOTE  :  didn't try strlen yet      ( Tried it)  -- > didn't work, seems to only work with array of chars                                                 
  int counter = 0;
    while ( led_array[counter] != -1){
        counter++;
    }
   return counter ;
}

void Light_leds(void){
  // This is the loop which lights the suitable leds after being put inside Led_arrays
  while(!Serial.available()){
  int y = 0;
 for( i = 0 ; i < 32 ; i+=2){                          // suspisious loop ( may appear to be unnecessary )    
  while(led_indicies[i] != -1){
    for( y = 2 ; y < 10 ; y++){
      if ( y == led_indicies[i]){
          digitalWrite(y,HIGH);
      }else {
        digitalWrite(y,LOW);
      }
    }
    //delay(0.11);
     for( y = 10 ; y < 18 ; y++){
      if ( y == led_indicies[i+1]){
          digitalWrite(y,LOW);
      }else {
        digitalWrite(y,HIGH);
      }
    }
    //delay(0.11);
    i++;
  }
 }
  }
}

void Fill_led_indicies(int length1){
  int col;
  int x = 0;
  for(i = 0; i < length1 ;i+=2){ 
    led_indicies[i]   = Get_row_col(led_array[x],&col);
    led_indicies[i+1] = col;
    x++;
  }
}

void Pawn_moves(int row,int col){
  int x =0 ;                                                               // counter for the array indicies
  int place_on_board = (row*8)+col;                                        // BECAREFUL that the row starts either from 0 or 2 (solved by making the Mega count from 2)
  int color1 = color[place_on_board];  
  // pawn, the board always starts with a black rock at 0,0 and a white one at 7,7
  // white pawn
  if (color1 == 0 && row < 7 && row >= 0 && board[(place_on_board-8)] ==7){
      // board[(place_on_board-8)] == 7 , this condition ensures that the block ahead of the pawn is free
      led_array[x++] = place_on_board - 8 ;
    if (row == 6 && board[(place_on_board-16)] ==7){
     led_array[x++] = place_on_board - 16 ;
    }
    if ( row == 0){
      // PROMOTION HERE ****************************************************************************************** <--------        DONE
      // Promotion assume that the player will only upgrade to queen
      Send_mega(0);
      board[col] = 2;
    }
  
    //attacking moves must be added diagonally if colour[row -1][col -1] == 1 means a black piece exist diagonally to the left
    if(color[(place_on_board-7)] == (color1^1) && col < 7){
     // This means an opposite color piece exists at the upper right block
    led_array[x++] = place_on_board - 7;
    }
    if (color[(place_on_board - 9)] == (color1^1) && col > 0){
      // This means an opposite color piece exists at the upper left block
      led_array[x++] = place_on_board - 9;
    }
}else  if (color1 == 1 && row > 0 && row <= 7 && board[(place_on_board + 8)] == 7){                                          //black pawn
    led_array[x++] = place_on_board + 8;
    if (row == 1 && board[(place_on_board + 16)] == 7){
      led_array[x++] = place_on_board + 16;
    }
    if (row == 7 ){
        Send_mega(0);                                                                          //promotion
        board[56+col] = 2;                                                                    // 2 for a queen
    }
    if ( color[(place_on_board + 9)] == (color1^1) && col < 7){
      led_array[x++] = place_on_board + 9;
    }
    if ((color[(place_on_board + 7)] == (color1^1)) && col > 0){
      led_array[x++] = place_on_board + 7;
    }
  }
}

  void Rock_moves(int row,int col){ 
    int place_on_board = row*8 + col;                          // BECAREFUL how you recieve row and col as it starts from either 0 or 2. solved by making the mega start counting from 2
    int color1 = color[row*8+col];
    int i = 0, x = 0;                                           // i for the loop, x for the array index increment
    // This function willn't need filteration
    // The next 2 loops will generate the moves allowed in the column of the rock
    for(i = row - 1; i >= 0; i--){
          if(board[((i*8)+col)] == 7 || (board[((i*8)+col)] != 7 && color[((i*8)+col)] == (color1^1)) && Is_allowed_relative_to_king(row,col,i,col)){
              // this condition ensures the place is empty                               
              led_array[x++] = i*8 + col;
            }else{
              // if a block contains a piece, it will block all the moves on the Rock
              break;
            }
       }
    for(i= row+1; i < 8 ;i++){
          if(board[((i*8)+col)] == 7 || (board[((i*8)+col)] != 7 && color[((i*8)+col)] == (color1^1))  && Is_allowed_relative_to_king(row,col,i,col)){
            // this condition ensures the place is empty
            led_array[x++] = i*8 + col;
            }else{
              // if a block contains a piece, it will block all the moves on the Rock
              break;
            }
       }
    // The next 2 loops will generate the moves allowed in the row of the rock
    for(i = col - 1; i >= 0; i--){
          if(board[((row*8)+i)] == 7 || (board[((row*8)+i)] != 7 && color[((row*8)+i)] == (color1^1)) && Is_allowed_relative_to_king(row,col,row,i)){
            // this condition ensures the place is empty
            led_array[x++] = row*8 + i;
            }else{
              // if a block contains a piece, it will block all the moves on the Rock
              break;
            }
       }
    for(i= col + 1; i < 8 ; i++){
          if(board[((row*8)+i)] == 7 || (board[((row*8)+i)] != 7 && color[((row*8)+i)] == (color1^1)) && Is_allowed_relative_to_king(row,col,row,i)){
            // this condition ensures the place is empty
            led_array[x++] = row*8 + i;
            }else{
              // if a block contains a piece, it will block all the moves on the Rock
              break;
            }
       }
    
  }


void Knight_moves(int row,int col){
  int x = 0;                                          // array index increment
                                                      // BECAREFUL of how you recieve the row as 0 or 2
                                                      // This function willn't need filteration
  int place_on_board = row*8 + col;
  int color1 = color[((row)*8)+ (col)];
  row -=2;                                            // Mega sends row starting from 2 , but columns start counting from 0
  if ((row -2) >= 0){
    // inside the board
    if( col < 7){                                                               // the 2 moves upwards
    if ( board[((row-2)*8)+ (col+1)] == 7 || (board[((row-2)*8)+ (col+1)] != 7 && color[((row-2)*8)+ (col+1)] == (color1^1)) && Is_allowed_relative_to_king(row,col,row-2,col+1)){
    // This condition ensures that the place on board is empty
    led_array[x++] = (row-2)*8 + (col+1);
    }
    }
    if( col >0){
    if ( board[((row-2)*8)+ (col-1)] == 7 || (board[((row-2)*8)+ (col-1)] != 7 && color[((row-2)*8)+ (col-1)] == (color1^1)) && Is_allowed_relative_to_king(row,col,row-2,col-1)){
    // This condition ensures that the place on board is empty
    led_array[x++] = (row-2)*8 + (col-1);
    }
    }
  }
  if((row +2) <= 7){                                                             // the 2 moves downwards
    // inside the board
    if(col < 7){
    if ( board[((row+2)*8)+ (col+1)] == 7 || (board[((row+2)*8)+ (col+1)] != 7 && color[((row+2)*8)+ (col+1)] == (color1^1)) && Is_allowed_relative_to_king(row,col,row+2,col+1)){
    // This condition ensures that the place on board is empty
    led_array[x++] = (row+2)*8 + (col+1);
    }
    }
    if ( col > 0){
    if ( board[((row+2)*8)+ (col-1)] == 7 || (board[((row+2)*8)+ (col-1)] != 7 && color[((row+2)*8)+ (col-1)] == (color1^1)) && Is_allowed_relative_to_king(row,col,row+2,col-1)){
    // This condition ensures that the place on board is empty
    led_array[x++] = (row+2)*8 + (col-1);
    }
    }
  }
  if ((col -2) >= 0){                                               // the 2 moves to the left
    if ( row < 7){
    if ( board[((row-1)*8)+ (col-2)] == 7 || (board[((row-1)*8)+ (col-2)] != 7 && color[((row-1)*8)+ (col-2)] == (color1^1)) && Is_allowed_relative_to_king(row,col,row-1,col-2)){
    // This condition ensures that the place on board is empty
    led_array[x++] = (row-1)*8 + (col-2);
    }
    }
    if ( row>0){
    if ( board[((row+1)*8)+ (col-2)] == 7 || (board[((row+1)*8)+ (col-2)] != 7 && color[((row+1)*8)+ (col-2)] == (color1^1)) && Is_allowed_relative_to_king(row,col,row+1,col-2)){
    // This condition ensures that the place on board is empty
    led_array[x++] = (row+1)*8 + (col-2);
    }
    }
  }
  if((col +2) <= 7){                                             // the 2 moves to the right
    if (row < 7){
    if ( board[((row-1)*8)+ (col+2)] == 7 || (board[((row-1)*8)+ (col+2)] != 7 && color[((row-1)*8)+ (col+2)] == (color1^1)) && Is_allowed_relative_to_king(row,col,row-1,col+2)){
    // This condition ensures that the place on board is empty
    led_array[x++] = (row-1)*8 + (col+2);
    }
    }
    if ( row > 0){
    if ( board[((row+1)*8)+ (col+2)] == 7 || (board[((row+1)*8)+ (col+2)] != 7 && color[((row+1)*8)+ (col+2)] == (color1^1)) && Is_allowed_relative_to_king(row,col,row+1,col+2)){
    // This condition ensures that the place on board is empty
    led_array[x++] = (row+1)*8 + (col+2);
    }
    }
  }
}

void Bishop_moves(int row ,int col){
  int x = 0;                                           // array index increment
  int place_on_board = row*8 + col;
  int color1 = color[row*8+col];
  int r = row;                                           // the rows upwards
  int c = 1 ;                                            // columns counter
  int index_0 = 0;                                       // This will let you decide wether to complete the diagonal line to the left
  int index_1 = 0;                                       // This will let you decide wether to complete the diagonal line to the right
                                                         // This function willn't need filteration
  for ( r = row; r >= 0; r--){
    // the upper portion of the board
    if(index_0 == 0){
      if (board[((r*8)+(col -c))] == 7 || (board[((r*8)+(col -c))] != 7 && color[((r*8)+(col -c))] == (color1^1)) && c <= col && Is_allowed_relative_to_king(row,col,r*8,col-c)){
          // free spot to the left 
          led_array[x++] = ((r*8)+(col -c));
        }else{
          index_0++;
        }
    }
    if (index_1 == 0){
      if (board[((r*8)+(col +c))] == 7 || (board[((r*8)+(col +c))] != 7 && color[((r*8)+(col +c))] == (color1^1)) && c < (7 - col) && Is_allowed_relative_to_king(row,col,r*8,col+c)){
        // free spot to the right
        led_array[x++] = ((r*8)+(col +c));
      }else{
        index_1++;
      }
    }
    c++;
  }
 
  for ( r = row; r < 8; r++){
    // the upper portion of the board
    if(index_0 == 1){
    if (board[((r*8)+(col -c))] == 7 || (board[((r*8)+(col -c))] != 7 && color[((r*8)+(col -c))] == (color1^1)) && c <= col && Is_allowed_relative_to_king(row,col,r*8,col-c)){
        // free spot to the left
        led_array[x++] = ((r*8)+(col -c));
      }else{
        index_0++;
      }
    }
    if(index_1 == 1){
    if (board[((r*8)+(col +c))] == 7 || (board[((r*8)+(col +c))] != 7 && color[((r*8)+(col +c))] == (color1^1)) && c < (7 - col) && Is_allowed_relative_to_king(row,col,r*8,col+c)){
        // free spot to the right
        led_array[x++] = ((r*8)+(col +c));
      }else{
        index_1++;
      }
    }
    c++;
  }
}

void Queen_moves(int row ,int col){
  Rock_moves(row,col);
  Bishop_moves(row,col);
}

void King_moves(int row, int col){
  int x = 0;                                                            // array index increment
  int place_on_board = row*8 +col;                                      // BECAREFUL how you recieve row as 0 and 2
  int c = col -1;                                                       // column counter
  int color_king = color[row*8+col];
                                                                        // This one needs FILTERING (Is_allowed_to_king() fn)   --> DONE
                                                                        // also we will need another function to check for castling   --> DONE
                                                                        // a suggested algorithm is to send each position to a function which finds the nearest piece in every direction of the 
                                                                        // surrounding 7 directions (excluding king) and excluding also the ones of the same color ,checking if the nearest piece
                                                                        // can attack the king if he was in such position then returning 1 or 0 to add the Led in Led_array or not. 
  if ( row < 7){
    // The lower 3 moves
    for(int i= 0; i< 3;i++){
        if((board[(row+1)*8+c] == 7 || ((board[(row+1)*8+c] != 7 && color[(row+1)*8+c] == (color_king^1))&& c >= 0 && c < 7)) && Is_allowed_to_king(row,col,row+1,c)){
          // free on board and the second condition prevents negative column number
          led_array[x++] = (row+1)*8+c;
        }
        c++;
  }
  c = col -1;
  if ( row > 0){
    // the upper 3 moves
    for(int i= 0; i< 3;i++){
        if((board[(row-1)*8+c] == 7 || ((board[(row-1)*8+c] != 7 && color[(row-1)*8+c] == (color_king^1)) && c >= 0 && c < 6)) && Is_allowed_to_king(row,col,row-1,c)){
          // free on board and the second condition prevents negative column number or outside of scope
          led_array[x++] = (row-1)*8+c;
        }
        c++; 
   }
   if ( col > 0){
    if ( board[(row*8)+(col-1)] == 7 || ((board[(row*8)+(col-1)] != 7 && color[(row*8)+(col-1)] == (color_king^1))) && Is_allowed_to_king(row,col,row,col-1)){
      // free spot to the left
      led_array[x++] = (row*8)+(col-1);
      if ( board[(row*8)+(col-1)] == 7 && board[(row*8)+(col-2)] == 7 && board[(row*8)+(col-3)] == 7 && board[(row*8)+(col-4)] == 5 && Is_allowed_to_king(row,col,row,col-1)){
        // Castling to the right
        led_array[x++] = (row*8)+(col-2);
      }
    }
   }
   if ( col < 7){
      if ( board[(row*8)+(col+1)] == 7 || (board[(row*8)+(col+1)] != 7 && color[(row*8)+(col+1)] == (color_king^1)) && Is_allowed_to_king(row,col,row,col+1)){
          // free spot to the right
          led_array[x++] = (row*8)+(col+1);
          if ( board[(row*8)+(col+1)] == 7 && board[(row*8)+(col+2)] == 7  && board[(row*8)+(col+3)] == 5 && Is_allowed_to_king(row,col,row,col+1)){
                // Castling to the left
                led_array[x++] = (row*8)+(col+2);
            }
      }
   }
}
  }
}


int Is_king_in_check(int row,int col){
  // this will check if a piece existing in a path where it can attack the king
  // this will be done piece by piece, returning 1 if king is in check and 0 if not.
  // first we check vertical then horizontal, and find the first piece we meet
  // then we check for its color to know if it belongs to the oppposite side
  // then for vertical and horizontal , if the piece is a queen or a rock . this means a check
  // then for diagonal , if the piece is a bishop,pawn,queen,king
  // for the Knight , special treatment.
  int place_on_board = row*8 + col;
  int color_king = color[place_on_board];
  // find first near piece verically upwards
  int i = 0;
  if ( row > 0 ){
    // avoiding going outside the board
      for( i = row -1 ; i >= 0;i--){
          if(board[((i*8)+col)] != 7){
            // there is a piece above the king
            // now we check its color and type ( remeber 2 and 5 are queen and rock)
            if ( color[(i*8)+col] == (color_king ^1) && (board[(i*8)+col] == 2 || board[(i*8)+col] == 5)){
                return 1;
            }
          }
      }
  }
  if ( row < 7){
    // find first near place vertically downwards
    for( i = row +1 ; i < 8; i++){
        if(board[((i*8)+col)] != 7){
           // there is a piece above the king
            // now we check its color and type ( remeber 2 and 5 are queen and rock)
            if ( color[(i*8)+col] == (color_king ^1) && (board[(i*8)+col] == 2 || board[(i*8)+col] == 5)){
            return 1;
            }
         }
      }
  }
  if ( col > 0){
      // find first near piece to the left
      for( i = col-1 ; i >= 0 ; i--){
          if(board[((row*8)+i)] != 7){
              // there is a piece above the king
              // now we check its color and type ( remeber 2 and 5 are queen and rock)
              if ( color[(row*8)+i] == (color_king ^1) && (board[(row*8)+i] == 2 || board[(col*8)+i] == 5)){
                  return 1;
              }
          }
        }
  }
  if ( col < 8){
        // find first near piece to the right
        for( i = col+1 ; i < 8 ; i++){
            if(board[((row*8)+i)] != 7){
                // there is a piece above the king
                // now we check its color and type ( remeber 2 and 5 are queen and rock)
                if ( color[(row*8)+i] == (color_king ^1) && (board[(row*8)+i] == 2 || board[(col*8)+i] == 5)){
                    return 1;
                }
            }
        }
  }
  // find first near piece diagonally upwards to the right
  int col1 = col+1;                        // to control the diagonal flow like stairs
  if ( row > 0 && col < 7){
      for ( i = row -1 ; i >= 0 ; i--){
           if ( board[(i*8)+col1] != 7){
              // there is a piece, now check if it is from the opposite color
              // now it could be a Pawn but it will have to be near or a queen or a bishop which doesn't have to be near
              if ( i == row -1 && color[(i*8)+col1] == (color_king ^ 1) && (board[(i*8)+col1] == 2 || board[(i*8)+col1] == 3 || board[(i*8)+col1] == 6 ) ){
                // that is the first upper right block to the king, could be a Pawn,Queen or a Bishop
                return 1;
              }
              if( color[(i*8)+col1] == (color_king ^ 1) && (board[(i*8)+col1] == 2 || board[(i*8)+col1] == 3)){
                   // there is a bishop or a queen putting the king in check
                   return 1;
              }
           }
           col1++;
          }
  }
  // find first near piece diagonally upwards to the left
  col1 = col -1;
  if ( row > 0 && col < 7){
      for ( i = row -1 ; i >= 0 ; i--){
           if ( board[(i*8)+col1] != 7){
              // there is a piece, now check if it is from the opposite color
              // now it could be a Pawn but it will have to be near or a queen or a bishop which doesn't have to be near
              if ( i == row -1 && color[(i*8)+col1] == (color_king ^ 1) && (board[(i*8)+col1] == 2 || board[(i*8)+col1] == 3 || board[(i*8)+col1] == 6 ) ){
                // that is the first upper right block to the king, could be a Pawn,Queen or a Bishop
                return 1;
              }
              if( color[(i*8)+col1] == (color_king ^ 1) && (board[(i*8)+col1] == 2 || board[(i*8)+col1] == 3)){
                   // there is a bishop or a queen putting the king in check
                   return 1;
              }
           }
           col1--;
          }
  }
  // find first near diagonally downwards to the right
  col1 = col +1;
  if ( row > 0 && col < 7 ){
      for ( i = row +1 ; i < 8 ; i++){
           if ( board[(i*8)+col1] != 7){
              // there is a piece, now check if it is from the opposite color
              // now it could be a queen or a bishop which doesn't have to be near
              if( color[(i*8)+col1] == (color_king ^ 1) && (board[(i*8)+col1] == 2 || board[(i*8)+col1] == 3)){
                   // there is a bishop or a queen putting the king in check
                   return 1;
              }
           }
           col1++;
          }
  }
  // find first near diagonally downwards to the left
  col1 = col -1;
  if ( row > 0 && col < 7 ){                // this if is dispensible
      for ( i = row +1 ; i < 8 ; i++){
           if ( board[(i*8)+col1] != 7){
              // there is a piece, now check if it is from the opposite color
              // now it could be a queen or a bishop which doesn't have to be near
              if( color[(i*8)+col1] == (color_king ^ 1) && (board[(i*8)+col1] == 2 || board[(i*8)+col1] == 3)){
                   // there is a bishop or a queen putting the king in check
                   return 1;
              }
           }
           col1--;
          }
  }
  // Now the horse's turn   (Done beautifully)
  if ((row -2) >= 0){
    // inside the board
    if( col < 7){
    if ( board[((row-2)*8)+ (col+1)] == 4){
    // This condition ensures that the place on board is empty
    return 1;
    }
    }
    if( col >0){
    if ( board[((row-2)*8)+ (col-1)] == 4){
    // This condition ensures that the place on board is empty
    return 1;
    }
    }
  }
  if((row +2) <= 7){
    // inside the board
    if(col < 7){
    if ( board[((row+2)*8)+ (col+1)] == 4){
    // This condition ensures that the place on board is empty
    return 1;
    }
    }
    if ( col > 0){
    if ( board[((row+2)*8)+ (col-1)] == 4){
    // This condition ensures that the place on board is empty
    return 1;
    }
    }
  }
  if ((col -2) >= 0){
    if ( row < 7){
    if ( board[((row-1)*8)+ (col-2)] == 4){
    // This condition ensures that the place on board is empty
    return 1;
    }
    }
    if ( row>0){
    if ( board[((row+1)*8)+ (col-2)] == 4){
    // This condition ensures that the place on board is empty
    return 1;
    }
    }
  }
  if((col +2) <= 7){
    if (row < 7){
    if ( board[((row-1)*8)+ (col+2)] == 4){
    // This condition ensures that the place on board is empty
    return 1;
    }
    }
    if ( row > 0){
    if ( board[((row+1)*8)+ (col+2)] == 4){
    // This condition ensures that the place on board is empty
    return 1;
    }
    }
  }
// It was done with the same algorithm because as Knight_moves()fn
  return 0;
}


int Is_allowed_to_king(int from_row, int from_col, int to_row, int to_col){
    board[to_row*8+to_col] = board[from_row*8 + from_col];                                      // send the king to the position that you want to check
    board[from_row*8 + from_col] = 7;                                                           // free the king 's position
    int check = Is_king_in_check(to_row,to_col);                                                // find if he is in danger
    board[from_row*8 + from_col] = board[to_row*8+to_col];                                      // return the king to where he was
    board[to_row*8+to_col] = 7;                                                                 // free the position again
    return check;
}

int Is_allowed_relative_to_king(int from_row, int from_col, int to_row, int to_col){
    int to_piece = board[to_row*8+to_col];
    board[to_row*8+to_col] = board[from_row*8 + from_col];                                      // send the piece to the position that you want to check
    board[from_row*8 + from_col] = to_piece;                                                    // free the position position
    int king_row, king_col;                                                                     // the same color king location
    if (color[from_row*8+from_col] == 0){               // white piece
      king_row = Get_row_col(white_king_loc,&king_col);
    }else{                                              // black piece
      king_row = Get_row_col(black_king_loc,&king_col);
    }
    int check = Is_king_in_check(king_row,king_col);                                            // find if king is in danger
    int from_piece = board[from_row*8 + from_col];
    board[from_row*8 + from_col] = board[to_row*8+to_col];                                      // return the king to where he was
    board[to_row*8+to_col] = from_piece;                                                        // free the position again
    return check;
}

void Send_mega(int code){
  char x[3];
  itoa(code,x,10);
  Serial.write(x);
}
