# include <stdio.h>
# include <unistd.h>

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
void motors(int val);

const int img_width = 320;
const int img_height = 240;
const int colour_threshold = 127;
const int distance_threshold = 255;
const int distance_port = 0;
const int normal_speed = 40;
const float Kp = 0.0025;
const float Kd = 0.004;
const float Ki = 0.006;

int prev_direction = 0;
int integral = 0;

int main(){
        printf("%d/n",getpid());
        init(0);
        while(true) {
                if(read_analog(distance_port) > distance_threshold){
                        open_gate();
                        while(read_analog(distance_port) > distance_threshold){
                                Sleep(0, 500);
			}
                }
                int direction = 0;
                take_picture();
                bool line = false;
                for(int i = 0; i < img_width-1; i++) {
                        int pixel = get_pixel(i, img_height/2, 3);
                        if(pixel > colour_threshold) {
                                direction += i-(img_width/2);
                                line = true;
                        }
//                      direction += pixel > colour_threshold ? 0 : i-(img_widt$
                }
//              printf("%f\n", direction*Kp);

                if(line) {
                        integral += direction;
                        int deriv = direction-prev_direction;
                        motors(direction*Kp+deriv*Kd);
                        prev_direction = direction;
                } else {
                        motors(prev_direction > 0 ? 10 : -10);
                }
        }
        return 0;
}

void open_gate(){
        printf("Attempting to open gate\n");
        connect_to_server("130.195.6.196", 1024);
        send_to_server("Please");
        char message[7];
        receive_from_server(message);
        printf(message);
        return;
}

void motors(int val) {
        printf("%d\n", val);
        int m1 = normal_speed+val;
        int m2 = normal_speed-val;
        set_motor(1, m1 >= 255 ? 255 : m1 <= -255 ? -255 : m1);
        set_motor(2, m2 >= 255 ? 255 : m2 <= -255 ? -255 : m2);
        Sleep(0, 500);
}