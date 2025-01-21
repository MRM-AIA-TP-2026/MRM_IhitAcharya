1.To navigate rover from one latitude/longitude to another in a world with no obstacles
2.The movement_pub is publishing to the topic cmd_vel which is responsible for sending movement msgs to rover.The gps_sub is subscribed to the /gps topic so that it can obtain it's current gps latitude and longitude
twist_msg is used to send twist messages to the /cmd_vel topic to cuause the rover to roatte and move
3.rqt_graph:![image](https://github.com/user-attachments/assets/5078a06f-c61d-4c30-b325-cf6e5e38a748)
4.Youtube Link : https://youtu.be/3py35EbUGPk
