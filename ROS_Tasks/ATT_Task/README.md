1.Summary:To accept target latiude and longitude and to traverse to this point on a plane with no obstacles
2.Usage of a movement_pub to publish msgs to the topic /cmd_vel.Usage of the gps_sub to obtain current longitude and latitude of the rover.end_pub a node also publishing msgs to cmd_vel.
twist_msg was sent by publishing movement_pub to the cmd_vel for changing rotation and movement of the rover.The rover first aligns to the target bearing direction then moves the distance between current lat/lon to target lat/lon
3.![image](https://github.com/user-attachments/assets/91e72278-17d5-4caf-abea-7c35a041401a)
4.Youtube link to the ATT task : https://youtu.be/3py35EbUGPk
