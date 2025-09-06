// 결선
#define AC_TRIAC_SWITCHING 27
#define TEMP_SENSOR 25
#define HUMI_I2C_SDL 21
#define HUMI_I2C_SCL 22
#define GPS_RX 17
#define GPS_TX 16

void setup() {
  pinMode(AC_TRIAC_SWITCHING, OUTPUT);
  pinMode(TEMP_SENSOR, INPUT);
  wifi_init();
  wss_init();
  gps_init();
}

typedef struct PresentStatus{
  double latit; // 위도
  double longi; // 경도
  double temp;
  double humi;
}PresentStatus;

void loop() {

}

void wifi_init(){

}

void wss_init(){

}

void gps_init(){

}

void GPS_RELODE(){

}

void wss_send(String server_send){
  
}

String make_json(String& input_string){

} 