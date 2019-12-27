#include <SoftwareSerial.h>
#include <string.h>
#include <stdlib.h>
SoftwareSerial gps(2, 3);     //Arduino Uno 的 pin 0是 Rx， pin 1是 Tx
int i = 0, j = 0;
char data[512];
int count = 0;
typedef struct {
  int hours;
  int minutes;
  int seconds;
} GPS_TIME;
typedef struct {
  GPS_TIME gps_time;
  char lon[16];
  char lon_type;
  char lat[16];
  char lat_type;
  int mode;//Position indicating  0:error,1:SPS mode,2:SPS mode,3:pps mode
  int num;//Number of satellites
} GPS_DATA;

GPS_DATA gps_data;


void setup()
{
  Serial.begin(115200);
  gps.begin(9600);
}

void loop()
{
  while (gps.available() > 0)
  {
    char c = gps.read();
    data[count++] = c;
    if (c == '\r') {
      j = 1;
    } else if (c == '\n') {
      i = 1;
      if (i == 1 && j == 1) {
        if (strstr(data, "$GNGGA") > 0)
        {
          Serial.write(data, count);
          get_gps_data();
          Serial.print("time: h:");
          Serial.print(gps_data.gps_time.hours);
          Serial.print(":");
          Serial.print(gps_data.gps_time.minutes);
          Serial.print(":");
          Serial.println(gps_data.gps_time.seconds);
          Serial.print("lat:");
          Serial.print(gps_data.lat);
          Serial.println(gps_data.lat_type);
          Serial.print("lon:");
          Serial.print(gps_data.lon);
          Serial.println(gps_data.lon_type);
          Serial.print("mode:");
          Serial.println(gps_data.mode);
          Serial.print("num:");
          Serial.println(gps_data.num);
        }
        count = 0;
        clearBufferArray();
      } else
      {
        i = 0; j = 0;
      }
    }
  }
}

void get_gps_data()
{
  char *p1, *p[16];
  char t[6] = "";
  char lat_c[32] = "";

  int j = 0;
  strtok((char *)data, ",");
  while ((p1 = strtok(NULL, ",")) != NULL)
  {
    p[j] = p1;
    j++;
  }
  //time:
  strcpy((char *)t, p[0]);
  gps_data.gps_time.hours = (t[0] - 48) * 10 + (t[1] - 48);
  gps_data.gps_time.minutes = (t[2] - 48) * 10 + (t[3] - 48);
  gps_data.gps_time.seconds = (t[4] - 48) * 10 + (t[5] - 48);

  //lat:
  strcpy(gps_data.lat, p[1]);
  gps_data.lat_type = p[2][0];

  //lon
  strcpy(gps_data.lon, p[3]);
  gps_data.lon_type = p[4][0];

  //mode and satellites num
  gps_data.mode = p[5][0] - 48;
  gps_data.num = p[6][0] - 48;

  //  for (int i = 0; i < 12; i++) {
  //    Serial.print(i);
  //    Serial.print(":");
  //    Serial.println(p[i]);
  //  }

}
void clearBufferArray()                     // function to clear buffer array
{
  for (int i = 0; i < count; i++)
  {
    data[i] = NULL;
  }                      // clear all index of array with command NULL
}
