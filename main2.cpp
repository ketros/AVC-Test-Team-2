# include <stdio.h>

extern "C" int init(int d_lev);
extern "C" int take_picture();
extern "C" char get_pixel(int row, int col, int color);
extern "C" int connect_to_server(char server_addr[15], int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);
extern "C" int read_analog(int ch_adc);
extern "C" int Sleep(int sec, int usec);
extern "C" int set_motor(int motor, int speed);
void open_gate();

const int img_width = 320;
const int img_height = 240;
const int colour_threshold = 125;
const int distance_threshold = 255;
const int speed = 40;
const int distance_port = 0;
const int square = 1;

int main() {
  init(0);
  while(true) {
    if(read_analog(distance_port) > distance_threshold) {
      open_gate();
      while(read_analog(distance_port) > distance_threshold) {
	Sleep(0, 500);
      }
    }
    take_picture();
    int left_col = 0;
    for(int x = (img_width/4)-(square/2); x < (img_width/4)+(square/2); x++) {
      for(int y = (img_height/2)-(square/2); y < (img_height/2)+(square/2); y++) {
	left_col += get_pixel(y, x, 3);
      }
    }
    left_col /= (square*square);
    
    int right_col = 0;
    for(int x = (3*img_width/4)-(square/2); x < (3*img_width/4)+(square/2); x++) {
      for(int y = (img_height/2)-(square/2); y < (img_height/2)+(square/2); y++) {
	right_col += get_pixel(y, x, 3);
      }
    }
    right_col /= (square*square);
    
    bool left = left_col > colour_threshold;
    bool right = right_col > colour_threshold;
    
    if(left && right) {
      set_motor(1, speed);
      set_motor(2, speed);
    }
    if(left && !right){
      
    }
    if(!left && !right){
      
    }
    if(!left && !right){
      
    }
  }
}

void open_gate(){
        printf("Attempting to open gate\n");
        connect_to_server("130.195.6.196", 1024);
        send_to_server("Please");
        char message[7];
        receive_from_server(message);
        printf(message);
        printf("\n");
        send_to_server(message);
        printf("Opening Gate\n");
        return;
}